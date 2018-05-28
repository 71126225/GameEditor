#include "txComponent.h"
#include "txAction.h"

#include "WindowHeader.h"

#include "ActionTreeEditorCore.h"

#include "EditorFrame.h"
#include "PropertyWindow.h"
#include "EditorCommandHeader.h"
#include "ComponentViewer.h"
#include "ActionPropertyPanel.h"
#include "ActionTreeResource.h"

void CommandEditorFrameNotifyActionSelectionChanged::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	// ѡ����ı�ʱ,��Ҫ��ʾѡ��������,����Ҫȷ���������б�����ѡ�и���
	txAction* action = mActionTreeEditorCore->getCurAction();
	mActionPropertyPanel->showReceiverProperty(action);
	mActionTreeResource->notifyActionSelectionChanged();
}

std::string CommandEditorFrameNotifyActionSelectionChanged::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}