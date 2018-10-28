#ifndef _SOCKET_PACKET_FRICTION_RET_H_
#define _SOCKET_PACKET_FRICTION_RET_H_

#include "SocketPacket.h"

// ���յ��������ظ�
class SocketPacketFrictionRet : public SocketPacket
{
public:
	SocketPacketFrictionRet(const SOCKET_PACKET& type)
		:
		SocketPacket(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void execute();
	virtual void fillParams()
	{
		pushArrayParam(mData, 6, "data");
	}
protected:
	unsigned char mData[6];
};

#endif