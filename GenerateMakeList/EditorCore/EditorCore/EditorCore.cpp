#include "txMemeryTrace.h"
#include "CommandSystem.h"
#include "txSerializer.h"
#include "SocketNetManager.h"
#include "EventSystem.h"
#include "EditorCore.h"
#include "File.h"
#include "LibCurlDownloadManager.h"

template<> EditorCore* txSingleton<EditorCore>::ms_Singleton = NULL;

EditorCore::EditorCore()
:
mSocketNetManager(NULL),
mEventSystem(NULL),
mCommandSystem(NULL),
mLibCurlDownloadManager(NULL),
mInitFlag(false)
{}

void EditorCore::init()
{
	// �����¼�ϵͳ
	mEventSystem = TRACE_NEW(EventSystem, mEventSystem);
	// �¼�ϵͳ������ɺ����Ƿ��д�����Ϣ
	checkErrorInfoBuffer();

	mCommandSystem = TRACE_NEW(CommandSystem, mCommandSystem);
	//mSocketNetManager = TRACE_NEW(SocketNetManager, mSocketNetManager);
	mLibCurlDownloadManager = TRACE_NEW(LibCurlDownloadManager, mLibCurlDownloadManager);

	EditorCoreBase::notifyConstructDone();
	
	mCommandSystem->init();
	//mSocketNetManager->init(60060, 5000);
	mLibCurlDownloadManager->init();
}

void EditorCore::update(float elapsedTime)
{
	mEventSystem->update(elapsedTime);
	mCommandSystem->update(elapsedTime);
	//mSocketNetManager->update(elapsedTime);
	mLibCurlDownloadManager->update(elapsedTime);
}

void EditorCore::sendEvent(const CORE_EVENT_TYPE& type, const std::string& param, bool sendImmediately)
{
	txVector<std::string> params;
	params.push_back(param);
	sendEvent(type, params, sendImmediately);
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

void EditorCore::sendDelayEvent(const CORE_EVENT_TYPE& type, const std::string& param)
{
	sendEvent(type, param, false);
}

void EditorCore::sendDelayEvent(const CORE_EVENT_TYPE& type, const txVector<std::string>& params)
{
	sendEvent(type, params, false);
}

void EditorCore::destroy()
{
	TRACE_DELETE(mCommandSystem);
	//TRACE_DELETE(mSocketNetManager);
	TRACE_DELETE(mEventSystem);
	TRACE_DELETE(mLibCurlDownloadManager);
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

void EditorCore::logError(const std::string& info, const bool& delay)
{
	if (mEventSystem == NULL)
	{
		mErrorInfoBuffer.push_back(info);
		return;
	}
	// �����û�г�ʼ�����,���Ƚ���Ϣ�����¼�������
	txVector<std::string> params;
	params.push_back(info);
	sendEvent(CET_ERROR_LOG, params, !delay);
}

void EditorCore::logInfo(const std::string& info, const bool& delay)
{
	if (mEventSystem == NULL)
	{
		return;
	}
	// �����û�г�ʼ�����,���Ƚ���Ϣ�����¼�������
	txVector<std::string> params;
	params.push_back(info);
	sendEvent(CET_INFO_LOG, params, !delay);
}