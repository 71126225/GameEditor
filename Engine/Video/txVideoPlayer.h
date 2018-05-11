#ifndef _TX_VIDEO_PLAYER_H_
#define _TX_VIDEO_PLAYER_H_

#include "txEngineDefine.h"

#if RUN_PLATFORM == PLATFORM_WINDOWS
#ifdef __cplusplus
extern "C"
{
#define inline _inline
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
#include "libavutil/mem.h"
#include "libavutil/imgutils.h"
}
#endif
#endif

#include "txEngineBase.h"
#include "ThreadLock.h"

class txTexture;
class txTextureManager;
struct AVFormatContext;
struct AVCodecContext;
struct AVCodec;
struct AVFrame;
struct SwsContext;

struct DecodedFrame
{
	txTexture* mTexture;
	char* mTextureBuffer;
	int mWidth;
	int mHeight;
	int mBytesPerPixel;
	DecodedFrame()
	{
		mTexture = NULL;
		mTextureBuffer = NULL;
		mWidth = 0;
		mHeight = 0;
		mBytesPerPixel = 0;
	}
};

// ����Ƶ������ֻ�ܴ�ǰ������벥��,���ܵ�����벥��
class txVideoPlayer : public txEngineBase
{
public:
	txVideoPlayer(const std::string& name, const std::string& fileName);
	virtual ~txVideoPlayer(){ destroy(); }
	virtual void init();
	virtual void update(float elapsedTime){}
	void destroy();
	txTexture* getFrame(const int& frame);
	void destroyFrame(const int& frame);
	void askDecodeFrame(const int& frame);
	const std::string& getName() { return mName; }
	const std::string& getFileName() { return mFileName; }
	VECTOR2 getVideoSize() { return VECTOR2((float)mVideoWidth, (float)mVideoHeight); }
	const float& getVideoLength(){ return mVideoLength; }
	const bool& getVideoEnd() { return mVideoEnd; }
	const float& getFramesPerSecond() { return mFramesPerSecond; }
protected:
	bool decode(const int& frame, DecodedFrame* frameData);
#if RUN_PLATFORM == PLATFORM_ANDROID
	static void* decodeFrame(void* arg);
#elif RUN_PLATFORM == PLATFORM_WINDOWS
	static DWORD WINAPI decodeFrame(LPVOID lpParameter);
#endif
protected:
	AVFormatContext* mFormatContext;
	AVCodecContext* mCodecContext;
	AVCodec* mCodec;
	AVFrame* mFrame;
	AVFrame* mFrameRGBA;
#if RUN_PLATFORM == PLATFORM_WINDOWS
	PixelFormat mConvertTargetFormat;
	AVPacket mPacket;
#endif
	SwsContext* mImgConvertContext;
	int mVideoStreamIndex;	// ��Ƶ�����±�
	std::string mName;		// ����
	std::string mFileName;	// �ļ���,��media�µ����·��
	int mBytesPerPixel;		// ÿ�����ص��ֽ�����
	uint8_t* mFrameBuffer;	// ffmpeg�����õĻ�����
	int mVideoWidth;		// ��Ƶ֡��
	int mVideoHeight;		// ��Ƶ֡��
	bool mVideoEnd;			// ��Ƶ�Ƿ��Ѿ��������
	int mLastFrame;			// ��һ��ȡ����Ƶ֡,���ڽ���ʱ�ҵ���Ҫ��֡
	float mFramesPerSecond;	// ��Ƶÿ���֡��
	float mVideoLength;		// ��Ƶ����,��
	volatile bool mRunDecodeThread;	// ���н����̱߳��
	txMap<int, txTexture*> mDecodeFrames;// first����Ƶ֡�±�,second��first����Ƶ֡����,���Ѿ�������ɵ�
	txMap<int, DecodedFrame*> mDoneFrames;// ��ɽ����֡
	txSet<int> mAddFrameSet;			// ��������֡
	txSet<int> mDestroyFrameSet;		// �������ٵ�֡
	ThreadLock mFrameLock;				// mDecodeFrames���߳���
	ThreadLock mDoneLock;				// mDoneFrameSet���߳���
	ThreadLock mAddLock;				// mAddFrameSet���߳���
	ThreadLock mDestroyLock;			// mRemoveFrameSet���߳���
	int mLastDecodeIndex;				// ��һ�ν����֡,���ڲ��ҵ�ǰ��Ҫ�����֡
#if RUN_PLATFORM == PLATFORM_WINDOWS
	volatile std::atomic<HANDLE> mDecodeThread;
#endif
};

#endif