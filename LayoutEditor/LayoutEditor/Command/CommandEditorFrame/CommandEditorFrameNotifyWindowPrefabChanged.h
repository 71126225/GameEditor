#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_WINDOW_PREFAB_CHANGED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_WINDOW_PREFAB_CHANGED_H_

#include "EditorCommand.h"

// ֪ͨEditorFrame���ڸı��˹�����Ԥ��
class CommandEditorFrameNotifyWindowPrefabChanged : public EditorCommand
{
public:
	virtual void reset()
	{
		mWindowName = EMPTY_STRING;
		mPrefabName = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mWindowName;
	std::string mPrefabName;
};

#endif