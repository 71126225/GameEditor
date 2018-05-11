#include "txAction.h"
#include "txActionTree.h"
#include "txMathUtility.h"
#include "Utility.h"

void txAction::update(float elapsedTime)
{
	if (mActionState == AES_EXECUTING)
	{
		mTimeCount += elapsedTime;
		if (mTimeCount >= mExecuteTime)
		{
			mTimeCount = 0.0f;
			setActionState(AES_EXECUTE_DONE);
		}
	}
}

void txAction::destroy()
{
	if (mParentActionTree != NULL)
	{
		mParentActionTree->notifyActionDestroied(this);
	}
	if (mParentAction != NULL)
	{
		mParentAction->notifyChildDestroied(this);
	}
	int childCount = mChildActionList.size();
	FOR_STL(mChildActionList, int i = 0; i < childCount; ++i)
	{
		mChildActionList[i]->notifyParentDestroied();
	}
	END_FOR_STL(mChildActionList);
	mChildActionList.clear();
	mChildActionSet.clear();
	mParentAction = NULL;
	mParentActionTree = NULL;
	mActionState = AES_NOT_EXECUTE;
	mStartCallback = NULL;
	mStartUserData = NULL;
	mDoneCallback = NULL;
	mDoneUserData = NULL;
	mInverseStartCallback = NULL;
	mInverseStartUserData = NULL;
	mInverseDoneCallback = NULL;
	mInverseDoneUserData = NULL;
}

void txAction::pause()
{
	setActionState(AES_PAUSE);
	pauseAction();
}

void txAction::resume(const bool& direciton)
{
	mDirection = direciton;
	setActionState(AES_EXECUTING);
	resumeAction(direciton);
}

void txAction::recover()
{
	setActionState(AES_NOT_EXECUTE);
	recoverAction();
}

void txAction::stop()
{
	// �������������Ϊ,���������ִ�е���Ϊ��ʼ����ֹͣ�ָ�ִ��ǰ��״̬
	// ��������Ϊ
	int childCount = mChildActionList.size();
	FOR_STL(mChildActionList, int i = 0; i < childCount; ++i)
	{
		mChildActionList[i]->stop();
	}
	END_FOR_STL(mChildActionList);
	// ������Ϊ����ʲô״̬,����Ҫ�ָ���ֹͣ,ȷ��������Ϊ����״̬����ȷ��
	recover();
	stopAction();
	if (mParentActionTree)
	{
		mParentActionTree->notifyActionStoped(this);
	}
}

void txAction::execute(const float& timeOffset, const bool& direction)
{
	mTimeCount = timeOffset;
	mDirection = direction;
	setActionState(AES_EXECUTING);
}

void txAction::setActionState(const ACTION_EXECUTE_STATE& state)
{
	mActionState = state;
	if (mActionState == AES_EXECUTE_DONE)
	{
		// ֪ͨ��Ϊ��,��ǰ��Ϊ�Ѿ�ִ�����
		if (mParentActionTree != NULL)
		{
			mParentActionTree->notifyActionExecutingDone(this);
		}
		// ������Ϊִ����Ļص�
		if (mDirection)
		{
			if (mDoneCallback != NULL)
			{
				mDoneCallback(this, mDoneUserData);
			}
		}
		else
		{
			if (mInverseDoneCallback != NULL)
			{
				mInverseDoneCallback(this, mInverseDoneUserData);
			}
		}
	}
	else if (mActionState == AES_EXECUTING)
	{
		// ������Ϊ��ʼִ�еĻص�
		if (mDirection)
		{
			if (mStartCallback != NULL)
			{
				mStartCallback(this, mStartUserData);
			}
		}
		else
		{
			if (mInverseStartCallback != NULL)
			{
				mInverseStartCallback(this, mInverseStartUserData);
			}
		}
		// ֪ͨ��Ϊ��,��ǰ��Ϊ����ִ��
		if (mParentActionTree != NULL)
		{
			mParentActionTree->notifyActionExecuting(this);
		}
	}
}

void txAction::addChild(txAction* child)
{
	if (child->getParent() != NULL)
	{
		ENGINE_ERROR("error : action has parent! can not attach to other action!");
		return;
	}
	auto iterAction = mChildActionSet.find(child);
	if (iterAction == mChildActionSet.end())
	{
		// ���ø���Ϊ
		child->setParent(this);
		mChildActionSet.insert(child);
		mChildActionList.push_back(child);
	}
}

void txAction::removeChild(txAction* child)
{
	auto iterAction = mChildActionSet.find(child);
	if (iterAction == mChildActionSet.end())
	{
		return;
	}
	mChildActionSet.erase(iterAction);
	int childPos = -1;
	auto iterChild = mChildActionList.begin();
	auto iterChildEnd = mChildActionList.end();
	FOR_STL(mChildActionList, int i = 0; iterChild != iterChildEnd; ++iterChild)
	{
		if (*iterChild == child)
		{
			// ��ո���Ϊ
			child->setParent(NULL);
			childPos = i;
			break;
		}
		++i;
	}
	END_FOR_STL(mChildActionList);
	if (childPos >= 0)
	{
		mChildActionList.erase(mChildActionList.begin() + childPos);
	}
}

