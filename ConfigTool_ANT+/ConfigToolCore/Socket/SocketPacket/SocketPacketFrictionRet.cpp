#include "SocketPacketFrictionRet.h"
#include "CommandHeader.h"
#include "Utility.h"
#include "SpeedDataManager.h"

void SocketPacketFrictionRet::execute()
{
	// ����ͷ��β
	if ((mData[0] != (unsigned char)0xFE || mData[5] != (unsigned char)0xFF))
	{
		return;
	}
	// ���У��λ
	int sum = 0;
	for (int i = 1; i < 4; ++i)
	{
		sum += BinaryUtility::crc_check(mData[i]);
	}
	// У�鲻ͨ����ֱ�ӷ���
	if (sum != mData[4])
	{
		return;
	}
	mSpeedDataManager->setFriction((int)((int)mData[2] / 10.0f), (int)mData[3]);
}