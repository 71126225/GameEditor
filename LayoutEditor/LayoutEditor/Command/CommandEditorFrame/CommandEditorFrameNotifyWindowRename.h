#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_WINDOW_RENAME_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_WINDOW_RENAME_H_

#include "EditorCommand.h"

// ֪ͨEditorFrame���ڱ༭��Layout�иı�
class CommandEditorFrameNotifyWindowRename : public EditorCommand
{
public:
	virtual void reset()
	{
		mOldName = EMPTY_STRING;
		mNewName = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mOldName;	// ��ɾ���ĸ����ڵ�����
	std::string mNewName;	// ��ɾ���Ĵ�������
};

#endif