#include "ConfigToolCore.h"
#include "SocketNetManager.h"
#include "CTEventSystem.h"
#include "txCommandSystem.h"
#include "DataManager.h"
#include "txSerializer.h"
#include "ToolCoreBase.h"

template<> ConfigToolCore* txSingleton<ConfigToolCore>::ms_Singleton = NULL;

ConfigToolCore::ConfigToolCore()
:
mSocketNetManager(NULL),
mEventSystem(NULL),
mCommandSystem(NULL),
mDataManager(NULL),
mInitFlag(false)
{}

void ConfigToolCore::init()
{
	// �����¼�ϵͳ
	mEventSystem = TRACE_NEW(CTEventSystem, mEventSystem);
	// �¼�ϵͳ������ɺ����Ƿ��д�����Ϣ
	checkErrorInfoBuffer();

	mCommandSystem = TRACE_NEW(txCommandSystem, mCommandSystem);
	mDataManager = TRACE_NEW(DataManager, mDataManager);
	mSocketNetManager = TRACE_NEW(SocketNetManager, mSocketNetManager);
	ToolCoreBase::notifyConstructDone();
	
	mCommandSystem->init();
	mDataManager->init();
	mSocketNetManager->init(9124);
}

void ConfigToolCore::update(float elapsedTime)
{
	mEventSystem->update(elapsedTime);
	mCommandSystem->update(elapsedTime);
	mSocketNetManager->update(elapsedTime);
	mDataManager->update(elapsedTime);
}

void ConfigToolCore::destroy()
{
	TRACE_DELETE(mCommandSystem);
	TRACE_DELETE(mDataManager);
	TRACE_DELETE(mSocketNetManager);
	TRACE_DELETE(mEventSystem);
}

void ConfigToolCore::checkErrorInfoBuffer()
{
	int infoCount = mErrorInfoBuffer.size();
	for (int i = 0; i < infoCount; ++i)
	{
		logError(mErrorInfoBuffer[i]);
	}
	mErrorInfoBuffer.clear();
}

void ConfigToolCore::logError(const std::string& info)
{
	if (mEventSystem == NULL)
	{
		mErrorInfoBuffer.push_back(info);
		return;
	}
	// �����û�г�ʼ�����,���Ƚ���Ϣ�����¼�������
	std::vector<std::string> params;
	params.push_back(info);
	mEventSystem->pushEvent(CET_ERROR_LOG, params, mInitFlag);
}

void ConfigToolCore::logInfo(const std::string& info)
{
	if (mEventSystem == NULL)
	{
		return;
	}
	// �����û�г�ʼ�����,���Ƚ���Ϣ�����¼�������
	std::vector<std::string> params;
	params.push_back(info);
	mEventSystem->pushEvent(CET_INFO_LOG, params, mInitFlag);
}