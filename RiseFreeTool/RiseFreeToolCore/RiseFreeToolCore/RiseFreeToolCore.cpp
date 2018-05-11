#include "RiseFreeToolCore.h"
#include "SocketNetManager.h"
#include "txMemeryTrace.h"
#include "EventSystem.h"
#include "CommandSystem.h"
#include "SpeedDataManager.h"
#include "SocketPacketFriction.h"
#include "txSerializer.h"
#include "ComManager.h"

template<> RiseFreeToolCore* txSingleton<RiseFreeToolCore>::ms_Singleton = NULL;

RiseFreeToolCore::RiseFreeToolCore()
:
mEventSystem(NULL),
mCommandSystem(NULL),
mSpeedDataManager(NULL),
mComManager(NULL),
mInitFlag(false)
{}

void RiseFreeToolCore::init()
{
	// �����¼�ϵͳ
	mEventSystem = TRACE_NEW(EventSystem, mEventSystem);
	// �¼�ϵͳ������ɺ����Ƿ��д�����Ϣ
	checkErrorInfoBuffer();

	mCommandSystem = TRACE_NEW(CommandSystem, mCommandSystem);
	mSpeedDataManager = TRACE_NEW(SpeedDataManager, mSpeedDataManager);
	mComManager = TRACE_NEW(ComManager, mComManager);

	EditorCoreBase::notifyConstructDone();
	
	mCommandSystem->init();
	mSpeedDataManager->init();
	mComManager->init();
}

void RiseFreeToolCore::update(float elapsedTime)
{
	mEventSystem->update(elapsedTime);
	mCommandSystem->update(elapsedTime);
	mSpeedDataManager->update(elapsedTime);
	mComManager->update(elapsedTime);
}

void RiseFreeToolCore::destroy()
{
	TRACE_DELETE(mComManager);
	TRACE_DELETE(mCommandSystem);
	TRACE_DELETE(mSpeedDataManager);
	TRACE_DELETE(mEventSystem);
}

void RiseFreeToolCore::checkErrorInfoBuffer()
{
	int infoCount = mErrorInfoBuffer.size();
	for (int i = 0; i < infoCount; ++i)
	{
		logError(mErrorInfoBuffer[i]);
	}
	mErrorInfoBuffer.clear();
}

void RiseFreeToolCore::sendEvent(const CORE_EVENT_TYPE& type, const std::string& param, bool sendImmediately)
{
	txVector<std::string> params;
	params.push_back(param);
	sendEvent(type, params, sendImmediately);
}

void RiseFreeToolCore::sendEvent(const CORE_EVENT_TYPE& type, const txVector<std::string>& params, bool sendImmediately)
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

void RiseFreeToolCore::sendDelayEvent(const CORE_EVENT_TYPE& type, const std::string& param)
{
	sendEvent(type, param, false);
}

void RiseFreeToolCore::sendDelayEvent(const CORE_EVENT_TYPE& type, const txVector<std::string>& params)
{
	sendEvent(type, params, false);
}

void RiseFreeToolCore::logError(const std::string& info, const bool& delay)
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

void RiseFreeToolCore::logInfo(const std::string& info, const bool& delay)
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