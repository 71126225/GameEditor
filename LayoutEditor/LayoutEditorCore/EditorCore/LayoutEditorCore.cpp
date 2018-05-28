#include "txRenderWindow.h"
#include "LayoutEditorCore.h"
#include "LayoutManager.h"
#include "Layout.h"
#include "EditorCoreRoot.h"
#include "ECUndoManager.h"
#include "WindowPrefab.h"
#include "WindowPrefabManager.h"
#include "WindowTemplateManager.h"
#include "UndoHeader.h"
#include "ECRenderWindowCoordinate.h"
#include "ECLog.h"
#include "CommandHeader.h"
#include "WindowProperty.h"
#include "ComponentEditorCore.h"
#include "InputSystem.h"
#include "Utility.h"

LayoutEditorCore::LayoutEditorCore()
:
mCurLayout(NULL),
mLayoutModified(false),
mCopyWindow(NULL),
mCopySrcLayout(NULL),
mCutWindow(false),
mWireframeVisible(true),
mCoordinateVisible(false)
{
	mRenderWindowCoordinate = TRACE_NEW(ECRenderWindowCoordinate, mRenderWindowCoordinate);
}

void LayoutEditorCore::init()
{
	mRenderWindowCoordinate->init();
}

void LayoutEditorCore::update(float elapsedTime)
{
	processKey();
}

Layout* LayoutEditorCore::createLayout(const std::string& layoutName)
{
	if (isLayoutModified())
	{
		if (!askSaveLayout("�ļ�δ����,�Ƿ񱣴�Ȼ���½��ļ�?"))
		{
			return NULL;
		}
	}

	if (mLayoutManager->getLayout(layoutName) != NULL)
	{
		EDITOR_CORE_ERROR("error : there is a layout named : %s, can not create again!", layoutName.c_str());
		return NULL;
	}
	if (mCurLayout != NULL)
	{
		closeLayout();
	}
	mCurLayout = mLayoutManager->createLayout(-1, layoutName, 0);
	mLayoutManager->setOnlyHoverLayoutName(mCurLayout->getName());
	setLayoutModified(true);
	// ���layout�����ɹ�,����layout�ı���¼�
	if (mCurLayout != NULL)
	{
		mEditorCoreRoot->sendEvent(CE_LAYOUT_ADDED, mCurLayout->getName());
		setLayoutVisible(mCurLayout, true);
	}
	return mCurLayout;
}

Layout* LayoutEditorCore::loadLayout(const std::string& fileName)
{
	if (isLayoutModified())
	{
		if (!askSaveLayout("��ǰ�ļ�δ����,�Ƿ񱣴��ļ�Ȼ����µ��ļ�?"))
		{
			return NULL;
		}
	}

	if (mCurLayout != NULL)
	{
		closeLayout();
	}
	std::string layoutName = fileName.substr(0, fileName.find_last_of('.'));
	mCurLayout = mLayoutManager->createLayout(-1, layoutName, 0, fileName);
	mLayoutManager->setOnlyHoverLayoutName(mCurLayout->getName());
	// ǿ�Ʒ��Ͳ��ָı���¼�,�Ա����ڼ��ز���ʱ��ʾ������޸Ĵ��ڱ���
	setLayoutModified(false, true);
	// ���layout���سɹ�,����layout�ı���¼�
	if (mCurLayout != NULL)
	{
		// ǿ����ʾ�ò���
		setLayoutVisible(mCurLayout, true);
		mEditorCoreRoot->sendEvent(CE_LAYOUT_ADDED, layoutName);
	}
	return mCurLayout;
}

void LayoutEditorCore::closeLayout(const std::string& layoutName)
{
	closeLayout(mLayoutManager->getLayout(layoutName));
}

