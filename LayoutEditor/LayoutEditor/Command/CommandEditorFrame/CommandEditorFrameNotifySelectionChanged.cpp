#include "WindowHeader.h"

#include "LayoutEditorCore.h"

#include "EditorFrame.h"
#include "PropertyWindow.h"
#include "LayoutHierarchy.h"
#include "EditorCommandHeader.h"
#include "InnerChildHierarchy.h"
#include "ComponentViewer.h"

void CommandEditorFrameNotifySelectionChanged::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	// ѡ����ı�ʱ,��Ҫ��ʾѡ��������,����Ҫȷ���������б�����ѡ�и���
	txVector<Window*>& selections = mLayoutEditorCore->getSelections();
	if (selections.size() > 0)
	{
		mPropertyWindow->showPropertyT(selections);
		txVector<std::string> selectionNameList;
		int selectionCount = selections.size();
		FOR_STL(selections, int i = 0; i < selectionCount; ++i)
		{
			selectionNameList.push_back(selections[i]->getName());
		}
		END_FOR_STL(selections);
		mLayoutHierarchy->setSelection(selectionNameList);
		// �����ڲ��Ӵ����б������ѡ,����ֻҪ�Ƕ�ѡ,��һ��Ҫ����ڲ��Ӵ�����ͼ
		if (selections.size() == 1)
		{
			// ���������,ѡ����һ�����ϴ���
			if (selections[0]->isComplexWindow())
			{
				mInnerChildHierarchy->displayInnerChild(static_cast<ComplexWindow*>(selections[0]));
			}
			else
			{
				// ѡ����һ���Ǹ��ϴ��ڲ��ҷ��ڲ��Ӵ���
				if (!selections[0]->isInnerChild())
				{
					mInnerChildHierarchy->displayInnerChild(NULL);
				}
				// ѡ����һ���ڲ��Ӵ���,ʲôҲ����
				else
				{
					;
				}
			}
		}
		else
		{
			mInnerChildHierarchy->displayInnerChild(NULL);
		}
	}
	else
	{
		mPropertyWindow->showReceiverProperty(NULL);
		mLayoutHierarchy->unSelectAll();
		mInnerChildHierarchy->displayInnerChild(NULL);
	}
	mComponentViewer->refreshList();
}

std::string CommandEditorFrameNotifySelectionChanged::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}