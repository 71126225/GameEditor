#ifndef _TX_SERIALIZER_H_
#define _TX_SERIALIZER_H_

#include <string>

#define LOGW(...)
#define TRACE_NEW_ARRAY(className, size, ptr) ptr = new className[size]
#define SAFE_DELETE_ARRAY(ptr) delete[] ptr;

// �������ɶ������ļ���
class txSerializer
{
public:
	txSerializer(char* buffer, int bufferSize);
	~txSerializer();
	template<typename T>
	void write(T value)
	{
		// �����ֻ����,����д��
		if (!mWriteFlag)
		{
			LOGW("error : the buffer is read only, can not write!");
			return;
		}
		int writeLen = sizeof(T);
		// ���������Ϊ��,�򴴽�������
		if (mBuffer == NULL)
		{
			mBufferSize = writeLen;
			TRACE_NEW_ARRAY(char, mBufferSize, mBuffer);
		}
		// ����������Ѿ���������,��������չ������
		else if (writeLen + mIndex > mBufferSize)
		{
			resizeBuffer(writeLen + mIndex);
		}
		memcpy(mBuffer + mIndex, &value, writeLen);
		mIndex += writeLen;
	}
	template<typename T>
	void read(T& value)
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
		int readLen = sizeof(T);
		if (mIndex + readLen > mBufferSize)
		{
			LOGW("error : read buffer out of range! cur index : %d, buffer size : %d, read length : %d", mIndex, mBufferSize, readLen);
			return;
		}
		value = *((T*)(mBuffer + mIndex));
		mIndex += readLen;
	}
	void writeBuffer(char* buffer, int bufferSize);
	void readBuffer(char* buffer, int bufferSize, int readLen);
	void writeString(const char* str);
	void readString(char* str, int strBufferSize);
	void writeToFile(std::string fullName);
protected:
	void resizeBuffer(int maxSize)
	{
		int newSize = maxSize > mBufferSize * 2 ? maxSize : mBufferSize * 2;
		char* newBuffer = TRACE_NEW_ARRAY(char, newSize, newBuffer);
		memcpy(newBuffer, mBuffer, mBufferSize);
		SAFE_DELETE_ARRAY(mBuffer);
		mBuffer = newBuffer;
		mBufferSize = newSize;
	}
protected:
	char* mBuffer;
	int mIndex;
	int mBufferSize;
	bool mWriteFlag;	// ���Ϊ��,���ʾ��ֻд��,Ϊ�����ʾ��ֻ����
};

#endif