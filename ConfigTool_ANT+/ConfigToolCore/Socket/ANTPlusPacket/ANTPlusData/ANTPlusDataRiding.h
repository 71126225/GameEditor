#ifndef _ANT_PLUS_DATA_RIDING_H_
#define _ANT_PLUS_DATA_RIDING_H_

#include "ANTPlusData.h"

class ANTPlusPacket;
class ANTPlusDataRiding : public ANTPlusData
{
public:
	static const int DEVICE_ID_LENGTH = 4;
	static const int PAGE_DATA_LENGTH = 8;
	RIDING_DEVICE mRidingDevice;
public:
	unsigned char mDeviceType;	// ȡֵ��ΧΪRIDING_DEVICEö��
	unsigned char mDeviceID[DEVICE_ID_LENGTH];
	unsigned char mPageData[PAGE_DATA_LENGTH];
	unsigned char mRSSIValue;
	short mReserve;
	int mMachineIndex;
public:
	ANTPlusDataRiding(ANTPlusPacket* packet);
	virtual void copyData(ANTPlusData* data);
	virtual int getHeartRate(){ return 0; }
	virtual int getCadence() { return 0; }
	virtual float getSpeed(float perimeter){ return 0.0f; }
	virtual bool isSameDataValue(ANTPlusDataRiding* newData){ return false; }
protected:
	virtual void parseData();
};

#endif