void LayoutEditorCore::closeLayout(Layout* layout)
{
	if (isLayoutModified())
	{
		if (!askSaveLayout("��ǰ�ļ�δ����,�Ƿ񱣴�Ȼ��ر��ļ�?"))
		{
			return;
		}
	}
	if (layout == NULL)
	{
		return;
	}
	// ����رյ�ǰlayoutʱ��Ҫ��ѡ�������
	if (layout == mCurLayout)
	{
		clearSelection();
		mCurLayout = NULL;
		// �رյ�ǰ���ֺ����ò����޸�״̬Ϊδ�޸�
		setLayoutModified(false);
		mLayoutManager->clearActiveHoverWindow();
	}
	// ���ز���
	setLayoutVisible(layout, false);
	// ����layout�Ѿ����رյ��¼�
	mEditorCoreRoot->sendEvent(CE_LAYOUT_CLOSED, layout->getName());

	// �رղ���ʱ������г�������
	mUndoManager->clearUndo();
	mUndoManager->clearRedo();
}

void LayoutEditorCore::destroyLayout(const std::string& layoutName)
{
	destroyLayout(mLayoutManager->getLayout(layoutName));
}

void LayoutEditorCore::destroyLayout(Layout* layout)
{
	if (mCurLayout == layout && isLayoutModified())
	{
		if (!askSaveLayout("��ǰ�ļ�δ����,�Ƿ񱣴��ļ�Ȼ�������ļ�?"))
		{
			return;
		}
	}
	if (layout == NULL)
	{
		return;
	}
	// �رղ���
	closeLayout(layout);
	// ��������ٵĿ���Դ�����Ĳ���,����Ҫ���Դ����ָ��
	if (mCopySrcLayout == layout)
	{
		mCopySrcLayout = NULL;
	}
	// ���ٲ���,�˴�����ʹ������,��Ϊ���滹Ҫʹ�øñ�����ֵ
	std::string layoutName = layout->getName();
	mLayoutManager->destroyLayout(layoutName);
	// ����layout�Ѿ������ٵ��¼�
	mEditorCoreRoot->sendEvent(CE_LAYOUT_DESTROIED, layoutName);
}

void LayoutEditorCore::saveLayout()
{
	if (mCurLayout == NULL || !mLayoutModified)
	{
		return;
	}
	std::string layoutStream;
	mCurLayout->saveLayout(layoutStream);
	std::string filePath = P_UI_LAYOUT_PATH + mCurLayout->getName() + ".json";
	txFileUtility::writeFile(filePath, layoutStream.length(), layoutStream.c_str());
	setLayoutModified(false);
}

void LayoutEditorCore::editPrefab(const std::string& prefabName)
{
	if (isLayoutModified())
	{
		if (!askSaveLayout("�ļ�δ����, �Ƿ񱣴��ļ�Ȼ�󴴽�Ԥ�貼��?"))
		{
			return;
		}
	}
	// �رյ�ǰ����
	if (mCurLayout != NULL)
	{
		closeLayout();
	}
	// ����Ԥ�貼��,��������
	Layout* prefabEditLayout = createLayout(prefabName + "_layout");
	if (prefabEditLayout == NULL)
	{
		return;
	}
	// ���Ĭ�ϸ�����
	const std::string& windowTypeName = LayoutUtility::getTypeNameFromWindowType(WT_WINDOW);
	Window* rootWindow = addWindow(prefabEditLayout, windowTypeName, prefabName);
	// Ȼ��Ԥ��ʵ��������������
	WindowPrefab* prefab = mWindowPrefabManager->getPrefab(prefabName);
	CommandWindowSetProperty* cmdProperty = NEW_CMD(cmdProperty);
	cmdProperty->mPropertyName = txProperty::getPropertyName<WindowPropertyPrefab>();
	cmdProperty->mPropertyValue = prefabName;
	cmdProperty->mOperator = this;
	mCommandSystem->pushCommand(cmdProperty, rootWindow);
	// ʵ����Ԥ�����뽫Ԥ������ȥ��
	rootWindow->setPrefabName(EMPTY_STRING);
	// ��Ԥ������ڵ����Կ�������ǰ���ָ�������
	prefab->copyProperty(rootWindow);
	// ʵ�������,�޸����д��ڵ�����,ȥ�������ڵ�ǰ׺,�����ڸ���ʱ���޸Ĳ����еĴ����б�,������Ҫ����һ���б�
	auto windowList = prefabEditLayout->getWindowList();
	auto iterWindow = windowList.begin();
	auto iterWindowEnd = windowList.end();
	FOR_STL(windowList, ; iterWindow != iterWindowEnd; ++iterWindow)
	{
		if (iterWindow->second != rootWindow)
		{
			CommandWindowRename* cmdRename = NEW_CMD(cmdRename);
			std::string windowName = iterWindow->second->getName();
			if (txStringUtility::startWith(windowName, prefabName))
			{
				windowName = windowName.substr(prefabName.length(), windowName.length() - prefabName.length());
			}
			cmdRename->mNewName = windowName;
			mCommandSystem->pushCommand(cmdRename, iterWindow->second);
		}
	}
	END_FOR_STL(windowList);
}

