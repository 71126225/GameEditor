#ifndef _TX_MEMORY_TRACE_H_
#define _TX_MEMORY_TRACE_H_

//#define TRACE_MEMORY
//#define WRITE_FILE
//#define CHECK_MEMORY
#include "EngineDefine.h"
#include "txThreadLock.h"
#include "txMemoryCheck.h"

// ����ʲôƽ̨����Ҫ���������������ڴ������
#define NEW_MEMORY(className, ptr, check, ...)		\
NULL;												\
try													\
{													\
	ptr = new className(__VA_ARGS__);				\
	if (check)										\
	{												\
		txMemoryCheck::usePtr(ptr);					\
	}												\
}													\
catch (...)											\
{													\
	ptr = NULL;										\
	TOOL_CORE_ERROR("error : can not alloc memory!");\
}

// ���������������ڴ�
#define NEW_MEMORY_ARRAY(className, count, ptr, check)		\
NULL;														\
if (count <= 0)												\
{															\
	ptr = NULL;												\
}															\
else														\
{															\
	try														\
	{														\
		ptr = new className[count];							\
		memset(ptr, 0, sizeof(className)* count);			\
		if (check)											\
		{													\
			txMemoryCheck::usePtr(ptr);						\
		}													\
	}														\
	catch (...)												\
	{														\
		ptr = NULL;											\
		TOOL_CORE_ERROR("error : can not alloc memory array!");\
	}														\
}															\

// �������ͷ��ڴ�
#define DELETE_MEMORY(ptr, check)		\
if (ptr != NULL)						\
{										\
	if (check)							\
	{									\
		txMemoryCheck::unusePtr(ptr);	\
	}									\
	delete ptr;							\
	ptr = NULL;							\
}

// �������ͷ������ڴ�
#define DELETE_MEMORY_ARRAY(ptr, check)	\
if (ptr != NULL)						\
{										\
	if (check)							\
	{									\
		txMemoryCheck::unusePtr(ptr);	\
	}									\
	delete[] ptr;						\
	ptr = NULL;							\
}

#ifdef CHECK_MEMORY
// ���ڴ�Ϸ����ĳ����ڴ�������ͷ�
#define NORMAL_NEW(className, ptr, ...)			NEW_MEMORY(className, ptr, true, __VA_ARGS__)
#define NORMAL_NEW_ARRAY(className, count, ptr)	NEW_MEMORY_ARRAY(className, count, ptr, true)
#define NORMAL_DELETE(ptr)						DELETE_MEMORY(ptr, true)
#define NORMAL_DELETE_ARRAY(ptr)				DELETE_MEMORY_ARRAY(ptr, true)
#else
// �����ڴ�Ϸ����ĳ����ڴ�������ͷ�
#define NORMAL_NEW(className, ptr, ...)			NEW_MEMORY(className, ptr, false, __VA_ARGS__)
#define NORMAL_NEW_ARRAY(className, count, ptr)	NEW_MEMORY_ARRAY(className, count, ptr, false)
#define NORMAL_DELETE(ptr)						DELETE_MEMORY(ptr, false)
#define NORMAL_DELETE_ARRAY(ptr)				DELETE_MEMORY_ARRAY(ptr, false)
#endif

#ifdef TRACE_MEMORY
#include "txShareMemoryServer.h"
// ע������!!!
// ��Ҫȷ��MemeryTrace����ֻ��һ��
// ���еĶ��ڴ�������ͷŶ�����ʹ��TRACE_NEW,TRACE_NEW_ARRAY,TRACE_DELETE,TRACE_DELETE_ARRAY
// TRACE_NEW������ڴ�ֻ����TRACE_DELETE�ͷ�,TRACE_NEW_ARRAY������ڴ�ֻ����TRACE_DELETE_ARRAY�ͷ�
// ��Ȼ������new��deleteһ�����﷨ʹ��TRACE��غ�,���ǲ���ֱ����return�����TRACE��غ�
// TRACE��غ�Ҳ�����ڹ��캯����ʼ���б���ʹ��

