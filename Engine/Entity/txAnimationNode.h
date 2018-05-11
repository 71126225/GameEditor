#ifndef _TX_ANIMATION_NODE_H_
#define _TX_ANIMATION_NODE_H_

#include "txEngineDefine.h"

class txBone;
class txSkeleton;

// �����ͷ�Ķ���
class txAnimationNode
{
public:
	txAnimationNode() {}
	virtual ~txAnimationNode() { ClearAll(); }
	void ClearAll();
	std::pair<float*, VECTOR3*> makePositionBuffer(const int& size)
	{
		if (size <= 0)
		{
			return std::pair<float*, VECTOR3*>(NULL, NULL);
		}
		mPosTime.resize(size);
		mPosKeyFrameList.resize(size);
		return std::make_pair(&(mPosTime[0]), &(mPosKeyFrameList[0]));
	}
	
	std::pair<float*, VECTOR3*> makeScaleBuffer(const int& size)
	{
		if (size <= 0)
		{
			return std::pair<float*, VECTOR3*>(NULL, NULL);
		}
		mSclTime.resize(size);
		mSclKeyFrameList.resize(size);
		return std::make_pair(&(mSclTime[0]), &(mSclKeyFrameList[0]));
	}
	
	std::pair<float*, QUATERNION*> makeRotationBuffer(const int& size)
	{
		if (size <= 0)
		{
			return std::pair<float*, QUATERNION*>(NULL, NULL);
		}
		mRotTime.resize(size);
		mRotKeyFrameList.resize(size);
		return std::make_pair(&(mRotTime[0]), &(mRotKeyFrameList[0]));
	}

	void pushPositionKeyFrame(const float& time, const VECTOR3& pos)
	{
		mPosTime.push_back(time);
		mPosKeyFrameList.push_back(pos);
	}

	void pushScaleKeyFrame(const float& time, const VECTOR3& scale)
	{
		mSclTime.push_back(time);
		mSclKeyFrameList.push_back(scale);
	}

	void pushRotationKeyFrame(const float& time, const QUATERNION& rotation)
	{
		mRotTime.push_back(time);
		mRotKeyFrameList.push_back(rotation);
	}

	VECTOR3 getInterpolatePosition(const float& time);
	VECTOR3 getInterpolateScale(const float& time);
	QUATERNION getInterpolateRotation(const float& time);
	// �õ����ʱ��
	float getMaxKeyFrameTime();

	// ����һ���ͷ�Ĺؼ�֡
	void CopyNodeAnim(txAnimationNode* pNodeAnim);
protected:
	void getKeyFrameTimeBetween(const txVector<float>& timeList, const float& time, int& preKeyFrame, int& nextKeyFrame);

	// �洢���������ԭʼģ�͵�ÿһ���ؼ�֡�ı任,ֻ�ǵ�����ͷ��
	txVector<VECTOR3> mPosKeyFrameList;
	txVector<float> mPosTime;
	txVector<VECTOR3> mSclKeyFrameList;
	txVector<float> mSclTime;
	txVector<QUATERNION> mRotKeyFrameList;
	txVector<float> mRotTime;
};

#endif