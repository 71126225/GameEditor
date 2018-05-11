#ifndef _ACTION_PROPERTY_SCALE_H_
#define _ACTION_PROPERTY_SCALE_H_

#include "WindowActionProperty.h"

DEFINE_BASE_PROPERTY(ActionWindowScale);

//---------------------------------------------------------------------------------------------------------
// ��ʼ����ֵ
class ActionWindowScalePropertyStartScale : public ActionWindowScaleProperty<txVector2Property, VECTOR2>
{
public:
	ActionWindowScalePropertyStartScale();
	virtual void setRealValue(const VECTOR2& value, ActionWindowScale* action);
	virtual VECTOR2 getRealValue(ActionWindowScale* action);
};
//---------------------------------------------------------------------------------------------------------
// ��ֹ����ֵ
class ActionWindowScalePropertyTargetScale : public ActionWindowScaleProperty<txVector2Property, VECTOR2>
{
public:
	ActionWindowScalePropertyTargetScale();
	virtual void setRealValue(const VECTOR2& value, ActionWindowScale* action);
	virtual VECTOR2 getRealValue(ActionWindowScale* action);
};

#endif