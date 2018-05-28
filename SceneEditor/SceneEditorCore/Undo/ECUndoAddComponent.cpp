#include "ECUndoAddComponent.h"
#include "EditorCoreRoot.h"
#include "ECScene.h"
#include "ECEditor.h"
#include "CoreCommandHeader.h"
#include "SceneEditorCore.h"

void ECUndoAddComponent::undo()
{
	// ɾ�������
	ECScene* scene = mSceneEditorCore->getCurScene();
	if (scene == NULL)
	{
		return;
	}
	ECEditor* editor = scene->getEditor(mEditorName);
	if (editor == NULL)
	{
		return;
	}
	CommandEditorDeleteComponent* cmdDelete = NEW_CMD(cmdDelete);
	cmdDelete->mComponentName = mComponentName;
	mCommandSystem->pushCommand(cmdDelete, editor);
}