#include "StreamBuffer.h"
#include "Utility.h"
#include "txMemoryTrace.h"
#include "ToolCoreLog.h"
#include "ToolCoreUtility.h"

void StreamBuffer::resizeBuffer(const int& size)
{
	if (mBufferSize >= size)
	{
		return;
	}
	if (mReadBuffer != NULL)
	{
		// �����µĻ�����,��ԭ�������ݿ������»�������,����ԭ������,ָ���»�����
		unsigned char* newBuffer = TRACE_NEW_ARRAY(unsigned char, size, newBuffer);
		if (mDataLength > 0)
		{
			memcpy(newBuffer, mReadBuffer, mDataLength);
		}
		TRACE_DELETE_ARRAY(mReadBuffer);
		mReadBuffer = newBuffer;
		mBufferSize = size;
	}
	else
	{
		mReadBuffer = TRACE_NEW_ARRAY(unsigned char, size, mReadBuffer);
		mBufferSize = size;
	}
}

void StreamBuffer::addDataToBuffer(unsigned char* data, const int& dataCount)
{
	// �����ǰ�Ѿ���Ų����µ�������,���ٴ���������
	if (mBufferSize - mDataLength < dataCount)
	{
		return;
	}
	memcpy(mReadBuffer + mDataLength, data, dataCount);
	mDataLength += dataCount;
}

void StreamBuffer::removeDataFromBuffer(const int& start, const int& count)
{
	if (mDataLength < start + count)
	{
		return;
	}
	memmove(mReadBuffer + start, mReadBuffer + start + count, mDataLength - start - count);
	mDataLength -= count;
}

void StreamBuffer::clearBuffer()
{
	mDataLength = 0;
}