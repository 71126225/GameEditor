#include "txComponent.h"

#include "CoreCommandHeader.h"
#include "ECEditor.h"
#include "EditorCoreRoot.h"
#include "ECUndoManager.h"
#include "ECUndoAddComponent.h"
#include "GameLog.h"
#include "SceneEditorCore.h"

void CommandEditorAddComponent::execute()
{
	ECEditor* editor = CMD_CAST<ECEditor*>(mReceiver);
	// ���Դ�����Ϊ��,��ʹ��Դ���������
	if (mComponent != NULL)
	{
		mType = mComponent->getType();
	}

	// ������
	txComponent* component = editor->addComponent(mComponentName, mType);
	txComponent* parent = NULL;
	if (mParentComponentName != EMPTY_STRING)
	{
		parent = editor->getComponent(mParentComponentName);
	}
	// ����������ײ���,������������������
	if (component->getBaseType() == TOSTRING(txComponentCollision) && (parent == NULL || parent->getBaseType() != TOSTRING(txComponentPhysics)))
	{
		GAME_ERROR("error : collision component must be attach to physics component!");
		editor->destroyComponent(component->getName());
		return;
	}
	// �����Դ���,��Դ������������Կ������������
	if (mComponent != NULL)
	{
		mComponent->copyProperty(component);
	}

	// �Ƚ��������ԭ�������ӵ���ߺ͸��������ϵ�Ͽ�
	component->detachOwnerParentComponent();
	// ���½��������ӵ���ߺ͸��������ϵ
	component->attachOwnerParentComponent(editor, parent, mComponentPos);

	// ����������볷������
	ECUndoAddComponent* undoAdd = mUndoManager->createUndo<ECUndoAddComponent>();
	undoAdd->mEditorName = editor->getName();
	undoAdd->mComponentName = component->getName();
	mUndoManager->addUndo(undoAdd);

	// ���������������¼�
	txVector<std::string> params;
	params.push_back(editor->getName());
	params.push_back(component->getName());
	params.push_back(txStringUtility::intToString(mComponentPos));
	mEditorCoreRoot->sendEvent(CE_COMPONENT_ADDED, params);

	// ��¼�����иı�
	mSceneEditorCore->setSceneModified(true);
}

std::string CommandEditorAddComponent::showDebugInfo()
{
	COMMAND_DEBUG("component type : %s, component name : %s", mType.c_str(), mComponentName.c_str());
}