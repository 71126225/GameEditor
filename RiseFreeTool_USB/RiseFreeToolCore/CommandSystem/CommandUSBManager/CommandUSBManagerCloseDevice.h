#ifndef _COMMAND_USB_MANAGER_CLOSE_DEVICE_H_
#define _COMMAND_USB_MANAGER_CLOSE_DEVICE_H_

#include "Command.h"

// ֪ͨEditorFrame�༭�����д�����ʾ״̬�ı�
class CommandUSBManagerCloseDevice : public Command
{
public:
	COMMAND_CONSTRUCT(CommandUSBManagerCloseDevice, Command)
	{}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
};

#endif