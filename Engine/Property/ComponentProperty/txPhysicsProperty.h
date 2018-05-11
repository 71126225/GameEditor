#ifndef _TX_PHYSICS_PROPERTY_H_
#define _TX_PHYSICS_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(txComponentPhysics)

// �Ƿ���ʾ���е���ײ��
class txPhysicsPropertyShowAllGeometry : public txComponentPhysicsProperty<txBoolProperty, bool>
{
public:
	txPhysicsPropertyShowAllGeometry();
	virtual void setRealValue(const bool& value, txComponentPhysics* component);
	virtual bool getRealValue(txComponentPhysics* component);
};

//---------------------------------------------------------------------------------------------------------
// �Ƿ�Ϊ����ѧ״̬
class txPhysicsPropertyKinematic : public txComponentPhysicsProperty<txBoolProperty, bool>
{
public:
	txPhysicsPropertyKinematic();
	virtual void setRealValue(const bool& value, txComponentPhysics* component);
	virtual bool getRealValue(txComponentPhysics* component);
};

//---------------------------------------------------------------------------------------------------------
// ��������ٶ�
class txPhysicsPropertyLinearVelocity : public txComponentPhysicsProperty<txVector3Property, VECTOR3>
{
public:
	txPhysicsPropertyLinearVelocity();
	virtual void setRealValue(const VECTOR3& value, txComponentPhysics* component);
	virtual VECTOR3 getRealValue(txComponentPhysics* component);
};

//---------------------------------------------------------------------------------------------------------
// ����Ľ��ٶ�
class txPhysicsPropertyAngularVelocity : public txComponentPhysicsProperty<txVector3Property, VECTOR3>
{
public:
	txPhysicsPropertyAngularVelocity();
	virtual void setRealValue(const VECTOR3& value, txComponentPhysics* component);
	virtual VECTOR3 getRealValue(txComponentPhysics* component);
};

//---------------------------------------------------------------------------------------------------------
// ����ĺ㶨����
class txPhysicsPropertyConstantForce : public txComponentPhysicsProperty<txVector3Property, VECTOR3>
{
public:
	txPhysicsPropertyConstantForce();
	virtual void setRealValue(const VECTOR3& value, txComponentPhysics* component);
	virtual VECTOR3 getRealValue(txComponentPhysics* component);
};

//---------------------------------------------------------------------------------------------------------
// �����˲������
class txPhysicsPropertyInstantForce : public txComponentPhysicsProperty<txVector3Property, VECTOR3>
{
public:
	txPhysicsPropertyInstantForce();
	virtual void setRealValue(const VECTOR3& value, txComponentPhysics* component);
	virtual VECTOR3 getRealValue(txComponentPhysics* component);
};

//---------------------------------------------------------------------------------------------------------
// ����ĺ㶨Ť��
class txPhysicsPropertyConstantTorque : public txComponentPhysicsProperty<txVector3Property, VECTOR3>
{
public:
	txPhysicsPropertyConstantTorque();
	virtual void setRealValue(const VECTOR3& value, txComponentPhysics* component);
	virtual VECTOR3 getRealValue(txComponentPhysics* component);
};

//---------------------------------------------------------------------------------------------------------
// �����˲��Ť��
class txPhysicsPropertyInstantTorque : public txComponentPhysicsProperty<txVector3Property, VECTOR3>
{
public:
	txPhysicsPropertyInstantTorque();
	virtual void setRealValue(const VECTOR3& value, txComponentPhysics* component);
	virtual VECTOR3 getRealValue(txComponentPhysics* component);
};

#endif