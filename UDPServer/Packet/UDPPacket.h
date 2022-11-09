#ifndef _UDP_PACKET_H_
#define _UDP_PACKET_H_

#include "Utility.h"
#include "CommonDefine.h"

class UDPPacket
{
public:
	virtual void read(char* buffer, int bufferSize, int& index);
	virtual void write(char* buffer, int bufferSize, int& index);
	PACKET_TYPE getType() const { return mType; }
	virtual llong getToken() { return 0; }
	void setType(PACKET_TYPE type) { mType = type; }
	// token��ʾִ�д���Ϣ�Ŀͻ���ƾ֤
	virtual void execute(llong token) {}
protected:
	PACKET_TYPE mType = PACKET_TYPE::NONE;
};

#endif