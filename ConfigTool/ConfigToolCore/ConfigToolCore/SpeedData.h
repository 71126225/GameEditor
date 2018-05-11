#ifndef _SPEED_DATA_H_
#define _SPEED_DATA_H_

#include <map>

#include "txCommandReceiver.h"
#include "CommonDefine.h"

class SpeedData
{
public:
	static const int mDataSize = 12;// �ֽ����ݵĳ���
	unsigned char mData[mDataSize];	// �ֽ�����
	int mPower;						// ����
	int mRotateSpeed;				// ת��
	int mMachineIndex;				// ������
	float mTimeCount;				// ��ʱ��ʱ
	int mFriction;					// ����ֵ

	SpeedData()
	{
		memset(mData, 0, mDataSize);
		mPower = 0;
		mRotateSpeed = 0;
		mMachineIndex = -1;
		mTimeCount = 0.0f;
		mFriction = 1;
	}
	SpeedData(unsigned char* data, int power, int rotateSpeed, int machineIndex, float timeCount, int friction)
	{
		memcpy(mData, data, mDataSize);
		mPower = power;
		mRotateSpeed = rotateSpeed;
		mMachineIndex = machineIndex;
		mTimeCount = timeCount;
		mFriction = friction;
	}
	SpeedData& operator = (SpeedData& that)
	{
		memcpy(mData, that.mData, mDataSize);
		mPower = that.mPower;
		mRotateSpeed = that.mRotateSpeed;
		mMachineIndex = that.mMachineIndex;
		mTimeCount = that.mTimeCount;
		mFriction = that.mFriction;
		return *this;
	}
};

#endif