#ifndef _SOCKET_PACKET_FRICTION_H_
#define _SOCKET_PACKET_FRICTION_H_

#include "SocketPacket.h"

// ���͵�������Ϣ
class SocketPacketFriction : public SocketPacket
{
public:
	SocketPacketFriction(const SOCKET_PACKET& type)
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