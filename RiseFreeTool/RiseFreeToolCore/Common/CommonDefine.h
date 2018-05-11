#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_

#include "EngineDefine.h"
#include "txSTLBase.h"
#include "txVector.h"
#include "txMap.h"
#include "txSet.h"
#include "txMemeryTrace.h"
#include "txLog.h"
#include "Utility.h"

const float TIME_OUT = 0.5f;
const int CMD_FIT = 1;
const int CMD_FRICTION = 2;
const int KEY_MACHINE = 1;
const int KEY_FRICTION = 1;

enum SOCKET_PACKET
{
	SP_SPEED,
	SP_FRICTION,
	SP_FRICTION_RET,
	SP_MAX,
};

enum CORE_EVENT_TYPE
{
	CET_INFO_LOG,				// ��ʾ��Ϣ,����1����Ϣ����
	CET_ERROR_LOG,				// ������Ϣ,����1����Ϣ����
	CET_DEVICE_LIST_CHANGED,	// ��⵽���豸����,�����豸�ѶϿ�����,����1Ϊ�Ƿ��⵽�豸
	CET_CUR_DEVICE_CHANGED,		// ѡ���������豸
	CET_DEVICE_OPENED,			// ���ڱ���
	CET_DEVICE_CLOSED,			// ���ڱ��ر�
	CET_MAX,
};

// ���ݽ������
enum PARSE_RESULT
{
	PR_SUCCESS,		// �����ɹ�
	PR_ERROR,		// ���ݴ���
	PR_NOT_ENOUGH,	// ���ݲ���
};

class Command;
class CommandReceiver;
struct DelayCommand
{
	DelayCommand(float delayTime, Command* cmd, CommandReceiver* receiver)
	:
	mDelayTime(delayTime),
	mCommand(cmd),
	mReceiver(receiver)
	{}
	float mDelayTime;
	Command* mCommand;
	CommandReceiver* mReceiver;
};

typedef void(*CommandCallback) (void* user_data, Command* cmd);

#endif