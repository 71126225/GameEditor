#include "txCollisionCapsuleProperty.h"

txCollisionCapsulePropertyCenterCylinderHeight::txCollisionCapsulePropertyCenterCylinderHeight()
{
	mReceiverType = TOSTRING(txComponentCollisionCapsule);
	mPropertyName = "CenterCylinderHeight";
	mDescription = "�м�Բ����߶�";
}
void txCollisionCapsulePropertyCenterCylinderHeight::setRealValue(const float& value, txComponentCollisionCapsule* component)
{
	component->setCylinderHeight(value);
}
float txCollisionCapsulePropertyCenterCylinderHeight::getRealValue(txComponentCollisionCapsule* component)
{
	return component->getCylinderHeight();
}

//----------------------------------------------------------------------------------------------------------------
txCollisionCapsulePropertyHalfSphereRadius::txCollisionCapsulePropertyHalfSphereRadius()
{
	mReceiverType = TOSTRING(txComponentCollisionCapsule);
	mPropertyName = "HalfSphereRadius";
	mDescription = "���˰�����뾶";
}
void txCollisionCapsulePropertyHalfSphereRadius::setRealValue(const float& value, txComponentCollisionCapsule* component)
{
	component->setRadius(value);
}
float txCollisionCapsulePropertyHalfSphereRadius::getRealValue(txComponentCollisionCapsule* component)
{
	return component->getRadius();
}