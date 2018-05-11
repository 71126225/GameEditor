#include "WindowHeader.h"
#include "LayoutManager.h"
#include "Layout.h"
#include "WindowProperty.h"

ComplexWindow::ComplexWindow(const std::string& name, const WINDOW_TYPE& type)
	:
	Window(name, type)
{
	mIsComplexWindow = true;
}
ComplexWindow::~ComplexWindow()
{
	destroy();
}
void ComplexWindow::render()
{
	// ����Ⱦ�ڲ��Ӵ���
	int innerChildCount = mInnerChildList.size();
	FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
	{
		if (mInnerChildList[i]->isVisible())
		{
			mInnerChildList[i]->render();
		}
	}
	END_FOR_STL(mInnerChildList);
	// Ȼ����Ⱦ�Ӵ���
	Window::render();
}
void ComplexWindow::update(const float& elapsedTime)
{
	if (!isVisible())
	{
		return;
	}
	// �ȵ��û���ĸ���
	Window::update(elapsedTime);
	// �ٸ����ڲ��Ӵ���
	int innerChildCount = mInnerChildList.size();
	FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
	{
		mInnerChildList[i]->update(elapsedTime);
	}
	END_FOR_STL(mInnerChildList);
}
void ComplexWindow::destroy()
{
	int innerChildCount = mInnerChildList.size();
	FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
	{
		mInnerChildList[i]->notifyParentDestroied();
	}
	END_FOR_STL(mInnerChildList);

	mInnerChildList.clear();
	mInnerChildMap.clear();
}
void ComplexWindow::copyProperty(Window* window)
{
	// �ȵ��û��ຯ��
	Window::copyProperty(window);
	// ����Ҫ���Լ����е��ڲ��Ӵ��ڶ�������Ŀ�괰����
	ComplexWindow* destComplexWindow = static_cast<ComplexWindow*>(window);
	// �����Ĵ���һ����ͬ���͵Ĵ���,��ô�Ӵ��ڵ�˳��Ҳ��ͬ
	int innerChildCount = mInnerChildList.size();
	FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
	{
		mInnerChildList[i]->copyProperty(destComplexWindow->mInnerChildList[i]);
		copyChildProperty(mInnerChildList[i], destComplexWindow->mInnerChildList[i]);
	}
	END_FOR_STL(mInnerChildList);
}
bool ComplexWindow::rename(const std::string& newName)
{
	if (!Window::rename(newName))
	{
		return false;
	}
	// ���Լ��ڲ��Ӵ��ڵ�����Ҳһ���޸�
	renameInnerChildren(mName);
	return true;
}
Window* ComplexWindow::getChild(const std::string& name)
{
	// �Ȼ����ͨ���Ӵ���
	Window* child = Window::getChild(name);
	// ����Ҳ���,����ҵ�һ�����ڲ��Ӵ���
	if (child != NULL)
	{
		return child;
	}
	else
	{
		auto iterInner = mInnerChildMap.find(name);
		if (iterInner != mInnerChildMap.end())
		{
			return iterInner->second;
		}
	}
	return NULL;
}
void ComplexWindow::detachLayoutParent(const bool& detachLayoutOnly)
{
	// �ȵ��û���ĺ���
	Window::detachLayoutParent(detachLayoutOnly);
	// ʹ�Լ����е��Ӵ��ڶ��Ͽ��벼�ֵ���ϵ,���ǲ��ܴ���Ӵ��ڵĸ��ӹ�ϵ
	int innerChildCount = mInnerChildList.size();
	FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
	{
		mInnerChildList[i]->detachLayoutParent(true);
	}
	END_FOR_STL(mInnerChildList);
}
// �����벼�ֺ͸����ڵ���ϵ,ʹ�ô��ڳ�Ϊ�����е�һ������,�ô����µ������Ӵ���Ҳ���ؽ��벼�ֵ���ϵ,���ӹ�ϵ��Ȼ����
void ComplexWindow::attachLayoutParent(Layout* layout, Window* parent, const int& childPos)
{
	// �ȵ��û���ĺ���
	Window::attachLayoutParent(layout, parent, childPos);
	// ʹ�Լ����е��Ӵ��ڶ������벼�ֵ���ϵ
	int innerChildCount = mInnerChildList.size();
	FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
	{
		mInnerChildList[i]->attachLayoutParent(layout, NULL, -1);
	}
	END_FOR_STL(mInnerChildList);
}
void ComplexWindow::setDirty(const bool& dirty, const bool& refreshNow)
{
	Window::setDirty(dirty, refreshNow);
	if (dirty)
	{
		int innerChildCount = mInnerChildList.size();
		FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
		{
			mInnerChildList[i]->setDirty(dirty, refreshNow);
		}
		END_FOR_STL(mInnerChildList);
	}
}
void ComplexWindow::getUsedResourceList(txVector<std::string>& resourceList)
{
	int innerChildCount = mInnerChildList.size();
	FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
	{
		mInnerChildList[i]->getUsedResourceList(resourceList);
	}
	END_FOR_STL(mInnerChildList);
}
void ComplexWindow::removeChild(const std::string& name)
{
	Window::removeChild(name);

	Window* window = NULL;
	auto iterMap = mInnerChildMap.find(name);
	if (iterMap != mInnerChildMap.end())
	{
		window = iterMap->second;
		mInnerChildMap.erase(iterMap);
	}

	if (window == NULL)
	{
		return;
	}

	auto iter = mInnerChildList.begin();
	auto iterEnd = mInnerChildList.end();
	FOR_STL(mInnerChildList, ; iter != iterEnd; ++iter)
	{
		if (*iter == window)
		{
			mInnerChildList.erase(iter, false);
			break;
		}
	}
	END_FOR_STL(mInnerChildList);
}

