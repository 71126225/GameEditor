#ifdef _TRACE_MEMERY

#include "txMemeryTrace.h"
#include "txLog.h"
#ifdef _WRITE_FILE
#include "txSerializer.h"
#endif

std::map<void*, MemeryInfo> MemeryTrace::mMemeryInfo;
std::map<std::string, MemeryType> MemeryTrace::mMemeryType;
std::set<std::string> MemeryTrace::mIgnoreClass;
std::set<std::string> MemeryTrace::mIgnoreClassKeyword;
std::set<std::string> MemeryTrace::mShowOnlyDetailClass;
std::set<std::string> MemeryTrace::mShowOnlyStatisticsClass;
bool MemeryTrace::mShowDetail = true;
bool MemeryTrace::mShowStatistics = true;
bool MemeryTrace::mShowTotalCount = true;
int MemeryTrace::mInstanceCount = 0;
bool MemeryTrace::mShowAll = true;
#ifdef _WRITE_FILE
std::map<std::string, int> MemeryTrace::mMemeryTypeIndex;
MemeryType MemeryTrace::mMemeryList[MAX_COUNT];
int MemeryTrace::mMemeryCount = 0;
#endif
CFFMServer* MemeryTrace::mServer = NULL;
bool MemeryTrace::mLockWriteInfo = false;
bool MemeryTrace::mLockReadInfo = false;

MemeryTrace::MemeryTrace()
{
	++mInstanceCount;
	assert(mInstanceCount <= 1);
	mShowDetail = true;
	mShowStatistics = true;
	mShowTotalCount = true;
	mShowAll = true;
	mServer = TRACE_NEW(CFFMServer, mServer);
	mServer->Create();
	mThread = NULL;
}

MemeryTrace::~MemeryTrace()
{
	if (mThread != NULL)
	{
		TerminateThread(mThread, 0);
		mThread = NULL;
	}
	Sleep(100);
	TRACE_DELETE(mServer);
}

DWORD WINAPI MemeryTrace::debugMemeryTrace(LPVOID lpParameter)
{
	while (true)
	{
		static DWORD beginTime = timeGetTime();
		DWORD curTime = timeGetTime();
		if (curTime - beginTime < 1000)
		{
			Sleep(1000 - (curTime - beginTime));
		}
		curTime = timeGetTime();
		int memCount = mMemeryInfo.size();
		int memSize = 0;
		if (mShowAll)
		{
			// ���ȼ���Ƿ���Զ�ȡ,���������,��ȴ�������ȡ
			waitUnlockRead();
			// ���ڶ��б���ж�ȡ����,����Ҫ�����б��д��
			lockWriteInfo();
			LOGI("\n\n---------------------------------------------memery info begin-----------------------------------------------------------\n");

			// �ڴ���ϸ��Ϣ
			auto iter = mMemeryInfo.begin();
			auto iterEnd = mMemeryInfo.end();
			for (; iter != iterEnd; ++iter)
			{
				memSize += iter->second.size;
				if (!mShowDetail)
				{
					continue;
				}
				// ����������Ѻ���,����ʾ
				if (mIgnoreClass.find(iter->second.type) != mIgnoreClass.end())
				{
					continue;
				}

				// �������ʾ�������б�Ϊ��,��ֻ��ʾ�б��е�����
				if (mShowOnlyDetailClass.size() > 0 && mShowOnlyDetailClass.find(iter->second.type) == mShowOnlyDetailClass.end())
				{
					continue;
				}

				// ������Ͱ����ؼ���,����ʾ
				bool show = true;
				auto iterKeyword = mIgnoreClassKeyword.begin();
				auto iterKeywordEnd = mIgnoreClassKeyword.end();
				for (; iterKeyword != iterKeywordEnd; ++iterKeyword)
				{
					if (strstr(iter->second.type.c_str(), iterKeyword->c_str()) != NULL)
					{
						show = false;
						break;
					}
				}

				if (show)
				{
					LOGI("size : %d, file : %s, line : %d, class : %s\n", iter->second.size, iter->second.file.c_str(), iter->second.line, iter->second.type.c_str());
				}
			}

			// �����б��д��
			unlockWriteInfo();

			if (mShowTotalCount)
			{
				LOGI("-------------------------------------------------memery count : %d, total size : %.3fKB\n", memCount, memSize / 1000.0f);
			}

			if (mShowStatistics)
			{
				auto iterType = mMemeryType.begin();
				auto iterTypeEnd = mMemeryType.end();
				for (; iterType != iterTypeEnd; ++iterType)
				{
					// ����������Ѻ���,����ʾ
					if (mIgnoreClass.find(iterType->first) != mIgnoreClass.end())
					{
						continue;
					}
					// �������ʾ�������б�Ϊ��,��ֻ��ʾ�б��е�����
					if (mShowOnlyStatisticsClass.size() > 0 && mShowOnlyStatisticsClass.find(iterType->first) == mShowOnlyStatisticsClass.end())
					{
						continue;
					}
					// ������Ͱ����ؼ���,����ʾ
					bool show = true;
					auto iterKeyword = mIgnoreClassKeyword.begin();
					auto iterKeywordEnd = mIgnoreClassKeyword.end();
					for (; iterKeyword != iterKeywordEnd; ++iterKeyword)
					{
						if (strstr(iterType->first.c_str(), iterKeyword->c_str()) != NULL)
						{
							show = false;
							break;
						}
					}
					if (show)
					{
						LOGI("%s : %d��, %.3fKB\n", iterType->first.c_str(), iterType->second.count, iterType->second.size / 1000.0f);
					}
				}
			}
			LOGI("---------------------------------------------memery info end-----------------------------------------------------------\n");
		}
		beginTime = curTime;
	}
	return 0;
}

