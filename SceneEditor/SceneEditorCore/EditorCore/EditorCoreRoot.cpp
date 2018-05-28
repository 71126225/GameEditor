#include "txTexture.h"
#include "txTextureManager.h"
#include "txEntityManager.h"
#include "txNodeManager.h"
#include "txNode.h"
#include "txEngineRoot.h"
#include "txSceneManager.h"
#include "txCamera.h"
#include "txScene.h"
#include "txRenderWindow.h"
#include "txLoadUtility.h"
#include "txUtility.h"
#include "txODEPhysics.h"
#include "txRigidBody.h"
#include "txGeometryPlane.h"
#include "txGeometryBox.h"
#include "txGeometrySphere.h"
#include "txGeometryCylinder.h"
#include "txGeometrySphere.h"
#include "txPropertyManager.h"
#include "txPropertyReceiver.h"
#include "json/json.h"
#include "txComponentFactory.h"
#include "txComponentFactoryManager.h"
#include "txComponentHeader.h"

#include "EditorCoreRoot.h"
#include "GameCamera.h"
#include "CameraManager.h"
#include "CoreEventSystem.h"
#include "ECEditor.h"
#include "InputSystem.h"
#include "ECEditorManager.h"
#include "CoreCommandHeader.h"
#include "ECGrid.h"
#include "ECScene.h"
#include "ECAxis.h"
#include "ECSceneManager.h"
#include "GameLog.h"
#include "CameraLinkerAcceleration.h"
#include "CameraLinkerFixed.h"
#include "CameraLinkerFixedTime.h"
#include "CameraLinkerSpring.h"
#include "CameraTrembling.h"
#include "ECUndoManager.h"
#include "ECUndoAddComponent.h"
#include "SceneEditorCore.h"
#include "ComponentEditorCore.h"

template<> EditorCoreRoot* txSingleton<EditorCoreRoot>::ms_Singleton = NULL;

EditorCoreRoot::EditorCoreRoot()
:
mInitFlag(false),
mEngineRoot(NULL),
mCoreEventSystem(NULL),
mEditorManager(NULL),
mEditorSceneManager(NULL)
{}

void EditorCoreRoot::init(const HWND& hwnd, const int& colourBits, const int& width, const int& height)
{
	// ��ʼ������
	mEngineRoot = TRACE_NEW(txEngineRoot, mEngineRoot);
	mEngineRoot->initialise(hwnd, colourBits, width, height);
	mRenderWindow->setBackgroundColour(VECTOR4(0.5f, 0.5f, 0.5f, 1.0f));

	// �����¼�ϵͳ
	mCoreEventSystem = TRACE_NEW(CoreEventSystem, mCoreEventSystem);
	// �¼�ϵͳ������ɺ����Ƿ��д���
	checkErrorInfoBuffer();
	mEditorManager = TRACE_NEW(ECEditorManager, mEditorManager);
	mEditorSceneManager = TRACE_NEW(ECSceneManager, mEditorSceneManager);
	mUndoManager = TRACE_NEW(ECUndoManager, mUndoManager);
	mSceneEditorCore = TRACE_NEW(SceneEditorCore, mSceneEditorCore);
	mComponentEditorCore = TRACE_NEW(ComponentEditorCore, mComponentEditorCore);

	// �����඼������ɺ�֪ͨECBase
	EditorCoreBase::notifyConstructDone();
	mEditorManager->init();
	mEditorSceneManager->init();
	mUndoManager->init();
	mSceneEditorCore->init();
	mComponentEditorCore->init();
	mCameraManager->getMainCamera()->setLockKeyProcess(false);
}

void EditorCoreRoot::preUpdate(float elapsedTime)
{
	mEngineRoot->preUpdate(elapsedTime);
}

void EditorCoreRoot::update(float elapsedTime)
{
	mSceneEditorCore->update(elapsedTime);
	mEngineRoot->update(elapsedTime);
	mComponentEditorCore->update(elapsedTime);
	mCoreEventSystem->update(elapsedTime);
	mEditorManager->update(elapsedTime);
}

