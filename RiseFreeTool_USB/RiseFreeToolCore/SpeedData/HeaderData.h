#ifndef _HEADER_DATA_H_
#define _HEADER_DATA_H_

#include "CommonDefine.h"

// ���ݰ�ͷ
class HeaderData
{
public:
	HeaderData()
	{
		mReportID = REPORT_IN;
		mMagicByte = (char)0xa5;
		mVersion = 0;
		mPayloadLength = 0;
		mCRC16 = 0;
		mSeqID = 0;
		mHeaderLength = sizeof(mReportID)+sizeof(mMagicByte)+sizeof(mVersion)+sizeof(mPayloadLength)+sizeof(mCRC16)+sizeof(mSeqID);
	}
	virtual ~HeaderData(){}
	PARSE_RESULT setData(unsigned char* data, const int& count);
	int getHeaderLength() { return mHeaderLength; }
public:
	int mHeaderLength;				// ��ͷ����
	unsigned char mReportID;		// �����reportID���̶�Ϊ0x02
	unsigned char mMagicByte;		// �̶�0xa5
	unsigned char mVersion;			// ��ǰ�汾��,0
	unsigned short mPayloadLength;	// payload���ݳ���
	unsigned short mCRC16;			// payload����CRC16У����
	unsigned short mSeqID;			// �����ֶ�,�̶�Ϊ0
};

#endif