void ComplexWindow::notifyChildNameChanged(const std::string& oldName, Window* window)
{
	Window::notifyChildNameChanged(oldName, window);

	// ���ж��ڲ��Ӵ��ڸ���ʧ�ܵ����
	auto iterMap = mInnerChildMap.find(oldName);
	if (iterMap != mInnerChildMap.end())
	{
		mInnerChildMap.erase(iterMap);
		mInnerChildMap.insert(window->getName(), window);
	}
}

void ComplexWindow::notifyInnerChildDestroied(const std::string& name)
{
	Window* window = NULL;
	auto iterMap = mInnerChildMap.find(name);
	if (iterMap != mInnerChildMap.end())
	{
		window = iterMap->second;
		mInnerChildMap.erase(iterMap);
	}
	// ������Ҳ����Ӵ���,���ʾ�����˵Ĵ��ڲ����ǵ�ǰ���ϴ��ڵĵ�һ���Ӵ���
	// Ҳ�п�������Ϊ��removeChild���Ѿ����Ӵ��ڴ��б����Ƴ���
	if (window == NULL)
	{
		return;
	}
	int innerChildCount = mInnerChildList.size();
	FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
	{
		if (mInnerChildList[i] == window)
		{
			mInnerChildList.erase(mInnerChildList.begin() + i, false);
			break;
		}
	}
	END_FOR_STL(mInnerChildList);
}

