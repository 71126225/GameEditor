#include "Layout.h"

#include "EditorFrame.h"
#include "LayoutHierarchy.h"
#include "EditorCommandHeader.h"

void CommandEditorFrameNotifyWindowDelete::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	// ˢ��layout�Ĳ㼶��ͼ
	mLayoutHierarchy->deleteItem(mName);
}

std::string CommandEditorFrameNotifyWindowDelete::showDebugInfo()
{
	COMMAND_DEBUG("window name : %s, parent name : %s", mName.c_str(), mParentName.c_str());
}