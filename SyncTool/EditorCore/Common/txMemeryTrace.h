#ifndef _TX_MEMERY_TRACE_H_
#define _TX_MEMERY_TRACE_H_

#ifdef _TRACE_MEMERY

#include <map>
#include <set>
#include <string>
#include <typeinfo.h>
#include <assert.h>

#define _WRITE_FILE

#include <windows.h>

#include "txShareMemeryServer.h"

// ע������!!!
// ��Ҫȷ��MemeryTrace����ֻ��һ��
// ���еĶ��ڴ�������ͷŶ�����ʹ��TRACE_NEW,TRACE_NEW_ARRAY,TRACE_DELETE,TRACE_DELETE_ARRAY
// TRACE_NEW������ڴ�ֻ����TRACE_DELETE�ͷ�,TRACE_NEW_ARRAY������ڴ�ֻ����TRACE_DELETE_ARRAY�ͷ�
// ��Ȼ������new��deleteһ�����﷨ʹ��TRACE��غ�,���ǲ���ֱ����return�����TRACE��غ�
// TRACE��غ�Ҳ�����ڹ��캯����ʼ���б���ʹ��

const int MAX_COUNT = 1000;

struct MemeryInfo
{
	MemeryInfo(int s, std::string f, int l, std::string t)
		:
		size(s),
		file(f),
		line(l),
		type(t)
	{}
	int size;			// �ڴ��С
	std::string file;	// �����ڴ���ļ�
	int line;			// �����ڴ�Ĵ����к�
	std::string type;	// �ڴ�Ķ�������
};

struct MemeryType
{
	MemeryType(std::string t = "")
	:
	type(t),
	count(0),
	size(0)
	{}
	MemeryType(std::string t, int c, int s)
		:
		type(t),
		count(c),
		size(s)
	{}
	std::string type;
	int count;
	int size;
};

