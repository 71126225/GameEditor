#ifndef _COMMAND_RECEIVER_H_
#define _COMMAND_RECEIVER_H_

#include "CommonDefine.h"

class Command;
class CommandReceiver
{
public:
	CommandReceiver(std::string name);
	virtual ~CommandReceiver();
	virtual void receiveCommand(Command* cmd);
	std::string& getName() { return mName; }
	// ����ʹ����������
	void setName(std::string name) { mName = name; }
protected:
	std::string mName;
};

#endif