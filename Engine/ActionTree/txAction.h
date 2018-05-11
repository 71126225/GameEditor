#ifndef _TX_ACTION_H_
#define _TX_ACTION_H_

#include "txEngineBase.h"
#include "txEngineDefine.h"
#include "txEngineLog.h"
#include "txPropertyReceiver.h"
#include "txCommandReceiver.h"

class txActionTree;
class txAction : public txPropertyReceiver, public txCommandReceiver
{
public:
	txAction(const std::string& typeName, const std::string& name)
		:
		txCommandReceiver(name),
		mTypeName(typeName),
		mParentActionTree(NULL),
		mActionState(AES_NOT_EXECUTE),
		mParentAction(NULL),
		mStartCallback(NULL),
		mStartUserData(NULL),
		mDoneCallback(NULL),
		mDoneUserData(NULL),
		mInverseStartCallback(NULL),
		mInverseStartUserData(NULL),
		mInverseDoneCallback(NULL),
		mInverseDoneUserData(NULL),
		mTimeCount(0.0f),
		mTimeDepth(0.0f),
		mExecuteTime(0.0f),
		mExecuteDoneTimeDepth(0.0f),
		mDirection(true)
	{}
	virtual ~txAction(){ destroy(); }
	virtual void init(txActionTree* tree)
	{
		mParentActionTree = tree;
		initReceiver();
	}
	virtual void initProperty(){}
	virtual void update(float elapsedTime);
	const std::string& getTypeName()			{ return mTypeName; }
	const float& getExecuteTime()				{ return mExecuteTime; }
	const float& getTimeDepth()					{ return mTimeDepth; }
	const float& getExecuteDoneTimeDepth()		{ return mExecuteDoneTimeDepth; }
	txActionTree* getActionTree()				{ return mParentActionTree; }
	const ACTION_EXECUTE_STATE& getActionState(){ return mActionState; }
	txAction* getParent()						{ return mParentAction; }
	txVector<txAction*>& getChildList(){ return mChildActionList; }
	// ���ڿ�ʼִ����Ϊʱ��ʱ������Ϊʵ�ʵ�ʱ�������ƫ��(��һ֡ʱ��Ӱ��),�����ڿ�ʼִ����Ϊʱ��Ҫ��һ����ʱ��ƫ��
	virtual void execute(const float& timeOffset, const bool& direction = true);
	virtual void setDefaultParam(){}
	void destroy();
	void pause();
	void resume(const bool& direciton = true);
	void stop();			// ֹͣ��ǰ�Լ���������Ϊ������ִ�к��Ѿ�ִ����ϵ���Ϊ,���ָ���ʼ״̬
	void recover();
	void startInverse();	// ����׶���Ϊ��ʼ���ϵ���ִ����Ϊ
	void setActionState(const ACTION_EXECUTE_STATE& state);
	void setParent(txAction* parent)
	{
		mParentAction = parent;
		refreshTimeDepth();
	}
	void addChild(txAction* child);
	void removeChild(txAction* child);
	void setActionCallback(ActionCallback startCallback, void* startUserData, ActionCallback doneCallback, void* doneUserData)
	{
		mStartCallback = startCallback;
		mStartUserData = startUserData;
		mDoneCallback = doneCallback;
		mDoneUserData = doneUserData;
	}
	void setInverseActionCallback(ActionCallback inverseStart, void* startUserData, ActionCallback inverseDone, void* doneUserData)
	{
		mInverseStartCallback = inverseStart;
		mInverseStartUserData = startUserData;
		mInverseDoneCallback = inverseDone;
		mInverseDoneUserData = doneUserData;
	}
	// �����������������Ϊ��ȫִ������Ҫ��ʱ��
	virtual void setExecuteTime(const float& time);
	float getMaxExecuteDoneTimeDepth();	// �õ�����Ϊ������ִ�����ʱ��ʱ�����
	void resetStateToNotExecute(const float& timeLine, const bool& direction = true);// �����ݵ�ǰʱ���߽���Ϊ�Լ�����Ϊ��״̬����Ϊδִ��״̬
	void getNeedExecuteAction(txMap<float, txVector<txAction*> >& actionTimeMap, const float& timeLine, const bool& direction = true);
	void getDeepestExecuteDoneAction(txAction*& action);	// �õ�ִ�����ʱ������������Ϊ
protected:
	virtual void pauseAction(){}
	virtual void resumeAction(const bool& direction = true){}
	virtual void recoverAction(){}
	virtual void stopAction(){}
	// ֪ͨ����Ϊ����Ϊ��������
	void notifyParentDestroied()
	{
		mParentAction = NULL;
		refreshTimeDepth();
	}
	// ֪ͨ����Ϊ����Ϊ��������
	void notifyChildDestroied(txAction* action)
	{
		removeChild(action);
	}
	// ֪ͨ����Ϊ����Ϊ��ʱ������Լ���ʱ���иı�,��Ҫ�����Լ���ʱ�����
	void notifyTimeChanged();
	// ˢ���Լ���ʱ�����
	void refreshTimeDepth()
	{
		if (mParentAction != NULL)
		{
			mTimeDepth = mParentAction->getTimeDepth() + mParentAction->getExecuteTime();
		}
		else
		{
			mTimeDepth = 0.0f;
		}
		mExecuteDoneTimeDepth = mTimeDepth + mExecuteTime;
	}
protected:
	std::string mTypeName;						// ��Ϊ������
	bool mDirection;							// ��Ϊִ�з���,Ϊtrue���ʾ����ִ��,Ϊfalse���ʾ����ִ��
	float mTimeCount;							// ��ǰ��ʱ
	float mTimeDepth;							// ��Ϊ��ǰ��ʱ�����,˳��ʼִ����Ϊʱ��ʱ��
	float mExecuteTime;							// ��Ϊ��ȫִ��������Ҫ��ʱ��
	float mExecuteDoneTimeDepth;				// ��Ϊִ����ʱ��ʱ�����,ֵ����mTimeDepth + mExecuteTime
	ACTION_EXECUTE_STATE mActionState;			// ��ǰ��Ϊ��ִ��״̬
	txActionTree* mParentActionTree;			// ������Ϊ��
	txAction* mParentAction;					// ����Ϊ
	txVector<txAction*> mChildActionList;	// ����Ϊ�б�,����������Ϊ֮���˳��
	txSet<txAction*> mChildActionSet;		// ��ֹ����Ϊ�ظ���Ӷ����ڵ��б�
	ActionCallback mStartCallback;				// ��Ϊ��ʼִ��ʱ�Ļص�����
	void* mStartUserData;
	ActionCallback mDoneCallback;				// ��Ϊִ�����ʱ�Ļص�����
	void* mDoneUserData;
	ActionCallback mInverseStartCallback;		// ��Ϊ��ʼ����ִ��ʱ�Ļص�����
	void* mInverseStartUserData;
	ActionCallback mInverseDoneCallback;		// ��Ϊ����ִ�����ʱ�Ļص�����
	void* mInverseDoneUserData;
};

#endif