void LayoutEditorCore::savePrefab(const std::string& windowName)
{
	if (mCurLayout == NULL)
	{
		return;
	}
	Window* window = mCurLayout->getWindow(windowName);
	if (window == NULL)
	{
		EDITOR_CORE_ERROR("error : can not find source window!");
		return;
	}
	if (window->getType() != WT_WINDOW)
	{
		EDITOR_CORE_ERROR("error : prefab must create from a base window! can not create from a %s window", window->getTypeName().c_str());
		return;
	}
	if (window->hasLinkedPrefab())
	{
		EDITOR_CORE_ERROR("error : window has link a prefab! can not create prefab from this window!");
		return;
	}
	// ����Ѿ�����ͬ��Ԥ��,��������ԭ�е�Ԥ��
	WindowPrefab* existPrefab = mWindowPrefabManager->getPrefab(windowName);
	if (existPrefab != NULL)
	{
		int ret = mEditorCoreRoot->showModalYesNo("�Ƿ񸲸����е�Ԥ��?");
		if (ret == MDR_YES)
		{
			mWindowPrefabManager->destroyPrefab(existPrefab);
		}
		else
		{
			return;
		}
	}
	WindowPrefab* prefab = mWindowPrefabManager->createPrefab(window, windowName);
	if (prefab == NULL)
	{
		return;
	}
	std::string stream;
	prefab->savePrefab(stream);
	std::string filePath = P_UI_WINDOW_PREFAB_PATH + prefab->getName() + PREFAB_SUFFIX;
	txFileUtility::writeFile(filePath, stream.length(), stream.c_str());
}

void LayoutEditorCore::saveTemplate(const std::string& windowName)
{
	if (mCurLayout == NULL)
	{
		return;
	}
	Window* window = mCurLayout->getWindow(windowName);
	if (window == NULL)
	{
		return;
	}
	// ����Ѿ�����ͬ��ģ��,��������ԭ��ģ��
	Window* existTemplate = mWindowTemplateManager->getTemplate(windowName);
	if (existTemplate != NULL)
	{
		int ret = mEditorCoreRoot->showModalYesNo("�Ƿ񸲸����е�ģ��?");
		if (ret == MDR_YES)
		{
			mWindowTemplateManager->destroyTemplate(existTemplate);
		}
		else
		{
			return;
		}
	}
	Window* templateWindow = mWindowTemplateManager->createTemplate(window, windowName);
	const std::string& fileName = mWindowTemplateManager->getTemplateFileName(windowName);
	std::string stream;
	window->saveTemplate(stream);
	txFileUtility::writeFile(P_UI_WINDOW_TEMPLATE_PATH + fileName, stream.length(), stream.c_str());
}

void LayoutEditorCore::setLayoutVisible(Layout* layout, const bool& visible)
{
	if (layout == NULL || layout->isVisible() == visible)
	{
		return;
	}
	layout->setVisible(visible, true, EMPTY_STRING);
	txVector<std::string> params;
	params.push_back(layout->getName());
	params.push_back(txStringUtility::boolToString(visible));
	mEditorCoreRoot->sendEvent(CE_LAYOUT_VISIBLE_CHANGED, params);

	ECUndoLayoutVisible* undo = mUndoManager->createUndo<ECUndoLayoutVisible>();
	undo->setLayout(layout);
	undo->setVisible(!visible);
	mUndoManager->addUndo(undo);
}

