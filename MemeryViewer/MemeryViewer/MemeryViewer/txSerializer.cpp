#include "txSerializer.h"

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
		SAFE_DELETE_ARRAY(mBuffer);
	}
}

void txSerializer::writeBuffer(char* buffer, int bufferSize)
{
	// �����ֻ����,����д��
	if (!mWriteFlag)
	{
		LOGW("error : the buffer is read only, can not write!");
		return;
	}
	// ���������Ϊ��,�򴴽�������
	if (mBuffer == NULL)
	{
		mBufferSize = bufferSize;
		mBuffer = TRACE_NEW_ARRAY(char, mBufferSize, mBuffer);
	}
	// ����������Ѿ���������,��������չ������
	else if (bufferSize + mIndex > mBufferSize)
	{
		resizeBuffer(bufferSize + mIndex);
	}
	memcpy(mBuffer + mIndex, buffer, bufferSize);
	mIndex += bufferSize;
}

void txSerializer::readBuffer(char* buffer, int bufferSize, int readLen)
{
	// �����ֻд��,���ܶ�ȡ
	if (mWriteFlag)
	{
		LOGW("error : the buffer is write only, can not read!");
		return;
	}
	if (mBuffer == NULL)
	{
		LOGW("error : buffer is NULL! can not read");
		return;
	}
	// �ȶ����ַ�������
	if (mIndex + readLen > mBufferSize)
	{
		LOGW("error : read buffer out of range! cur index : %d, buffer size : %d, read length : %d", mIndex, mBufferSize, readLen);
		return;
	}
	// ���������ݵĿռ��С�����Է��뵱ǰҪ��ȡ������,��ֻ���������ɵĳ���,�����±�Ӧ��������ת
	if (bufferSize <= readLen)
	{
		memcpy(buffer, mBuffer + mIndex, bufferSize);
		mIndex += readLen;
	}
	else
	{
		memcpy(buffer, mBuffer + mIndex, readLen);
		mIndex += readLen;
	}
}

void txSerializer::writeString(const char* str)
{
	// �����ֻ����,����д��
	if (!mWriteFlag)
	{
		LOGW("error : the buffer is read only, can not write!");
		return;
	}
	// ��д���ַ�������
	int writeLen = strlen(str);
	write(writeLen);
	// ���������Ϊ��,�򴴽�������
	if (mBuffer == NULL)
	{
		mBufferSize = writeLen;
		mBuffer = TRACE_NEW_ARRAY(char, mBufferSize, mBuffer);
	}
	// ����������Ѿ���������,��������չ������
	else if (writeLen + mIndex > mBufferSize)
	{
		resizeBuffer(writeLen + mIndex);
	}
	memcpy(mBuffer + mIndex, str, writeLen);
	mIndex += writeLen;
}

void txSerializer::readString(char* str, int strBufferSize)
{
	// �����ֻд��,���ܶ�ȡ
	if (mWriteFlag)
	{
		LOGW("error : the buffer is write only, can not read!");
		return;
	}
	if (mBuffer == NULL)
	{
		LOGW("error : buffer is NULL! can not read");
		return;
	}
	// �ȶ����ַ�������
	int readLen = 0;
	read(readLen);
	if (mIndex + readLen > mBufferSize)
	{
		LOGW("error : read buffer out of range! cur index : %d, buffer size : %d, read length : %d", mIndex, mBufferSize, readLen);
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
	FILE* pf = NULL;
	fopen_s(&pf, fullName.c_str(), "wb");
	if (pf == NULL)
	{
		LOGW("error : can not open write file!");
		return;
	}
	fwrite(mBuffer, sizeof(char), mIndex, pf);
	fclose(pf);
}