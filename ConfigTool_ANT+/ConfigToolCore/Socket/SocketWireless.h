#ifndef _SOCKET_WIRELESS_H_
#define _SOCKET_WIRELESS_H_

#include "CommonDefine.h"
#include "ThreadLock.h"
#include "ToolCoreBase.h"

class WriteData
{
public:
	char* mData;
	int mDataCount;
};

// �����豸��UDP���������
class StreamBuffer;
class ANTPlusPacket;
class SocketWireless : public ToolCoreBase
{
public:
	SocketWireless();
	~SocketWireless();
	void init(int port);
	void update(float elapsedTime);
	void destroy();
	static ANTPlusPacket* createPacket(ANT_PLUS_PACKET_TYPE type);
protected:
	static DWORD WINAPI updateUdpServer(LPVOID lpParameter);
	static DWORD WINAPI updateOutput(LPVOID lpParameter);
	static DWORD WINAPI praseDataThread(LPVOID lpParameter);
protected:
	float					mCurElapsedTime;		// ��ǰ��ʱ 
	int						mPort;                  // �˿ں�
	SOCKET					mServer;				// ���ڽ�����Ϣ���׽���
	HANDLE					mUDPThread;				// �׽����߳̾��
	HANDLE					mParseDataThread;		// �������ݵ��߳�
	ThreadLock				mReadBufferLock;
	std::vector<WriteData*>	mWriteDataBuffer;		// д�����ݵĻ����б�,�����������
	std::vector<WriteData*>	mWriteDataList;			// ��Ҫд��������б�,����д������
	StreamBuffer*			mWirelessStreamBuffer;
	txVector<ANTPlusPacket*>mReceivePacketBuffer;
	ThreadLock				mReceivePacketBufferLock;
};

#endif