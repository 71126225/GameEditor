#include "txTransformProperty.h"

//---------------------------------------------------------------------------------------------------------------------------------------------
txTransformPropertyPosition::txTransformPropertyPosition()
{
	mPropertyName = "Position";
	mDescription = "λ��";
}
void txTransformPropertyPosition::setRealValue(const VECTOR3& value, txComponentTransform* component)
{
	component->setPosition(value);
}
VECTOR3 txTransformPropertyPosition::getRealValue(txComponentTransform* component)
{
	return component->getPosition();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
txTransformPropertyRotation::txTransformPropertyRotation()
{
	mPropertyName = "Rotation";
	mDescription = "��ת";
}
void txTransformPropertyRotation::setRealValue(const VECTOR3& value, txComponentTransform* component)
{
	component->setRotation(txMath::eulerAngleToMatrix3(value), true);
}
VECTOR3 txTransformPropertyRotation::getRealValue(txComponentTransform* component)
{
	return txMath::matrix3ToEulerAngle(component->getRotation());
}

//---------------------------------------------------------------------------------------------------------------------------------------------
txTransformPropertyScale::txTransformPropertyScale()
{
	mPropertyName = "Scale";
	mDescription = "����";
}
void txTransformPropertyScale::setRealValue(const VECTOR3& value, txComponentTransform* component)
{
	component->setScale(value);
}
VECTOR3 txTransformPropertyScale::getRealValue(txComponentTransform* component)
{
	return component->getScale();
}