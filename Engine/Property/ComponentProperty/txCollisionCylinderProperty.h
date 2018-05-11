#ifndef _TX_COLLISION_CYLINDER_PROPERTY_H_
#define _TX_COLLISION_CYLINDER_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(txComponentCollisionCylinder)

// �м�Բ����ĸ߶�
class txCollisionCylinderPropertyCylinderHeight : public txComponentCollisionCylinderProperty<txFloatProperty, float>
{
public:
	txCollisionCylinderPropertyCylinderHeight();
	virtual void setRealValue(const float& value, txComponentCollisionCylinder* component);
	virtual float getRealValue(txComponentCollisionCylinder* component);
};

//---------------------------------------------------------------------------------------------------------
// ���˰�����뾶
class txCollisionCylinderPropertyBottomRadius : public txComponentCollisionCylinderProperty<txFloatProperty, float>
{
public:
	txCollisionCylinderPropertyBottomRadius();
	virtual void setRealValue(const float& value, txComponentCollisionCylinder* component);
	virtual float getRealValue(txComponentCollisionCylinder* component);
};

#endif