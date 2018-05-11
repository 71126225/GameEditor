#include "HeaderData.h"

PARSE_RESULT HeaderData::setData(unsigned char* data, const int& count)
{
	// ���Ȳ�����ͷʱ,���ݲ���
	if (count < mHeaderLength)
	{
		return PR_NOT_ENOUGH;
	}
	// ȷ��һЩ�̶���ֵ����ȷ��
	if (data[0] != mMagicByte || data[1] != mVersion || data[mHeaderLength - 1] != mSeqID)
	{
		return PR_ERROR;
	}
	int offset = sizeof(mMagicByte) + sizeof(mVersion);
	unsigned short payloadLength = 0;
	BinaryUtility::read(payloadLength, data, offset, true);
	// ���Ȳ�����¼�����ݳ���,���ݲ���
	if (payloadLength > count - mHeaderLength)
	{
		return PR_NOT_ENOUGH;
	}
	unsigned short crc = 0;
	BinaryUtility::read(crc, data, offset, true);
	// У��ʧ��,���ݴ���
	unsigned short dataCRC16 = BinaryUtility::crc16(0xFF, data + mHeaderLength, payloadLength);
	if (crc != dataCRC16)
	{
		return PR_ERROR;
	}
	// ֻ�н����ɹ�ʱ,�ű�������
	mPayloadLength = payloadLength;
	mCRC16 = crc;
	return PR_SUCCESS;
}