class MemeryTrace
{
public:
	MemeryTrace();
	virtual ~MemeryTrace();
	// writeOrDebugΪ�����ʾ����Ϣд�빲���ڴ�,Ȼ��ʹ��MemeryViewer�鿴,Ϊ��ʱ��ʾ�ڿ���̨��ʾ�ڴ���Ϣ
	void init(bool writeOrDebug)
	{
#ifdef _WRITE_FILE
		if (writeOrDebug)
		{
			mThread = CreateThread(NULL, 0, MemeryTrace::writeMemeryTrace, NULL, 0, NULL);
		}
		else
		{
			mThread = CreateThread(NULL, 0, MemeryTrace::debugMemeryTrace, NULL, 0, NULL);
		}
#else
		mThread = CreateThread(NULL, 0, MemeryTrace::debugMemeryTrace, NULL, 0, NULL);
#endif
	}
	static DWORD WINAPI debugMemeryTrace(LPVOID lpParameter);
#ifdef _WRITE_FILE
	static DWORD WINAPI writeMemeryTrace(LPVOID lpParameter);
#endif
	static void insertPtr(void* ptr, MemeryInfo info)
	{
		// ���ȼ���Ƿ����д��,���������,��ȴ�����д��
		waitUnlockWrite();
		// ���ڶ��б����д�����,����Ҫ�����б�Ķ�ȡ
		lockReadInfo();
		int lastPos = info.file.find_last_of('\\');
		if (lastPos != -1)
		{
			info.file = info.file.substr(lastPos + 1, info.file.length() - lastPos - 1);
		}
		mMemeryInfo.insert(std::make_pair(ptr, info));

		auto iterType = mMemeryType.find(info.type);
		if (iterType != mMemeryType.end())
		{
			++(iterType->second.count);
			iterType->second.size += info.size;
		}
		else
		{
			mMemeryType.insert(std::make_pair(info.type, MemeryType(info.type, 1, info.size)));
		}

#ifdef _WRITE_FILE
		// �������±��б��в��Ҹ�����,�����,�����������Ϣ
		auto iterIndex = mMemeryTypeIndex.find(info.type);
		if (iterIndex != mMemeryTypeIndex.end())
		{
			auto iterType = mMemeryType.find(info.type);
			mMemeryList[iterIndex->second] = iterType->second;
		}
		// ���û��,�����������Ϣ
		else
		{
			if (mMemeryCount < MAX_COUNT)
			{
				auto iterType = mMemeryType.find(info.type);
				mMemeryTypeIndex.insert(std::make_pair(info.type, mMemeryCount));
				mMemeryList[mMemeryCount] = iterType->second;
				++mMemeryCount;
			}
			else
			{
				unlockReadInfo();
				return;
			}
		}
#endif

		// �����б�Ķ�ȡ
		unlockReadInfo();
	}
	static void erasePtr(void* ptr)
	{
		// ���ȼ���Ƿ����д��,���������,��ȴ�����д��
		waitUnlockWrite();
		// ���ڶ��б����д�����,����Ҫ�����б�Ķ�ȡ
		lockReadInfo();
		
		auto iterTrace = mMemeryInfo.find(ptr);
		std::string type;
		int size = 0;
		if (iterTrace != mMemeryInfo.end())
		{
			type = iterTrace->second.type;
			size = iterTrace->second.size;
			mMemeryInfo.erase(iterTrace);
		}
		else
		{
			unlockReadInfo();
			return;
		}

		auto iterType = mMemeryType.find(type);
		if (iterType != mMemeryType.end())
		{
			--(iterType->second.count);
			iterType->second.size -= size;
		}
		else
		{
			unlockReadInfo();
			return;
		}

#ifdef _WRITE_FILE
		// ���±��б��в��Ҹ����͵��±�,�����,��������Ϣ�е���Ϣ���
		auto iterIndex = mMemeryTypeIndex.find(type);
		if (iterIndex != mMemeryTypeIndex.end())
		{
			std::string type = mMemeryList[iterIndex->second].type;
			--(mMemeryList[iterIndex->second].count);
			mMemeryList[iterIndex->second].size -= size;
		}
		else
		{
			unlockReadInfo();
			return;
		}
#endif

		// �����б�Ķ�ȡ
		unlockReadInfo();
	}
	static void setIgnoreClass(std::set<std::string>& classList){mIgnoreClass = classList;}
	static void setIgnoreClassKeyword(std::set<std::string>& classList){mIgnoreClassKeyword = classList;}
	static void setShowOnlyDetailClass(std::set<std::string>& classList){mShowOnlyDetailClass = classList;}
	static void setShowOnlyStatisticsClass(std::set<std::string>& classList){mShowOnlyStatisticsClass = classList;}
	static void setShowDetail(bool show){mShowDetail = show;}
	static void setShowStatistics(bool show){mShowStatistics = show;}
	static void setShowAll(bool show){mShowAll = show;}
	static void lockWriteInfo() { mLockWriteInfo = true; }
	static void unlockWriteInfo() { mLockWriteInfo = false; }
	static void waitUnlockWrite()
	{
		while (mLockWriteInfo)
		{}
	}
	static void lockReadInfo() { mLockReadInfo = true; }
	static void unlockReadInfo() { mLockReadInfo = false; }
	static void waitUnlockRead()
	{
		while (mLockReadInfo)
		{}
	}
protected:
	// �ڴ���������Ϣ��
	static std::map<void*, MemeryInfo> mMemeryInfo;
	// �ڴ�ͳ����Ϣ��, first��������,second��first�Ǹ����������ڴ����,second�Ǹ�����ռ�����ڴ��С,��λ���ֽ�
	static std::map<std::string, MemeryType> mMemeryType;
	// ����ʾ���б������͵��ڴ���ϸ��Ϣ�Լ�ͳ����Ϣ
	static std::set<std::string> mIgnoreClass;
	// �����ϸ��Ϣ�Լ�ͳ����Ϣ�е����Ͱ������б��еĹؼ���,����ʾ
	static std::set<std::string> mIgnoreClassKeyword;
	// ֻ��ʾ���б������͵��ڴ���ϸ��Ϣ,������б�Ϊ��,��ȫ����ʾ
	static std::set<std::string> mShowOnlyDetailClass;
	// ֻ��ʾ���б������͵��ڴ�ͳ����Ϣ,������б�Ϊ��,��ȫ����ʾ
	static std::set<std::string> mShowOnlyStatisticsClass;
	// �Ƿ���ʾ����Ϣ�����ϸ����
	static bool mShowDetail;
	// �Ƿ���ʾ�ڴ�ͳ����Ϣ
	static bool mShowStatistics;
	// �Ƿ���ʾ�ڴ��ܸ���
	static bool mShowTotalCount;
	static int mInstanceCount;
	static bool mShowAll;
	static bool mLockWriteInfo;
	static bool mLockReadInfo;
	HANDLE mThread;

#ifdef _WRITE_FILE
	static std::map<std::string, int> mMemeryTypeIndex;
	static MemeryType mMemeryList[MAX_COUNT];
	static int mMemeryCount;
#endif

