#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_LAYOUT_ADDED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_LAYOUT_ADDED_H_

#include "EditorCommand.h"

// ֪ͨEditorFrame���ڱ༭��Layout�иı�
class CommandEditorFrameNotifyLayoutAdded : public EditorCommand
{
public:
	virtual void reset(){}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mName;			// ��ɾ���Ĵ�������
};

#endif