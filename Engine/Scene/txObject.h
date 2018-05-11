#ifndef	_TX_OBJECT_H_
#define _TX_OBJECT_H_

#include "txEngineDefine.h"
#include "txNode.h"
#include "txEngineBase.h"

class txObject : public txEngineBase
{
public:
	txObject(OBJECT_TYPE type)
		:
		mType(type)
	{
		memset(mName, 0, 200);
		mParent = NULL;
	}
	virtual ~txObject()
	{
		// �����ǰ���ҽ��ڽڵ���,��ӽڵ���ȡ����
		if (mParent != NULL)
		{
			// �����ָ��,Ȼ���ٵ��ýڵ���Ƴ�����,��������ѭ������
			txNode* parent = mParent;
			mParent = NULL;
			parent->DetachObject();
		}
		memset(mName, 0, 200);
	}
	void setName(const char *name)
	{
		memcpy(mName, name, strlen(name));
	}
	const char* getName(){ return mName; }
	const OBJECT_TYPE& getType() { return mType; }
	txNode* GetParent(){ return mParent; }
	void SetParent(txNode* node)
	{
		// ����Լ��Ѿ��и��ڵ���,���ȴӸ��ڵ���ȡ����
		if (mParent != NULL)
		{
			// �����ָ��,Ȼ���ٵ��ýڵ���Ƴ�����,��������ѭ������
			txNode* parent = mParent;
			mParent = NULL;
			parent->DetachObject();
		}
		// Ȼ��ҽӵ��µĸ��ڵ���
		mParent = node;
		if (mParent != NULL)
		{
			mParent->AttachObject(this);
		}
		// �����˸��ڵ��֪ͨ�Լ����ڵ��иı�
		NotifyParentUpdated();
	}

	// �����ڵ㷢���˸ı���߱�ˢ��ʱ,֪ͨ����
	virtual void NotifyParentUpdated() = 0;
	virtual void Update(float fElaspedTime) = 0;
protected:
	char mName[200];
	txNode* mParent;
	OBJECT_TYPE mType;
};

#endif
