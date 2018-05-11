#include "txMathUtility.h"

#include "CameraLinkerSwitchLinear.h"
#include "CameraLinker.h"

void CameraLinkerSwitchLinear::init(const VECTOR3& origin, const VECTOR3& target, const float& speed)
{
	CameraLinkerSwitch::init(origin, target, speed);
	mMovedDistance = 0.0f;
	mDistance = txMath::getLength(mOriginRelative - mTargetRelative);
	mDirection = txMath::normalize(mTargetRelative - mOriginRelative);
}

void CameraLinkerSwitchLinear::update(float elapsedTime)
{
	if (mParentLinker == NULL)
	{
		return;
	}
	mMovedDistance += mSpeed * elapsedTime;
	if (mMovedDistance < mDistance)
	{
		VECTOR3 newPos = mOriginRelative + mDirection * mMovedDistance;
		mParentLinker->setRelativePosition(newPos);
	}
	else
	{
		// ����Ѿ�������ת������,������У��λ�õ�ָ����,����֪ͨ������ת�����
		mMovedDistance = mDistance;
		mParentLinker->setRelativePosition(mTargetRelative);
		mParentLinker->notifyFinishSwitching(this);
	}
}