#include "UDPServer.h"
#include "UDPPacket.h"
#include "CSPacketInit.h"
#include "CSPacketMove.h"
#include "SCPacketOtherMove.h"
#include "ThreadManager.h"
#include "CustomThread.h"
#include "UDPServerClient.h"
#include "UDPClientInfo.h"

UDPServer* UDPServer::mInstance = nullptr;

UDPServer::UDPServer()
{
	mInstance = this;
	mClient = new UDPServerClient();
}

void UDPServer::connect(int port)
{
#ifdef WIN32
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		ERROR("WSAStartup failed!");
		return;
	}
#endif
	//����������Socket
	mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mSocket == INVALID_SOCKET)
	{
		ERROR("socket failed!");
		return;
	}

	//���÷�����Socket��ַ
	sockaddr_in addrServ;
	makeSockAddr(addrServ, INADDR_ANY, port);
	//��Sockets Server
	if (bind(mSocket, (const struct sockaddr*)&addrServ, sizeof(sockaddr_in)) != 0)
	{
		mSocket = INVALID_SOCKET;
		ERROR("bind failed!");
		return;
	}
#ifdef WIN32
	int timeout = 2000;
	int ret = setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
#else
	struct timeval timeout = { 2, 0 };
	const int ret = setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(struct timeval));
#endif
	if (ret != 0)
	{
		ERROR("����socketѡ��ʧ��");
		return;
	}
	cout << "����������:IP:" << getLocalIP(mSocket) << ",�˿�" << port << endl;
	if (mUseMultiThread)
	{
		mSendThread = ThreadManager::get()->createThread("SendSocketUDP", sendSocket, this);
		mReceiveThread = ThreadManager::get()->createThread("ReceiveSocketUDP", receiveSocket, this);
		mSendThread->setTime(10);
		mReceiveThread->setTime(0, 0);
	}
}

void UDPServer::close()
{
	if (mSocket != INVALID_SOCKET)
	{
		CLOSE_SOCKET(mSocket);
		mSocket = INVALID_SOCKET;
	}
}

void UDPServer::sendSocket(CustomThread* thread)
{
	const auto* netManager = static_cast<UDPServer*>(thread->getArgs());
	// ������Ϣ
	try
	{
		thread_local static int mIndex = 0;
		auto* sendListPtr = netManager->mClient->startReadSendList();
		if (sendListPtr != nullptr)
		{
			auto& sendList = *sendListPtr;
			for(auto iter : sendList)
			{
				UDPClientInfo* clientInfo = iter.second;
				const sockaddr_in& addr = clientInfo->getAddress();
				auto& curSendList = clientInfo->getSendList();
				for (int i = 0; i < curSendList.size(); ++i)
				{
					LOG("������Ϣ,����:" + intToString(curSendList[i].second) + ", �±�:" + intToString(mIndex++) + ",Ŀ��˿�:" + intToString(addr.sin_port));
					sendto(netManager->mSocket, curSendList[i].first, curSendList[i].second, 0, (sockaddr*)&addr, sizeof(addr));
				}
				clientInfo->clear();
			}
		}
		netManager->mClient->endReadSendList();
	}
	catch (const exception& e)
	{
		LOG("exception : " + string(e.what()));
	}
}

void UDPServer::receiveSocket(CustomThread* thread)
{
	const auto* netManager = static_cast<UDPServer*>(thread->getArgs());
	// ������Ϣ
	try
	{
		sockaddr_in fromAddr;
#ifdef WIN32
		int fromLen = sizeof(fromAddr);
#else
		socklen_t fromLen = sizeof(fromAddr);
#endif
		constexpr int RecvBufferSize = 2048;
		thread_local static char RecvBuffer[RecvBufferSize];
		thread_local static int mIndex;
		const int nRecv = recvfrom(netManager->mSocket, RecvBuffer, RecvBufferSize, 0, (sockaddr*)&fromAddr, &fromLen);
		if (nRecv > 0)
		{
			LOG("�յ���Ϣ,����:" + intToString(nRecv) + ", �±�:" + intToString(mIndex++) + ",���ն˿�:" + intToString(fromAddr.sin_port) + ", ʱ��:" + getTime());
			netManager->mClient->recvData(RecvBuffer, nRecv, RecvBufferSize, fromAddr);
		}
	}
	catch (const exception& e)
	{
		LOG("exception : " + string(e.what()));
	}
}

void UDPServer::update()
{
	if (!mUseMultiThread)
	{
		constexpr int RecvBufferSize = 2048;
		thread_local static char RecvBuffer[RecvBufferSize];
		sockaddr_in fromAddr;
#ifdef WIN32
		int fromLen = sizeof(fromAddr);
#else
		socklen_t fromLen = sizeof(fromAddr);
#endif
		const int nRecv = recvfrom(mSocket, RecvBuffer, RecvBufferSize, 0, (sockaddr*)&fromAddr, &fromLen);
		if (nRecv < 0)
		{
			cout << "������:" << errno << endl;
		}
		else if (nRecv > 0)
		{
			int readIndex = 0;
			UDPPacket* packet = createPacket((PACKET_TYPE)RecvBuffer[0]);
			packet->read(RecvBuffer, RecvBufferSize, readIndex);
			static llong lastTime = timeGetTime();
			LOG("�յ���Ϣ,����:" + intToString(nRecv) + ",���ն˿�:" + intToString(fromAddr.sin_port) + ", ʱ��:" + getTime() + ", ʱ���:" + intToString(timeGetTime() - lastTime));
			lastTime = timeGetTime();
			bool hasThisClient = false;
			for (const auto& item : mClientList)
			{
				if (item.second.sin_port == fromAddr.sin_port)
				{
					hasThisClient = true;
					break;
				}
			}
			if (!hasThisClient)
			{
				mClientList.insert(fromAddr.sin_port, fromAddr);
			}

			// ���յ�һ�������Ժ�,�������ͻ��˹㲥������

			if (readIndex > nRecv)
			{
				ERROR("�������ݴ���");
			}
			packet->execute(fromAddr.sin_port);
			delete packet;
		}
	}
}

void UDPServer::doSend(char* data, int count, const sockaddr_in& addr)
{
	LOG("������Ϣ,����:" + intToString(count) + ",Ŀ��˿�:" + intToString(addr.sin_port));
	sendto(mSocket, data, count, 0, (sockaddr*)&addr, sizeof(addr));
}

UDPPacket* UDPServer::createPacket(const PACKET_TYPE type)
{
	UDPPacket* packet = nullptr;
	switch (type)
	{
	case PACKET_TYPE::CS_INIT: packet = new CSPacketInit(); break;
	case PACKET_TYPE::CS_MOVE: packet = new CSPacketMove(); break;
	case PACKET_TYPE::SC_OTHER_MOVE: packet = new SCPacketOtherMove(); break;
	default:break;
	}
	if (packet != nullptr)
	{
		packet->setType(type);
	}
	return packet;
}

void UDPServer::broadcastPacket(UDPPacket* packet, llong except)
{
	if (mUseMultiThread)
	{
		mClient->boradcast(packet, except);
	}
	else
	{
		char sendBuffer[2048]{ 0 };
		int packetSize = 0;
		packet->write(sendBuffer, 2048, packetSize);
		for (const auto& item : mClientList)
		{
			if (item.first == except)
			{
				continue;
			}
			doSend(sendBuffer, packetSize, item.second);
		}
	}
}