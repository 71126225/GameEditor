#include "Utility.h"
#include "CommandSystem.h"
#include "txCommandReceiver.h"
#include "txCommand.h"
#include "EditorCoreLog.h"

void CommandSystem::update(float elapsedTime)
{
	// ͬ�����������б�������б���
	// �ȴ�����������
	waitUnlockBuffer();
	// ����������
	lockBuffer();
	int inputCount = mCommandBufferInput.size();
	for (int i = 0; i < inputCount; ++i)
	{
		mCommandBufferProcess.push_back(mCommandBufferInput[i]);
	}
	mCommandBufferInput.clear();
	// ����������
	unlockBuffer();

	// ���һ֡ʱ�����1��,����Ϊ����Ч����
	if (elapsedTime >= 1.0f)
	{
		return;
	}
	// ��ʼ����������б�
	std::vector<DelayCommand>::iterator iter = mCommandBufferProcess.begin();
	for (; iter != mCommandBufferProcess.end();)
	{
		iter->mDelayTime -= elapsedTime;
		if (iter->mDelayTime <= 0.0f)
		{
			// ������ӳ�ִ��ʱ�䵽��,��ִ������
			pushCommand(iter->mCommand, iter->mReceiver);
			TRACE_DELETE(iter->mCommand);
			iter = mCommandBufferProcess.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

bool CommandSystem::interruptCommand(txCommand* cmd)
{
	if (cmd == NULL || !cmd->isDelayCommand())
	{
		return false;
	}
	std::vector<DelayCommand>::iterator iter = mCommandBufferProcess.begin();
	std::vector<DelayCommand>::iterator iterEnd = mCommandBufferProcess.end();
	for (; iter != iterEnd; ++iter)
	{
		// �ҵ�������,Ȼ�����ٸ�����,���б����Ƴ�
		if (iter->mCommand == cmd)
		{
			LOGI("CommandSystem : interrupt 0x%p, %s, receiver : %s, file : %s, line : %d",
				cmd, cmd->showDebugInfo().c_str(), iter->mReceiver->getName().c_str(), StringUtility::getFileName(cmd->getFile()).c_str(), cmd->getLine());
			TRACE_DELETE(iter->mCommand);
			mCommandBufferProcess.erase(iter);
			return true;
		}
	}
	return false;
}

void CommandSystem::pushCommand(txCommand* cmd, txCommandReceiver* cmdReceiver)
{
	if (cmd == NULL || cmdReceiver == NULL)
	{
		return;
	}
	cmd->setReceiver(cmdReceiver);
	if (mShowDebugInfo && cmd->getShowDebugInfo())
	{
		LOGI("CommandSystem : 0x%p, %s, receiver : %s, file : %s, line : %d",
			cmd, cmd->showDebugInfo().c_str(), cmdReceiver->getName().c_str(), StringUtility::getFileName(cmd->getFile()).c_str(), cmd->getLine());
	}
	cmdReceiver->receiveCommand(cmd);
}

void CommandSystem::pushDelayCommand(txCommand* cmd, txCommandReceiver* cmdReceiver, float delayExecute)
{
	if (cmd == NULL || cmdReceiver == NULL)
	{
		return;
	}
	if (delayExecute <= 0.0f)
	{
		delayExecute = 0.01f;
	}
	if (mShowDebugInfo && cmd->getShowDebugInfo())
	{
		LOGI("CommandSystem : delay cmd : %f, info : 0x%p, %s, receiver : %s, file : %s, line : %d",
			delayExecute, cmd, cmd->showDebugInfo().c_str(), cmdReceiver->getName().c_str(), StringUtility::getFileName(cmd->getFile()).c_str(), cmd->getLine());
	}
	if (cmd->isDelayCommand())
	{
		DelayCommand delayCommand(delayExecute, cmd, cmdReceiver);

		// �ȴ�����������
		waitUnlockBuffer();
		// ����������
		lockBuffer();
		mCommandBufferInput.push_back(delayCommand);
		// ����������
		unlockBuffer();
	}
	else
	{
		EDITOR_CORE_ERROR("error : cmd is not a delay command, please use Command::createDelayCommand to create a delay command! \n\
command type : %s, file : %s, line : %d", typeid(*cmd).name(), cmd->getFile(), cmd->getLine());
	}
}

void CommandSystem::destroy()
{
	int inputSize = mCommandBufferInput.size();
	for (int i = 0; i < inputSize; ++i)
	{
		TRACE_DELETE(mCommandBufferInput[i].mCommand);
	}
	mCommandBufferInput.clear();

	int processSize = mCommandBufferProcess.size();
	for (int i = 0; i < processSize; ++i)
	{
		TRACE_DELETE(mCommandBufferProcess[i].mCommand);
	}
	mCommandBufferProcess.clear();
}

void CommandSystem::waitUnlockBuffer()
{
	while (mLockBuffer)
	{}
}

void CommandSystem::notifyReceiverDestroied(txCommandReceiver* receiver)
{
	// �ȴ�����������
	waitUnlockBuffer();
	// ����������
	lockBuffer();
	std::vector<DelayCommand>::iterator iterCommandInput = mCommandBufferInput.begin();
	for (; iterCommandInput != mCommandBufferInput.end();)
	{
		if (iterCommandInput->mReceiver == receiver)
		{
			TRACE_DELETE(iterCommandInput->mCommand);
			iterCommandInput = mCommandBufferInput.erase(iterCommandInput);
		}
		else
		{
			++iterCommandInput;
		}
	}
	// ����������
	unlockBuffer();

	std::vector<DelayCommand>::iterator iterCommand = mCommandBufferProcess.begin();
	for (; iterCommand != mCommandBufferProcess.end();)
	{
		if (iterCommand->mReceiver == receiver)
		{
			TRACE_DELETE(iterCommand->mCommand);
			iterCommand = mCommandBufferProcess.erase(iterCommand);
		}
		else
		{
			++iterCommand;
		}
	}
}