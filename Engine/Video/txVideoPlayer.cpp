#include "txVideoPlayer.h"
#include "txEngineLog.h"
#include "txEngineRoot.h"
#include "txTexture.h"
#include "txTextureManager.h"
#include "Utility.h"
#include "TimeLock.h"

txVideoPlayer::txVideoPlayer(const std::string& name, const std::string& fileName)
:
mFormatContext(NULL),
mCodecContext(NULL),
mCodec(NULL),
mFrame(NULL),
mFrameRGBA(NULL),
#if RUN_PLATFORM == PLATFORM_WINDOWS
mConvertTargetFormat(PIX_FMT_RGBA),
#endif
mFrameBuffer(NULL),
mImgConvertContext(NULL),
mVideoStreamIndex(-1),
mName(name),
mFileName(fileName),
mVideoWidth(0),
mVideoHeight(0),
mBytesPerPixel(4),
mVideoEnd(false),
mLastFrame(-1),
mFramesPerSecond(29),
mLastDecodeIndex(-1),
mVideoLength(0.0f),
mRunDecodeThread(true)
{
	;
}

void txVideoPlayer::init()
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	// ��һ����Ƶ�ļ�,��ȡ�ļ�ͷ
	int ret = avformat_open_input(&mFormatContext, mFileName.c_str(), NULL, NULL);
	if (ret != 0)
	{
		ENGINE_ERROR("error : open video failed! error code : %d, file name : %s", ret, mFileName.c_str());
	}

	// ����ļ��е�����Ϣ
	int retInfo = avformat_find_stream_info(mFormatContext, NULL);
	if (retInfo < 0)
	{
		ENGINE_ERROR("error : find stream info failed! error code : %d", retInfo);
	}

	// �ҵ����е���Ƶ��
	mVideoStreamIndex = -1;
	for (int i = 0; i < (int)mFormatContext->nb_streams; ++i)
	{
		if (mFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			mVideoStreamIndex = i;
			break;
		}
	}
	
	if (mVideoStreamIndex == -1)
	{
		return;
	}

	AVStream* videoStream = mFormatContext->streams[mVideoStreamIndex];
	mFramesPerSecond = (float)(videoStream->avg_frame_rate.num) / videoStream->avg_frame_rate.den;
	mVideoLength = (float)(videoStream->duration) / videoStream->time_base.den;
	// �����Ƶ��ʽ��Ӧ�Ľ�����
	mCodecContext = mFormatContext->streams[mVideoStreamIndex]->codec;
	mCodec = avcodec_find_decoder(mCodecContext->codec_id);
	if (mCodec == NULL) 
	{
		return;
	}
	// �򿪽�����
	if (avcodec_open2(mCodecContext, mCodec, NULL) < 0)
	{
		return;
	}
	// ������,Ϊ������������
	mVideoWidth = mCodecContext->width;
	mVideoHeight = mCodecContext->height;

	// Ϊÿһ֡�Ļ�����������ڴ�
	int numBytes = avpicture_get_size(mConvertTargetFormat, mVideoWidth, mVideoHeight);
	mFrameBuffer = (uint8_t*)av_malloc(numBytes);
	mFrame = avcodec_alloc_frame();
	if (mFrame == NULL)
	{
		return;
	}
	mFrameRGBA = avcodec_alloc_frame();
	avpicture_fill((AVPicture*)mFrameRGBA, mFrameBuffer, mConvertTargetFormat, mVideoWidth, mVideoHeight);
	if (mFrameRGBA == NULL)
	{
		return;
	}

	// ��ʼ����ɺ󴴽������߳�
#if RUN_PLATFORM == PLATFORM_ANDROID
	pthread_t thread;
	pthread_create(&thread, NULL, txVideoPlayer::decodeFrame, this);
	pthread_detach(thread);
#elif RUN_PLATFORM == PLATFORM_WINDOWS
	mDecodeThread = CreateThread(NULL, 0, txVideoPlayer::decodeFrame, this, 0, NULL);
#endif
#endif
}

txTexture* txVideoPlayer::getFrame(const int& frame)
{
	txTexture* texture = NULL;
	LOCK(mDoneLock);
	// �鿴���֡�б�����û��ָ��֡
	auto iterDone = mDoneFrames.find(frame);
	if (iterDone != mDoneFrames.end())
	{
		// �ȵ���ȡ����ʱ�Ŵ�������,���������������������ɫ������
		if (iterDone->second->mTextureBuffer != NULL)
		{
			iterDone->second->mTexture->createCustomTexture(iterDone->second->mTextureBuffer, iterDone->second->mWidth, iterDone->second->mHeight, iterDone->second->mBytesPerPixel, false);
			TRACE_DELETE_ARRAY(iterDone->second->mTextureBuffer);
		}
		if (iterDone->second->mTexture->getTextureID() != 0)
		{
			texture = iterDone->second->mTexture;
		}
	}
	UNLOCK(mDoneLock);
	return texture;
}