struct MemoryInfo
{
	MemoryInfo(int s, std::string f, int l, std::string t)
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

struct MemoryType
{
	MemoryType(const std::string& t = "")
	:
	type(t),
	count(0),
	size(0)
	{}
	MemoryType(const std::string& t, int c, int s)
		:
		type(t),
		count(c),
		size(s)
	{}
	std::string type;
	int count;
	int size;
};
const int MAX_COUNT = 1000;
class txMemoryTrace
{
public:
	txMemoryTrace();
	virtual ~txMemoryTrace();
	// writeOrDebugΪ�����ʾ����Ϣд�빲���ڴ�,Ȼ��ʹ��MemeryViewer�鿴,Ϊ��ʱ��ʾ�ڿ���̨��ʾ�ڴ���Ϣ
	void init(bool writeOrDebug);
	static DWORD WINAPI debugMemoryTrace(LPVOID lpParameter);
#ifdef WRITE_FILE
	static DWORD WINAPI writeMemoryTrace(LPVOID lpParameter);
#endif
	static void insertPtr(void* ptr, MemoryInfo info);
	static void erasePtr(void* ptr);
	static void setIgnoreClass(txSet<std::string>& classList){mIgnoreClass = classList;}
	static void setIgnoreClassKeyword(txSet<std::string>& classList){mIgnoreClassKeyword = classList;}
	static void setShowOnlyDetailClass(txSet<std::string>& classList){mShowOnlyDetailClass = classList;}
	static void setShowOnlyStatisticsClass(txSet<std::string>& classList){mShowOnlyStatisticsClass = classList;}
	static void setShowDetail(bool show){mShowDetail = show;}
	static void setShowStatistics(bool show){mShowStatistics = show;}
	static void setShowAll(bool show){mShowAll = show;}
protected:
	// �ڴ���������Ϣ��
	static txMap<void*, MemoryInfo> mMemoryInfo;
	// �ڴ�ͳ����Ϣ��, first��������,second��first�Ǹ����������ڴ����,second�Ǹ�����ռ�����ڴ��С,��λ���ֽ�
	static txMap<std::string, MemoryType> mMemoryType;
	// ����ʾ���б������͵��ڴ���ϸ��Ϣ�Լ�ͳ����Ϣ
	static txSet<std::string> mIgnoreClass;
	// �����ϸ��Ϣ�Լ�ͳ����Ϣ�е����Ͱ������б��еĹؼ���,����ʾ
	static txSet<std::string> mIgnoreClassKeyword;
	// ֻ��ʾ���б������͵��ڴ���ϸ��Ϣ,������б�Ϊ��,��ȫ����ʾ
	static txSet<std::string> mShowOnlyDetailClass;
	// ֻ��ʾ���б������͵��ڴ�ͳ����Ϣ,������б�Ϊ��,��ȫ����ʾ
	static txSet<std::string> mShowOnlyStatisticsClass;
	// �Ƿ���ʾ����Ϣ�����ϸ����
	static bool mShowDetail;
	// �Ƿ���ʾ�ڴ�ͳ����Ϣ
	static bool mShowStatistics;
	// �Ƿ���ʾ�ڴ��ܸ���
	static bool mShowTotalCount;
	static int mInstanceCount;
	static bool mShowAll;
	static txThreadLock mInfoLock;
	HANDLE mThread;
	static txShareMemoryServer* mShareMemoryServer;
#ifdef WRITE_FILE
	static txMap<std::string, int> mMemoryTypeIndex;
	static MemoryType mMemoryList[MAX_COUNT];
	static int mMemoryCount;
#endif
};
#endif

#ifdef TRACE_MEMORY
// �����޲λ��ߴ��ι�������ڴ�
#define TRACE_NEW(className, ptr, ...)		\
NORMAL_NEW(className, ptr, __VA_ARGS__)		\
if(ptr != NULL)								\
{											\
	txMemoryTrace::insertPtr(ptr, MemoryInfo(sizeof(className), __FILE__, __LINE__, typeid(className).name())); \
}

// �����޲ι��������߻����������������ڴ�
#define TRACE_NEW_ARRAY(className, count, ptr)  \
NORMAL_NEW_ARRAY(className, count, ptr)			\
if(ptr != NULL)									\
{												\
	txMemoryTrace::insertPtr(ptr, MemoryInfo(sizeof(className)* count, __FILE__, __LINE__, typeid(className).name())); \
}

// �ͷ�TRACE_NEW������ڴ�
#define TRACE_DELETE(ptr)			\
txMemoryTrace::erasePtr((void*)ptr);\
NORMAL_DELETE(ptr)

// �ͷ�TRACE_NEW_ARRAY������ڴ�
#define TRACE_DELETE_ARRAY(ptr)		\
txMemoryTrace::erasePtr((void*)ptr);\
NORMAL_DELETE_ARRAY(ptr)
#else
#define TRACE_NEW(className, ptr, ...)			NORMAL_NEW(className, ptr, __VA_ARGS__)
#define TRACE_NEW_ARRAY(className, count, ptr)  NORMAL_NEW_ARRAY(className, count, ptr)
#define TRACE_DELETE(ptr)						NORMAL_DELETE(ptr)
#define TRACE_DELETE_ARRAY(ptr)					NORMAL_DELETE_ARRAY(ptr)
#endif

#endif