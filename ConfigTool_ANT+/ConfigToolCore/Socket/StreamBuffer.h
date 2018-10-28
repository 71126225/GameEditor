#ifndef _STREAM_BUFFER_H_
#define _STREAM_BUFFER_H_

#include "CommonDefine.h"

class StreamBuffer
{
public:
	void resizeBuffer(const int& size);
	void addDataToBuffer(unsigned char* data, const int& dataCount);
	void removeDataFromBuffer(const int& start, const int& count);
	void clearBuffer();
public:
	unsigned char*			mReadBuffer;			// ���յ������ݵĻ�����
	std::atomic<int>		mDataLength;			// ��������ʣ����յ������ݳ���
	std::atomic<int>		mBufferSize;			// �������ܴ�С
};

#endif