float txAction::getMaxExecuteDoneTimeDepth()
{
	int childCount = mChildActionList.size();
	// ������Ϊ,�򷵻�����Ϊ�����ִ�����ʱ��ʱ�����
	if (childCount > 0)
	{
		float maxDepth = 0.0f;
		FOR_STL(mChildActionList, int i = 0; i < childCount; ++i)
		{
			float doneTimeDepth = mChildActionList[i]->getMaxExecuteDoneTimeDepth();
			maxDepth = txMath::getMax(doneTimeDepth, maxDepth);
		}
		END_FOR_STL(mChildActionList);
		return maxDepth;
	}
	// û������Ϊ,�����Լ���ִ�����ʱ��ʱ�����
	else
	{
		return mExecuteDoneTimeDepth;
	}
}

void txAction::resetStateToNotExecute(const float& timeLine, const bool& direction)
{
	// �������,��ʱ����ȴ��ڵ���ʱ���ߵ���Ϊ������Ϊδִ�е�
	if (direction)
	{
		int childCount = mChildActionList.size();
		FOR_STL(mChildActionList, int i = 0; i < childCount; ++i)
		{
			mChildActionList[i]->resetStateToNotExecute(timeLine, direction);
		}
		END_FOR_STL(mChildActionList);
		if (mTimeDepth >= timeLine)
		{
			setActionState(AES_NOT_EXECUTE);
		}
	}
	// �������,��ִ����ϵ�ʱ�����С�ڵ���ʱ���ߵ���Ϊ������Ϊδִ��
	else
	{
		// ִ����ϵ�ʱ����ȴ�����ʱ����,�����ٱ�����
		if (mExecuteDoneTimeDepth > timeLine)
		{
			return;
		}
		int childCount = mChildActionList.size();
		FOR_STL(mChildActionList, int i = 0; i < childCount; ++i)
		{
			mChildActionList[i]->resetStateToNotExecute(timeLine, direction);
		}
		END_FOR_STL(mChildActionList);
		// ֱ�����ü���,ǰ���Ѿ��жϳ���
		setActionState(AES_NOT_EXECUTE);
	}
}

void txAction::getNeedExecuteAction(txMap<float, txVector<txAction*> >& actionTimeMap, const float& timeLine, const bool& direction)
{
	// �������ʱ,��ǰ��Ϊ��ʱ������Ѿ�����ʱ����,ֱ�ӷ���,�����ٵݹ������
	if ((direction && mTimeDepth > timeLine))
	{
		return;
	}
	if (!direction && mActionState != AES_NOT_EXECUTE)
	{
		return;
	}
	// �������ʱ,��ǰ��Ϊ����ִ�����ʱ������Ѿ����ڵ���ʱ��������ҷ���
	// ���������ֱ�Ӳ��ҷ���(��Ϊǰ����������������ʱ�Ѿ��˳��ݹ���)
	if (direction || !direction && mExecuteDoneTimeDepth >= timeLine)
	{
		// ֻ����δִ�е���Ϊ
		if (mActionState == AES_NOT_EXECUTE)
		{
			// timeDistance��ʾ��Ϊ������߷���ʼִ��ʱ��ʱ����ȵ���ǰʱ���ߵľ���
			float timeDistance = direction ? std::abs(mTimeDepth - timeLine) : std::abs(mExecuteDoneTimeDepth - timeLine);
			auto iterTime = actionTimeMap.find(timeDistance);
			if (iterTime == actionTimeMap.end())
			{
				txVector<txAction*> actionList;
				actionList.push_back(this);
				actionTimeMap.insert(timeDistance, actionList);
			}
			else
			{
				iterTime->second.push_back(this);
			}
		}
	}
	// ��������Ϊ
	int childCount = mChildActionList.size();
	FOR_STL(mChildActionList, int i = 0; i < childCount; ++i)
	{
		mChildActionList[i]->getNeedExecuteAction(actionTimeMap, timeLine, direction);
	}
	END_FOR_STL(mChildActionList);
}

void txAction::getDeepestExecuteDoneAction(txAction*& action)
{
	int childCount = mChildActionList.size();
	if (childCount == 0)
	{
		if (mExecuteDoneTimeDepth >= action->getExecuteDoneTimeDepth())
		{
			action = this;
		}
	}
	else
	{
		FOR_STL(mChildActionList, int i = 0; i < childCount; ++i)
		{
			mChildActionList[i]->getDeepestExecuteDoneAction(action);
		}
		END_FOR_STL(mChildActionList);
	}
}

// �����������������Ϊ��ȫִ������Ҫ��ʱ��
void txAction::setExecuteTime(const float& time)
{
	if (txMath::isFloatEqual(mExecuteTime, time))
	{
		return;
	}
	mExecuteTime = time;
	notifyTimeChanged();
}

// ֪ͨ����Ϊ����Ϊ��ʱ������Լ���ʱ���иı�,��Ҫ�����Լ���ʱ�����
void txAction::notifyTimeChanged()
{
	// ��ˢ���Լ���ʱ�����
	refreshTimeDepth();
	// Ȼ��֪ͨ�Լ��������ӽڵ�ʱ����ȸı�
	int actionCount = mChildActionList.size();
	FOR_STL(mChildActionList, int i = 0; i < actionCount; ++i)
	{
		mChildActionList[i]->notifyTimeChanged();
	}
	END_FOR_STL(mChildActionList);
}