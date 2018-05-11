#ifndef _COM_MANAGER_H_
#define _COM_MANAGER_H_

#include "EditorCoreBase.h"
#include "CommonDefine.h"
#include "txSerializer.h"
#include "txThreadLock.h"

class WriteData
{
public:
	char* mData;
	int mDataCount;
};

class ComManager : public EditorCoreBase
{
public:
	ComManager();
	virtual ~ComManager(){ destroy(); }
	void init();
	void destroy();
	void update(float elapsedTime);
	// ˢ�´����豸�б�
	void checkDevice();
	void setCurDevice(const std::string& device);
	void setFriction(const int& friction);
	const std::string& getCurDevice() { return mCurDevice; }
	txVector<std::string>& getDeviceList() { return mDeviceList; }
protected:
	static DWORD WINAPI writeThread(LPVOID lpParameter);
	static DWORD WINAPI readThread(LPVOID lpParameter);
	static DWORD WINAPI praseDataThread(LPVOID lpParameter);
	void closeCurDeviceHandle();
	void resizeBuffer(int size);
	void addDataToBuffer(unsigned char* data, int dataCount);
	void removeDataFromBuffer(int start, int count);
	void clearBuffer();
protected:
	txVector<std::string> mDeviceList;
	std::string mCurDevice;
	std::atomic<HANDLE> mCurDeviceHandle;
	HANDLE mWriteThread;
	HANDLE mReadThread;
	HANDLE mParseDataThread;
	txThreadLock mReadBufferLock;
	txThreadLock mWriteBufferLock;
	unsigned char* mReadBuffer;			// ���յ������ݵĻ�����
	std::atomic<int> mDataLength;		// ��������ʣ����յ������ݳ���
	std::atomic<int> mBufferSize;		// �������ܴ�С
	txVector<WriteData*> mWriteDataBuffer;	// д�����ݵĻ����б�,�����������
	txVector<WriteData*> mWriteDataList;	// ��Ҫд��������б�,����д������
};

#endif