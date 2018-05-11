#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_

#include <windows.h>
#include <string>

const float SPEED_DATA_TIME_OUT = 0.5f;

enum SOCKET_PACKET
{
	SP_SPEED,
	SP_CARD,
	SP_FRICTION,
	SP_FRICTION_RET,
	SP_HEART_RATE,
	SP_COLOUR,
	SP_MAX,
};

enum CORE_EVENT_TYPE
{
	CET_INFO_LOG,				// ��ʾ��Ϣ,����1����Ϣ����
	CET_ERROR_LOG,				// ������Ϣ,����1����Ϣ����
	CET_DEVICE_CHANGED,			// ��⵽���豸����,�����豸�ѶϿ�����,����1Ϊ�Ƿ��⵽�豸
	CET_RECEIVE_DATA,			// ���յ�����,����1�ǽ��յ�����,����2�ǽ������ݵĳ���
	CET_SEND_DATA,				// ����������,����1�Ƿ��͵�����,����2�Ƿ������ݵĳ���
	CET_HEART_RATE_TIME,		// ������ʱʱ��ı�
	CET_MAX,
};

class txCommand;
class txCommandReceiver;
struct DelayCommand
{
	DelayCommand(float delayTime, txCommand* cmd, txCommandReceiver* receiver)
	:
	mDelayTime(delayTime),
	mCommand(cmd),
	mReceiver(receiver)
	{}
	float mDelayTime;
	txCommand* mCommand;
	txCommandReceiver* mReceiver;
};

typedef void(*CommandCallback) (void* user_data, txCommand* cmd);

#define DEBUG_EMPTY "%s", ""

#define COMMAND_PARAM __FILE__, __LINE__

#define COMMAND_CONSTRUCT(className, baseCommand)					\
	className(const char* file, int line, bool showInfo = true)		\
	:																\
	baseCommand(file, line, showInfo)

#define COMMAND_TOOL_CORE_CONSTRUCT(className) COMMAND_CONSTRUCT(className, ToolCoreCommand)

#define COMMAND_DECLARE_FUNCTION					\
	virtual void execute();							\
	virtual std::string showDebugInfo();

#define COMMAND_DEBUG(...)																\
{																						\
	static char strBuf[256];															\
	sprintf_s(strBuf, 256, __VA_ARGS__);												\
	std::string cmdName = typeid(*this).name();											\
	cmdName = cmdName.substr(strlen("class "), cmdName.length() - strlen("class "));	\
	std::string str = cmdName;															\
	if (strlen(strBuf) > 0)																\
	{																					\
		str += std::string(" : ") + strBuf;												\
	}																					\
	return str;																			\
}


#endif