void ComplexWindow::saveWindow(std::string& windowStream)
{
	// ����������д���,�ȱ��游����,Ȼ���ٱ��������Ӵ���
	txStringUtility::jsonStartStruct(windowStream, 1, true);
	txStringUtility::jsonAddPair(windowStream, TYPE_PROPERTY_NAME, mTypeName, 2, true);
	txStringUtility::jsonAddPair(windowStream, NAME_PROPERTY_NAME, mName, 2, true);
	if (mParent != NULL)
	{
		txStringUtility::jsonAddPair(windowStream, PARENT_PROPERTY_NAME, mParent->getName(), 2, true);
	}
	// �����Լ�����������
	auto iterProperty = mPropertyMap.begin();
	auto iterPropertyEnd = mPropertyMap.end();
	int propertyCount = mPropertyMap.size();
	FOR_STL(mPropertyMap, ; iterProperty != iterPropertyEnd; ++iterProperty)
	{
		txStringUtility::jsonAddPair(windowStream, iterProperty->second->getName(), iterProperty->second->get(this), 2, true);
	}
	END_FOR_STL(mPropertyMap);
	txStringUtility::jsonEndStruct(windowStream, 1, true);
	// �����Լ����ڲ��Ӵ���
	int innerChildCount = mInnerChildList.size();
	FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
	{
		mInnerChildList[i]->saveWindow(windowStream);
	}
	END_FOR_STL(mInnerChildList);
	// ���û�й����κ�Ԥ��,�򱣴��Ӵ�����Ϣ
	if (mPrefabName == EMPTY_STRING)
	{
		int childCount = mChildren.size();
		FOR_STL(mChildren, int i = 0; i < childCount; ++i)
		{
			mChildren[i]->saveWindow(windowStream);
		}
		END_FOR_STL(mChildren);
	}
}

void ComplexWindow::saveTemplate(std::string& windowStream)
{
	// ����������д���,�ȱ��游����,Ȼ���ٱ��������Ӵ���
	txStringUtility::jsonStartArray(windowStream, 0, true);
	txStringUtility::jsonStartStruct(windowStream, 1, true);
	txStringUtility::jsonAddPair(windowStream, TYPE_PROPERTY_NAME, mTypeName, 2, true);
	txStringUtility::jsonAddPair(windowStream, NAME_PROPERTY_NAME, mName, 2, true);
	// �����Լ�����������
	auto iterProperty = mPropertyMap.begin();
	auto iterPropertyEnd = mPropertyMap.end();
	int propertyCount = mPropertyMap.size();
	FOR_STL(mPropertyMap, ; iterProperty != iterPropertyEnd; ++iterProperty)
	{
		txStringUtility::jsonAddPair(windowStream, iterProperty->second->getName(), iterProperty->second->get(this), 2, true);
	}
	END_FOR_STL(mPropertyMap);
	txStringUtility::jsonEndStruct(windowStream, 1, true);
	// �����Լ����ڲ��Ӵ���
	int innerChildCount = mInnerChildList.size();
	FOR_STL(mInnerChildList, int i = 0; i < innerChildCount; ++i)
	{
		mInnerChildList[i]->saveWindow(windowStream);
	}
	END_FOR_STL(mInnerChildList);
	txStringUtility::jsonEndArray(windowStream, 0, true);
}

Window* ComplexWindow::createInnerChild(const std::string& windowType, const std::string& name)
{
	Window* window = NULL;
	if (mParentLayout != NULL)
	{
		window = mParentLayout->createWindow(windowType, name);
	}
	else
	{
		window = Layout::createIndependentWindow(windowType, name);
	}
	// ���Ϊ�ڲ��Ӵ���,���ұ�����������¼�
	window->setInnerChild(true);
	window->setEnableHandleInput(false);
	window->setComplexParent(this);
	return window;
}
void ComplexWindow::copyChildProperty(Window* sourceWindow, Window* destWindow)
{
	// �����Ӵ���
	auto& destChildren = destWindow->getChildren();
	auto& sourceChildren = sourceWindow->getChildren();
	int childCount = sourceChildren.size();
	FOR_STL(sourceChildren, int i = 0; i < childCount; ++i)
	{
		// �ȿ����Լ�������
		sourceChildren[i]->copyProperty(destChildren[i]);
		// Ȼ���ٿ����Ӵ��ڵ�����
		copyChildProperty(sourceChildren[i], destChildren[i]);
	}
	END_FOR_STL(sourceChildren);
}
void ComplexWindow::addInnerChild(Window* child)
{
	child->setParent(this);
	mInnerChildMap.insert(child->getName(), child);
	mInnerChildList.push_back(child);
	notifyChildAdded();
}