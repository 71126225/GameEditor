#ifndef _TX_THREAD_LOCK_H_
#define _TX_THREAD_LOCK_H_

#include "EngineDefine.h"

#define LOCK(l) l.waitForUnlock(__FILE__, __LINE__)
#define UNLOCK(l) l.unlock()

class txThreadLock
{
public:
	txThreadLock()
	{
		mLock = 0;
		mLine = 0;
		mThreadID = 0;
	}
	~txThreadLock()
	{
		;
	}
	void waitForUnlock(const char* file, const int& line);
	void unlock();
public:
	volatile unsigned int mLock;	// 1��ʾ����,0��ʾδ����
	volatile char mFile[256];		// �������ļ���
	volatile int mLine;				// �������к�
	volatile int mThreadID;			// �����̵߳�ID
};

#endif