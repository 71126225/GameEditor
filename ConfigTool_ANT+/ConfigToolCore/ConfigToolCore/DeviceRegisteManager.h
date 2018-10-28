#ifndef _DEVICE_REGISTE_MANAGER_H_
#define _DEVICE_REGISTE_MANAGER_H_

#include "txCommandReceiver.h"
#include "CommonDefine.h"
#include "ToolCoreBase.h"
#include "ThreadLock.h"

class DeviceInfo
{
public:
	int mID;
	int mMachineIndex;
	char mIDBytes[4];
	char mDeviceType;
	std::string mDeviceTypeName;
};

class DeviceRegisteManager : public txCommandReceiver, public ToolCoreBase
{
public:
	DeviceRegisteManager()
	:
	txCommandReceiver(TOSTRING(DeviceRegisteManager))
	{}
	virtual ~DeviceRegisteManager(){ destroy(); }
	virtual void init();
	virtual void update(float elapsedTime);
	void destroy();
	// 0��ʾ�ɹ�,1��ʾ�������Ѿ����˸��豸,2��ʾ�������Ѿ����������豸,3��ʾ��������Ч
	int registeDevice(char* deviceID, int machineIndex, char deviceType);
	bool unregisteDevice(char* deviceID);
	bool unregisteDeviceFromMachineIndex(int machineIndex);
	bool isDeviceRegisted(char* deviceID, int& machineIndex);
	// checkExist��ʾ��Ŀ¼��û�б��ʱ�Ƿ�Ҫ��ʾ��ʾ��
	void readFromDataBase(bool checkExist);
	void saveToDataBase();
	void uploadRegisteDeviceInfoList();
	int getRegistedCount() { return mMachineIndexDeviceList.size(); }
	txMap<int, txMap<char, DeviceInfo*>>& getRegisteDeviceList() { return mMachineIndexDeviceList; }
protected:
	void removeFromList(DeviceInfo* info);
	void addToList(DeviceInfo* info);
	void unregisteAllDevice();
protected:
	txMap<int, txMap<char, DeviceInfo*>> mMachineIndexDeviceList;	// һ�������ſ��԰󶨶���豸,���Ǳ����ǲ�ͬ���͵��豸,second��first���豸������
	txMap<int, DeviceInfo*> mDeviceMachineList;
};

#endif