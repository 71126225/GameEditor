#ifndef _WINDOW_ACTION_PROPERTY_H_
#define _WINDOW_ACTION_PROPERTY_H_

#include "txBaseProperty.h"
#include "WindowActionHeader.h"

DEFINE_BASE_PROPERTY(ActionWindow);

//---------------------------------------------------------------------------------------------------------
// ��Ϊ���ҽӵĴ���
class ActionWindowPropertyActionWindow : public ActionWindowProperty<txStringProperty, std::string>
{
public:
	ActionWindowPropertyActionWindow();
	virtual void setRealValue(const std::string& value, ActionWindow* action);
	virtual std::string getRealValue(ActionWindow* action);
};

//---------------------------------------------------------------------------------------------------------
// ��Ϊִ��ʱ��
class ActionWindowPropertyExecuteTime : public ActionWindowProperty<txFloatProperty, float>
{
public:
	ActionWindowPropertyExecuteTime();
	virtual void setRealValue(const float& value, ActionWindow* action);
	virtual float getRealValue(ActionWindow* action);
};

#endif