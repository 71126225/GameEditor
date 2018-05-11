#ifndef _CONFIG_TOOL_CORE_H_
#define _CONFIG_TOOL_CORE_H_

#include <vector>
#include <windows.h>

#include "txSingleton.h"

class SocketNetManager;
class CTEventSystem;
class txCommandSystem;
class DataManager;
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
	SocketNetManager* getSocketNetManager() { return mSocketNetManager; }
	CTEventSystem* getEventSystem() { return mEventSystem; }
	txCommandSystem* getCommandSystem() { return mCommandSystem; }
	DataManager* getDataManager() { return mDataManager; }
protected:
protected:
	SocketNetManager* mSocketNetManager;
	CTEventSystem* mEventSystem;
	txCommandSystem* mCommandSystem;
	DataManager* mDataManager;
	std::vector<std::string> mErrorInfoBuffer;	// ������Ϣ����,���ڱ����¼�ϵͳ����֮ǰ�����Ĵ�����Ϣ
	bool mInitFlag;								// ��ʼ�����,Ϊfalse��ʾ��δ��ʼ�����,���ⲿ���б��
};

#endif