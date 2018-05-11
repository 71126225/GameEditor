#ifndef _VIDEO_WINDOW_H_
#define _VIDEO_WINDOW_H_

#include "TextureWindow.h"

class txVideoPlayer;
class txVideoManager;
class txTextureManager;
class VideoWindow : public TextureWindow
{
public:
	VideoWindow(const std::string& name, const WINDOW_TYPE& type);
	virtual ~VideoWindow();
	virtual void update(const float& elapsedTime);
	void destroy();
	void setRate(const float& rate) { mRate = rate; }
	void setPlayPercent(const float& percent){}
	void setPlayTime(const float& fTime) {}
	void setPlayState(const PLAY_STATE& state, const bool& callback = true);	// callback����ֹͣ��Ƶʱ�Ƿ���ûص�����
	void setFileName(const std::string& fileName){ mFileName = fileName; setPlayState(PS_STOP); }
	void setLoop(const bool& loop){ mLoop = loop; }

	const float& getRate() { return mRate; }
	float getPlayPercent() { return 0.0f; }
	const float& getPlayTime() { return mVideoTime; }
	const PLAY_STATE& getPlayState() { return mVideoState; }
	const std::string& getFileName(){ return mFileName; }
	const bool& getLoop(){ return mLoop; }

	float getVideoLength();
	float getVideoFPS();
	VECTOR2 getVideoSize();
	void setVideoPlayEndCallback(VideoPlayEndCallback endCallback, void* userData)
	{
		callAndClearEndCallback(true);
		mEndCallback = endCallback;
		mEndUserData = userData;
	}
	void preDecode();
protected:
	virtual void initProperty();
	void createPlayer();
	void destroyPlayer();
	void callAndClearEndCallback(const bool& isBreak)
	{
		VideoPlayEndCallback curCallback = mEndCallback;
		void* curUserData = mEndUserData;
		mEndCallback = NULL;
		mEndUserData = NULL;
		// ����ص�������ǰ��Ϊ��,�����ж��˸���
		if (curCallback != NULL)
		{
			curCallback(this, curUserData, isBreak);
		}
	}
protected:
	txVideoPlayer* mVideoPlayer;
	std::string mFileName;
	int mCurFrame;			// ��ǰ֡,-1��ʾ��֡�Ѿ���Ⱦ��,�����ʾ��û����Ⱦ
	int mLastFrame;
	float mVideoTime;		// ��Ƶʵ�ʲ��ŵ���ʱ��
	float mRate;
	PLAY_STATE mVideoState;
	bool mLoop;
	VideoPlayEndCallback mEndCallback;
	void* mEndUserData;
};

#endif