#ifndef _SOCKET_PACKET_FRICTION_H_
#define _SOCKET_PACKET_FRICTION_H_

#include "SocketPacket.h"

#ifdef _USE_SOCKET_UDP

// ���͵�������Ϣ
class SocketPacketFriction : public SocketPacket
{
public:
	SocketPacketFriction(SOCKET_PACKET type)
		:
		SocketPacket(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void execute(){}
	virtual void fillParams()
	{
		pushArrayParam(mData, 6, "data");
	}
	// Ƶ��,ռ�ձ�
	void setFrictionParam(char friction);
protected:
	unsigned char mData[6];
};

#endif

#endif