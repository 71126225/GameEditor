#ifndef _TIME_LOCK_H_
#define _TIME_LOCK_H_

#include "txEngineDefine.h"

class TimeLock
{
public:
	TimeLock(const long& frameTimeMS)
	{
		mFrameTimeMS = frameTimeMS;
		mLastTime = 0;
		mForceSleep = 0;
	}
	void update();
	void setForceSleep(const long& forceTimeMS)
	{
		mForceSleep = forceTimeMS;
	}
protected:
	long mFrameTimeMS;
	long mLastTime;
	long mForceSleep;          // ÿ֡����ͣʱ��ʱǿ����ͣ�ĺ�����,�����̵߳�֡������ʱ,���µ�֡����ʱ������趨�Ĺ̶���֡ʱ��ʱ,CPUռ�ù��ߵ�����
};

#endif