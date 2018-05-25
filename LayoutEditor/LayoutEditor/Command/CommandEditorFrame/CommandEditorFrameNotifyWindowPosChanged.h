#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_WINDOW_POS_CHANGED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_WINDOW_POS_CHANGED_H_

#include "EditorCommand.h"

class CommandEditorFrameNotifyWindowPosChanged : public EditorCommand
{
public:
	virtual void reset()
	{
		mParentName = EMPTY_STRING;
		mName = EMPTY_STRING;
		mOldPosition = 0;
		mNewPosition = 0;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mParentName;	// �����ڵ�����
	std::string mName;			// ��������
	int mOldPosition;
	int mNewPosition;
};

#endif