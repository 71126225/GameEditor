#include "txCollisionPlaneProperty.h"

txCollisionPlanePropertyPlaneWidthX::txCollisionPlanePropertyPlaneWidthX()
{
	mReceiverType = TOSTRING(txComponentCollisionPlane);
	mPropertyName = "PlaneWidthX";
	mDescription = "ƽ��X�����Ͽ��";
}
void txCollisionPlanePropertyPlaneWidthX::setRealValue(const float& value, txComponentCollisionPlane* component)
{
	component->setWidthX(value);
}
float txCollisionPlanePropertyPlaneWidthX::getRealValue(txComponentCollisionPlane* component)
{
	return component->getWidthX();
}

//------------------------------------------------------------------------------------------------------------
txCollisionPlanePropertyPlaneLengthZ::txCollisionPlanePropertyPlaneLengthZ()
{
	mReceiverType = TOSTRING(txComponentCollisionPlane);
	mPropertyName = "PlaneLengthZ";
	mDescription = "ƽ��Z�����ϳ���";
}
void txCollisionPlanePropertyPlaneLengthZ::setRealValue(const float& value, txComponentCollisionPlane* component)
{
	component->setLengthZ(value);
}
float txCollisionPlanePropertyPlaneLengthZ::getRealValue(txComponentCollisionPlane* component)
{
	return component->getLengthZ();
}