#ifndef _SPEED_DATA_MANAGER_H_
#define _SPEED_DATA_MANAGER_H_

#include "CommandReceiver.h"
#include "CommonDefine.h"
#include "DataPacket.h"
#include "txThreadLock.h"
#include "EditorCoreBase.h"
#include "DeviceData.h"

class SpeedDataManager : public CommandReceiver, public EditorCoreBase
{
public:
	SpeedDataManager();
	virtual ~SpeedDataManager(){ destroy(); }
	virtual void init(){}
	virtual void update(float elapsedTime);
	virtual void destroy(){ clearData(); }
	// dataΪ���ݻ�����,dataCount��ʾ����������Ч���ݵĳ���,parsedCount��ʾ��������ɹ�,���������ݳ���
	PARSE_RESULT setData(const std::string& deviceName, unsigned char* data, const int& dataCount, int& parsedCount);
	void clearData();
	DeviceData* getCurDeviceData();
	txMap<std::string, DeviceData*>& getDataList() { return mSpeedDataList; }
	void generateFrictionData(const int& friction, char*& data, int& dataCount);
protected:
	void syncList();
protected:
	txMap<std::string, DeviceData*> mReceivedPacketList;
	txMap<std::string, DeviceData*> mSpeedDataList;	// �Դ�����Ϊ����
	txThreadLock mReceivedListLock;
	txMap<int, DataPacket*> mEmptyDataList;
};

#endif