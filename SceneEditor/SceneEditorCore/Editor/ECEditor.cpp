#include "txComponent.h"
#include "txProperty.h"

#include "ECEditor.h"
#include "ECEditorManager.h"
#include "ECScene.h"

void ECEditor::setComponentProperty(txComponent* component, txProperty* prop, const std::string& propertyValue)
{
	txMovableObject::setComponentProperty(component, prop, propertyValue);

	// ֪ͨ�����༭��������Ըı�
	if (mScene != NULL)
	{
		mScene->notifyEditorComponentProertyChanged(component, prop->getName(), propertyValue);
	}
}

void ECEditor::destroyComponent(const std::string& name)
{
	txComponent* component = getComponent(name);
	// ֪ͨ�����༭��ɾ�������
	if (mScene != NULL)
	{
		mScene->notifyEditorDeleteComponent(component);
	}
	txMovableObject::destroyComponent(name);
}

bool ECEditor::rename(const std::string& newName)
{
	if (newName == mName)
	{
		return false;
	}
	std::string oldName = mName;
	mName = newName;
	if (!mEditorManager->notifyEditorRenamed(oldName, this))
	{
		mName = oldName;
		return false;
	}
	// ����Ҫ���жϳ����Ƿ��������
	mScene->notifyEditorRenamed(oldName, this);
	return true;
}

void ECEditor::saveEditor(std::string& stream, const int& tableCount)
{
	txStringUtility::jsonStartArray(stream, tableCount, true);
	txStringUtility::jsonStartStruct(stream, tableCount + 1, true);
	// ����
	txStringUtility::jsonAddPair(stream, "name", mName, tableCount + 2, true);
	txStringUtility::jsonEndStruct(stream, tableCount + 1, true);
	// �������������
	int rootComponentCount = mRootComponentList.size();
	FOR_STL(mRootComponentList, int i = 0; i < rootComponentCount; ++i)
	{
		saveComponent(mRootComponentList[i], stream, tableCount + 1);
	}
	END_FOR_STL(mRootComponentList);
	txStringUtility::jsonEndArray(stream, tableCount, true);
}

void ECEditor::saveComponent(txComponent* component, std::string& stream, const int& tableCount)
{
	// ���ȱ�������Լ�������
	component->saveProperty(stream, tableCount);
	// Ȼ�󱣴������������
	txVector<txComponent*>& childList = component->getChildComponentList();
	int childCount = childList.size();
	FOR_STL(childList, int i = 0; i < childCount; ++i)
	{
		saveComponent(childList[i], stream, tableCount);
	}
	END_FOR_STL(childList);
}