#include "EditorCoreRoot.h"
#include "ECEditor.h"
#include "SceneEditorCore.h"
#include "ComponentEditorCore.h"

#include "EditorCommandHeader.h"
#include "EditorFrame.h"
#include "PropertyWindow.h"
#include "ComponentHierarchy.h"

void CommandEditorFrameNotifyComponentPropertyRefreshed::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	// ˢ���������
	mPropertyWindow->showProperty(mComponentEditorCore->getComponentSelections()[0]);
	// ˢ�������ͼ
	mComponentHierarchy->refreshActiveIcon(mSceneEditorCore->getEditorSelections()[0]);
}

std::string CommandEditorFrameNotifyComponentPropertyRefreshed::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}