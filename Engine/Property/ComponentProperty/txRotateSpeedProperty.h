#ifndef _TX_ROTATE_SPEED_PROPERTY_H_
#define _TX_ROTATE_SPEED_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(txComponentRotateSpeed)

// ��ת�ٶ�
class txRotateSpeedPropertyRotateSpeed : public txComponentRotateSpeedProperty<txVector3Property, VECTOR3>
{
public:
	txRotateSpeedPropertyRotateSpeed();
	virtual void setRealValue(const VECTOR3& value, txComponentRotateSpeed* component);
	virtual VECTOR3 getRealValue(txComponentRotateSpeed* component);
};

//---------------------------------------------------------------------------------------------------------
// ��ת���ٶ�
class txRotateSpeedPropertyRotateAcceleration : public txComponentRotateSpeedProperty<txVector3Property, VECTOR3>
{
public:
	txRotateSpeedPropertyRotateAcceleration();
	virtual void setRealValue(const VECTOR3& value, txComponentRotateSpeed* component);
	virtual VECTOR3 getRealValue(txComponentRotateSpeed* component);
};

//---------------------------------------------------------------------------------------------------------
// ��ת���ٶ�
class txRotateSpeedPropertyRotateSpeedPlayState : public txComponentRotateSpeedProperty<txEnumProperty<PLAY_STATE>, PLAY_STATE>
{
public:
	txRotateSpeedPropertyRotateSpeedPlayState();
	virtual void setRealValue(const PLAY_STATE& value, txComponentRotateSpeed* component);
	virtual PLAY_STATE getRealValue(txComponentRotateSpeed* component);
};

#endif