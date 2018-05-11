#include "txCollisionProperty.h"

//---------------------------------------------------------------------------------------------------------
txCollisionPropertyGeometryMass::txCollisionPropertyGeometryMass()
{
	mReceiverType = TOSTRING(txComponentCollision);
	mPropertyName = "GeometryMass";
	mDescription = "��ײ������";
}
void txCollisionPropertyGeometryMass::setRealValue(const float& value, txComponentCollision* component)
{
	component->setMass(value);
}
float txCollisionPropertyGeometryMass::getRealValue(txComponentCollision* component)
{
	return component->getMass();
}

//---------------------------------------------------------------------------------------------------------
txCollisionPropertyGeometryShowGeometry::txCollisionPropertyGeometryShowGeometry()
{
	mReceiverType = TOSTRING(txComponentCollision);
	mPropertyName = "ShowGeometry";
	mDescription = "�Ƿ���ʾ��ײ��";
}
void txCollisionPropertyGeometryShowGeometry::setRealValue(const bool& value, txComponentCollision* component)
{
	component->setShowGeometry(value);
}
bool txCollisionPropertyGeometryShowGeometry::getRealValue(txComponentCollision* component)
{
	return component->getShowGeometry();
}

//---------------------------------------------------------------------------------------------------------
txCollisionPropertyGeometryRelativePosition::txCollisionPropertyGeometryRelativePosition()
{
	mReceiverType = TOSTRING(txComponentCollision);
	mPropertyName = "GeometryRelativePosition";
	mDescription = "��ײ������ڸ����λ��";
}
void txCollisionPropertyGeometryRelativePosition::setRealValue(const VECTOR3& value, txComponentCollision* component)
{
	component->setRelativePosition(value);
}
VECTOR3 txCollisionPropertyGeometryRelativePosition::getRealValue(txComponentCollision* component)
{
	return component->getRelativePosition();
}

//---------------------------------------------------------------------------------------------------------
txCollisionPropertyGeometryRelativeRotation::txCollisionPropertyGeometryRelativeRotation()
{
	mReceiverType = TOSTRING(txComponentCollision);
	mPropertyName = "GeometryRelativeRotation";
	mDescription = "��ײ������ڸ������תŷ����";
}
void txCollisionPropertyGeometryRelativeRotation::setRealValue(const VECTOR3& value, txComponentCollision* component)
{
	component->setRelativeRotation(txMath::eulerAngleToMatrix3(value));
}
VECTOR3 txCollisionPropertyGeometryRelativeRotation::getRealValue(txComponentCollision* component)
{
	return txMath::matrix3ToEulerAngle(component->getRelativeRotation());
}