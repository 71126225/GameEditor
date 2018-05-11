#include "txMemeryTrace.h"
#include "txCommandSystem.h"
#include "txSerializer.h"
#include "SocketNetManager.h"
#include "CTEventSystem.h"
#include "EditorCore.h"
#include "txSystemInfo.h"
#include "LibCurlNetManager.h"
#include "GymInfo.h"

template<> EditorCore* txSingleton<EditorCore>::ms_Singleton = NULL;

EditorCore::EditorCore()
:
mSocketNetManager(NULL),
mEventSystem(NULL),
mCommandSystem(NULL),
mSystemInfo(NULL),
mLibCurlNetManager(NULL),
mGymInfo(NULL),
mInitFlag(false)
{}

void EditorCore::init()
{
	// �����¼�ϵͳ
	mEventSystem = TRACE_NEW(CTEventSystem, mEventSystem);
	// �¼�ϵͳ������ɺ����Ƿ��д�����Ϣ
	checkErrorInfoBuffer();

	mCommandSystem = TRACE_NEW(txCommandSystem, mCommandSystem);
	mSystemInfo = TRACE_NEW(txSystemInfo, mSystemInfo);
	mLibCurlNetManager = TRACE_NEW(LibCurlNetManager, mLibCurlNetManager);
	mGymInfo = TRACE_NEW(GymInfo, mGymInfo);

	EditorCoreBase::notifyConstructDone();
	
	mSystemInfo->init();
	mCommandSystem->init();
	mLibCurlNetManager->init();
	mGymInfo->init();
}

void EditorCore::update(float elapsedTime)
{
	mEventSystem->update(elapsedTime);
	mCommandSystem->update(elapsedTime);
	mLibCurlNetManager->update(elapsedTime);
}

void EditorCore::sendEvent(const CORE_EVENT_TYPE& type, const txVector<std::string>& params, bool sendImmediately)
{
	// ������������͵��¼�,����Ҫ���ݳ�ʼ������ж��Ƿ�Ӧ����������
	if (sendImmediately)
	{
		sendImmediately = mInitFlag;
	}
	if (mEventSystem != NULL)
	{
		mEventSystem->pushEvent(type, params, sendImmediately);
	}
}

void EditorCore::destroy()
{
	TRACE_DELETE(mCommandSystem);
	TRACE_DELETE(mEventSystem);
	TRACE_DELETE(mLibCurlNetManager);
}

void EditorCore::checkErrorInfoBuffer()
{
	int infoCount = mErrorInfoBuffer.size();
	for (int i = 0; i < infoCount; ++i)
	{
		logError(mErrorInfoBuffer[i]);
	}
	mErrorInfoBuffer.clear();
}

void EditorCore::logError(const std::string& info)
{
	if (mEventSystem == NULL)
	{
		mErrorInfoBuffer.push_back(info);
		return;
	}
	txVector<std::string> params;
	params.push_back(info);
	sendEvent(CET_ERROR_LOG, params);
}

void EditorCore::logInfo(const std::string& info)
{
	if (mEventSystem == NULL)
	{
		return;
	}
	txVector<std::string> params;
	params.push_back(info);
	sendEvent(CET_INFO_LOG, params);
}