	static CFFMServer* mServer;
};

// �����޲λ��ߴ��ι�������ڴ�
#define TRACE_NEW(className, ptr, ...)  NULL;\
										ptr = new className(__VA_ARGS__);	\
										MemeryTrace::insertPtr(ptr, MemeryInfo(sizeof(className), __FILE__, __LINE__, typeid(className).name()));

// �����޲ι��������߻����������������ڴ�
#define TRACE_NEW_ARRAY(className, count, ptr)  \
NULL;											\
if (count <= 0)									\
{												\
	ptr = NULL;									\
}												\
else											\
{												\
	ptr = new className[count];					\
}												\
MemeryTrace::insertPtr(ptr, MemeryInfo(sizeof(className) * count, __FILE__, __LINE__, typeid(className).name()));

// �ͷ�TRACE_NEW������ڴ�
#define TRACE_DELETE(ptr)			\
MemeryTrace::erasePtr((void*)ptr);	\
if(ptr != NULL)						\
{									\
	delete ptr;						\
	ptr = NULL;						\
}

// �ͷ�TRACE_NEW_ARRAY������ڴ�
#define TRACE_DELETE_ARRAY(ptr)		\
MemeryTrace::erasePtr((void*)ptr);	\
if(ptr != NULL)						\
{									\
	delete[] ptr;					\
	ptr = NULL;						\
}

#else

// �����޲λ��ߴ��ι�������ڴ�
#define TRACE_NEW(className, ptr, ...)  NULL;\
										ptr = new className(__VA_ARGS__);

// �����޲ι��������߻����������������ڴ�
#define TRACE_NEW_ARRAY(className, count, ptr)  \
NULL;											\
if (count <= 0)									\
{												\
	ptr = NULL;									\
}												\
else											\
{												\
	ptr = new className[count];					\
}

// �ͷ�TRACE_NEW������ڴ�
#define TRACE_DELETE(ptr)	\
if (ptr != NULL)			\
{							\
	delete ptr;				\
	ptr = NULL;				\
}

// �ͷ�TRACE_NEW_ARRAY������ڴ�
#define TRACE_DELETE_ARRAY(ptr) \
if (ptr != NULL)				\
{								\
	delete[] ptr;				\
	ptr = NULL;					\
}
#endif

// ����ʲôƽ̨����Ҫ���������������ڴ������
#define NORMAL_NEW(className, ptr, ...) NULL;\
	ptr = new className(__VA_ARGS__);

// ���������������ڴ�
#define NORMAL_NEW_ARRAY(className, count, ptr) \
NULL;											\
if (count <= 0)									\
{												\
	ptr = NULL;									\
}												\
else											\
{												\
	ptr = new className[count];					\
}												\

// �������ͷ��ڴ�
#define NORMAL_DELETE(ptr)	\
if (ptr != NULL)			\
{							\
	delete ptr;				\
	ptr = NULL;				\
}

// �������ͷ������ڴ�
#define NORMAL_DELETE_ARRAY(ptr)	\
if (ptr != NULL)					\
{									\
	delete[] ptr;					\
	ptr = NULL;						\
}

#endif