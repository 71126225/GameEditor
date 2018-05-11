#ifndef _ACTION_PROPERTY_ROTATE_H_
#define _ACTION_PROPERTY_ROTATE_H_

#include "WindowActionProperty.h"

DEFINE_BASE_PROPERTY(ActionWindowRotate);

//---------------------------------------------------------------------------------------------------------
// ��ʼ��תֵ
class ActionWindowRotatePropertyStartRotation : public ActionWindowRotateProperty<txVector3Property, VECTOR3>
{
public:
	ActionWindowRotatePropertyStartRotation();
	virtual void setRealValue(const VECTOR3& value, ActionWindowRotate* action);
	virtual VECTOR3 getRealValue(ActionWindowRotate* action);
};
//---------------------------------------------------------------------------------------------------------
// ��ֹ��תֵ
class ActionWindowRotatePropertyTargetRotation : public ActionWindowRotateProperty<txVector3Property, VECTOR3>
{
public:
	ActionWindowRotatePropertyTargetRotation();
	virtual void setRealValue(const VECTOR3& value, ActionWindowRotate* action);
	virtual VECTOR3 getRealValue(ActionWindowRotate* action);
};

#endif