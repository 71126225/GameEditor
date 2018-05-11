#ifndef _SOCKET_NET_MANAGER_H_
#define _SOCKET_NET_MANAGER_H_

#include "CommonDefine.h"
#include "txThreadLock.h"
#include "ToolCoreBase.h"

class WriteData
{
public:
	char* mData;
	int mDataCount;
};

class SocketNetManager : public ToolCoreBase
{
public:
	SocketNetManager();
	~SocketNetManager();
	void init(int port);
	void update(float elapsedTime){}
	void destroy();
protected:
	static DWORD WINAPI updateUdpServer(LPVOID lpParameter);
	static DWORD WINAPI updateOutput(LPVOID lpParameter);
	static DWORD WINAPI praseDataThread(LPVOID lpParameter);
	void resizeBuffer(const int& size);
	void addDataToBuffer(unsigned char* data, const int& dataCount);
	void removeDataFromBuffer(const int& start, const int& count);
	void clearBuffer();
protected:
	float					mCurElapsedTime;		// ��ǰ��ʱ 
	int						mPort;                  // �˿ں�
	SOCKET					mServer;				// ���ڽ�����Ϣ���׽���
	HANDLE					mUDPThread;				// �׽����߳̾��
	HANDLE					mParseDataThread;		// �������ݵ��߳�
	txThreadLock			mReadBufferLock;
	unsigned char*			mReadBuffer;			// ���յ������ݵĻ�����
	std::atomic<int>		mDataLength;			// ��������ʣ����յ������ݳ���
	std::atomic<int>		mBufferSize;			// �������ܴ�С
	std::vector<WriteData*>	mWriteDataBuffer;		// д�����ݵĻ����б�,�����������
	std::vector<WriteData*>	mWriteDataList;			// ��Ҫд��������б�,����д������
};

#endif