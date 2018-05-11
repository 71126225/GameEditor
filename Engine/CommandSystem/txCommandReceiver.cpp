#include "txCommandReceiver.h"
#include "txCommand.h"
#include "txCommandSystem.h"
#include "txEngineRoot.h"

txCommandReceiver::txCommandReceiver(const std::string& name)
:
mName(name)
{}

txCommandReceiver::~txCommandReceiver()
{
	// ֪ͨ����ϵͳ��һ������������Ѿ���������,��Ҫȡ����������еļ��������ý����ߵ�����
	// ��ʱ��Ҫȷ�����������ϵͳû�б�����
	if (txEngineRoot::getSingletonPtr() != NULL && txEngineRoot::getSingletonPtr()->getCommandSystem() != NULL)
	{
		txEngineRoot::getSingletonPtr()->getCommandSystem()->notifyReceiverDestroied(this);
	}
}

void txCommandReceiver::receiveCommand(txCommand* cmd)
{
	cmd->runStartCallBack();
	cmd->setExecuteState(ES_EXECUTING);
	cmd->execute();
	cmd->setExecuteState(ES_EXECUTED);
	cmd->runEndCallBack();
}