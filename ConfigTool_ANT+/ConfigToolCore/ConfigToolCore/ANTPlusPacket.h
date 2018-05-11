#ifndef _ANT_PLUS_PACKET_H_
#define _ANT_PLUS_PACKET_H_

#include "CommonDefine.h"

class ANTPlusData;
class ANTPlusPacket
{
public:
	bool mHasRead;					// �Ƿ��Ѿ���ȡ���˸�����
	float mLastPacketTime;			// ��һ�θ��¸����ݰ�����ǰ��ʱ��,��λ��
	ANTPlusPacket* mLastPacket;		// ��һ�ν��յ��İ�
public:
	unsigned char* mBuffer;			// ����������ݵĻ�����
	int mBufferLength;				// ��������С
	unsigned char mLength;			// ������
	unsigned char mCmdID;			// ����ID
	unsigned char mVersionInfo[2];	// �汾��
	int mHubID;						// hub ID
	ANTPlusData* mData;				// ������
	unsigned short mCRC16;			// crc16У����
	ANTPlusPacket();
	~ANTPlusPacket();
	PARSE_RESULT parseData(unsigned char* data, const int& length);
	void setNewData(ANTPlusPacket* that);
protected:
	void resizeBuffer(const int& size);
	void checkANTPlusData(const unsigned char& cmdID);
	void saveLastPacket();
};

#endif