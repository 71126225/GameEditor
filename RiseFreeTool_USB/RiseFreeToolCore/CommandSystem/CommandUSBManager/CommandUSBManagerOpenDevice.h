#ifndef _COMMAND_USB_MANAGER_OPEN_DEVICE_H_
#define _COMMAND_USB_MANAGER_OPEN_DEVICE_H_

#include "Command.h"

// ֪ͨEditorFrame�༭�����д�����ʾ״̬�ı�
class CommandUSBManagerOpenDevice : public Command
{
public:
	COMMAND_CONSTRUCT(CommandUSBManagerOpenDevice, Command)
	{}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
};

#endif