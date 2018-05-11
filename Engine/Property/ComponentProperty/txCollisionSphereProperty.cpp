#include "txCollisionSphereProperty.h"

txCollisionSpherePropertySphereRadius::txCollisionSpherePropertySphereRadius()
{
	mReceiverType = TOSTRING(txComponentCollisionSphere);
	mPropertyName = "SphereRadius";
	mDescription = "����뾶";
}
void txCollisionSpherePropertySphereRadius::setRealValue(const float& value, txComponentCollisionSphere* component)
{
	component->setRadius(value);
}
float txCollisionSpherePropertySphereRadius::getRealValue(txComponentCollisionSphere* component)
{
	return component->getRadius();
}