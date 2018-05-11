#include "txRotateSpeedProperty.h"

txRotateSpeedPropertyRotateSpeed::txRotateSpeedPropertyRotateSpeed()
{
	mPropertyName = "RotateSpeed";
	mDescription = "��ת�ٶ�";
}
void txRotateSpeedPropertyRotateSpeed::setRealValue(const VECTOR3& value, txComponentRotateSpeed* component)
{
	component->setRotateSpeed(value);
}
VECTOR3 txRotateSpeedPropertyRotateSpeed::getRealValue(txComponentRotateSpeed* component)
{
	return component->getRotateSpeed();
}

//-------------------------------------------------------------------------------------------------------------
txRotateSpeedPropertyRotateAcceleration::txRotateSpeedPropertyRotateAcceleration()
{
	mPropertyName = "RotateAcceleration";
	mDescription = "��ת���ٶ�";
}
void txRotateSpeedPropertyRotateAcceleration::setRealValue(const VECTOR3& value, txComponentRotateSpeed* component)
{
	component->setRotateAcceleration(value);
}
VECTOR3 txRotateSpeedPropertyRotateAcceleration::getRealValue(txComponentRotateSpeed* component)
{
	return component->getRotateAcceleration();
}

//-------------------------------------------------------------------------------------------------------------
txRotateSpeedPropertyRotateSpeedPlayState::txRotateSpeedPropertyRotateSpeedPlayState()
{
	mPropertyName = "RotatSpeedPlayState";
	mDescription = "��ת״̬";
}
void txRotateSpeedPropertyRotateSpeedPlayState::setRealValue(const PLAY_STATE& value, txComponentRotateSpeed* component)
{
	component->setPlayState(value);
}
PLAY_STATE txRotateSpeedPropertyRotateSpeedPlayState::getRealValue(txComponentRotateSpeed* component)
{
	return component->getPlayState();
}