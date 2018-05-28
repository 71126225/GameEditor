#include "LayoutEditorCore.h"

#include "EditorFrame.h"
#include "LayoutHierarchy.h"
#include "LoadedLayout.h"
#include "EditorCommandHeader.h"

void CommandEditorFrameNotifyLayoutAdded::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	Layout* layout = mLayoutEditorCore->getCurLayout();
	// ˢ��layout�Ĳ㼶��ͼ
	mLayoutHierarchy->displayLayout(layout);
	// ����Ѽ����б���û��,����뵽�Ѽ����б���
	mLoadedLayout->refreshList();
}

std::string CommandEditorFrameNotifyLayoutAdded::showDebugInfo()
{
	COMMAND_DEBUG("layout name : %s", mName.c_str());
}