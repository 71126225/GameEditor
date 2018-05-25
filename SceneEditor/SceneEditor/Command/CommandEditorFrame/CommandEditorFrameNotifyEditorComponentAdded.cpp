#include "EditorCoreRoot.h"
#include "ECScene.h"
#include "ECEditor.h"
#include "SceneEditorCore.h"

#include "EditorCommandHeader.h"
#include "EditorFrame.h"
#include "PropertyWindow.h"
#include "ComponentHierarchy.h"

void CommandEditorFrameNotifyEditorComponentAdded::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
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
	// �������ͼ��ɾ�������
	mComponentHierarchy->addItem(editor->getComponent(mComponentName), mComponentPos);
	// ������Դ���
	mPropertyWindow->showProperty(NULL);
}

std::string CommandEditorFrameNotifyEditorComponentAdded::showDebugInfo()
{
	COMMAND_DEBUG("editor name : %s, component name : %s, component pos : %d", mEditorName.c_str(), mComponentName.c_str(), mComponentPos);
}