void LayoutEditorCore::setLayoutVisible(const std::string& layoutName, const bool& visible)
{
	setLayoutVisible(mLayoutManager->getLayout(layoutName), visible);
}

void LayoutEditorCore::setWireframeVisible(const bool& visible)
{
	if (mWireframeVisible == visible)
	{
		return;
	}
	mWireframeVisible = visible;
	// ���õ�ǰѡ������߿���ʾ״̬
	int selectionCount = mMultiSelections.size();
	FOR_STL(mMultiSelections, int i = 0; i < selectionCount; ++i)
	{
		mMultiSelections[i]->setShowWireframe(mWireframeVisible);
	}
	END_FOR_STL(mMultiSelections);
}

void LayoutEditorCore::setSelectionWindowCoordinateVisible(const bool& visible)
{
	if (mCoordinateVisible == visible)
	{
		return;
	}
	mCoordinateVisible = visible;
	// ���õ�ǰѡ����ı�������ϵ��ʾ״̬
	int selectionCount = mMultiSelections.size();
	FOR_STL(mMultiSelections, int i = 0; i < selectionCount; ++i)
	{
		mMultiSelections[i]->setShowCoordinate(mCoordinateVisible);
	}
	END_FOR_STL(mMultiSelections);
}

void LayoutEditorCore::setRenderWindowCoordinateVisible(const bool& visible)
{
	mRenderWindowCoordinate->setVisible(visible);
}

const bool& LayoutEditorCore::getRenderWindowCoordinateVisible()
{
	return mRenderWindowCoordinate->isVisible();
}

void LayoutEditorCore::setScreenDensity(const float& density)
{
	mRenderWindow->setScreenDensity(density);
	// ֪ͨˢ�����д���
	mLayoutManager->notifyRenderWindowChanged();
}

void LayoutEditorCore::pickupAllResources(const std::string& targetPath)
{
	if (isLayoutModified())
	{
		if (!askSaveLayout("�ļ�δ����,�Ƿ񱣴��ļ�Ȼ��ʼ��ȡ������Դ?"))
		{
			return;
		}
	}
	// ���Ŀ��Ŀ¼������,����Ŀ��Ŀ¼
	txFileUtility::createFolder(targetPath);

	// ���ص�ǰĿ¼�е����в���
	txVector<std::string> layoutFiles;
	txFileUtility::findFiles(P_UI_LAYOUT_PATH, layoutFiles, ".json");
	int fileCount = layoutFiles.size();
	FOR_STL(layoutFiles, int i = 0; i < fileCount; ++i)
	{
		std::string fileName = txStringUtility::getFileName(layoutFiles[i]);
		// ����layout,���һ�����е�����ʹ�õ���Դ�������б�,Ȼ������layout
		Layout* layout = loadLayout(fileName);
		txVector<std::string> resourceList;
		layout->getUsedResourcesList(resourceList);
		destroyLayout(layout);

		// ��������ʹ�õ���Դ������Ŀ��Ŀ¼��
		int resourceCount = resourceList.size();
		FOR_STL(resourceList, int j = 0; j < resourceCount; ++j)
		{
			txFileUtility::copyFile(resourceList[j], targetPath + "/" + resourceList[j]);
		}
		END_FOR_STL(resourceList);
	}
	END_FOR_STL(layoutFiles);
	mEditorCoreRoot->logInfo("��Դ��ȡ���!");
}

void LayoutEditorCore::moveSelectedWindow(const VECTOR2& moveDelta)
{
	int selectCount = mMultiSelections.size();
	FOR_STL(mMultiSelections, int i = 0; i < selectCount; ++i)
	{
		txPoint windowPos = mMultiSelections[i]->getPosition();
		windowPos.x.mAbsolute += (int)moveDelta.x;
		windowPos.y.mAbsolute += (int)moveDelta.y;
		mMultiSelections[i]->setPosition(windowPos);
	}
	END_FOR_STL(mMultiSelections);
}

