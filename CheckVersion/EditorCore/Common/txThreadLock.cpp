#include "txThreadLock.h"

void txThreadLock::waitForUnlock(const char* file, const int& line)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	int threadID = GetCurrentThreadId();
#elif RUN_PLATFORM == PLATFORM_ANDROID
	int threadID = 0;
#endif
	// �ݲ����ֶ�������д����
	while (InterlockedExchange(&mLock, 1) == 1)
	{
		Sleep(0);
	}
	memset((char*)mFile, 0, 256);
	memcpy((char*)mFile, file, strlen(file));
	mLine = line;
	mThreadID = threadID;
}

void txThreadLock::unlock()
{
	InterlockedExchange(&mLock, 0);
}
