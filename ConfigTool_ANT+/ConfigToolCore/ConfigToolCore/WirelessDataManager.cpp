#include "WirelessDataManager.h"
#include "CTEventSystem.h"
#include "ANTPlusPacket.h"
#include "ANTPlusData.h"
#include "Utility.h"
#include "LogSystem.h"
#include "GameConfig.h"
#include "DeviceRegisteManager.h"
#include "ANTPlusDataRiding.h"

WirelessDataManager::WirelessDataManager()
:
txCommandReceiver(TOSTRING(DataManager)),
mSpeedRatio(4.5f),
mHeartBeatPacket(NULL),
mWheelPerimeter(2.0f)
{
	;
}

void WirelessDataManager::init()
{
	;
}

void WirelessDataManager::update(float elapsedTime)
{
	auto iter = mPacketList.begin();
	auto iterEnd = mPacketList.end();
	FOR_STL(mPacketList, ; iter != iterEnd; ++iter)
	{
		iter->second->update(elapsedTime);
	}
	END_FOR_STL(mPacketList);
	if (mHeartBeatPacket != NULL)
	{
		mHeartBeatPacket->update(elapsedTime);
	}
}

void WirelessDataManager::destroy()
{
	clearData(); 
}

void WirelessDataManager::receivePacket(ANTPlusPacket* packet)
{
	if (packet->mType == APPT_HEART_BEAT)
	{
		TRACE_DELETE(mHeartBeatPacket);
		mHeartBeatPacket = packet;
	}
	else
	{
		ANTPlusDataRiding* newDeviceData = static_cast<ANTPlusDataRiding*>(packet->mData);
		// �����豸��Ӧ�ĵ�����,���浽�豸��
		newDeviceData->mMachineIndex = findMachineIndex((char*)newDeviceData->mDeviceID);
		int deviceID = ToolCoreUtility::toDeviceID((char*)newDeviceData->mDeviceID);
		if (mPacketList.find(deviceID) == mPacketList.end())
		{
			mPacketList.insert(deviceID, packet);
		}
		else
		{
			ANTPlusPacket* thisPacket = mPacketList[deviceID];
			thisPacket->notifyReceived();
			// �ж����������Ƿ���ͬ,�����һ��ʱ��������ͬ����,����������
			if (thisPacket->mLastUpdatePacketTime < PACKET_TIME_OUT)
			{
				ANTPlusDataRiding* thisRidingData = static_cast<ANTPlusDataRiding*>(thisPacket->mData);
				if (thisRidingData->isSameDataValue(newDeviceData))
				{
					TRACE_DELETE(packet);
					return;
				}
			}
			// ��������,���߳�ʱ��Ҫ��������
			packet->updateData(thisPacket);
			mPacketList[deviceID] = packet;
		}
	}
}

void WirelessDataManager::clearData()
{
	auto iter = mPacketList.begin();
	auto iterEnd = mPacketList.end();
	FOR_STL(mPacketList, ; iter != iterEnd; ++iter)
	{
		TRACE_DELETE(iter->second);
	}
	END_FOR_STL(mPacketList);
	TRACE_DELETE(mHeartBeatPacket);
}

int WirelessDataManager::findMachineIndex(char* deviceID)
{
	int machineIndex = -2;
	mDeviceRegisteManager->isDeviceRegisted(deviceID, machineIndex);
	return machineIndex;
}