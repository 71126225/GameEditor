#include "txCollisionBoxProperty.h"

//---------------------------------------------------------------------------------------------------------
txCollisionBoxPropertyBoxWidth::txCollisionBoxPropertyBoxWidth()
{
	mReceiverType = TOSTRING(txComponentCollisionBox);
	mPropertyName = "BoxWidth";
	mDescription = "���ӿ��";
}
void txCollisionBoxPropertyBoxWidth::setRealValue(const float& value, txComponentCollisionBox* component)
{
	component->setWidth(value);
}
float txCollisionBoxPropertyBoxWidth::getRealValue(txComponentCollisionBox* component)
{
	return component->getWidth();
}

//---------------------------------------------------------------------------------------------------------
txCollisionBoxPropertyBoxHeight::txCollisionBoxPropertyBoxHeight()
{
	mReceiverType = TOSTRING(txComponentCollisionBox);
	mPropertyName = "BoxHeight";
	mDescription = "���Ӹ߶�";
}
void txCollisionBoxPropertyBoxHeight::setRealValue(const float& value, txComponentCollisionBox* component)
{
	component->setHeight(value);
}
float txCollisionBoxPropertyBoxHeight::getRealValue(txComponentCollisionBox* component)
{
	return component->getHeight();
}

//---------------------------------------------------------------------------------------------------------
txCollisionBoxPropertyBoxLength::txCollisionBoxPropertyBoxLength()
{
	mReceiverType = TOSTRING(txComponentCollisionBox);
	mPropertyName = "BoxLength";
	mDescription = "���ӳ���";
}
void txCollisionBoxPropertyBoxLength::setRealValue(const float& value, txComponentCollisionBox* component)
{
	component->setLength(value);
}
float txCollisionBoxPropertyBoxLength::getRealValue(txComponentCollisionBox* component)
{
	return component->getLength();
}