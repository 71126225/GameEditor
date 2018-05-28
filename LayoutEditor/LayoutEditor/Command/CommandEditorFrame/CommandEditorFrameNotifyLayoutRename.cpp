#include "EditorFrame.h"
#include "LayoutHierarchy.h"
#include "LoadedLayout.h"
#include "EditorCommandHeader.h"

void CommandEditorFrameNotifyLayoutRename::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	// ˢ��layout�Ĳ㼶��ͼ
	mLayoutHierarchy->renameItem(mOldName, mNewName);
	// ˢ���Ѽ��ز����б�
	mLoadedLayout->refreshList();
}

std::string CommandEditorFrameNotifyLayoutRename::showDebugInfo()
{
	COMMAND_DEBUG("old name : %s, new name : %s", mOldName.c_str(), mNewName.c_str());
}