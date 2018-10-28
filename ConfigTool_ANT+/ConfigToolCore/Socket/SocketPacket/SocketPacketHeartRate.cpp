#include "SocketPacketHeartRate.h"
#include "CommandHeader.h"
#include "Utility.h"
#include "SpeedDataManager.h"
#include "HeartRateData.h"

void SocketPacketHeartRate::execute()
{
	// ����ͷ��β
	if ((mData[0] != (unsigned char)0xFE || mData[8] != (unsigned char)0xFF))
	{
		return;
	}
	// ���У��λ
	int sum = 0;
	for (int i = 1; i < 7; ++i)
	{
		sum += BinaryUtility::crc_check(mData[i]);
	}
	// У�鲻ͨ����ֱ�ӷ���
	if (sum != mData[7])
	{
		return;
	}
	HeartRateData* heartRateData = TRACE_NEW(HeartRateData, heartRateData);
	memcpy(heartRateData->mData, mData, heartRateData->mDataSize);
	heartRateData->mNumber = StringUtility::charArrayToHexString(mData + 2, 3, false);
	heartRateData->mHeartRate = mData[5];
	mSpeedDataManager->setHeartRateData(heartRateData);
}