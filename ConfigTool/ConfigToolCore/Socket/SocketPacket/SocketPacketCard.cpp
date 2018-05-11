#include "SocketPacketCard.h"

#ifdef _USE_SOCKET_UDP

#include "txUtility.h"
#include "CommandHeader.h"
#include "SpeedDataManager.h"
#include "CardData.h"

void SocketPacketCard::execute()
{
	if ((mData[0] != (unsigned char)0xFE || mData[20] != (unsigned char)0xFF))
	{
		return;
	}
	// ���У��λ ǰ18���ַ���1�ĸ���֮�͵��ڵ�19λ
	int sum = 0;
	for (int i = 1; i < 19; ++i)
	{
		sum += txUtility::crc_check(mData[i]);
	}
	// У�鲻��ȷ
	if (sum != mData[19])
	{
		return;
	}
	// 0 : FE
	// 1 : ������
	// 2-5 : ����
	// 6-17 : ����
	// 18 : ������
	// 19 : У����
	// 20 : FF
	long cardID = mData[2] << 24 | mData[3] << 16 | mData[4] << 8 | mData[5] << 0;
	wchar_t nameBuffer[13];
	memset(nameBuffer, 0, sizeof(wchar_t)* 13);
	memcpy(nameBuffer, mData + 6, 12);
	std::wstring unicodeName(nameBuffer);
	int machineIndex = (int)mData[18];

	CardData* cardData = TRACE_NEW(CardData, cardData);
	memcpy(cardData->mData, mData, cardData->mDataSize);
	cardData->mName = unicodeName;
	cardData->mCardID = cardID;
	cardData->mMachineIndex = machineIndex;
	mSpeedDataManager->setCardData(cardData);
}

#endif