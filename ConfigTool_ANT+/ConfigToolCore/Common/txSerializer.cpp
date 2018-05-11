#include "txSerializer.h"

txSerializer::txSerializer(bool traceMemery)
:
mTraceMemery(traceMemery),
mWriteFlag(true),
mIndex(0),
mBufferSize(0),
mBuffer(NULL)
{}

txSerializer::txSerializer(char* buffer, int bufferSize)
:
mWriteFlag(false),
mIndex(0),
mBufferSize(bufferSize),
mBuffer(buffer)
{}

txSerializer::~txSerializer()
{
	if (mWriteFlag)
	{
		if (mTraceMemery)
		{
			TRACE_DELETE_ARRAY(mBuffer);
		}
		else
		{
			NORMAL_DELETE_ARRAY(mBuffer);
		}
	}
}

void txSerializer::writeBuffer(char* buffer, int bufferSize)
{
	// �����ֻ����,����д��
	if (!mWriteFlag)
	{
		TOOL_CORE_ERROR("error : the buffer is read only, can not write!");
		return;
	}
	// ���������Ϊ��,�򴴽�������
	if (mBuffer == NULL)
	{
		createBuffer(bufferSize);
	}
	// ����������Ѿ���������,��������չ������
	else if (bufferSize + mIndex > mBufferSize)
	{
		resizeBuffer(bufferSize + mIndex);
	}
	BinaryUtility::writeBuffer((unsigned char*)buffer, (unsigned char*)mBuffer, bufferSize, mIndex);
}

void txSerializer::readBuffer(char* buffer, int bufferSize, int readLen)
{
	// �����ֻд��,���ܶ�ȡ
	if (mWriteFlag)
	{
		TOOL_CORE_ERROR("error : the buffer is write only, can not read!");
		return;
	}
	if (mBuffer == NULL)
	{
		TOOL_CORE_ERROR("error : buffer is NULL! can not read");
		return;
	}
	// �ȶ����ַ�������
	if (mIndex + readLen > mBufferSize)
	{
		TOOL_CORE_ERROR("error : read buffer out of range! cur index : %d, buffer size : %d, read length : %d", mIndex, mBufferSize, readLen);
		return;
	}
	// ���������ݵĿռ��С�����Է��뵱ǰҪ��ȡ������,��ֻ���������ɵĳ���,�����±�Ӧ��������ת
	int minSize = min(bufferSize, readLen);
	memcpy(buffer, mBuffer + mIndex, minSize);
	mIndex += readLen;
}

void txSerializer::writeString(const char* str)
{
	// �����ֻ����,����д��
	if (!mWriteFlag)
	{
		TOOL_CORE_ERROR("error : the buffer is read only, can not write!");
		return;
	}
	// ��д���ַ�������
	int writeLen = strlen(str);
	write(writeLen);
	writeBuffer((char*)str, writeLen);
}

void txSerializer::readString(char* str, int strBufferSize)
{
	// �����ֻд��,���ܶ�ȡ
	if (mWriteFlag)
	{
		TOOL_CORE_ERROR("error : the buffer is write only, can not read!");
		return;
	}
	if (mBuffer == NULL)
	{
		TOOL_CORE_ERROR("error : buffer is NULL! can not read");
		return;
	}
	// �ȶ����ַ�������
	int readLen = 0;
	read(readLen);
	if (mIndex + readLen > mBufferSize)
	{
		TOOL_CORE_ERROR("error : read buffer out of range! cur index : %d, buffer size : %d, read length : %d", mIndex, mBufferSize, readLen);
		return;
	}
	// �������ַ����Ŀռ��С�����Է��뵱ǰҪ��ȡ���ַ���,��ֻ���������ɵĳ���,�����±�Ӧ��������ת
	if (strBufferSize <= readLen)
	{
		memcpy(str, mBuffer + mIndex, strBufferSize - 1);
		mIndex += readLen;
		// ���Ͻ�����
		str[strBufferSize - 1] = 0;
	}
	else
	{
		memcpy(str, mBuffer + mIndex, readLen);
		mIndex += readLen;
		// ���Ͻ�����
		str[readLen] = 0;
	}
}

void txSerializer::writeToFile(std::string fullName)
{
	// ȷ����ֻд��,�������ݲ�Ϊ��
	if (!mWriteFlag || mBuffer == NULL || mIndex <= 0)
	{
		return;
	}
	FileUtility::writeFile(fullName, mIndex, mBuffer);
}