void LayoutEditorCore::setLayoutModified(const bool& modified, const bool& force, const bool& notifyEvent)
{
	if (modified != mLayoutModified || force)
	{
		mLayoutModified = modified;
		if (notifyEvent)
		{
			// �����޸�״̬�ı���¼�
			txVector<std::string> params;
			params.push_back(mCurLayout ? mCurLayout->getName() : EMPTY_STRING);
			params.push_back(txStringUtility::boolToString(mLayoutModified));
			mEditorCoreRoot->sendEvent(CE_LAYOUT_MODIFIED, params);
		}
	}
}

void LayoutEditorCore::deleteWindow(const std::string& name, const std::string& layoutName)
{
	Layout* layout = layoutName != EMPTY_STRING ? mLayoutManager->getLayout(layoutName) : mCurLayout;
	Window* window = layout->getWindow(name);
	if (NULL == window)
	{
		return;
	}

	// �ڴ��ڱ�ɾ��֮ǰ���ɾ�����ڵĳ�������
	ECUndoDeleteWindow* undo = mUndoManager->createUndo<ECUndoDeleteWindow>();
	undo->setWindow(window);
	mUndoManager->addUndo(undo);

	clearSelection();

	Window* parent = window->getParent();
	std::string parentName = EMPTY_STRING;
	if (NULL != parent)
	{
		parentName = parent->getName();
	}
	layout->deleteWindow(name);
	txVector<std::string> params;
	params.push_back(parentName);
	params.push_back(name);
	mEditorCoreRoot->sendEvent(CE_WINDOW_DELETED, params);

	// ��¼�����иı�
	setLayoutModified(true);
}

Window* LayoutEditorCore::addWindow(Layout* parent, const std::string& typeName, const std::string& windowName, const int& childPos)
{
	if (parent == NULL)
	{
		EDITOR_CORE_ERROR("Layout����Ϊ��!");
		return NULL;
	}
	else if (parent->getRootWindow() != NULL)
	{
		EDITOR_CORE_ERROR("Layoutֻ����һ��������,��ѡ��������������Ӵ���");
		return NULL;
	}
	Window* window = parent->createWindow(typeName, windowName);
	addWindow(parent, NULL, window, childPos);
	return window;
}

Window* LayoutEditorCore::addWindow(Window* parent, const std::string& typeName, const std::string& windowName, const int& childPos)
{
	Layout* layout = parent->getLayout();
	Window* windowNew = layout->createWindow(typeName, windowName);
	addWindow(layout, parent, windowNew, childPos);
	return windowNew;
}

void LayoutEditorCore::addWindow(Layout* layout, Window* parent, Window* child, const int& childPos)
{
	if (layout == NULL || child == NULL)
	{
		return;
	}

	// �Ƚ��Ӵ�����ԭ���Ĳ��ֺ͸����ڵ���ϵ�Ͽ�
	child->detachLayoutParent();
	// ���½����벼�ֺ͸����ڵ���ϵ
	child->attachLayoutParent(layout, parent, childPos);

	// ��������Ӵ��ڵ��¼�
	txVector<std::string> params;
	params.push_back(parent != NULL ? parent->getName() : EMPTY_STRING);
	params.push_back(child->getName());
	params.push_back(txStringUtility::intToString(childPos));
	mEditorCoreRoot->sendEvent(CE_WINDOW_ADDED, params);

	// ��¼�����иı�
	setLayoutModified(true);

	// ������Ӵ��ڵĳ�������
	ECUndoAddWindow* undo = mUndoManager->createUndo<ECUndoAddWindow>();
	undo->setWindowName(child->getName());
	undo->setLayoutName(layout != NULL ? layout->getName() : EMPTY_STRING);
	mUndoManager->addUndo(undo);
}

void LayoutEditorCore::setSelection(Window* selection)
{
	// �����ظ�����ѡ����,��������ѭ���ĵݹ����
	if (mMultiSelections.size() == 1 && mMultiSelections[0] == selection)
	{
		return;
	}

	clearSelection(false);
	if (selection != NULL)
	{
		mMultiSelSet.insert(selection);
		mMultiSelections.push_back(selection);
	}

	notifySelectionChanged();
}

