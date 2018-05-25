#include "EditorCoreRoot.h"

#include "EditorCommandHeader.h"
#include "EditorFrame.h"
#include "PropertyWindow.h"
#include "ComponentHierarchy.h"

void CommandEditorFrameNotifyEditorComponentDeleted::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	// �������ͼ��ɾ�������
	mComponentHierarchy->deleteItem(mComponentName);
	// ������Դ���
	mPropertyWindow->showProperty(NULL);
}

std::string CommandEditorFrameNotifyEditorComponentDeleted::showDebugInfo()
{
	COMMAND_DEBUG("editor name : %s, component name : %s", mEditorName.c_str(), mComponentName.c_str());
}