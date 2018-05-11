#include "ActionPropertyRotate.h"
#include "ActionWindowRotate.h"

ActionWindowRotatePropertyStartRotation::ActionWindowRotatePropertyStartRotation()
{
	mPropertyName = "StartRotation";
	mDescription = "��ʼ��תֵ";
}
void ActionWindowRotatePropertyStartRotation::setRealValue(const VECTOR3& value, ActionWindowRotate* action)
{
	action->setStartRotation(value);
}
VECTOR3 ActionWindowRotatePropertyStartRotation::getRealValue(ActionWindowRotate* action)
{
	return action->getStartRotation();
}
//---------------------------------------------------------------------------------------------------------
ActionWindowRotatePropertyTargetRotation::ActionWindowRotatePropertyTargetRotation()
{
	mPropertyName = "TargetRotation";
	mDescription = "��ֹ��תֵ";
}
void ActionWindowRotatePropertyTargetRotation::setRealValue(const VECTOR3& value, ActionWindowRotate* action)
{
	action->setTargetRotation(value);
}
VECTOR3 ActionWindowRotatePropertyTargetRotation::getRealValue(ActionWindowRotate* action)
{
	return action->getTargetRotation();
}