void txVideoPlayer::destroyFrame(const int& frame)
{
	LOCK(mDestroyLock);
	mDestroyFrameSet.insert(frame);
	UNLOCK(mDestroyLock);
}

void txVideoPlayer::askDecodeFrame(const int& frame)
{
	LOCK(mAddLock);
	mAddFrameSet.insert(frame);
	UNLOCK(mAddLock);
}

bool txVideoPlayer::decode(const int& frame, DecodedFrame* frameData)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	if (frame <= mLastFrame || frameData == NULL)
	{
		return false;
	}
	// ��ȡһ֡���ݴ�ŵ�packet��
	while (true)
	{
		int readRet = av_read_frame(mFormatContext, &mPacket);
		if (readRet < 0)
		{
			mVideoEnd = true;
			break;
		}
		// �ҵ���Ƶ��
		if (mPacket.stream_index == mVideoStreamIndex)
		{
			// ������Ƶ��
			int frameFinished = 0;
			avcodec_decode_video2(mCodecContext, mFrame, &frameFinished, &mPacket);
			if (frameFinished != 0)
			{
				++mLastFrame;
				if (frame == mLastFrame)
				{
					// ���ת��ΪRGBA�Ļ���,���ҽ�ԭʼ��������ת��ΪRGBA��������
					mImgConvertContext = sws_getCachedContext(mImgConvertContext, mVideoWidth, mVideoHeight, mCodecContext->pix_fmt, mVideoWidth, mVideoHeight, mConvertTargetFormat, SWS_BICUBIC, NULL, NULL, NULL);
					if (mImgConvertContext == NULL)
					{
						ENGINE_ERROR("error : Cannot initialize sws conversion context");
					}
					sws_scale(mImgConvertContext, (const uint8_t * const *)mFrame->data, mFrame->linesize, 0, mVideoHeight, mFrameRGBA->data, mFrameRGBA->linesize);
					av_free_packet(&mPacket);

					// ��Ҫȷ��һ�еĴ�С������Ƶ��*ÿ�����ص��ֽ���
					if (mFrameRGBA->linesize[0] * mVideoHeight != mVideoWidth * mVideoHeight * mBytesPerPixel)
					{
						ENGINE_ERROR("error : frame size error! line size : %d, video height : %d, video width : %d, bytes per pixel : %d", 
							mFrameRGBA->linesize[0], mVideoHeight, mVideoWidth, mBytesPerPixel);
					}

					// ������Ƶ����
					frameData->mTextureBuffer = TRACE_NEW_ARRAY(char, mFrameRGBA->linesize[0] * mVideoHeight, frameData->mTextureBuffer);
					memcpy(frameData->mTextureBuffer, mFrameRGBA->data[0], mFrameRGBA->linesize[0] * mVideoHeight);
					txBinaryUtility::swapPixelBuffer(frameData->mTextureBuffer, mVideoWidth, mVideoHeight, mBytesPerPixel);
					frameData->mWidth = mVideoWidth;
					frameData->mHeight = mVideoHeight;
					frameData->mBytesPerPixel = mBytesPerPixel;
					break;
				}
			}
		}
		av_free_packet(&mPacket);
	}
#endif
	return true;
}

