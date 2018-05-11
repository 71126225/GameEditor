#include "ActionPropertyAlpha.h"
#include "ActionWindowAlpha.h"

ActionWindowAlphaPropertyStartAlpha::ActionWindowAlphaPropertyStartAlpha()
{
	mPropertyName = "StartAlpha";
	mDescription = "��ʼ͸����";
}
void ActionWindowAlphaPropertyStartAlpha::setRealValue(const float& value, ActionWindowAlpha* action)
{
	action->setStartAlpha(value);
}
float ActionWindowAlphaPropertyStartAlpha::getRealValue(ActionWindowAlpha* action)
{
	return action->getStartAlpha();
}
//---------------------------------------------------------------------------------------------------------
ActionWindowAlphaPropertyTargetAlpha::ActionWindowAlphaPropertyTargetAlpha()
{
	mPropertyName = "TargetAlpha";
	mDescription = "��ֹ͸����";
}
void ActionWindowAlphaPropertyTargetAlpha::setRealValue(const float& value, ActionWindowAlpha* action)
{
	action->setTargetAlpha(value);
}
float ActionWindowAlphaPropertyTargetAlpha::getRealValue(ActionWindowAlpha* action)
{
	return action->getTargetAlpha();
}