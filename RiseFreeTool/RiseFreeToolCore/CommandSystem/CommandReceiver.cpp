#include "CommandReceiver.h"
#include "Command.h"
#include "CommandSystem.h"
#include "RiseFreeToolCore.h"

CommandReceiver::CommandReceiver(std::string name)
:
mName(name)
{}

CommandReceiver::~CommandReceiver()
{
	// ֪ͨ����ϵͳ��һ������������Ѿ���������,��Ҫȡ����������еļ��������ý����ߵ�����
	// ��ʱ��Ҫȷ�����������ϵͳû�б�����
	if (RiseFreeToolCore::getSingletonPtr() != NULL && RiseFreeToolCore::getSingletonPtr()->getCommandSystem() != NULL)
	{
		RiseFreeToolCore::getSingletonPtr()->getCommandSystem()->notifyReceiverDestroied(this);
	}
}

void CommandReceiver::receiveCommand(Command *cmd)
{
	cmd->runStartCallBack();
	cmd->execute();
	cmd->runEndCallBack();
}