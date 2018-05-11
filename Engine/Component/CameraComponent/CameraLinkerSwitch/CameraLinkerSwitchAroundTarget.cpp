#include "txMathUtility.h"

#include "CameraLinkerSwitchAroundTarget.h"
#include "CameraLinker.h"
#include "txMovableObject.h"

CameraLinkerSwitchAroundTarget::CameraLinkerSwitchAroundTarget(const CAMERA_LINKER_SWITCH& type, CameraLinker* parentLinker)
:
CameraLinkerSwitch(type, parentLinker),
mTotalAngle(0.0f),
mRotatedAngle(0.0f),
mClockwise(true),
mDistanceDelta(0.0f),
mDistanceCurrent(0.0f)
{
	mSpeed = txMath::MATH_PI / 2.0f;
}

void CameraLinkerSwitchAroundTarget::init(const VECTOR3& origin, const VECTOR3& target, const float& speed)
{
	CameraLinkerSwitch::init(origin, target, speed);
	if(mClockwise)
	{
		mTotalAngle = txMath::getAngleFromVector(mTargetRelative) - txMath::getAngleFromVector(mOriginRelative);
		txMath::adjustAngle360(mTotalAngle);
		mSpeed = std::abs(mSpeed);
	}
	else
	{
		mTotalAngle = txMath::getAngleFromVector(mOriginRelative) - txMath::getAngleFromVector(mTargetRelative);
		txMath::adjustAngle360(mTotalAngle);
		mSpeed = -std::abs(mSpeed);
	} 
	mDistanceDelta = txMath::getLength(mTargetRelative) - txMath::getLength(mOriginRelative);
	mDistanceCurrent = 0.0f;
	mRotatedAngle = 0.0f;
}

void CameraLinkerSwitchAroundTarget::update(float elapsedTime)
{
	if (mParentLinker == NULL)
	{
		return;
	}

	mRotatedAngle += mSpeed * elapsedTime;
	// �����ٶ�
	float time = mTotalAngle / mSpeed;
	float distanceSpeed = mDistanceDelta / time;
	mDistanceCurrent += distanceSpeed * elapsedTime;

	// ˳ʱ��
	if (mClockwise)
	{
		if (mRotatedAngle >= mTotalAngle)
		{
			mRotatedAngle = mTotalAngle;
			mParentLinker->setRelativePosition(mTargetRelative);
			mParentLinker->notifyFinishSwitching(this);
			mDistanceCurrent = 0.0f;
			mRotatedAngle = 0.0f;
		}
		else
		{
			// z��������ת�����
			VECTOR3 rotateAxis = txMath::rotateVector3(mOriginRelative, mRotatedAngle);
			// ����仯
			VECTOR3 projectVec = rotateAxis;
			projectVec.y = 0;
			projectVec = txMath::normalize(projectVec);
			projectVec = projectVec * (txMath::getLength(mOriginRelative) + mDistanceCurrent);
			// �߶ȱ仯
			rotateAxis.y = (mTargetRelative.y - mOriginRelative.y) * (mRotatedAngle / mTotalAngle) + mOriginRelative.y;
			// ����ֵ
			rotateAxis.x = projectVec.x;
			rotateAxis.z = projectVec.z;

			mParentLinker->setRelativePosition(rotateAxis);
		}
	}
	// ��ʱ��
	else
	{
		if (mRotatedAngle <= mTotalAngle)
		{
			mRotatedAngle = mTotalAngle;
			mParentLinker->setRelativePosition(mTargetRelative);
			mParentLinker->notifyFinishSwitching(this);
			mDistanceCurrent = 0.0f;
			mRotatedAngle = 0.0f;
		}
		else
		{
			VECTOR3 rotateAxis = txMath::rotateVector3(mOriginRelative, mRotatedAngle);
			VECTOR3 projectVec = rotateAxis;
			projectVec.y = 0.0f;
			projectVec = txMath::normalize(projectVec);
			projectVec = projectVec * (txMath::getLength(mOriginRelative) + mDistanceCurrent);

			rotateAxis.y = (mTargetRelative.y - mOriginRelative.y) * (mRotatedAngle / mTotalAngle) + mOriginRelative.y;
			rotateAxis.x = projectVec.x;
			rotateAxis.z = projectVec.z;

			mParentLinker->setRelativePosition(rotateAxis);
		}
	}
}