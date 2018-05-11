#include "txActionTree.h"
#include "txAction.h"
#include "txActionTreeManager.h"
#include "txActionFactory.h"
#include "txActionFactoryManager.h"

void txActionTree::update(float elapsedTime)
{
	// ����Ϊ����������ǰִ���ĸ���Ϊ
	if (mTreeState == PS_PLAY)
	{
		// ����ʱ�����ж��Ƿ��ִ���ĸ���Ϊ,��ʼִ�к�,���ۼ�ʱ��
		// �ҵ�ʱ�����С�ڵ��ڵ�ǰʱ����,���һ�û��ִ�е���Ϊ
		// first��ʾ��Ϊ������߷���ʼִ��ʱ��ʱ����ȵ���ǰʱ���ߵľ���
		txMap<float, txVector<txAction*> > actionList;
		mRootAction->getNeedExecuteAction(actionList, mTimeLine, mTimeDirction);
		if (actionList.size() > 0)
		{
			auto iterAction = actionList.begin();
			auto iterActionEnd = actionList.end();
			FOR_STL(actionList, ; iterAction != iterActionEnd; ++iterAction)
			{
				auto& actionLine = iterAction->second;
				int actionCount = actionLine.size();
				FOR_STL(actionLine, int i = 0; i < actionCount; ++i)
				{
					txAction* action = actionLine[i];
					action->execute(std::abs(mTimeLine - (mTimeDirction ? action->getTimeDepth() : action->getExecuteDoneTimeDepth())), mTimeDirction);
				}
				END_FOR_STL(actionLine);
			}
			END_FOR_STL(actionList);
		}
		// ������������ִ�е���Ϊ,�˴�����һ���б�,�����ڸ��¹����ж�mExecutingAction���в�����ʹ������ʧЧ
		auto executingActionList = mExecutingAction;
		auto iterExecuting = executingActionList.begin();
		auto iterExecutingEnd = executingActionList.end();
		FOR_STL(executingActionList, ; iterExecuting != iterExecutingEnd; ++iterExecuting)
		{
			(*iterExecuting)->update(elapsedTime);
		}
		END_FOR_STL(executingActionList);
		mTimeLine += elapsedTime * (mTimeDirction ? 1.0f : -1.0f);
	}
}

void txActionTree::destroy()
{
	// ����һ���б�,����������ʱ������ʧЧ
	if (mRootAction != NULL)
	{
		mActionTreeManager->destroyAction(mRootAction);
		mRootAction = NULL;
	}
	mActionList.clear();
	mExecutingAction.clear();
}

txAction* txActionTree::getAction(const std::string& name)
{
	auto iterAction = mActionList.find(name);
	if (iterAction != mActionList.end())
	{
		return iterAction->second;
	}
	return NULL;
}

txAction* txActionTree::addRoot()
{
	if (mRootAction != NULL)
	{
		ENGINE_ERROR("error : there is a root action! can not add again!");
		return mRootAction;
	}
	txAction* action = addAction(TOSTRING(txAction), "root", NULL);
	return action;
}

void txActionTree::setTreeState(const PLAY_STATE& state)
{
	if (state == PS_PLAY)
	{
		play();
	}
	else if (state == PS_PAUSE)
	{
		pause();
	}
}

void txActionTree::play(const bool& direction)
{
	if (mTreeState == PS_PLAY)
	{
		return;
	}
	if (mRootAction == NULL)
	{
		ENGINE_ERROR("error : no root action, can not play action tree!");
		return;
	}
	mTimeDirction = direction;
	// ����Ǵ���ͣ״̬��ʼ����,����Ҫ��������ִ���б��еı���ͣ����Ϊ�ָ�ִ��
	if (mTreeState == PS_PAUSE)
	{
		auto iterAction = mExecutingAction.begin();
		auto iterActionEnd = mExecutingAction.end();
		FOR_STL(mExecutingAction, ; iterAction != iterActionEnd; ++iterAction)
		{
			(*iterAction)->resume(mTimeDirction);
		}
		END_FOR_STL(mExecutingAction);
	}
	// ��λ��ʱ����ǰ������Ϊȫ������Ϊδִ��״̬
	mRootAction->resetStateToNotExecute(mTimeLine, mTimeDirction);
	mTreeState = PS_PLAY;

	// �õ����һ��ִ����ϵ���Ϊ,֪ͨ����Ϊ��ִ�����ʱӦ��֪ͨ��Ϊ��,ֻ������ִ��ʱ�Ż���,����ִ��ʱ,0ʱ�����Ϊ��ֹ
	if (direction)
	{
		mDeepestAction = mRootAction;
		mRootAction->getDeepestExecuteDoneAction(mDeepestAction);
	}
	else
	{
		mDeepestAction = mRootAction;
	}
}

