#ifndef _ACTION_PROPERTY_ALPHA_H_
#define _ACTION_PROPERTY_ALPHA_H_

#include "WindowActionProperty.h"

DEFINE_BASE_PROPERTY(ActionWindowAlpha);

//---------------------------------------------------------------------------------------------------------
// ��ʼ͸����
class ActionWindowAlphaPropertyStartAlpha : public ActionWindowAlphaProperty<txFloatProperty, float>
{
public:
	ActionWindowAlphaPropertyStartAlpha();
	virtual void setRealValue(const float& value, ActionWindowAlpha* action);
	virtual float getRealValue(ActionWindowAlpha* action);
};
//---------------------------------------------------------------------------------------------------------
// ��ֹ͸����
class ActionWindowAlphaPropertyTargetAlpha : public ActionWindowAlphaProperty<txFloatProperty, float>
{
public:
	ActionWindowAlphaPropertyTargetAlpha();
	virtual void setRealValue(const float& value, ActionWindowAlpha* action);
	virtual float getRealValue(ActionWindowAlpha* action);
};

#endif