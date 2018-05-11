#include "txMathUtility.h"

#include "CameraLinkerSwitchCircle.h"
#include "CameraLinker.h"

void CameraLinkerSwitchCircle::init(const VECTOR3& origin, const VECTOR3& target, const float& speed)
{
	CameraLinkerSwitch::init(origin, target, speed);
	mRotatedAngle = 0.0f;
	mRotateCenter = mOriginRelative + (mTargetRelative - mOriginRelative) / 2.0f;
	mRotateCenter.y = 0.0f;
	mTotalAngle = txMath::MATH_PI;
}

void CameraLinkerSwitchCircle::update(float elapsedTime)
{
	if (mParentLinker == NULL)
	{
		return;
	}

	// ��תת����Ϊ��������,һ������ˮƽ�����ϵĻ����˶�, ��һ��������ֱ�����ϵ�ֱ���˶�
	mRotatedAngle += mSpeed * elapsedTime;
	if(mRotatedAngle >= mTotalAngle)
	{
		mRotatedAngle = mTotalAngle;
		mParentLinker->setRelativePosition(mTargetRelative);
		mParentLinker->notifyFinishSwitching(this);
	}
	else
	{
		VECTOR3 rotateVec = mOriginRelative - mRotateCenter;
		rotateVec.y = 0.0f;
		rotateVec = txMath::rotateVector3(rotateVec, mRotatedAngle);
		rotateVec += mRotateCenter;

		rotateVec.y = (mTargetRelative.y - mOriginRelative.y) * (mRotatedAngle / mTotalAngle) + mOriginRelative.y;
		mParentLinker->setRelativePosition(rotateVec);
	}
}