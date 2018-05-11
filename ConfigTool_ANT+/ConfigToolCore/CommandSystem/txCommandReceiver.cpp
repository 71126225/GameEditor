#include "txCommandReceiver.h"
#include "txCommand.h"
#include "txCommandSystem.h"
#include "ConfigToolCore.h"

txCommandReceiver::txCommandReceiver(std::string name)
:
mName(name)
{}

txCommandReceiver::~txCommandReceiver()
{
	// ֪ͨ����ϵͳ��һ������������Ѿ���������,��Ҫȡ����������еļ��������ý����ߵ�����
	// ��ʱ��Ҫȷ�����������ϵͳû�б�����
	if (ConfigToolCore::getSingletonPtr() != NULL && ConfigToolCore::getSingletonPtr()->getCommandSystem() != NULL)
	{
		ConfigToolCore::getSingletonPtr()->getCommandSystem()->notifyReceiverDestroied(this);
	}
}

void txCommandReceiver::receiveCommand(txCommand *cmd)
{
	cmd->runStartCallBack();
	cmd->execute();
	cmd->runEndCallBack();
}