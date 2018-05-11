#include "VideoWindowProperty.h"

//---------------------------------------------------------------------------------------
txVector<std::string> VideoWindowPropertyVideo::mChoices;
VideoWindowPropertyVideo::VideoWindowPropertyVideo()
{
	mPropertyName = "FileNameVideo";
	mDescription = "��Ƶ�ļ���";
}
void VideoWindowPropertyVideo::setRealValue(const std::string& value, VideoWindow* window)
{
	window->setFileName(value);
}
std::string VideoWindowPropertyVideo::getRealValue(VideoWindow* window)
{
	return window->getFileName();
}

//---------------------------------------------------------------------------------------
VideoWindowPropertyPlayState::VideoWindowPropertyPlayState()
{
	mPropertyName = "PlayState";
	mDescription = "����״̬";
	ADD_ENUM(PS_PLAY);
	ADD_ENUM(PS_PAUSE);
	ADD_ENUM(PS_STOP);
}
void VideoWindowPropertyPlayState::setRealValue(const PLAY_STATE& value, VideoWindow* window)
{
	window->setPlayState(value);
}
PLAY_STATE VideoWindowPropertyPlayState::getRealValue(VideoWindow* window)
{
	return window->getPlayState();
}

//---------------------------------------------------------------------------------------
VideoWindowPropertyVideoLoop::VideoWindowPropertyVideoLoop()
{
	mPropertyName = "VideoLoop";
	mDescription = "��Ƶѭ������";
}
void VideoWindowPropertyVideoLoop::setRealValue(const bool& value, VideoWindow* window)
{
	window->setLoop(value);
}
bool VideoWindowPropertyVideoLoop::getRealValue(VideoWindow* window)
{
	return window->getLoop();
}

//---------------------------------------------------------------------------------------
VideoWindowPropertyRate::VideoWindowPropertyRate()
{
	mPropertyName = "Rate";
	mDescription = "��Ƶ��������";
}
void VideoWindowPropertyRate::setRealValue(const float& value, VideoWindow* window)
{
	window->setRate(value);
}
float VideoWindowPropertyRate::getRealValue(VideoWindow* window)
{
	return window->getRate();
}

//---------------------------------------------------------------------------------------
VideoWindowPropertyVideoLength::VideoWindowPropertyVideoLength()
{
	mPropertyName = "VideoLength";
	mDescription = "��Ƶ����";
	mReadOnly = true;
}
void VideoWindowPropertyVideoLength::setRealValue(const float& value, VideoWindow* window)
{
	// ��ʵ��������Ƶ����
}
float VideoWindowPropertyVideoLength::getRealValue(VideoWindow* window)
{
	return window->getVideoLength();
}

//---------------------------------------------------------------------------------------
VideoWindowPropertyPlayTime::VideoWindowPropertyPlayTime()
{
	mPropertyName = "PlayTime";
	mDescription = "��Ƶ��ǰ���ŵ�ʱ��";
}
void VideoWindowPropertyPlayTime::setRealValue(const float& value, VideoWindow* window)
{
	window->setPlayTime(value);
}
float VideoWindowPropertyPlayTime::getRealValue(VideoWindow* window)
{
	return window->getPlayTime();
}

//---------------------------------------------------------------------------------------
VideoWindowPropertyPlayPercent::VideoWindowPropertyPlayPercent()
{
	mPropertyName = "PlayPercent";
	mDescription = "��Ƶ��ǰ���ŵİٷֱ�";
}
void VideoWindowPropertyPlayPercent::setRealValue(const float& value, VideoWindow* window)
{
	window->setPlayPercent(value);
}
float VideoWindowPropertyPlayPercent::getRealValue(VideoWindow* window)
{
	return window->getPlayPercent();
}

//---------------------------------------------------------------------------------------
VideoWindowPropertyVideoSize::VideoWindowPropertyVideoSize()
{
	mPropertyName = "VideoSize";
	mDescription = "��Ƶ�ֱ���";
	mReadOnly = true;
}
void VideoWindowPropertyVideoSize::setRealValue(const VECTOR2& value, VideoWindow* window)
{
	// ��ʵ�����÷ֱ���
}
VECTOR2 VideoWindowPropertyVideoSize::getRealValue(VideoWindow* window)
{
	return window->getVideoSize();
}

//---------------------------------------------------------------------------------------
VideoWindowPropertyVideoFPS::VideoWindowPropertyVideoFPS()
{
	mPropertyName = "VideoFPS";
	mDescription = "��Ƶ֡��";
	mReadOnly = true;
}
void VideoWindowPropertyVideoFPS::setRealValue(const float& value, VideoWindow* window)
{
	// ��ʵ������֡��
}
float VideoWindowPropertyVideoFPS::getRealValue(VideoWindow* window)
{
	return window->getVideoFPS();
}