void LayoutEditorCore::setSelection(txVector<Window*>& selection)
{
	bool sameSelection = true;
	// �����ظ�����ѡ����,��������ѭ���ĵݹ����
	if (mMultiSelections.size() == selection.size())
	{
		int selectionCount = mMultiSelections.size();
		FOR_STL(mMultiSelections, int i = 0; i < selectionCount; ++i)
		{
			if (mMultiSelections[i] != selection[i])
			{
				sameSelection = false;
				break;
			}
		}
		END_FOR_STL(mMultiSelections);
	}
	else
	{
		sameSelection = false;
	}
	if (sameSelection)
	{
		return;
	}

	clearSelection(false);
	int selCount = selection.size();
	FOR_STL(selection, int i = 0; i < selCount; ++i)
	{
		auto iter = mMultiSelSet.find(selection[i]);
		if (iter == mMultiSelSet.end())
		{
			mMultiSelections.push_back(selection[i]);
			mMultiSelSet.insert(selection[i]);
		}
	}
	END_FOR_STL(selection);

	notifySelectionChanged();
}

void LayoutEditorCore::reselect()
{
	mEditorCoreRoot->sendEvent(CE_SELECTION_CHANGED);
}

void LayoutEditorCore::clearSelection(const bool& sendEvt)
{
	// ��մ���ѡ����ʱҲ��Ҫ������ѡ����
	mComponentEditorCore->clearComponentSelection(sendEvt);
	// �Ƚ���ǰ����ѡ������߿�����
	if (mWireframeVisible)
	{
		int selectWindowNumber = mMultiSelections.size();
		FOR_STL(mMultiSelections, int i = 0; i < selectWindowNumber; ++i)
		{
			mMultiSelections[i]->setShowWireframe(false);
		}
		END_FOR_STL(mMultiSelections);
	}
	if (mCoordinateVisible)
	{
		int selectWindowNumber = mMultiSelections.size();
		FOR_STL(mMultiSelections, int i = 0; i < selectWindowNumber; ++i)
		{
			mMultiSelections[i]->setShowCoordinate(false);
		}
		END_FOR_STL(mMultiSelections);
	}
	mMultiSelSet.clear();
	mMultiSelections.clear();
	notifySelectionChanged(sendEvt);
}

void LayoutEditorCore::copyWindow(const std::string& name, const bool& copyChild, const bool& cutWindow)
{
	if (mCurLayout == NULL)
	{
		return;
	}
	Window* window = mCurLayout->getWindow(name);
	if (window == NULL)
	{
		return;
	}
	if (mCopyWindow != NULL)
	{
		mLayoutManager->destroyWindow(mCopyWindow);
		mCopyWindow = NULL;
	}
	mCopySrcLayout = mCurLayout;
	if (cutWindow)
	{
		mCutWindowName = name;
	}
	else
	{
		mCopyWindow = Layout::createIndependentWindow(window, name);
		// ֻ��û������Ԥ��ʱ���ܿ����Ӵ���
		if (copyChild && mCopyWindow->getPrefabName() == EMPTY_STRING)
		{
			// ���ڿ��������Ĵ��ڲ������κβ���,�����Ӵ������ֲ����κ�ǰ׺
			window->copyChildren(mCopyWindow, false, EMPTY_STRING);
		}
	}
	mCutWindow = cutWindow;
}

