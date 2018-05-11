#include "txThreadLock.h"

void txThreadLock::waitForUnlock(const char* file, const int& line, const LOCK_TYPE& read, const bool& showDebug)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	int threadID = GetCurrentThreadId();
#elif RUN_PLATFORM == PLATFORM_ANDROID
	int threadID = 0;
#endif
	// �ݲ����ֶ�������д����
	// ֻ�е�ǰ��д����������Ҫ��д����ʱ�Ų���Ҫ�ȴ�����,�����������Ҫ�ȴ�����
	//if (read != LT_READ || mRead != LT_READ)
	{
		WaitForSingleObject(mMutex, INFINITE);
		//while (mLock)
		//{
		//}
		// ��������������
		//mLock = true;
	}
	//mLock = true;
	memset((char*)mFile, 0, 256);
	memcpy((char*)mFile, file, strlen(file));
	mLine = line;
	mThreadID = threadID;
	mRead = read;
	//	if (read == LT_READ)
	//	{
	//		++mReadLockCount;
	//		if (mReadLockCount <= 0 && mShowLockError < 2)
	//		{
	//			++mShowLockError;
	//			GAME_ERROR("error : read lock count error! read lock : %d", mReadLockCount);
	//			ENGINE_INFO("lock : %s, %d, read : %s", mFile.c_str(), mLine, mRead == LT_READ ? "true" : "false");
	//		}
	//	}
}

void txThreadLock::unlock(const LOCK_TYPE& read, const bool& showDebug)
{
	// ��ȡ����ʱ,����ֻ�Ǽ�����������
	//	if (read == LT_READ)
	//	{
	//		--mReadLockCount;
	//		mLock = mReadLockCount > 0;
	//		if (mReadLockCount < 0 && mShowLockError < 2)
	//		{
	//			++mShowLockError;
	//			GAME_ERROR("error : thread read lock error! read lock : %d", mReadLockCount);
	//			LOG_INFO("lock : %s, %d, read : %s", mFile.c_str(), mLine, mRead == LT_READ ? "true" : "false");
	//		}
	//	}
	//	// д������ʱ,���ڲ��ɶ�д,���Կ���ȫ����
	//	else if (read == LT_WRITE)
	{
		ReleaseMutex(mMutex);
		//mLock = false;
	}
}
