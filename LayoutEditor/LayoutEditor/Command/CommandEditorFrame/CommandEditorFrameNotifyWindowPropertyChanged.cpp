#include "WindowProperty.h"
#include "Layout.h"
#include "Window.h"

#include "LayoutEditorCore.h"

#include "EditorFrame.h"
#include "PropertyWindow.h"
#include "LayoutHierarchy.h"
#include "EditorCommandHeader.h"
#include "InnerChildHierarchy.h"

void CommandEditorFrameNotifyWindowPropertyChanged::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	txVector<Window*>& selections = mLayoutEditorCore->getSelections();
	// ������������Դ����иı�Ĵ�������,��ˢ�����Դ����е���ʾ
	if (mChanger != mPropertyWindow)
	{
		mPropertyWindow->showPropertyT(selections);
	}
	// ����Ǵ��ڵĿɼ������Ըı���
	if (mPropertyName == txProperty::getPropertyName<WindowPropertyVisible>() && mLayoutEditorCore->getCurLayout() != NULL)
	{
		Window* window = mLayoutEditorCore->getCurLayout()->getWindow(mWindowName);
		// ��ǰֻѡ����һ������,����ѡ�е����ڲ��Ӵ���,��ı��ڲ��Ӵ�����ͼ�еĿ���ͼ��
		if (selections.size() == 1 && selections[0]->isInnerChild())
		{
			mInnerChildHierarchy->setTreeItemVisibleIcon(window);
		}
		else
		{
			mLayoutHierarchy->setVisibleIcon(window);
		}
	}
}

std::string CommandEditorFrameNotifyWindowPropertyChanged::showDebugInfo()
{
	COMMAND_DEBUG("property name : %s, old property value : %s, property value : %s",
		mPropertyName.c_str(), mOldPropertyValue.c_str(), mPropertyValue.c_str());
}