#ifndef _TEXTURE_ANIM_WINDOW_H_
#define _TEXTURE_ANIM_WINDOW_H_

#include "TextureWindow.h"

class TextureAnimWindow : public TextureWindow
{
public:
	TextureAnimWindow(const std::string& name, const WINDOW_TYPE& type);
	virtual ~TextureAnimWindow(){}
	virtual void update(const float& elapsedTime);

	const LOOP_MODE& getLoop() const	{ return mLoopMode; }
	const std::string& getTextureSet()	{ return mTextureSet; }
	const float& getInterval()			{ return mInterval; }
	const int& getStartIndex()			{ return mStartIndex; }
	const PLAY_STATE& getPlayState()	{ return mPlayState; }
	const bool& getPlayDirection()		{ return mPlayDirection; }
	const int& getEndIndex()			{ return mEndIndex; }
	int getTextureFrameCount()			{ return (int)mTextureCache.size(); }
	const bool& getAutoHide()			{ return mAutoHide; }
	// ���ʵ�ʵ���ֹ�±�,������Զ����,�򷵻����һ�ŵ��±�
	int getRealEndIndex()
	{
		if (mEndIndex < 0)
		{
			return txMath::getMax(getTextureFrameCount() - 1, 0);
		}
		else
		{
			return mEndIndex;
		}
	}

	void setLoop(const LOOP_MODE& loop)				{ mLoopMode = loop; }
	void setInterval(const float& interval)			{ mInterval = interval; }
	void setPlayDirection(const bool& direction)	{ mPlayDirection = direction; }
	void setAutoHide(const bool& autoHide)			{ mAutoHide = autoHide; }
	void setPlayState(const PLAY_STATE& state);
	void setStartIndex(const int& startIndex)		
	{
		mStartIndex = startIndex;
		txMath::clamp(mStartIndex, 0, getTextureFrameCount() - 1);
	}
	void setEndIndex(const int& endIndex)			
	{ 
		mEndIndex = endIndex;
		if (mEndIndex >= 0)
		{
			txMath::clamp(mEndIndex, 0, getTextureFrameCount() - 1);
		}
	}
	void setTextureSet(const std::string& textureSetName, const bool& async);	
	void stop(const bool& resetStartIndex = true, const bool& callback = true, const bool& isBreak = true)
	{
		mPlayState = PS_STOP;
		if (resetStartIndex)
		{
			setCurFrameIndex(mStartIndex);
		}
		// �ж�����֡����ʱ���ûص�����,ֻ�ڷ�ѭ������ʱ�ŵ���
		if (callback && mLoopMode == LM_NONE)
		{
			callAndClearEndCallback(isBreak);
		}
	}
		
	void play() { mPlayState = PS_PLAY; }
	void pause() { mPlayState = PS_PAUSE; }
	void setPlayEndCallback(TextureAnimPlayEndCallback callback, void* userData)
	{
		callAndClearEndCallback(true);
		mPlayEndCallback = callback;
		mPlayEndUserData = userData;
	}
	void setCurFrameIndex(const int& index)
	{
		mCurTextureIndex = index;
		// ��������ʼ֡����ֹ֮֡��
		txMath::clamp(mCurTextureIndex, mStartIndex, getRealEndIndex());
		if (mCurTextureIndex >= 0 && mCurTextureIndex < (int)mTextureCache.size())
		{
			mTexture = mTextureCache[mCurTextureIndex];
		}
		mCurTimeCount = 0.0f;
	}
	virtual void finishAsycLoad();
	virtual void getUsedResourceList(txVector<std::string>& resourceList);
	static const txSet<std::string>& getTextureAnimNameList() { return mTextureAnimNameList; }
	static void refreshTextureAnimNameList();
protected:
	virtual void initProperty();
	// ���ò�����ջص�,������ڵ���֮ǰ
	void callAndClearEndCallback(bool isBreak)
	{
		TextureAnimPlayEndCallback curCallback = mPlayEndCallback;
		void* curUserData = mPlayEndUserData;
		mPlayEndCallback = NULL;
		mPlayEndUserData = NULL;
		// ����ص�������ǰ��Ϊ��,�����ж��˸���
		if (curCallback != NULL)
		{
			curCallback(this, curUserData, isBreak);
		}
	}
protected:
	txVector<txTexture*> mTextureCache;
	int mStartIndex;		// ����֡����ʼ֡�±�,Ĭ��Ϊ0,����ͷ��ʼ
	int mEndIndex;			// ����֡����ֹ֡�±�,Ĭ��Ϊ-1,�����ŵ�β��
	bool mPlayDirection;	// ���ŷ���,trueΪ���򲥷�(��mStartIndex��mEndIndex),falseΪ���򲥷�(��mEndIndex��mStartIndex)
	int mCurTextureIndex;
	LOOP_MODE mLoopMode;
	std::string mTextureSet;
	float mCurTimeCount;
	PLAY_STATE mPlayState;
	float mInterval;	// ���������л�ͼƬ
	bool mAutoHide;		// �Ƿ��ڲ�����Ϻ��Զ����ز������õ�ǰ֡�±�
	TextureAnimPlayEndCallback mPlayEndCallback;	// һ�����в�����ʱ�Ļص�����,ֻ�ڷ�ѭ������״̬����Ч
	void* mPlayEndUserData;
	static txSet<std::string> mTextureAnimNameList;
};

#endif