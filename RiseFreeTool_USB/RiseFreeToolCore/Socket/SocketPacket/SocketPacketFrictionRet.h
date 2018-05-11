#ifndef _SOCKET_PACKET_FRICTION_RET_H_
#define _SOCKET_PACKET_FRICTION_RET_H_

#include "SocketPacket.h"

#ifdef _USE_SOCKET_UDP

// ���յ��������ظ�
class SocketPacketFrictionRet : public SocketPacket
{
public:
	SocketPacketFrictionRet(SOCKET_PACKET type)
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
#endif