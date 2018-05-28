#include "SceneEditorCore.h"
#include "ECScene.h"
#include "CameraManager.h"
#include "txCamera.h"
#include "EditorCoreRoot.h"
#include "GameCamera.h"
#include "ECSceneManager.h"
#include "txUtility.h"
#include "ECUndoManager.h"
#include "ComponentEditorCore.h"
#include "ECAxis.h"
#include "ECGrid.h"

SceneEditorCore::SceneEditorCore()
:
mSceneModified(false),
mCurScene(NULL)
{
	;
}

SceneEditorCore::~SceneEditorCore()
{
	destroyScene();
}

void SceneEditorCore::update(const float& elapsedTime)
{
	if (mCurScene != NULL)
	{
		// ���³���
		mCurScene->update(elapsedTime);
	}
}

void SceneEditorCore::render()
{
	// ��Ⱦ��������
	if (mCurScene != NULL)
	{
		txCamera* engineCamera = mCameraManager->getMainCamera()->getCamera();
		mCurScene->renderHelper(engineCamera);
	}
}

void SceneEditorCore::setSceneModified(bool modified, bool force, bool notifyEvent)
{
	if (modified != mSceneModified || force)
	{
		mSceneModified = modified;
		if (notifyEvent)
		{
			// �����޸�״̬�ı���¼�
			txVector<std::string> params;
			params.push_back(mCurScene ? mCurScene->getName() : EMPTY_STRING);
			params.push_back(txStringUtility::boolToString(mSceneModified));
			mEditorCoreRoot->sendEvent(CE_SCENE_MODIFIED, params);
		}
	}
}

void SceneEditorCore::newScene(const std::string& newSceneName)
{
	if (isSceneModified())
	{
		if (!askSaveScene("�ļ�δ����,�Ƿ񱣴�Ȼ���½��ļ�?"))
		{
			return;
		}
	}

	// ���ȹرճ���
	destroyScene();
	// Ȼ�󴴽�����
	mCurScene = mEditorSceneManager->createScene(newSceneName);
	mCurScene->readSceneFile(true);

	// �����½��������¼�
	mEditorCoreRoot->sendEvent(CE_SCENE_NEW, newSceneName);

	setSceneModified(true, true);
}

void SceneEditorCore::openAdvanceScene(const std::string& sceneName)
{
	if (isSceneModified())
	{
		if (!askSaveScene("��ǰ�ļ�δ����,�Ƿ񱣴��ļ�Ȼ����µ��ļ�?"))
		{
			return;
		}
	}
	// ���ȹرճ���
	destroyScene();

	// Ȼ�󴴽�����
	mCurScene = mEditorSceneManager->createScene(sceneName);
	mCurScene->readAdvanceScene(false);

	// ���ʹ򿪳������¼�
	mEditorCoreRoot->sendEvent(CE_SCENE_OPEN, sceneName);

	// ǿ�Ʒ��Ͳ��ָı���¼�,�Ա����ڼ��ز���ʱ��ʾ������޸Ĵ��ڱ���
	setSceneModified(false, true);
}

void SceneEditorCore::saveAdvanceScene()
{
	if (mCurScene != NULL)
	{
		mCurScene->saveAdvanceScene();
	}
	setSceneModified(false);
}

void SceneEditorCore::openScene(const std::string& sceneName, const bool& binary)
{
	if (isSceneModified())
	{
		if (!askSaveScene("��ǰ�ļ�δ����,�Ƿ񱣴��ļ�Ȼ����µ��ļ�?"))
		{
			return;
		}
	}

	// ���ȹرճ���
	destroyScene();
	// Ȼ�󴴽�����
	mCurScene = mEditorSceneManager->createScene(sceneName);
	mCurScene->readSceneFile(true);

	// ���ʹ򿪳������¼�
	mEditorCoreRoot->sendEvent(CE_SCENE_OPEN, sceneName);

	// ǿ�Ʒ��Ͳ��ָı���¼�,�Ա����ڼ��ز���ʱ��ʾ������޸Ĵ��ڱ���
	setSceneModified(false, true);
}

bool SceneEditorCore::transSceneFile(const std::string& fullFileName, const bool& tranMeshFile)
{
	std::string newSceneName;
	return txLoadUtility::jsonSceneFileToBinarySceneFile(fullFileName, newSceneName, tranMeshFile);
}

