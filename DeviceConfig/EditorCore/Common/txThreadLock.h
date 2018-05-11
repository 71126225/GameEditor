#ifndef _TX_THREAD_LOCK_H_
#define _TX_THREAD_LOCK_H_

#include "EngineDefine.h"

// �̼߳�������
enum LOCK_TYPE
{
	LT_READ,		// ��������Ҫ���ж�ȡ
	LT_WRITE,		// ��������Ҫ����д��
};
#define LOCK(l, flag) l.waitForUnlock(__FILE__, __LINE__, flag)
#define UNLOCK(l, flag) l.unlock(flag)

class txThreadLock
{
public:
	txThreadLock()
	{
		mLock = false;
		mLine = 0;
		mRead = LT_WRITE;
		mReadLockCount = 0;
		mThreadID = 0;
		mShowLockError = 0;
	}
	void waitForUnlock(const char* file, const int& line, const LOCK_TYPE& read, const bool& showDebug = false);
	void unlock(const LOCK_TYPE& read, const bool& showDebug = false);
public:
	volatile bool mLock;			// �Ƿ�����
	volatile char mFile[256];		// �������ļ���
	volatile int mLine;				// �������к�
	volatile int mThreadID;			// �����̵߳�ID
	volatile LOCK_TYPE mRead;		// ����������Ҫ��(true)����д(false)
	volatile int mReadLockCount;	// �������Ĵ���,���������Ĵ���Ϊ0ʱ������ȫ����
	volatile int mShowLockError;
};

#endif