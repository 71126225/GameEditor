#ifndef _SPEED_DATA_MANAGER_H_
#define _SPEED_DATA_MANAGER_H_

#include "txCommandReceiver.h"
#include "CommonDefine.h"
#include "ToolCoreBase.h"

class SpeedData;
class HeartRateData;
class CardData;
class SpeedDataManager : public txCommandReceiver, public ToolCoreBase
{
public:
	SpeedDataManager();
	virtual ~SpeedDataManager(){ destroy(); }
	virtual void init(){}
	virtual void update(float elapsedTime);
	virtual void destroy(){ clearData(); }
	void setSpeedData(SpeedData* data);
	void setHeartRateData(HeartRateData* data);
	void setFriction(int friction, int machineIndex);
	void setCardData(CardData* cardData);
	void clearData();
	txMap<int, SpeedData*>& getSpeedDataList() { return mSpeedDataList; }
	txMap<std::string, HeartRateData*>& getHeartRateDataList() { return mHeartRateDataList; }
	bool isSpeedDataDirty() { return mSpeedDataDirty; }
	void setSpeedDataDirty(bool dirty) { mSpeedDataDirty = dirty; }
	bool isHeartRateDataDirty() { return mHeartRateDataDirty; }
	void setHeartRateDataDirty(bool dirty) { mHeartRateDataDirty = dirty; }
	void setSpeedRatio(float speedRatio) { mSpeedRatio = speedRatio; }
	float getSpeedRatio() { return mSpeedRatio; }
	float getHeartRateTimeOut() { return mHeartRateTimeOut; }
	void setHeartRateTimeOut(float time);
protected:
	txMap<int, SpeedData*> mSpeedDataList;	// �Ի�����������
	bool mSpeedDataDirty;	// �ٶ������Ƿ��Ѿ��ı����
	txMap<std::string, HeartRateData*> mHeartRateDataList;	// �����ʴ����������
	bool mHeartRateDataDirty;
	float mHeartRateTimeOut;
	float mSpeedRatio;
};

#endif