#ifndef _CONFIG_TOOL_CORE_H_
#define _CONFIG_TOOL_CORE_H_

#include "txSingleton.h"
#include "CommonDefine.h"

class SocketWireless;
class CTEventSystem;
class txCommandSystem;
class WirelessDataManager;
class DataBase;
class DeviceRegisteManager;
class SpeedDataManager;
class SocketNetManager;
class LibcurlNetManager;
class LogSystem;
class GameConfig;
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
	// ����ʹ��ECRoot��sendEvent�����EventSystem��pushEvent
	void sendEvent(const CORE_EVENT& type, const std::string& param, const bool& sendImmediately = true);
	void sendEvent(const CORE_EVENT& type, txVector<std::string>& params = txVector<std::string>(), bool sendImmediately = true);
	void sendDelayEvent(const CORE_EVENT& type, const std::string& param);
	void sendDelayEvent(const CORE_EVENT& type, txVector<std::string>& params = txVector<std::string>());
	void logError(const std::string& info);
	void logInfo(const std::string& info);
	SocketWireless* getSocketWireless() { return mSocketWireless; }
	CTEventSystem* getEventSystem() { return mEventSystem; }
	txCommandSystem* getCommandSystem() { return mCommandSystem; }
	WirelessDataManager* getWirelessDataManager() { return mWirelessDataManager; }
	DataBase* getDataBase() { return mDataBase; }
	DeviceRegisteManager* getDeviceRegisteManager() { return mDeviceRegisteManager; }
	SpeedDataManager* getSpeedDataManager() { return mSpeedDataManager; }
	SocketNetManager* getSocketNetManager() { return mSocketNetManager; }
	LibcurlNetManager* getLibcurlNetManager() { return mLibcurlNetManager; }
	LogSystem* getLogSystem() { return mLogSystem; }
	GameConfig* getGameConfig() { return mGameConfig; }
	void setDataPath(const std::string& path);
protected:
	SocketWireless* mSocketWireless;
	CTEventSystem* mEventSystem;
	txCommandSystem* mCommandSystem;
	WirelessDataManager* mWirelessDataManager;
	DataBase* mDataBase;
	DeviceRegisteManager* mDeviceRegisteManager;
	SpeedDataManager* mSpeedDataManager;
	SocketNetManager* mSocketNetManager;
	LibcurlNetManager* mLibcurlNetManager;
	LogSystem* mLogSystem;
	GameConfig* mGameConfig;
	txVector<std::string> mErrorInfoBuffer;	// ������Ϣ����,���ڱ����¼�ϵͳ����֮ǰ�����Ĵ�����Ϣ
	bool mInitFlag;								// ��ʼ�����,Ϊfalse��ʾ��δ��ʼ�����,���ⲿ���б��
};

#endif