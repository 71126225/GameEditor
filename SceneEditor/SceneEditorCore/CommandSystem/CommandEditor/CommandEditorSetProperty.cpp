#include "txProperty.h"
#include "PropertyHeader.h"

#include "CoreCommandHeader.h"
#include "ECEditor.h"
#include "EditorCoreRoot.h"
#include "ECUndoManager.h"
#include "ECUndoChangeComponentProperty.h"

void CommandEditorSetProperty::execute()
{
	ECEditor* editor = CMD_CAST<ECEditor*>(mReceiver);
	txComponent* component = editor->getComponent(mComponentName);
	txProperty* prop = component->getProperty(mPropertyName);
	if (prop == NULL)
	{
		return;
	}
	// ���������ֵ
	std::string oldValue = prop->get(component);
	// ��������ֵ
	editor->setComponentProperty(component, prop, mPropertyValue);
	// ���ʹ������Ըı���¼�
	txVector<std::string> params;
	params.push_back(mPropertyName);
	params.push_back(mComponentName);
	params.push_back(oldValue);
	params.push_back(mPropertyValue);
	params.push_back(editor->getName());
	params.push_back(txStringUtility::intToString((int)(mChangeOperator)));
	mEditorCoreRoot->sendEvent(CE_COMPONENT_PROPERTY_CHANGED, params);

	// ��������õĶ�̬ģ������Ķ�̬ģ������,����Ҫ��ˢ������
	const std::string& componentType = component->getType();
	const std::string& componentBaseType = component->getBaseType();
	if (componentType == TOSTRING(txComponentDynamicMesh) && mPropertyName == txProperty::getPropertyName<txDynamicMeshPropertyDynamicMesh>())
	{
		mEditorCoreRoot->sendEvent(CE_COMPONENT_PROPERTY_REFRESH);
	}

	// ��ӳ�������
	ECUndoChangeComponentProperty* propertyUndo = mUndoManager->createUndo<ECUndoChangeComponentProperty>();
	propertyUndo->mEditorName = editor->getName();
	propertyUndo->mComponentName = mComponentName;
	propertyUndo->mPropertyName = mPropertyName;
	propertyUndo->mPropertyValue = oldValue;
	mUndoManager->addUndo(propertyUndo);
}

std::string CommandEditorSetProperty::showDebugInfo()
{
	COMMAND_DEBUG("component name : %s, property name : %s, property value : %s, operator : 0x%p", 
		mComponentName.c_str(), mPropertyName.c_str(), mPropertyValue.c_str(), mChangeOperator);
}