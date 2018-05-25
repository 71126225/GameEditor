#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_LAYOUT_DESTROIED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_LAYOUT_DESTROIED_H_

#include "EditorCommand.h"

// ֪ͨEditorFrame��һ��Layout��������
class CommandEditorFrameNotifyLayoutDestroied : public EditorCommand
{
public:
	virtual void reset()
	{
		mLayoutName = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mLayoutName;
};

#endif