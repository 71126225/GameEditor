#ifndef _ANT_PLUS_PACKET_H_
#define _ANT_PLUS_PACKET_H_

#include "CommonDefine.h"
#include "ToolCoreBase.h"

class ANTPlusData;
class ANTPlusPacket : public ToolCoreBase
{
public:
	float mLastReceivePacketTime;	// ��һ�ν��յ������ݰ�����ǰ��ʱ��,��λ��.
	float mLastUpdatePacketTime;	// ��һ�θ��¸����ݰ�����ǰ��ʱ��,��λ��,����յ�ʱ�䲻ͬ,���յ�����ʱ��һ�����������
	ANTPlusPacket* mLastPacket;		// ��һ�ν��յ��İ�
	ANT_PLUS_PACKET_TYPE mType;		// ��������
public:
	unsigned char* mBuffer;			// ����������ݵĻ�����
	int mBufferLength;				// ��������С
	unsigned char mLength;			// ������
	unsigned char mCmdID;			// ����ID
	unsigned char mVersionInfo[2];	// �汾��
	int mHubID;						// hub ID
	ANTPlusData* mData;				// ������
	unsigned short mCRC16;			// crc16У����
public:
	ANTPlusPacket(ANT_PLUS_PACKET_TYPE type);
	~ANTPlusPacket();
	void update(float elapsedTime);
	PARSE_RESULT read(unsigned char* data, int length, int& readCount);
	void execute();
	void updateData(ANTPlusPacket* lastPacket);
	void notifyReceived();
	static PARSE_RESULT parse(ANTPlusPacket* packet, unsigned char* data, int length);
	static void copyPacket(ANTPlusPacket* source, ANTPlusPacket* dest);
	static ANT_PLUS_PACKET_TYPE checkType(unsigned char* data, int length);
protected:
	void resizeBuffer(int size);
	virtual void checkANTPlusData() = 0;
	virtual void writeReceiveLog(){}
	virtual void writeUpdateLog(){}
	void resetLastReceiveTime() { mLastReceivePacketTime = 0.0f; }
	void resetLastUpdateTime() { mLastUpdatePacketTime = 0.0f; }
};

#endif