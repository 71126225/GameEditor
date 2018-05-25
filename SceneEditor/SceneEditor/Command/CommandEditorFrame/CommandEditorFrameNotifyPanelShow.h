#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_PANEL_SHOW_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_PANEL_SHOW_H_

#include "EditorCommand.h"

// ֪ͨEditorFrame�༭�����д�����ʾ״̬�ı�
class CommandEditorFrameNotifyPanelShow : public EditorCommand
{
public:
	virtual void reset()
	{
		mPanelName = EMPTY_STRING;
		mShow = true;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mPanelName;
	bool mShow;
};

#endif