#include "txMathUtility.h"
#include "txSpring.h"

txSpring::txSpring()
:
mNormalLength(0.0f),
mCurLength(0.0f),
mSpringK(1.0f),
mObjectMass(1.0f),
mForce(0.0f),
mObjectSpeed(0.0f),
mMinLength(0.5f)
{}

void txSpring::update(float fElaspedTime)
{
	// ��������
	float elasticForce = calculateElasticForce() * -1.0f;

	// ���ٶ�
	float acceleration = (mForce + elasticForce) / mObjectMass;
	if (txMath::isFloatZero(acceleration) || (acceleration < 0.0f && mPreAcce > 0.0f) || (acceleration > 0.0f && mPreAcce < 0.0f))
	{
		mObjectSpeed = 0.0f;
		acceleration = 0.0f;
	}
	else
	{
		// �ٶ�
		mObjectSpeed += acceleration * fElaspedTime;
	}

	// ����
	mCurLength += mObjectSpeed * fElaspedTime;
	if (mCurLength <= mMinLength)
	{
		mCurLength = mMinLength;
		mObjectSpeed = 0.0f;
		acceleration = 0.0f;
	}
	mPreAcce = acceleration;
}