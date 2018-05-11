#ifndef _TX_SERIALIZER_H_
#define _TX_SERIALIZER_H_

#include "EditorCoreLog.h"
#include "CommonDefine.h"

// �������ɶ������ļ���
class txSerializer
{
public:
	txSerializer(bool traceMemery = true);
	txSerializer(char* buffer, int bufferSize);
	~txSerializer();
	template<typename T>
	void write(T value, bool inverse = false)
	{
		// �����ֻ����,����д��
		if (!mWriteFlag)
		{
			EDITOR_CORE_ERROR("error : the buffer is read only, can not write!");
			return;
		}
		int writeLen = sizeof(T);
		// ���������Ϊ��,�򴴽�������
		if (mBuffer == NULL)
		{
			createBuffer(writeLen);
		}
		// ����������Ѿ���������,��������չ������
		else if (writeLen + mIndex > mBufferSize)
		{
			resizeBuffer(writeLen + mIndex);
		}
		BinaryUtility::write(value, (unsigned char*)mBuffer, mIndex, inverse);
	}
	template<typename T>
	void read(T& value, bool inverse = false)
	{
		// �����ֻд��,���ܶ�ȡ
		if (mWriteFlag)
		{
			EDITOR_CORE_ERROR("error : the buffer is write only, can not read!");
			return;
		}
		if (mBuffer == NULL)
		{
			EDITOR_CORE_ERROR("error : buffer is NULL! can not read");
			return;
		}
		int readLen = sizeof(T);
		if (mIndex + readLen > mBufferSize)
		{
			EDITOR_CORE_ERROR("error : read buffer out of range! cur index : %d, buffer size : %d, read length : %d", mIndex, mBufferSize, readLen);
			return;
		}
		BinaryUtility::read(value, (unsigned char*)mBuffer, mIndex, inverse);
	}
	void writeBuffer(char* buffer, int bufferSize);
	void readBuffer(char* buffer, int bufferSize, int readLen);
	void writeString(const char* str);
	void readString(char* str, int strBufferSize);
	void writeToFile(std::string fullName);
	const char* getBuffer() const { return mBuffer; }
	int getBufferSize() { return mBufferSize; }
	int getDataSize() { return mIndex; }
protected:
	void resizeBuffer(int maxSize)
	{
		int newSize = maxSize > mBufferSize * 2 ? maxSize : mBufferSize * 2;
		char* newBuffer = NULL;
		if (mTraceMemery)
		{
			newBuffer = TRACE_NEW_ARRAY(char, newSize, newBuffer);
		}
		else
		{
			newBuffer = NORMAL_NEW_ARRAY(char, newSize, newBuffer);
		}
		memcpy(newBuffer, mBuffer, mBufferSize);
		if (mTraceMemery)
		{
			TRACE_DELETE_ARRAY(mBuffer);
		}
		else
		{
			NORMAL_DELETE_ARRAY(mBuffer);
		}
		mBuffer = newBuffer;
		mBufferSize = newSize;
	}
	void createBuffer(int bufferSize)
	{
		if (mBuffer == NULL)
		{
			mBufferSize = bufferSize;
			if (mTraceMemery)
			{
				mBuffer = TRACE_NEW_ARRAY(char, mBufferSize, mBuffer);
			}
			else
			{
				mBuffer = NORMAL_NEW_ARRAY(char, mBufferSize, mBuffer);
			}
		}
	}
protected:
	char* mBuffer;
	int mIndex;
	int mBufferSize;
	bool mWriteFlag;	// ���Ϊ��,���ʾ��ֻд��,Ϊ�����ʾ��ֻ����
	bool mTraceMemery;	// Ϊ�����ʾ���е��ڴ������ͷŲ������ᱻ��¼����,Ϊ���򲻻ᱻ��¼,�ڼ�¼�ڴ������Ϣʱ���õ�
};

#endif