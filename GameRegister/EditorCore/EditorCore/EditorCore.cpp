#include "txMemeryTrace.h"
#include "txCommandSystem.h"
#include "txSerializer.h"
#include "txSystemInfo.h"
#include "CTEventSystem.h"
#include "EditorCore.h"
#include "EditorCoreBase.h"

template<> EditorCore* txSingleton<EditorCore>::ms_Singleton = NULL;

EditorCore::EditorCore()
:
mEventSystem(NULL),
mCommandSystem(NULL),
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

	EditorCoreBase::notifyConstructDone();
	
	mSystemInfo->init();
	mCommandSystem->init();
}

void EditorCore::update(float elapsedTime)
{
	mEventSystem->update(elapsedTime);
	mCommandSystem->update(elapsedTime);
}

void EditorCore::destroy()
{
	TRACE_DELETE(mCommandSystem);
	TRACE_DELETE(mEventSystem);
	TRACE_DELETE(mSystemInfo);
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
	// �����û�г�ʼ�����,���Ƚ���Ϣ�����¼�������
	std::vector<std::string> params;
	params.push_back(info);
	mEventSystem->pushEvent(CET_ERROR_LOG, params, mInitFlag);
}

void EditorCore::logInfo(const std::string& info)
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