void LayoutEditorCore::pasteWindow(const std::string& destParent, const int& pos)
{
	if (mCurLayout == NULL)
	{
		return;
	}
	Window* destWindow = mCurLayout->getWindow(destParent);
	if (destWindow == NULL)
	{
		mEditorCoreRoot->logInfo("�Ҳ���������Ŀ�괰��");
		return;
	}
	if (mCutWindow)
	{
		if (mCopySrcLayout != mCurLayout)
		{
			mEditorCoreRoot->logInfo("���в���ֻ����ͬһ�����н��е�");
			return;
		}
		if (mCutWindowName == destParent)
		{
			mEditorCoreRoot->logInfo("���е���Ŀ�괰�ڲ�����Դ����");
			return;
		}
		Window* cutWindow = mCopySrcLayout->getWindow(mCutWindowName);
		if (cutWindow == NULL)
		{
			return;
		}
		if (cutWindow->isChild(destWindow))
		{
			mEditorCoreRoot->logInfo("���е���Ŀ�괰�ڲ�����Դ���ڵ��Ӵ���");
			return;
		}
		const std::string& oldParent = cutWindow->getParent()->getName();
		int oldPos = cutWindow->getParent()->getChildPos(cutWindow);
		// �������븸���ڵ����ӶϿ�
		cutWindow->detachLayoutParent();
		// �ҽӵ��µĸ�������
		cutWindow->attachLayoutParent(mCurLayout, destWindow, pos);

		// ���ͼ��д��ڵ��¼�
		mEditorCoreRoot->sendEvent(CE_WINDOW_CUTED, mCutWindowName);

		// ������Ӵ��ڵĳ�������
		ECUndoCutWindow* undo = mUndoManager->createUndo<ECUndoCutWindow>();
		undo->setCutWindowInfo(mCutWindowName, oldParent, oldPos);
		mUndoManager->addUndo(undo);

		// ��ǲ������޸�
		setLayoutModified(true);
	}
	else
	{
		if (mCopyWindow == NULL)
		{
			mEditorCoreRoot->logInfo("�Ҳ���������Դ����");
			return;
		}
		// ���ݿ����Ĵ����´���һ������,���Զ������Կ������½�������
		Window* newWindow = Layout::createIndependentWindow(mCopyWindow, EMPTY_STRING);
		// û������Ԥ��ʱ,�ٽ����е��Ӵ��ڿ������´����Ĵ�����
		if (mCopyWindow->getPrefabName() == EMPTY_STRING)
		{
			mCopyWindow->copyChildren(newWindow, false, newWindow->getName());
		}
		// ���´����Ĵ�����ӵ�Ŀ�괰������
		addWindow(mCurLayout, destWindow, newWindow, pos);
	}
}

bool LayoutEditorCore::renameLayout(const std::string& newName)
{
	if (mCurLayout != NULL)
	{
		// �˴�����ʹ�ó�������,��Ϊ��ʱ��ı�����,����oldName��newName��ͬ
		std::string oldName = mCurLayout->getName();
		bool ret = mCurLayout->rename(newName);
		// �����ɹ�
		if (ret)
		{
			// ���ò��ֹ���������Ҫ��������ͣ�Ĳ�����
			mLayoutManager->setOnlyHoverLayoutName(mCurLayout->getName());

			// ���Ͳ��ָ������¼�
			txVector<std::string> params;
			params.push_back(oldName);
			params.push_back(newName);
			mEditorCoreRoot->sendEvent(CE_LAYOUT_RENAMED, params);

			// ��¼�����иı�
			setLayoutModified(true);
		}
		else
		{
			mEditorCoreRoot->logInfo("���ָ���ʧ��,�Ѵ���ͬ�����ֻ��벼���д�������");
		}
		return ret;
	}
	return false;
}

void LayoutEditorCore::moveWindowPos(Window* window, const int& destPos)
{
	if (window == NULL || destPos < 0)
	{
		return;
	}

	Window* parent = window->getParent();
	if (NULL != parent)
	{
		int oldPos = parent->getChildPos(window);
		if (parent->moveChildPos(window, destPos))
		{
			// ���ʹ���λ��˳�����ı�
			txVector<std::string> params;
			params.push_back(window->getName());
			params.push_back(parent->getName());
			params.push_back(txStringUtility::intToString(oldPos));
			params.push_back(txStringUtility::intToString(destPos));
			mEditorCoreRoot->sendEvent(CE_WINDOW_POS_CHANGED, params);

			// ��¼�����иı�
			setLayoutModified(true);

			// ��ӵ������б�
			ECUndoMoveWindowPos* movePosUndo = mUndoManager->createUndo<ECUndoMoveWindowPos>();
			if (NULL != movePosUndo)
			{
				movePosUndo->setPos(oldPos);
				movePosUndo->setWindowName(window->getName());
				mUndoManager->addUndo(movePosUndo);
			}
		}
	}
}

