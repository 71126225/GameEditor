#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_WINDOW_ADDED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_WINDOW_ADDED_H_

#include "EditorCommand.h"

class CommandEditorFrameNotifyWindowAdded : public EditorCommand
{
public:
	virtual void reset()
	{
		mParentName = EMPTY_STRING;
		mName = EMPTY_STRING;
		mWindowPos = 0;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mParentName;	// ��ӵĸ����ڵ�����
	std::string mName;			// ��������
	int mWindowPos;
};

#endif