#include "txAlphaProperty.h"

//---------------------------------------------------------------------------------------------------------
txAlphaPropertyStartAlpha::txAlphaPropertyStartAlpha()
{
	mReceiverType = TOSTRING(txComponentAlpha);
	mPropertyName = "StartAlpha";
	mDescription = "��ʼ͸����";
}
void txAlphaPropertyStartAlpha::setRealValue(const float& value, txComponentAlpha* component)
{
	component->setStartAlpha(value);
}
float txAlphaPropertyStartAlpha::getRealValue(txComponentAlpha* component)
{
	return component->getStartAlpha();
}

//---------------------------------------------------------------------------------------------------------
txAlphaPropertyTargetAlpha::txAlphaPropertyTargetAlpha()
{
	mReceiverType = TOSTRING(txComponentAlpha);
	mPropertyName = "TargetAlpha";
	mDescription = "Ŀ��͸����";
}
void txAlphaPropertyTargetAlpha::setRealValue(const float& value, txComponentAlpha* component)
{
	component->setTargetAlpha(value);
}
float txAlphaPropertyTargetAlpha::getRealValue(txComponentAlpha* component)
{
	return component->getTargetAlpha();
}