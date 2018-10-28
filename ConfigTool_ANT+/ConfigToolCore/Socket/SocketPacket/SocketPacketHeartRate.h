#ifndef _SOCKET_PACKET_HEART_RATE_H_
#define _SOCKET_PACKET_HEART_RATE_H_

#include "SocketPacket.h"

// ���յ�����������
class SocketPacketHeartRate : public SocketPacket
{
public:
	SocketPacketHeartRate(const SOCKET_PACKET& type)
		:
		SocketPacket(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void execute();
	virtual void fillParams()
	{
		pushArrayParam(mData, 9, "data");
	}
protected:
	unsigned char mData[9];
};

#endif