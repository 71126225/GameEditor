#include "EditorFrame.h"
#include "LoadedLayout.h"
#include "EditorCommandHeader.h"

void CommandEditorFrameNotifyLayoutVisibleChanged::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	// �����Ѽ��ز����еĶ�Ӧ���ֵ���ʾͼ��
	mLoadedLayout->setItemImage(mLayoutName, mVisible);
}

std::string CommandEditorFrameNotifyLayoutVisibleChanged::showDebugInfo()
{
	COMMAND_DEBUG("layout name : %s, visible : %s", mLayoutName.c_str(), mVisible ? "true" : "false");
}