void EditorCoreRoot::lateUpdate(float elapsedTime)
{
	mEngineRoot->lateUpdate(elapsedTime);
}

void EditorCoreRoot::render()
{
	mEngineRoot->beginRender();
	// ��Ⱦ����
	mCameraManager->getMainCamera()->getCamera()->render(mEngineRoot->getSceneManager());
	mSceneEditorCore->render();
	mComponentEditorCore->render();
	mEngineRoot->endRender();
}

void EditorCoreRoot::destroy()
{
	TRACE_DELETE(mSceneEditorCore);
	TRACE_DELETE(mComponentEditorCore);
	TRACE_DELETE(mCoreEventSystem);	
	TRACE_DELETE(mEditorManager);
	TRACE_DELETE(mUndoManager);
	TRACE_DELETE(mEngineRoot);
}

void EditorCoreRoot::setModalDialog(ModalDialog dialog, int style[MDS_MAX], int result[MDR_MAX])
{
	mModalDialog = dialog;
	memcpy(mDialogStyle, style, MDS_MAX * sizeof(int));
	memcpy(mDialogResult, result, MDR_MAX * sizeof(int));
}

void EditorCoreRoot::logError(const std::string& info)
{
	// ����¼�ϵͳû�г�ʼ��,����������Ϣ������
	if (mCoreEventSystem == NULL)
	{
		mErrorInfoBuffer.push_back(info);
		return;
	}
	// �����û�г�ʼ�����,���Ƚ���Ϣ�����¼�������
	sendEvent(CE_EDITOR_ERROR, info);
}

void EditorCoreRoot::sendEvent(const CORE_EVENT& type, const std::string& param, const bool& sendImmediately)
{
	txVector<std::string> paramList;
	paramList.push_back(param);
	sendEvent(type, paramList, sendImmediately);
}

void EditorCoreRoot::sendEvent(const CORE_EVENT& type, const txVector<std::string>& params, bool sendImmediately)
{
	// ������������͵��¼�,����Ҫ���ݳ�ʼ������ж��Ƿ�Ӧ����������
	if (sendImmediately)
	{
		sendImmediately = mInitFlag;
	}
	if (mCoreEventSystem != NULL)
	{
		mCoreEventSystem->pushEvent(type, params, sendImmediately);
	}
}

void EditorCoreRoot::checkErrorInfoBuffer()
{
	int infoCount = mErrorInfoBuffer.size();
	FOR_STL(mErrorInfoBuffer, int i = 0; i < infoCount; ++i)
	{
		logError(mErrorInfoBuffer[i]);
	}
	END_FOR_STL(mErrorInfoBuffer);
	mErrorInfoBuffer.clear();
}

MODAL_DIALOG_RESULT EditorCoreRoot::showModalYesNoCancel(const std::string& info)
{
	return showModal(info, mDialogStyle[MDS_YES] | mDialogStyle[MDS_NO] | mDialogStyle[MDS_CANCEL]);
}

MODAL_DIALOG_RESULT EditorCoreRoot::showModalYesNo(const std::string& info)
{
	return showModal(info, mDialogStyle[MDS_YES] | mDialogStyle[MDS_NO]);
}

MODAL_DIALOG_RESULT EditorCoreRoot::showModal(const std::string& info, const int& flag)
{
	// ���û�����ô���ʾ�Ի���ĺ���,��ֱ�Ӽ���ִ����ȥ
	if (mModalDialog == NULL)
	{
		return MDR_MAX;
	}
	int ret = mModalDialog("��ʾ", info, flag);
	for (int i = 0; i < MDR_MAX; ++i)
	{
		if (ret == mDialogResult[i])
		{
			return (MODAL_DIALOG_RESULT)i;
		}
	}
	return MDR_MAX;
}