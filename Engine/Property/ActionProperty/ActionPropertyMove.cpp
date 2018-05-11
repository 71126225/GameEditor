#include "ActionPropertyMove.h"
#include "ActionWindowMove.h"

ActionWindowMovePropertyStartPos::ActionWindowMovePropertyStartPos()
{
	mPropertyName = "StartPos";
	mDescription = "��ʼHSLƫ��";
}
void ActionWindowMovePropertyStartPos::setRealValue(const txPoint& value, ActionWindowMove* action)
{
	action->setStartPos(value);
}
txPoint ActionWindowMovePropertyStartPos::getRealValue(ActionWindowMove* action)
{
	return action->getStartPos();
}
//---------------------------------------------------------------------------------------------------------
ActionWindowMovePropertyTargetPos::ActionWindowMovePropertyTargetPos()
{
	mPropertyName = "TargetPos";
	mDescription = "��ֹHSLƫ��";
}
void ActionWindowMovePropertyTargetPos::setRealValue(const txPoint& value, ActionWindowMove* action)
{
	action->setTargetPos(value);
}
txPoint ActionWindowMovePropertyTargetPos::getRealValue(ActionWindowMove* action)
{
	return action->getTargetPos();
}