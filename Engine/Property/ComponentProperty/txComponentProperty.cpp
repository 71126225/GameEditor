#include "txComponentProperty.h"

//---------------------------------------------------------------------------------------------------------------------------------------------
txComponentPropertyActive::txComponentPropertyActive()
{
	mPropertyName = "Active";
	mDescription = "������Ƿ񼤻�";
}
void txComponentPropertyActive::setRealValue(const bool& value, txComponent* component)
{
	component->setActive(value);
}
bool txComponentPropertyActive::getRealValue(txComponent* component)
{
	return component->isActive();
}