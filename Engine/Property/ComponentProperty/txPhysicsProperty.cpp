#include "txPhysicsProperty.h"

//---------------------------------------------------------------------------------------------
txPhysicsPropertyShowAllGeometry::txPhysicsPropertyShowAllGeometry()
{
	mPropertyName = "ShowAllGeometry";
	mDescription = "�Ƿ���ʾ���е���ײ��";
}
void txPhysicsPropertyShowAllGeometry::setRealValue(const bool& value, txComponentPhysics* component)
{
	component->setShowAllGeometry(value);
}
bool txPhysicsPropertyShowAllGeometry::getRealValue(txComponentPhysics* component)
{
	return component->getShowAllGeometry();
}

//---------------------------------------------------------------------------------------------
txPhysicsPropertyKinematic::txPhysicsPropertyKinematic()
{
	mPropertyName = "Kinematic";
	mDescription = "�Ƿ�Ϊ����ѧ״̬,����ѧ״̬�²����������ģ��,���ǻ�����ײ���";
}
void txPhysicsPropertyKinematic::setRealValue(const bool& value, txComponentPhysics* component)
{
	component->setKinematic(value);
}
bool txPhysicsPropertyKinematic::getRealValue(txComponentPhysics* component)
{
	return component->getKinematic();
}

//---------------------------------------------------------------------------------------------
txPhysicsPropertyLinearVelocity::txPhysicsPropertyLinearVelocity()
{
	mPropertyName = "LinearVelocity";
	mDescription = "��������ٶ�";
}
void txPhysicsPropertyLinearVelocity::setRealValue(const VECTOR3& value, txComponentPhysics* component)
{
	component->setLinearVelocity(value);
}
VECTOR3 txPhysicsPropertyLinearVelocity::getRealValue(txComponentPhysics* component)
{
	return component->getLinearVelocity();
}

//---------------------------------------------------------------------------------------------
txPhysicsPropertyAngularVelocity::txPhysicsPropertyAngularVelocity()
{
	mPropertyName = "AngularVelocity";
	mDescription = "����Ľ��ٶ�,�ֱ�����X��,Y��,Z��ת�����ٶ�,������";
}
void txPhysicsPropertyAngularVelocity::setRealValue(const VECTOR3& value, txComponentPhysics* component)
{
	component->setAngularVelocity(value);
}
VECTOR3 txPhysicsPropertyAngularVelocity::getRealValue(txComponentPhysics* component)
{
	return component->getAngularVelocity();
}

//---------------------------------------------------------------------------------------------
txPhysicsPropertyConstantForce::txPhysicsPropertyConstantForce()
{
	mPropertyName = "ConstantForce";
	mDescription = "����ĺ㶨����,��һֱ����������";
}
void txPhysicsPropertyConstantForce::setRealValue(const VECTOR3& value, txComponentPhysics* component)
{
	component->setConstantForce(value);
}
VECTOR3 txPhysicsPropertyConstantForce::getRealValue(txComponentPhysics* component)
{
	return component->getConstantForce();
}

//---------------------------------------------------------------------------------------------
txPhysicsPropertyInstantForce::txPhysicsPropertyInstantForce()
{
	mPropertyName = "InstantForce";
	mDescription = "�����˲������,ֻ�Ǹ������һ��˲�������,����һֱ����������";
}
void txPhysicsPropertyInstantForce::setRealValue(const VECTOR3& value, txComponentPhysics* component)
{
	component->setInstantForce(value);
}
VECTOR3 txPhysicsPropertyInstantForce::getRealValue(txComponentPhysics* component)
{
	return component->getInstantForce();
}

//---------------------------------------------------------------------------------------------
txPhysicsPropertyConstantTorque::txPhysicsPropertyConstantTorque()
{
	mPropertyName = "ConstantTorque";
	mDescription = "����ĺ㶨Ť��,��һֱ����������";
}
void txPhysicsPropertyConstantTorque::setRealValue(const VECTOR3& value, txComponentPhysics* component)
{
	component->setConstantTorque(value);
}
VECTOR3 txPhysicsPropertyConstantTorque::getRealValue(txComponentPhysics* component)
{
	return component->getConstantTorque();
}

//---------------------------------------------------------------------------------------------
txPhysicsPropertyInstantTorque::txPhysicsPropertyInstantTorque()
{
	mPropertyName = "InstantTorque";
	mDescription = "�����˲��Ť��,ֻ�Ǹ�����һ��˲���Ť��,����һֱ����������";
}
void txPhysicsPropertyInstantTorque::setRealValue(const VECTOR3& value, txComponentPhysics* component)
{
	component->setInstantTorque(value);
}
VECTOR3 txPhysicsPropertyInstantTorque::getRealValue(txComponentPhysics* component)
{
	return component->getInstantTorque();
}