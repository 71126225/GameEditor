#include "PropertyHeader.h"

#include "WindowProperty.h"
#include "Layout.h"
#include "Window.h"

#include "ComponentEditorCore.h"
#include "LayoutEditorCore.h"

#include "EditorFrame.h"
#include "PropertyWindow.h"
#include "EditorCommandHeader.h"
#include "ComponentPropertyPanel.h"
#include "ComponentViewer.h"

void CommandEditorFrameNotifyComponentPropertyChanged::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	txVector<txComponent*>& selections = mComponentEditorCore->getComponentSelections();
	// ������������Դ����иı���������,��ˢ�����Դ����е���ʾ
	if (mChanger != editorFrame->getComponentPropertyPanel())
	{
		mComponentPropertyPanel->showPropertyT(selections);
	}
	// ���������ļ��������Ըı���
	if (mPropertyName == txProperty::getPropertyName<txComponentPropertyActive>() && mLayoutEditorCore->getCurLayout() != NULL)
	{
		Window* window = mLayoutEditorCore->getCurLayout()->getWindow(mWindowName);
		mComponentViewer->notifyComponentActived(window->getComponent(mComponentName));
	}
}

std::string CommandEditorFrameNotifyComponentPropertyChanged::showDebugInfo()
{
	COMMAND_DEBUG("property name : %s, old property value : %s, property value : %s",
		mPropertyName.c_str(), mOldPropertyValue.c_str(), mPropertyValue.c_str());
}