#ifdef _WRITE_FILE
DWORD WINAPI MemeryTrace::writeMemeryTrace(LPVOID lpParameter)
{
	while (true)
	{
		static DWORD beginTime = timeGetTime();
		DWORD curTime = timeGetTime();
		if (curTime - beginTime < 1000)
		{
			Sleep(1000 - (curTime - beginTime));
		}
		curTime = timeGetTime();
		// ����Ͳ����ڶ����л����ڴ���и���,�������������
		txSerializer serializer(false);

		// ���ȼ���Ƿ���Զ�ȡ,���������,��ȴ�������ȡ
		waitUnlockRead();
		// ���ڶ��б���ж�ȡ����,����Ҫ�����б��д��
		lockWriteInfo();
		// д����ϸ��Ϣ����
		int infoCount = mMemeryInfo.size();
		serializer.write(infoCount);
		auto iterInfo = mMemeryInfo.begin();
		auto iterInfoEnd = mMemeryInfo.end();
		for (; iterInfo != iterInfoEnd; ++iterInfo)
		{
			// д���ַ
			serializer.write(iterInfo->first);
			// д���ڴ��С
			serializer.write(iterInfo->second.size);
			// д���ļ���
			serializer.writeString(iterInfo->second.file.c_str());
			// д���к�
			serializer.write(iterInfo->second.line);
			// д������
			serializer.writeString(iterInfo->second.type.c_str());
		}

		// д����������
		int typeCount = mMemeryTypeIndex.size();
		serializer.write(typeCount);
		auto iterIndex = mMemeryTypeIndex.begin();
		auto iterIndexEnd = mMemeryTypeIndex.end();
		for (; iterIndex != iterIndexEnd; ++iterIndex)
		{
			// д��������
			serializer.writeString(mMemeryList[iterIndex->second].type.c_str());
			// д�����
			serializer.write(mMemeryList[iterIndex->second].count);
			// д���С
			serializer.write(mMemeryList[iterIndex->second].size);
		}

		// �����б��д��
		unlockWriteInfo();

		mServer->WriteCmdData(1, serializer.getDataSize(), (void*)serializer.getBuffer());
		
		beginTime = curTime;
	}
	return 0;
}
#endif

#endif