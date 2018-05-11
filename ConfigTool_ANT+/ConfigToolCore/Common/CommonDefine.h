#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_

#include "EngineDefine.h"
#include "txVector.h"
#include "txMap.h"
#include "txSet.h"

const float SPEED_DATA_TIME_OUT = 0.5f;

enum CORE_EVENT_TYPE
{
	CET_INFO_LOG,				// ��ʾ��Ϣ,����1����Ϣ����
	CET_ERROR_LOG,				// ������Ϣ,����1����Ϣ����
	CET_MAX,
};

// ���ݽ������
enum PARSE_RESULT
{
	PR_SUCCESS,		// �����ɹ�
	PR_ERROR,		// ���ݴ���
	PR_NOT_ENOUGH,	// ���ݲ���
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

#endif