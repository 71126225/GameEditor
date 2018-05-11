#include "SocketPacketSpeed.h"

#ifdef _USE_SOCKET_UDP

#include "EditorCoreUtility.h"
#include "CommandHeader.h"
#include "SpeedDataManager.h"

void SocketPacketSpeed::execute()
{
	// [10 --- 16)
	if ((mData[0] == (unsigned char)0xFE && mData[11] == (unsigned char)0xFF))
	{
		// ���У��λ ��1 - 7����1�ĸ���֮�͵��ڵ�10λ
		int sum = 0;
		for (int i = 1; i < 8; ++i)
		{
			sum += BinaryUtility::crc_check(mData[i]);
		}
		// У����ȷ
		if (sum == mData[10])
		{
			// [0]	  [1]		  [2] [3][4]		 [5][6]	  [7]   [8]		    [9]			[10]		[11]
			// FE	  00(������)  00   ����			  �ٶ�	   00   FF      ������[0-19]	crc		     FF
			// 3-4ʹ�ù�������Ϊ�ٶ�(��ʹ��5-6���ٶ�ֵ)
			int power = mData[4] * 256 + mData[3];
			int rotateSpeed = mData[6] * 256 + mData[5];
		}
	}
}

#endif