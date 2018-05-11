#include "txRotateProperty.h"

txRotatePropertyTargetAngle::txRotatePropertyTargetAngle()
{
	mPropertyName = "TargetAngle";
	mDescription = "Ŀ����ת�Ƕ�";
}

void txRotatePropertyTargetAngle::setRealValue(const VECTOR3& value, txComponentRotate* component)
{
	component->setTargetAngle(value);
}

VECTOR3 txRotatePropertyTargetAngle::getRealValue(txComponentRotate* component)
{
	return component->getTargetAngle();
}

//----------------------------------------------------------------------------------------------------
txRotatePropertyStartAngle::txRotatePropertyStartAngle()
{
	mPropertyName = "StartAngle";
	mDescription = "��ʼ��ת�Ƕ�";
}
void txRotatePropertyStartAngle::setRealValue(const VECTOR3& value, txComponentRotate* component)
{
	component->setStartAngle(value);
}
VECTOR3 txRotatePropertyStartAngle::getRealValue(txComponentRotate* component)
{
	return component->getStartAngle();
}