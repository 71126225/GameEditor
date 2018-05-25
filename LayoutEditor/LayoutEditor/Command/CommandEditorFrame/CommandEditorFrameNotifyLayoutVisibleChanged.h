#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_LAYOUT_VISIBLE_CHANGED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_LAYOUT_VISIBLE_CHANGED_H_

#include "EditorCommand.h"

// ֪ͨEditorFrame���ڱ༭��Layout����ʾ״̬�иı�
class CommandEditorFrameNotifyLayoutVisibleChanged : public EditorCommand
{
public:
	virtual void reset()
	{
		mVisible = true;
		mLayoutName = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	bool mVisible;
	std::string mLayoutName;
};

#endif