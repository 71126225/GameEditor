#include "ActionPropertyHSL.h"
#include "ActionWindowHSL.h"

ActionWindowHSLPropertyStartHSL::ActionWindowHSLPropertyStartHSL()
{
	mPropertyName = "StartHSL";
	mDescription = "��ʼHSLƫ��";
}
void ActionWindowHSLPropertyStartHSL::setRealValue(const VECTOR3& value, ActionWindowHSL* action)
{
	action->setStartHSL(value);
}
VECTOR3 ActionWindowHSLPropertyStartHSL::getRealValue(ActionWindowHSL* action)
{
	return action->getStartHSL();
}
//---------------------------------------------------------------------------------------------------------
ActionWindowHSLPropertyTargetHSL::ActionWindowHSLPropertyTargetHSL()
{
	mPropertyName = "TargetHSL";
	mDescription = "��ֹHSLƫ��";
}
void ActionWindowHSLPropertyTargetHSL::setRealValue(const VECTOR3& value, ActionWindowHSL* action)
{
	action->setTargetHSL(value);
}
VECTOR3 ActionWindowHSLPropertyTargetHSL::getRealValue(ActionWindowHSL* action)
{
	return action->getTargetHSL();
}