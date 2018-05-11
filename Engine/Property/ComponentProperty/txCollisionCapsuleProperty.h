#ifndef _TX_COLLISION_CAPSULE_PROPERTY_H_
#define _TX_COLLISION_CAPSULE_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(txComponentCollisionCapsule);

// �м�Բ����ĸ߶�
class txCollisionCapsulePropertyCenterCylinderHeight : public txComponentCollisionCapsuleProperty<txFloatProperty, float>
{
public:
	txCollisionCapsulePropertyCenterCylinderHeight();
	virtual void setRealValue(const float& value, txComponentCollisionCapsule* component);
	virtual float getRealValue(txComponentCollisionCapsule* component);
};

//---------------------------------------------------------------------------------------------------------
// ���˰�����뾶
class txCollisionCapsulePropertyHalfSphereRadius : public txComponentCollisionCapsuleProperty<txFloatProperty, float>
{
public:
	txCollisionCapsulePropertyHalfSphereRadius();
	virtual void setRealValue(const float& value, txComponentCollisionCapsule* component);
	virtual float getRealValue(txComponentCollisionCapsule* component);
};

#endif