bool SceneEditorCore::transSkinMeshFile(const std::string& fullFileName, const bool& transAllFile)
{
	// ��ȫ����\ת��Ϊ/
	std::string fileName = fullFileName;
	txStringUtility::rightToLeft(fileName);
	if (!transAllFile)
	{
		std::string newFileName;
		return txLoadUtility::jsonSkeletonMeshFileToBinarySkeletonMeshFile(fileName, newFileName);
	}
	else
	{
		// �����ļ�����Ŀ¼�е������ļ�,�ҳ���̬ģ���ļ�
		std::string filPath = txStringUtility::getFilePath(fileName);
		txVector<std::string> fileList;
		txFileUtility::findFiles(filPath, fileList, ".json");
		int fileCount = fileList.size();
		FOR_STL(fileList, int i = 0; i < fileCount; ++i)
		{
			const std::string& filename = fileList[i];
			// ȥ����_skeleton.json��β�Ĺ����ļ���_animation.json��β�Ķ����ļ�
			if (!txStringUtility::endWith(filename, "_skeleton.json") && !txStringUtility::endWith(filename, "_animation.json"))
			{
				std::string newFileName;
				if (!txLoadUtility::jsonSkeletonMeshFileToBinarySkeletonMeshFile(filename, newFileName))
				{
					END_FOR_STL(fileList);
					return false;
				}
			}
		}
		END_FOR_STL(fileList);
		return true;
	}
}

void SceneEditorCore::destroyScene(ECScene* scene)
{
	if (isSceneModified())
	{
		if (!askSaveScene("��ǰ�ļ�δ����,�Ƿ񱣴�Ȼ��ر��ļ�?"))
		{
			return;
		}
	}
	if (mCurScene == NULL)
	{
		return;
	}
	if (mCurScene == scene)
	{
		// �����������ѡ�е�����
		clearEditorSelection();
		mComponentEditorCore->clearComponentSelections();
		// �ȷ��͹رճ������¼�
		mEditorCoreRoot->sendEvent(CE_SCENE_DESTROIED, mCurScene->getName());
		// ��ǳ���δ�޸�
		setSceneModified(false);
		// ���ٳ���
		mEditorSceneManager->destroyScene(mCurScene->getName());
	}

	// ���ٳ���ʱ������г�������
	mUndoManager->clearRedo();
	mUndoManager->clearUndo();
}

void SceneEditorCore::setEditorSelection(ECEditor* selection)
{
	txVector<ECEditor*> editorList;
	editorList.push_back(selection);
	setEditorSelection(editorList);
}

void SceneEditorCore::setEditorSelection(txVector<ECEditor*>& selection)
{
	bool sameSelection = true;
	// �����ظ�����ѡ����,��������ѭ���ĵݹ����
	if (mEditorMultiSelections.size() == selection.size())
	{
		int selectionCount = mEditorMultiSelections.size();
		FOR_STL(mEditorMultiSelections, int i = 0; i < selectionCount; ++i)
		{
			if (mEditorMultiSelections[i] != selection[i])
			{
				sameSelection = false;
				break;
			}
		}
		END_FOR_STL(mEditorMultiSelections);
	}
	else
	{
		sameSelection = false;
	}
	if (sameSelection)
	{
		return;
	}

	clearEditorSelection();
	int selCount = selection.size();
	FOR_STL(selection, int i = 0; i < selCount; ++i)
	{
		auto iter = mEditorMultiSelSet.find(selection[i]);
		if (iter == mEditorMultiSelSet.end())
		{
			mEditorMultiSelections.push_back(selection[i]);
			mEditorMultiSelSet.insert(selection[i]);
		}
	}
	END_FOR_STL(selection);
	// ����ѡ����ı���¼�
	notifyEditorSelectionChanged();
}

void SceneEditorCore::reselectEditor()
{
	notifyEditorSelectionChanged();
}

void SceneEditorCore::clearEditorSelection()
{
	mEditorMultiSelSet.clear();
	mEditorMultiSelections.clear();
	// ����ѡ����ı���¼�
	notifyEditorSelectionChanged();
	// �༭��ѡ����ı�ʱ,������ѡ����
	mComponentEditorCore->clearComponentSelections();
}

void SceneEditorCore::notifyEditorSelectionChanged(const bool& notifyEvent)
{
	if (notifyEvent)
	{
		mEditorCoreRoot->sendEvent(CE_EDITOR_SELECTION_CHANGED);
	}
	if (mCurScene != NULL)
	{
		// ѡ����ı�ʱ��Ҫ����������������õ��ñ༭����
		ECAxis* axis = mCurScene->getEditorAxis();
		ECGrid* grid = mCurScene->getEditorGrid();
		if (mEditorMultiSelections.size() == 0)
		{
			axis->attachTarget(NULL);
			grid->attachTarget(NULL);
		}
		else
		{
			axis->attachTarget(mEditorMultiSelections[0]);
			grid->attachTarget(mEditorMultiSelections[0]);
		}
	}
}

bool SceneEditorCore::askSaveScene(const std::string& info)
{
	MODAL_DIALOG_RESULT ret = mEditorCoreRoot->showModalYesNoCancel(info);
	// �����,�򱣴��ļ�
	if (ret == MDR_YES)
	{
		saveAdvanceScene();
	}
	// �����,���ǵ�ǰ����Ϊδ�޸�״̬
	else if (ret == MDR_NO)
	{
		setSceneModified(false, false, false);
	}
	else
	{
		return false;
	}
	return true;
}