#include "txEngineRoot.h"
#include "txRenderWindow.h"
#include "txTextureManager.h"
#include "txVideoPlayer.h"
#include "txVideoManager.h"
#include "txTexture.h"

#include "WindowHeader.h"
#include "VideoWindowProperty.h"

VideoWindow::VideoWindow(const std::string& name, const WINDOW_TYPE& type)
	:
	TextureWindow(name, type),
	mVideoPlayer(NULL),
	mVideoTime(0.0f),
	mRate(1.0f),
	mVideoState(PS_STOP),
	mLoop(false),
	mCurFrame(-1),
	mLastFrame(-1),
	mEndCallback(NULL),
	mEndUserData(NULL)
{
	// ��Ƶ���ڲ�ʹ�ö༶��������,��߽������Ⱦ�ٶ�
	mUseMipmap = false;
}
	
VideoWindow::~VideoWindow()
{
	destroy();
}

void VideoWindow::update(const float& elapsedTime)
{
	if (!isVisible())
	{
		return;
	}

	Window::update(elapsedTime);
	if (mVideoPlayer != NULL)
	{
		if (mVideoState == PS_PLAY)
		{
			// �ۼ���Ƶʵ�ʲ��ŵ�ʱ��
			mVideoTime += elapsedTime * mRate;
			// ��ǰ֡�Ѿ���Ⱦ��,��ʼ��һ֡�Ľ������Ⱦ
			if (mCurFrame == -1)
			{
				// ������Ƶ��ǰ֡
				int frame = (int)(mVideoTime * mVideoPlayer->getFramesPerSecond());
				// ���������֡����һ֡��ͬ
				if (mLastFrame != frame)
				{
					mCurFrame = frame;
					mVideoPlayer->askDecodeFrame(mCurFrame);
				}
			}
				
			// ��ѯ��ǰ֡�Ƿ��Ѿ��������
			if (mCurFrame >= 0)
			{
				txTexture* frameTexture = mVideoPlayer->getFrame(mCurFrame);
				// ����Ѿ��������,���������õ�ǰ��Ⱦ����
				if (frameTexture != NULL)
				{
					// ��������һ֡
					if (mLastFrame >= 0)
					{
						mVideoPlayer->destroyFrame(mLastFrame);
					}
					// �������ڴ��ڵ�update����������,��Ҫ����ˢ��
					setTexture(frameTexture, true);
					mLastFrame = mCurFrame;
					mCurFrame = -1;
				}
			}

			if (mVideoPlayer->getVideoEnd())
			{
				// ������һ֡
				if (mLastFrame >= 0)
				{
					mVideoPlayer->destroyFrame(mLastFrame);
				}
				setPlayState(PS_STOP, false);
				if (mLoop)
				{
					setPlayState(PS_PLAY);
				}
				// ��ѭ������ʱ,���ò��Ž����Ļص�����
				else
				{
					callAndClearEndCallback(false);
				}
				// ������ͣһ��ʱ��,������Ƶ�����²���ʱ��һ֡�ڵ�,����ֻ�������������ٶ��¿�����Ч����
				txUtility::sleep(30);
			}
		}
	}
	else
	{
		// ����Ϊ������ʱ,���Բ�����ˢ��
		setTexture(NULL);
	}
}

void VideoWindow::destroy()
{
	destroyPlayer();
}

void VideoWindow::setPlayState(const PLAY_STATE& state, const bool& callback)
{
	if (mVideoState == state)
	{
		return;
	}
	mVideoState = state;
	if (state == PS_PLAY)
	{
		// �����δ����������,���ȴ���������,��Ϊ�����Ǵ���ͣ״̬�ָ�������״̬
		if (mVideoPlayer == NULL)
		{
			createPlayer();
			// ��ֹͣ״̬��ʼ������Ƶʱ,Ԥ�Ƚ���һ֡
			mCurFrame = 0;
			mVideoPlayer->askDecodeFrame(mCurFrame);
		}
	}
	else if (state == PS_PAUSE)
	{
		;
	}
	else if (state == PS_STOP)
	{
		// ֹͣ����ʱ���ٲ�����
		mVideoTime = 0.0f;
		mLastFrame = -1;
		mCurFrame = -1;
		destroyPlayer();
		if (callback)
		{
			callAndClearEndCallback(true);
		}
	}
}

void VideoWindow::createPlayer()
{
	if (mVideoPlayer == NULL && mFileName != EMPTY_STRING)
	{
		mVideoPlayer = mVideoManager->createPlayer(mName, P_VIDEO_PATH + mFileName);
	}
}

void VideoWindow::destroyPlayer()
{
	if (mVideoPlayer != NULL)
	{
		mVideoManager->destroyPlayer(mVideoPlayer->getName());
		mVideoPlayer = NULL;
	}
}

float VideoWindow::getVideoLength()
{
	if (mVideoPlayer == NULL)
	{
		return 0.0f;
	}
	return mVideoPlayer->getVideoLength();
}

float VideoWindow::getVideoFPS()
{
	if (mVideoPlayer == NULL)
	{
		return 0.0f;
	}
	return mVideoPlayer->getFramesPerSecond();
}

VECTOR2 VideoWindow::getVideoSize()
{
	if (mVideoPlayer == NULL)
	{
		return txMath::VEC2_ZERO;
	}
	return mVideoPlayer->getVideoSize();
}

void VideoWindow::initProperty()
{
	TextureWindow::initProperty(); 
	addProperty<VideoWindowPropertyVideo>();
	addProperty<VideoWindowPropertyPlayState>();
	addProperty<VideoWindowPropertyVideoLoop>();
	addProperty<VideoWindowPropertyRate>();
	addProperty<VideoWindowPropertyVideoLength>();
	addProperty<VideoWindowPropertyPlayPercent>();
	addProperty<VideoWindowPropertyPlayTime>();
	addProperty<VideoWindowPropertyVideoSize>();
	addProperty<VideoWindowPropertyVideoFPS>();

	// ��ʼ������֡��ѡ�б�
	VideoWindowPropertyVideo* videoProperty = getProperty<VideoWindowPropertyVideo>();
	if (videoProperty->getChoices() != NULL && videoProperty->getChoices()->size() == 0)
	{
		txVector<std::string> fileList;
		txVector<std::string> patterns;
		patterns.push_back(".mp4");
		patterns.push_back(".avi");
		patterns.push_back(".rmvb");
		txFileUtility::findFiles(P_VIDEO_PATH, fileList, patterns, false);
		txVector<std::string> videoList;
		int fileCount = fileList.size();
		FOR_STL(fileList, int i = 0; i < fileCount; ++i)
		{
			videoList.push_back(txStringUtility::getFileName(fileList[i]));
		}
		END_FOR_STL(fileList);
		videoProperty->setChoices(videoList);
	}
}