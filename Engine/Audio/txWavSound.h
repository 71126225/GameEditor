#ifndef _TX_WAV_SOUND_H_
#define _TX_WAV_SOUND_H_

#include "txEngineDefine.h"
#include "txEngineBase.h"

class txSerializer;
class txWavSound : public txEngineBase
{
public:
	txWavSound();
	txWavSound(const std::string& file);
	virtual ~txWavSound();
	void init();
	bool readFile(const std::string& file);
	void writeFile(const std::string& newFileName);
	const char* getPCMBuffer(){ return mDataBuffer; }
	const short* getShortPCMData() { return (short*)(mDataBuffer); }
	const short* getMixPCMData() { return mMixPCMData; }
	const DWORD& getPCMBufferSize() { return mDataSize; }
	const WORD& getSoundChannels() { return mSoundChannels; }
	DWORD getPCMShortDataCount() { return mDataSize / sizeof(short); }
	DWORD getMixPCMDataCount() { return mDataSize / sizeof(short) / mSoundChannels; }
	bool playSound();
#if RUN_PLATFORM == PLATFORM_WINDOWS
	void startWaveStream(const WAVEFORMATEX& waveHeader);
#endif
	void pushWaveStream(char* data, const int& dataSize);
	void endWaveStream();
	static void generateMixPCMData(short* mixPCMData, const int& mixDataCount, const int& channelCount, const char* dataBuffer, const int& bufferSize);
protected:
#if RUN_PLATFORM == PLATFORM_WINDOWS
	static DWORD WINAPI playThread(LPVOID lpParameter);
#endif
	void refreshFileSize();
public:
	std::string mFileName;
	DWORD mRiffMark;			// riff���
	DWORD mFileSize;			// ��Ƶ�ļ���С - 8,Ҳ���Ǵ��ļ���С�ֽں��ļ���β�ĳ���
	DWORD mWaveMark;			// wave���
	DWORD mFmtMark;				// fmt ���
	DWORD mFmtChunkSize;		// fmt���С
	WORD mFormatType;			// �����ʽ,Ϊ1��PCM����
	WORD mSoundChannels;		// ������
	DWORD mSamplesPerSec;		// ����Ƶ��
	DWORD mAvgBytesPerSec;		// �������ݴ������ʣ�ÿ��ƽ���ֽ�����
	WORD mBlockAlign;			// DATA���ݿ鳤��
	WORD mBitsPerSample;		// �����������ݴ�С,���˫����16λ,����4���ֽ�,Ҳ��PCMλ��
	WORD mOtherSize;			// ������Ϣ����ѡ�����Ϸ������ֽ�ȷ����
	char mDataMark[4];			// data���
	DWORD mDataSize;
	char* mDataBuffer;
	short* mMixPCMData;
	volatile bool mPlay;
	volatile bool mIsPlaying;
	txSerializer* mWaveDataSerializer;
};

#endif