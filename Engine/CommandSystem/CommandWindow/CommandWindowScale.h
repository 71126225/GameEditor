#ifndef _COMMAND_WINDOW_SCALE_H_
#define _COMMAND_WINDOW_SCALE_H_

#include "txCommand.h"

class CommandWindowScale : public txCommand
{
public:
	virtual void reset()
	{
		mName = EMPTY_STRING;
		mLoop = false;
		mOnceLength = 1.0f;
		mTimeOffset = 0.0f;
		mFullOnce = false;
		mAmplitude = 1.0f;
		mStartScale = txMath::VEC2_ZERO;
		mTargetScale = txMath::VEC2_ONE;
		mDoingCallback = NULL;
		mDoingUserData = NULL;
		mDoneCallback = NULL;
		mDoneUserData = NULL;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
	void setDoingCallback(KeyFrameCallback scalCallback, void* userData)
	{
		mDoingCallback = scalCallback;
		mDoingUserData = userData;
	}
	void setDoneCallback(KeyFrameCallback scalCallback, void* userData)
	{
		mDoneCallback = scalCallback;
		mDoneUserData = userData;
	}
public:
	std::string mName;
	bool mLoop;
	float mOnceLength;					// ���ŵ���ʱ��
	float mTimeOffset;					// ���ŵ���ʼʱ��ƫ��
	bool mFullOnce;
	float mAmplitude;
	VECTOR2 mStartScale;				// ������ʼ����ֵ
	VECTOR2 mTargetScale;				// ���ڵ�Ŀ������ֵ
protected:
	KeyFrameCallback mDoingCallback;
	void* mDoingUserData;
	KeyFrameCallback mDoneCallback;
	void* mDoneUserData;
};

#endif