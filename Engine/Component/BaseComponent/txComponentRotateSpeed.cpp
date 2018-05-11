#include "txComponentHeader.h"
#include "txRotateSpeedProperty.h"
#include "txNode.h"

void txComponentRotateSpeed::initProperty()
{
	txComponent::initProperty();
	addProperty<txRotateSpeedPropertyRotateSpeed>();
	addProperty<txRotateSpeedPropertyRotateAcceleration>();
	addProperty<txRotateSpeedPropertyRotateSpeedPlayState>();
}

void txComponentRotateSpeed::update(float elapsedTime)
{
	if (mPlayState == PS_PLAY && (!txMath::isFloatZero(txMath::getLength(mRotateSpeed)) || !txMath::isFloatZero(txMath::getLength(mRotateAcceleration))))
	{
		mCurRotation += mRotateSpeed * elapsedTime;
		mRotateSpeed += mRotateAcceleration * elapsedTime;
		applyRotation(txMath::eulerAngleToMatrix3(mCurRotation));
	}
	txComponent::update(elapsedTime);
}

void txComponentRotateSpeed::startRotateSpeed(const VECTOR3& startAngle, const VECTOR3& rotateSpeed, const VECTOR3& rotateAcceleration)
{
	mPlayState = PS_PLAY;
	mCurRotation = startAngle;
	mRotateSpeed = rotateSpeed;
	mRotateAcceleration = rotateAcceleration;
	applyRotation(txMath::eulerAngleToMatrix3(mCurRotation));
	// ����ٶȺͼ��ٶȶ�Ϊ0,��ֹͣ��ת
	if (txMath::isVector3Zero(rotateSpeed) && txMath::isVector3Zero(rotateAcceleration))
	{
		setActive(false);
	}
}

void txComponentRotateSpeed::applyRotation(const MATRIX3& rot)
{
	if (mComponentOwner != NULL)
	{
		// ����ǵ�һ�����,��Ӱ�����е�ģ�����
		if (mParent == NULL)
		{
			txComponentTransform* transform = static_cast<txComponentTransform*>(mComponentOwner->getFirstActiveComponent(TOSTRING(txComponentTransform)));
			transform->setRotation(rot, true);
		}
		// ���ӵ�и����,��ֻӰ�츸���
		else
		{
			mParent->getNode()->SetRotationMatrix(rot, true);
		}
	}
}