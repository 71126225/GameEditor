#include "txComponent.h"

#include "CoreCommandHeader.h"
#include "ECEditor.h"
#include "EditorCoreRoot.h"
#include "ECUndoManager.h"
#include "ECUndoDeleteComponent.h"
#include "GameLog.h"

void CommandEditorDeleteComponent::execute()
{
	ECEditor* editor = CMD_CAST<ECEditor*>(mReceiver);

	txComponent* component = editor->getComponent(mComponentName);
	if (component == NULL)
	{
		GAME_ERROR("error : can not find component!");
		return;
	}
	if (component->getBaseType() == TOSTRING(txComponentTransform) || component->getBaseType() == TOSTRING(txComponentPhysics))
	{
		GAME_ERROR("error : can not delete transform or physics component!");
		return;
	}
	
	// �������֮ǰ�ȼ��볷������
	ECUndoDeleteComponent* undoDelete = mUndoManager->createUndo<ECUndoDeleteComponent>();
	undoDelete->setComponent(editor->getComponent(mComponentName));
	mUndoManager->addUndo(undoDelete);

	// �������
	editor->destroyComponent(mComponentName);
	
	// ���ٳɹ�����������ٵ��¼�
	txVector<std::string> params;
	params.push_back(editor->getName());
	params.push_back(mComponentName);
	mEditorCoreRoot->sendEvent(CE_EDITOR_COMPONENT_DELETED, params);
}

std::string CommandEditorDeleteComponent::showDebugInfo()
{
	COMMAND_DEBUG("component name : %s", mComponentName.c_str());
}