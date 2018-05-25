#include "CoreCommandHeader.h"
#include "ECScene.h"
#include "EditorCoreRoot.h"
#include "ECEditor.h"
#include "SceneEditorCore.h"
#include "ComponentEditorCore.h"

void CommandSceneDestroyEditor::execute()
{
	ECScene* scene = CMD_CAST<ECScene*>(mReceiver);
	mSceneEditorCore->clearEditorSelection();
	mComponentEditorCore->clearComponentSelections();
	// ���ٱ༭��
	scene->destroyEditor(mEditorName);
	
	// ���ͱ༭�������¼�
	mEditorCoreRoot->sendEvent(CE_EDITOR_DELETED, mEditorName);
}

std::string CommandSceneDestroyEditor::showDebugInfo()
{
	COMMAND_DEBUG("editor name : %s", mEditorName.c_str());
}