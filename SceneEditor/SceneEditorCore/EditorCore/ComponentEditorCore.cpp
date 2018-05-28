#include "ComponentEditorCore.h"
#include "EditorCoreRoot.h"

void ComponentEditorCore::setComponentSelection(txComponent* component)
{
	txVector<txComponent*> componentList;
	componentList.push_back(component);
	setComponentSelection(componentList);
}

void ComponentEditorCore::setComponentSelection(txVector<txComponent*>& selections)
{
	bool sameSelection = true;
	// �����ظ�����ѡ����,��������ѭ���ĵݹ����
	if (mComponentMultiSelections.size() == selections.size())
	{
		int selectionCount = mComponentMultiSelections.size();
		FOR_STL(mComponentMultiSelections, int i = 0; i < selectionCount; ++i)
		{
			if (mComponentMultiSelections[i] != selections[i])
			{
				sameSelection = false;
				break;
			}
		}
		END_FOR_STL(mComponentMultiSelections);
	}
	else
	{
		sameSelection = false;
	}
	if (sameSelection)
	{
		return;
	}

	clearComponentSelections();
	int selCount = selections.size();
	FOR_STL(selections, int i = 0; i < selCount; ++i)
	{
		auto iter = mComponentMultiSelSet.find(selections[i]);
		if (iter == mComponentMultiSelSet.end())
		{
			mComponentMultiSelections.push_back(selections[i]);
			mComponentMultiSelSet.insert(selections[i]);
		}
	}
	END_FOR_STL(selections);
	// ����ѡ����ı���¼�
	notifyComponentSelectionChanged();
}

void ComponentEditorCore::reselectComponent()
{
	notifyComponentSelectionChanged();
}

void ComponentEditorCore::clearComponentSelections()
{
	mComponentMultiSelections.clear();
	mComponentMultiSelSet.clear();
	// ����ѡ����ı���¼�
	notifyComponentSelectionChanged();
}

void ComponentEditorCore::notifyComponentSelectionChanged(const bool& notifyEvent)
{
	if (notifyEvent)
	{
		mEditorCoreRoot->sendEvent(CE_COMPONENT_SELECTION_CHANGED);
	}
}