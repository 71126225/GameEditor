#ifndef _SOCKET_NET_MANAGER_H_
#define _SOCKET_NET_MANAGER_H_

#include "CommonDefine.h"
#include "SocketPacketFactory.h"
#include "ThreadLock.h"

struct OUTPUT_STREAM
{
	OUTPUT_STREAM(char* data, int dataSize)
	:
	mData(data),
	mDataSize(dataSize)
	{}
	char* mData;
	int mDataSize;
};

struct INPUT_ELEMENT
{
	INPUT_ELEMENT(SOCKET_PACKET type, char* data, int dataSize)
	:
	mType(type),
	mData(data),
	mDataSize(dataSize)
	{}
	SOCKET_PACKET mType;
	char* mData;
	int mDataSize;
};

class SocketPacketFactoryManager;
class StreamBuffer;
// ʹ��socketʱ����Ϊ��������
class SocketNetManager
{
public:
	SocketNetManager();
	~SocketNetManager(){ destroy(); }
	void init(const int& port, const int& broadcastPort);
	void update(float elapsedTime);
	void destroy();
	void processInput();
	SocketPacket* createPacket(const SOCKET_PACKET& type);
	void destroyPacket(SocketPacket* packet);
	void sendMessage(SocketPacket* packet, const bool& destroyPacketEndSend = true);
protected:
	static DWORD WINAPI updateUdpServer(LPVOID lpParameter);
	static DWORD WINAPI updateOutput(LPVOID lpParameter);
	void receivePacket(const SOCKET_PACKET& type, char* data, const int& dataSize);
protected:
	SocketPacketFactoryManager*		mSocketPacketFactoryManager;
	std::vector<OUTPUT_STREAM>		mOutputStreamList;		// �����
	int								mPort;                  // �˿ں�
	int								mBroadcastPort;			// �㲥�Ķ˿ں�
	SOCKET							mServer;				// ���ڽ�����Ϣ���׽���
	SOCKET							mBroadcastSocket;		// ���ڹ㲥���׽���
	SOCKADDR_IN						mBroadcastAddr;			// �㲥��ַ
	std::vector<INPUT_ELEMENT>		mInputStreamList;		// ������
	ThreadLock						mInputMutex;			// ������д������,�������ܶ�д������
	HANDLE							mSocketThread;			// �׽����߳̾��
	ThreadLock						mOutputMutex;			// �����������
	HANDLE							mOutputThread;			// ����������߳�
	std::vector<INPUT_ELEMENT>		mReceiverStream;		// ���յ���
};

#endif