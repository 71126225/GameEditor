#include "txCommandReceiver.h"
#include "txCommand.h"
#include "txCommandSystem.h"
#include "EditorCore.h"

txCommandReceiver::txCommandReceiver(std::string name)
:
mName(name)
{}

txCommandReceiver::~txCommandReceiver()
{
	// ֪ͨ����ϵͳ��һ������������Ѿ���������,��Ҫȡ����������еļ��������ý����ߵ�����
	// ��ʱ��Ҫȷ�����������ϵͳû�б�����
	if (EditorCore::getSingletonPtr() != NULL && EditorCore::getSingletonPtr()->getCommandSystem() != NULL)
	{
		EditorCore::getSingletonPtr()->getCommandSystem()->notifyReceiverDestroied(this);
	}
}

void txCommandReceiver::receiveCommand(txCommand *cmd)
{
	cmd->runStartCallBack();
	cmd->execute();
	cmd->runEndCallBack();
}