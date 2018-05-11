#ifndef _TX_COLLISION_BOX_PROPERTY_H_
#define _TX_COLLISION_BOX_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(txComponentCollisionBox);

//---------------------------------------------------------------------------------------------------------
// ���ӿ��
class txCollisionBoxPropertyBoxWidth : public txComponentCollisionBoxProperty<txFloatProperty, float>
{
public:
	txCollisionBoxPropertyBoxWidth();
	virtual void setRealValue(const float& value, txComponentCollisionBox* component);
	virtual float getRealValue(txComponentCollisionBox* component);
};

//---------------------------------------------------------------------------------------------------------
// ���Ӹ߶�
class txCollisionBoxPropertyBoxHeight : public txComponentCollisionBoxProperty<txFloatProperty, float>
{
public:
	txCollisionBoxPropertyBoxHeight();
	virtual void setRealValue(const float& value, txComponentCollisionBox* component);
	virtual float getRealValue(txComponentCollisionBox* component);
};

//---------------------------------------------------------------------------------------------------------
// ���ӳ���
class txCollisionBoxPropertyBoxLength : public txComponentCollisionBoxProperty<txFloatProperty, float>
{
public:
	txCollisionBoxPropertyBoxLength();
	virtual void setRealValue(const float& value, txComponentCollisionBox* component);
	virtual float getRealValue(txComponentCollisionBox* component);
};

#endif