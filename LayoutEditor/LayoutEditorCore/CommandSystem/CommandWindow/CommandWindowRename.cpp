#include "WindowHeader.h"

#include "CommandHeader.h"
#include "EditorCoreRoot.h"
#include "LayoutEditorCore.h"
#include "ECUndoManager.h"
#include "ECUndoWindowRename.h"

void CommandWindowRename::execute()
{
	Window* window = CMD_CAST<Window*>(mReceiver);
	if (window != NULL)
	{
		// �˴�����ʹ�ó�������,��Ϊ��ʱ��ı�����,����oldName��newName��ͬ
		std::string oldName = window->getName();
		bool result = window->rename(mNewName);
		if (mResult != NULL)
		{
			*mResult = result;
		}
		// ��������ɹ�,����Ҫ���ʹ��ڸ������¼�
		if (result)
		{
			txVector<std::string> params;
			params.push_back(oldName);
			params.push_back(window->getName());
			mEditorCoreRoot->sendEvent(CE_WINDOW_RENAMED, params);
			mLayoutEditorCore->reselect();

			// ��¼�����иı�
			mLayoutEditorCore->setLayoutModified(true);

			// ��ӵ������б�
			ECUndoWindowRename* renameUndo = mUndoManager->createUndo<ECUndoWindowRename>();
			if (NULL != renameUndo)
			{
				renameUndo->setName(oldName);
				renameUndo->setWindowName(window->getName());
				mUndoManager->addUndo(renameUndo);
			}
		}
	}
}

std::string CommandWindowRename::showDebugInfo()
{
	COMMAND_DEBUG("new name : %s", mNewName.c_str());
}