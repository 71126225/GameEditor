#include "Layout.h"

#include "LayoutEditorCore.h"

#include "EditorFrame.h"
#include "LayoutHierarchy.h"
#include "PropertyWindow.h"
#include "EditorCommandHeader.h"

void CommandEditorFrameNotifyLayoutClosed::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	// ����رյ��ǵ�ǰ���ڱ༭�Ĳ���(Ϊ�����ʾ��ǰ�����Ѿ����ر���)
	if (mLayoutEditorCore->getCurLayout() == NULL || mLayoutName == mLayoutEditorCore->getCurLayout()->getName())
	{
		// ˢ��layout�Ĳ㼶��ͼ
		mLayoutHierarchy->displayLayout(NULL);
		// ������Դ���
		mPropertyWindow->clearProperty();
	}
}

std::string CommandEditorFrameNotifyLayoutClosed::showDebugInfo()
{
	COMMAND_DEBUG("layout name : %s", mLayoutName.c_str());
}