#ifndef _COMMAND_EDITOR_MANAGER_DESTROY_EDITOR_H_
#define _COMMAND_EDITOR_MANAGER_DESTROY_EDITOR_H_

#include "EditorCoreCommand.h"

class CommandEditorManagerDestroyEditor : public EditorCoreCommand
{
public:
	virtual void reset()
	{
		mEditor = NULL;
		mEditorName = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	ECEditor* mEditor;			// �༭��ָ�����������һ������,���������,��ʹ�ñ༭��ָ��
	std::string mEditorName;
};

#endif