void txActionTree::pause()
{
	mTreeState = PS_PAUSE;
	// ��Ҫ����������ִ�е���Ϊ����ͣ
	auto iterAction = mExecutingAction.begin();
	auto iterActionEnd = mExecutingAction.end();
	FOR_STL(mExecutingAction, ; iterAction != iterActionEnd; ++iterAction)
	{
		(*iterAction)->pause();
	}
	END_FOR_STL(mExecutingAction);
}

void txActionTree::stop()
{
	mTreeState = PS_STOP;
	mTimeLine = 0.0f;
	// �Ӹ���Ϊ��ʼ��������Ϊֹͣ,�˴���������Ҫ�޸�!����Ϊ������ִ�й�����ֹͣ��Ϊ��,Ȼ���ٴο�ʼִ��ʱ,ִ�й��̻��д���
	if (mRootAction != NULL)
	{
		mRootAction->stop();
	}
}

void txActionTree::complete()
{
	// ʱ����У��Ϊ���һ����Ϊִ����ϵ�ʱ�����,״̬�ָ���ʼ״̬
	if (mDeepestAction != NULL)
	{
		mTimeLine = mDeepestAction->getExecuteDoneTimeDepth();
	}
	else
	{
		mTimeLine = 0.0f;
	}
	mTreeState = PS_STOP;
}

txAction* txActionTree::createIndependentAction(const std::string& type, const std::string& name)
{
	txActionFactoryBase* factory = mActionFactoryManager->getFactory(type);
	if (factory != NULL)
	{
		return factory->createAction(name);
	}
	return NULL;
}

txAction* txActionTree::addAction(const std::string& type, const std::string& name, txAction* parentAction)
{
	if (parentAction == NULL && mRootAction != NULL)
	{
		return NULL;
	}
	txAction* action = createIndependentAction(type, name);
	if (action == NULL)
	{
		return NULL;
	}
	action->init(this);
	action->setDefaultParam();
	addAction(action, parentAction);
	return action;
}

void txActionTree::addAction(txAction* action, txAction* parent)
{
	if (mTreeState != PS_STOP)
	{
		ENGINE_ERROR("error : can not add action during playing actions!");
		return;
	}
	// �����Ҫ����ǰ��ӵ���Ϊ��Ϊ����Ϊ
	if (parent == NULL)
	{
		if (mRootAction == NULL)
		{
			mRootAction = action;
		}
		else
		{
			ENGINE_ERROR("error : there is a root action! can not set again!");
		}
	}
	else
	{
		parent->addChild(action);
	}
	mActionList.insert(action->getName(), action);
}

bool txActionTree::deleteAction(const std::string& name)
{
	if (mTreeState != PS_STOP)
	{
		ENGINE_ERROR("error : can not destroy action during playing actions!");
		return false;
	}
	txAction* action = getAction(name);
	if (action == NULL)
	{
		return false;
	}
	auto iterAction = mActionList.find(name);
	if (iterAction != mActionList.end())
	{
		mActionTreeManager->destroyAction(action);
		mActionList.erase(iterAction);
		return true;
	}
	return false;
}

void txActionTree::notifyActionDestroied(txAction* action)
{
	if (action == NULL)
	{
		return;
	}
	if (mRootAction == action)
	{
		mRootAction = NULL;
	}
	auto iterAction = mActionList.find(action->getName());
	if (iterAction != mActionList.end())
	{
		mActionList.erase(iterAction);
	}
	auto iterExecutingAction = mExecutingAction.find(action);
	if (iterExecutingAction != mExecutingAction.end())
	{
		mExecutingAction.erase(iterExecutingAction);
	}
}