#include "txScene.h"

#include "CoreCommandHeader.h"
#include "ECScene.h"
#include "CoreCommandHeader.h"
#include "ECEditor.h"
#include "EditorCoreRoot.h"
#include "SceneEditorCore.h"

void CommandSceneNewEditor::execute()
{
	ECScene* scene = CMD_CAST<ECScene*>(mReceiver);
	ECEditor* editor = scene->createEditor(mEditorName);
	// ���ʹ����༭����¼�
	mEditorCoreRoot->sendEvent(CE_EDITOR_ADDED, editor->getName());
	// ѡ��
	mSceneEditorCore->setEditorSelection(editor);
	// ˢ�³��������С
	scene->setRectDirty(true);
}

std::string CommandSceneNewEditor::showDebugInfo()
{
	COMMAND_DEBUG("editor name : %s", mEditorName.c_str());
}