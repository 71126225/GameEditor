#include "WindowActionProperty.h"
#include "ActionWindow.h"
#include "txAction.h"
#include "txActionTree.h"
#include "Layout.h"

ActionWindowPropertyActionWindow::ActionWindowPropertyActionWindow()
{
	mPropertyName = "ActionWindow";
	mDescription = "��Ϊ���ҽӵĴ���";
}
void ActionWindowPropertyActionWindow::setRealValue(const std::string& value, ActionWindow* action)
{
	Layout* layout = (Layout*)action->getActionTree()->getUserData();
	if (layout != NULL)
	{
		action->setWindow(layout->getWindow(value));
	}
}
std::string ActionWindowPropertyActionWindow::getRealValue(ActionWindow* action)
{
	if (action->getWindow() != NULL)
	{
		return action->getWindow()->getName();
	}
	return EMPTY_STRING;
}
//---------------------------------------------------------------------------------------------------------
ActionWindowPropertyExecuteTime::ActionWindowPropertyExecuteTime()
{
	mPropertyName = "ExecuteTime";
	mDescription = "��Ϊִ��ʱ��";
}
void ActionWindowPropertyExecuteTime::setRealValue(const float& value, ActionWindow* action)
{
	action->setExecuteTime(value);
}
float ActionWindowPropertyExecuteTime::getRealValue(ActionWindow* action)
{
	return action->getExecuteTime();
}