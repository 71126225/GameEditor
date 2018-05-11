#ifndef _CONFIG_TOOL_CORE_H_
#define _CONFIG_TOOL_CORE_H_

#include <vector>
#include <windows.h>

#include "txSingleton.h"

class SocketNetManager;
class CTEventSystem;
class txCommandSystem;
class SpeedDataManager;
class ConfigToolCore : public txSingleton<ConfigToolCore>
{
public:
	ConfigToolCore();
	virtual ~ConfigToolCore(){ destroy(); }
	virtual void init();
	virtual void update(float elapsedTime);
	virtual void destroy();
	void notifyInitDone() { mInitFlag = true; }
	void checkErrorInfoBuffer();
	void logError(const std::string& info);
	void logInfo(const std::string& info);
	// ˢ�´����豸�б�
	void checkDevice();
	// ��������������㲥��Ϣ
	void setFriction(const int& friction);
	void setColor(const int& r, const int& g, const int& b);
	bool setCardParameter(const std::string& name);
	bool setDeviceParameter(const std::string& ip, const int& machineIndex);
	void setCurDeviceName(const std::string& deviceName) { mCurDeviceName = deviceName; }
	std::vector<std::pair<std::string, std::string> >& getDeviceList() { return mDeviceList; }
	SocketNetManager* getSocketNetManager() { return mSocketNetManager; }
	CTEventSystem* getEventSystem() { return mEventSystem; }
	txCommandSystem* getCommandSystem() { return mCommandSystem; }
	SpeedDataManager* getSpeedDataManager() { return mSpeedDataManager; }
protected:
	static DWORD WINAPI readCOMResult(LPVOID lpParameter);
	// ����д���豸����������
	void generateDeviceData(std::string& str, const std::string& ip, const int& machineIndex);
	// ����д�뿨����������
	void generateCardData(std::wstring& str, const std::string& cardName);
	// ��ָ������д������
	bool writeCOMData(const std::string& comName, const char* data, const int& dataSize);
protected:
	SocketNetManager* mSocketNetManager;
	CTEventSystem* mEventSystem;
	txCommandSystem* mCommandSystem;
	SpeedDataManager* mSpeedDataManager;
	std::vector<std::string> mErrorInfoBuffer;	// ������Ϣ����,���ڱ����¼�ϵͳ����֮ǰ�����Ĵ�����Ϣ
	bool mInitFlag;								// ��ʼ�����,Ϊfalse��ʾ��δ��ʼ�����,���ⲿ���б��
	std::vector<std::pair<std::string, std::string> > mDeviceList;
	std::string mCurDeviceName;
	HANDLE mReadRetThread;
	HANDLE mDeviceNameMutex;
	volatile HANDLE mHcom;	// ��ǰ�����Ĵ��ھ��
	volatile char* mReceiveData;
	volatile int mReceiveDataCount;
};

#endif