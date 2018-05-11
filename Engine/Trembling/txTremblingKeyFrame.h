#ifndef _TX_TREMBLING_KEY_FRAME_H_
#define _TX_TREMBLING_KEY_FRAME_H_

#include "txEngineDefine.h"

// �𶯹ؼ�֡
class txTremblingNode;
class txTremblingKeyFrame
{
public:
	txTremblingKeyFrame()
		:
		mTime(0.0f),
		mOffset(0.0f),
		mIndexInParent(-1),
		mParent(NULL)
	{}
	virtual ~txTremblingKeyFrame() {}
	void setKeyFrame(const float& time, const float& offset)
	{
		setTime(time);
		setOffset(offset);
	}
	void setTime(const float& time);
	void setOffset(const float& offset) { mOffset = offset; }
	void setIndexInParent(const int& index) { mIndexInParent = index; }
	void setParent(txTremblingNode* parent) { mParent = parent; }
	const float& getTime(){ return mTime; }
	const float& getOffset(){ return mOffset; }
	const int& getIndexInParent() { return mIndexInParent; }
	txTremblingNode* getParent() { return mParent; }
	txTremblingKeyFrame& operator=(const txTremblingKeyFrame& other)
	{
		mTime = other.mTime;
		mOffset = other.mOffset;
		return *this;
	}
protected:
	float mTime;				// �ؼ�֡ʱ���
	float mOffset;				// ƫ����
	int mIndexInParent;			// �ڹؼ�֡�ڵ��е�λ��
	txTremblingNode* mParent;	// �����Ĺؼ�֡�ڵ�
};

#endif