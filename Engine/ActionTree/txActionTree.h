#ifndef _TX_ACTION_TREE_H_
#define _TX_ACTION_TREE_H_

#include "txEngineBase.h"
#include "txEngineDefine.h"

class txAction;
class txActionTree : public txEngineBase
{
public:
	txActionTree(const std::string& name)
		:
		mName(name),
		mTreeState(PS_STOP),
		mTimeLine(0.0f),
		mTimeDirction(true),
		mRootAction(NULL),
		mUserData(NULL)
	{}
	virtual ~txActionTree(){ destroy(); }
	virtual void init(const bool& createRoot = true)
	{
		if (createRoot)
		{
			addRoot();
		}
	}
	virtual void update(float elapsedTime);
	void destroy();
	const std::string& getName() { return mName; }
	txAction* getRootAction() { return mRootAction; }
	txAction* getAction(const std::string& name);
	txAction* addRoot();
	void setTreeState(const PLAY_STATE& state);
	void play(const bool& direction = true);
	void pause();
	void stop();	// �ָ�������Ϊ��δִ��ʱ��״̬
	static txAction* createIndependentAction(const std::string& type, const std::string& name);
	txAction* addAction(const std::string& type, const std::string& name, txAction* parentAction);
	void addAction(txAction* action, txAction* parent);
	bool deleteAction(const std::string& name);
	void notifyActionDestroied(txAction* action);
	void notifyActionExecuting(txAction* action)
	{
		if (mExecutingAction.find(action) == mExecutingAction.end())
		{
			mExecutingAction.insert(action);
		}
	}
	void notifyActionExecutingDone(txAction* action)
	{
		if (mDeepestAction == action)
		{
			complete();
		}
		auto iterAction = mExecutingAction.find(action);
		if (iterAction != mExecutingAction.end())
		{
			mExecutingAction.erase(iterAction);
		}
	}
	void notifyActionStoped(txAction* action)
	{
		auto iterAction = mExecutingAction.find(action);
		if (iterAction != mExecutingAction.end())
		{
			mExecutingAction.erase(iterAction);
		}
	}
	const float& getTimeLine() { return mTimeLine; }
	void setUserData(void* userData) { mUserData = userData; }
	void* getUserData() { return mUserData; }
protected:
	void complete();
protected:
	std::string mName;
	PLAY_STATE mTreeState;						// ��Ϊ����ǰ��״̬
	float mTimeLine;							// ��ǰʱ����,��ʾִ�е�����Ϊ�����ĸ�ʱ���
	bool mTimeDirction;							// ʱ�����ŷ���,true��ʾ����,false��ʾ�ݼ�
	txAction* mRootAction;						// ����Ϊ
	txAction* mDeepestAction;					// ���һ��ִ�������Ϊ
	txMap<std::string, txAction*> mActionList;	// ��Ϊ�������е���Ϊ
	txSet<txAction*> mExecutingAction;			// ����ִ�е���Ϊ�б�
	void* mUserData;							// ������չ���Զ������
};

#endif