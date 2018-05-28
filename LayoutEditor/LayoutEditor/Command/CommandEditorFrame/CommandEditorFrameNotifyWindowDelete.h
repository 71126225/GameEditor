#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_WINDOW_DELETE_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_WINDOW_DELETE_H_

#include "EditorCommand.h"

class CommandEditorFrameNotifyWindowDelete : public EditorCommand
{
public:
	virtual void reset()
	{
		mParentName = EMPTY_STRING;
		mName = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mParentName;	// ��ɾ���ĸ����ڵ�����
	std::string mName;			// ��ɾ���Ĵ�������
};

#endif