bool LayoutEditorCore::askSaveLayout(const std::string& info)
{
	MODAL_DIALOG_RESULT ret = mEditorCoreRoot->showModalYesNoCancel(info);
	// �����,�򱣴��ļ�
	if (ret == MDR_YES)
	{
		saveLayout();
	}
	// �����,���ǵ�ǰ����Ϊδ�޸�״̬
	else if (ret == MDR_NO)
	{
		setLayoutModified(false, false, false);
	}
	// ���ȡ���򲻴��ļ�
	else
	{
		return false;
	}
	return true;
}

void LayoutEditorCore::notifySelectionChanged(const bool& notifyEvent)
{
	if (notifyEvent)
	{
		mEditorCoreRoot->sendEvent(CE_SELECTION_CHANGED);
	}

	// �����Ҫ��ʾ�߿�,��������ʾ,����������
	if (mWireframeVisible)
	{
		int selectWindowNumber = mMultiSelections.size();
		FOR_STL(mMultiSelections, int i = 0; i < selectWindowNumber; ++i)
		{
			mMultiSelections[i]->setShowWireframe(true);
		}
		END_FOR_STL(mMultiSelections);
	}
	if (mCoordinateVisible)
	{
		int selectWindowNumber = mMultiSelections.size();
		FOR_STL(mMultiSelections, int i = 0; i < selectWindowNumber; ++i)
		{
			mMultiSelections[i]->setShowCoordinate(true);
		}
		END_FOR_STL(mMultiSelections);
	}
}

void LayoutEditorCore::notifyMouseEnterWindow(const std::string& windowName)
{
	if (mCurLayout != NULL)
	{
		Window* window = mCurLayout->getWindow(windowName);
		mPreWireframeColor = window->getWireframeColour();
		mPreCoordinateColor = window->getCoordinateColour();
		window->setShowCoordinate(mCoordinateVisible);
		window->setShowWireframe(mWireframeVisible);
		window->setWireframeColour(VECTOR4(1.0f, 0.0f, 1.0f, 1.0f));
		window->setCoordinateColour(VECTOR4(1.0f, 0.0f, 1.0f, 1.0f));
	}
}

void LayoutEditorCore::notifyMouseLeaveWindow(const std::string& windowName)
{
	if (mCurLayout != NULL)
	{
		Window* window = mCurLayout->getWindow(windowName);
		window->setWireframeColour(mPreWireframeColor);
		window->setCoordinateColour(mPreCoordinateColor);
		auto iter = mMultiSelSet.find(window);
		if (iter == mMultiSelSet.end())
		{
			window->setShowWireframe(false);
			window->setShowCoordinate(false);
		}
	}
}

void LayoutEditorCore::processKey()
{
	float moveSpeed = 0.0f;
	if (mInputSystem->isKeyDown(VK_SHIFT))
	{
		moveSpeed = 1.0f;
	}
	else if (mInputSystem->isKeyDown(VK_CONTROL))
	{
		moveSpeed = 10.0f;
	}
	if (moveSpeed > 0.0f)
	{
		// �������Ҽ��ƶ�����
		if (mInputSystem->isKeyDown(VK_UP))
		{
			moveSelectedWindow(VECTOR2(0.0f, 1.0f) * moveSpeed);
		}
		else if (mInputSystem->isKeyDown(VK_DOWN))
		{
			moveSelectedWindow(VECTOR2(0.0f, -1.0f) * moveSpeed);
		}
		else if (mInputSystem->isKeyDown(VK_LEFT))
		{
			moveSelectedWindow(VECTOR2(-1.0f, 0.0f) * moveSpeed);
		}
		else if (mInputSystem->isKeyDown(VK_RIGHT))
		{
			moveSelectedWindow(VECTOR2(1.0f, 0.0f) * moveSpeed);
		}
	}
}