#if RUN_PLATFORM == PLATFORM_ANDROID
void* txVideoPlayer::decodeFrame(void* arg)
{
	txVideoPlayer* videoPlayer = (txVideoPlayer*)(arg);
	return 0;
}
#elif RUN_PLATFORM == PLATFORM_WINDOWS
DWORD WINAPI txVideoPlayer::decodeFrame(LPVOID lpParameter)
{
	TimeLock timeLock(15);
	txVideoPlayer* videoPlayer = (txVideoPlayer*)(lpParameter);
	while (videoPlayer->mRunDecodeThread)
	{
		// ��֡,����ֻ����δ������Ƶʱ��ʹ��
		timeLock.update();

		LOCK(videoPlayer->mFrameLock);
		
		// �����һ��ѭ����Ϊ�������������continue����breakʱ������ȷִ�н�������,�Լ���֡����		
		for (int i = 0; i < 1; ++i)
		{
			// ���������֡�б�ϲ��������б���
			LOCK(videoPlayer->mAddLock);
			if (videoPlayer->mAddFrameSet.size() > 0)
			{
				auto iterAdd = videoPlayer->mAddFrameSet.begin();
				auto iterAddEnd = videoPlayer->mAddFrameSet.end();
				FOR_STL(videoPlayer->mAddFrameSet, ; iterAdd != iterAddEnd; ++iterAdd)
				{
					// ������������֡���ܺϲ����б���
					if (videoPlayer->mDecodeFrames.size() > 0 && (videoPlayer->mDecodeFrames.rbegin()->first >= *iterAdd
						|| videoPlayer->mDecodeFrames.find(*iterAdd) != videoPlayer->mDecodeFrames.end()))
					{
						continue;
					}
					videoPlayer->mDecodeFrames.insert(*iterAdd, (txTexture*)NULL);
				}
				END_FOR_STL(videoPlayer->mAddFrameSet);
				// �ϲ���֮����������б�
				videoPlayer->mAddFrameSet.clear();
			}
			UNLOCK(videoPlayer->mAddLock);

			// ����������֡�б�ϲ����������
			LOCK(videoPlayer->mDestroyLock);
			if (videoPlayer->mDestroyFrameSet.size() > 0)
			{
				auto iterDestroy = videoPlayer->mDestroyFrameSet.begin();
				auto iterDestroyEnd = videoPlayer->mDestroyFrameSet.end();
				FOR_STL(videoPlayer->mDestroyFrameSet, ; iterDestroy != iterDestroyEnd; ++iterDestroy)
				{
					auto iterDecode = videoPlayer->mDecodeFrames.find(*iterDestroy);
					if (iterDecode != videoPlayer->mDecodeFrames.end())
					{
						if (iterDecode->second != NULL)
						{
							mTextureManager->destroyTexture(iterDecode->second->getName());
						}
						videoPlayer->mDecodeFrames.erase(iterDecode);
					}
					auto iterDone = videoPlayer->mDoneFrames.find(*iterDestroy);
					if (iterDone != videoPlayer->mDoneFrames.end())
					{
						// ��Ϊ�ⲿ����û�л�ȡ��һ֡�����ٸ�֡,����������ʱ��Ҫȷ���������ݻ������Ѿ����ͷ�
						TRACE_DELETE_ARRAY(iterDone->second->mTextureBuffer);
						TRACE_DELETE(iterDone->second);
						videoPlayer->mDoneFrames.erase(iterDone);
					}
				}
				END_FOR_STL(videoPlayer->mDestroyFrameSet);
				videoPlayer->mDestroyFrameSet.clear();
			}
			UNLOCK(videoPlayer->mDestroyLock);

			if (videoPlayer->mDecodeFrames.size() == 0)
			{
				continue;
			}
			// �������Ҫ�������Ƶ֡
			// �ҵ���һ�ν�����±�,Ȼ��Ӹ�λ���������������Ҫ�������Ƶ֡
			auto iterDecode = videoPlayer->mDecodeFrames.find(videoPlayer->mLastDecodeIndex);
			if (iterDecode != videoPlayer->mDecodeFrames.end())
			{
				// ����Ѿ�������������Ҫ�����֡,�򲻽���
				if (++iterDecode == videoPlayer->mDecodeFrames.end())
				{
					continue;
				}
			}
			// ����Ҳ���,��Ӧ������Ҫ���һ֡
			else
			{
				iterDecode = videoPlayer->mDecodeFrames.begin();
			}

			DecodedFrame* frameData = TRACE_NEW(DecodedFrame, frameData);
			videoPlayer->decode(iterDecode->first, frameData);
			// ��Ƶ������ɺ��˳����߳�
			if (videoPlayer->getVideoEnd())
			{
				videoPlayer->mRunDecodeThread = false;
				break;
			}
			videoPlayer->mLastDecodeIndex = iterDecode->first;

			// �˴���������,����ֵ�������б�
			frameData->mTexture = mTextureManager->createTexture(videoPlayer->mName + "_Video_" + txStringUtility::intToString(iterDecode->first));
			// ������ɺ�����ɵ�֡�������֡�б���
			LOCK(videoPlayer->mDoneLock);
			videoPlayer->mDoneFrames.insert(iterDecode->first, frameData);
			UNLOCK(videoPlayer->mDoneLock);
			iterDecode->second = frameData->mTexture;
		}

		UNLOCK(videoPlayer->mFrameLock);
	}
	videoPlayer->mDecodeThread = NULL;
	return 0;
}
#endif

void txVideoPlayer::destroy()
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	// �ȴ������߳��˳�
	mRunDecodeThread = false;
	while (mDecodeThread != NULL) 
	{}
	av_free((void*)mFrameBuffer);
	av_free(mFrameRGBA);
	av_free(mFrame);
	avcodec_close(mCodecContext);
	avformat_close_input(&mFormatContext);
	mImgConvertContext = NULL;
	mLastFrame = -1;
	auto iterDecode = mDecodeFrames.begin();
	auto iterDecodeEnd = mDecodeFrames.end();
	FOR_STL(mDecodeFrames, ; iterDecode != iterDecodeEnd; ++iterDecode)
	{
		if (iterDecode->second != NULL)
		{
			mTextureManager->destroyTexture(iterDecode->second->getName());
		}
	}
	END_FOR_STL(mDecodeFrames);
	auto iterDone = mDoneFrames.begin();
	auto iterDoneEnd = mDoneFrames.end();
	FOR_STL(mDoneFrames, ; iterDone != iterDoneEnd; ++iterDone)
	{
		TRACE_DELETE_ARRAY(iterDone->second->mTextureBuffer);
		TRACE_DELETE(iterDone->second);
	}
	END_FOR_STL(mDoneFrames);
#endif
}