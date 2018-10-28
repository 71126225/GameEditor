#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_

#include "EngineDefine.h"
#include "txVector.h"
#include "txMap.h"
#include "txSet.h"

const float SPEED_DATA_TIME_OUT = 0.5f;

enum SOCKET_PACKET
{
	SP_SPEED,
	SP_FRICTION,
	SP_FRICTION_RET,
	SP_HEART_RATE,
	SP_COLOUR,
	SP_MAX,
};

enum CORE_EVENT
{
	CE_INFO_LOG,			// ��ʾ��Ϣ,����1����Ϣ����
	CE_ERROR_LOG,			// ������Ϣ,����1����Ϣ����
	CE_HEART_RATE_TIME,		// ������ʱʱ��ı�
	CE_REGISTE_DEVICE_LIST_MODIFIED,	// �豸����Ϣ�б��޸�,����1Ϊ�Ƿ��޸��Ժ�δ����
	CE_UPLOAD_STATE,		// �豸����Ϣ�ϴ�״̬,����1Ϊ״̬
	CE_US_UNUPLOAD,			// �豸����Ϣ�ϴ����,����1Ϊ״̬
	CE_UPLOAD_FAILED,		// �豸����Ϣ�ϴ�ʧ��,����1Ϊ״̬
	CE_MAX,
};

enum LIBCURL_PACKET
{
	LP_SETUP_DEVICE,
	LP_MAX,
};

enum HTTP_METHOD
{
	HM_GET,
	HM_POST,
};

enum UPLOAD_STATE
{
	US_NONE,
	US_UNUPLOAD,
	US_UPLOADING,
	US_UPLOADED,
	US_UPLOAD_FAILED,
};

// ���ݽ������
enum PARSE_RESULT
{
	PR_SUCCESS,		// �����ɹ�
	PR_ERROR,		// ���ݴ���
	PR_NOT_ENOUGH,	// ���ݲ���
};

enum ANT_PLUS_PACKET_TYPE
{
	APPT_NONE,
	APPT_HEART_BEAT,	// ����������
	APPT_HEART_RATE,	// ����
	APPT_CADENCE,		// ��̤Ƶ
	APPT_SPEED,			// ���ٶ�
	APPT_CADENCE_SPEED,	// �ٶ�̤Ƶ
};

enum PACKET_CMD
{
	PC_RIDING = 0x01,
	PC_HEART_BEAT = 0x02,
};

enum RIDING_DEVICE
{
	RD_NONE,
	RD_HEART_RATE = 0x78,
	RD_CADENCE_SPEED = 0x79,
	RD_CADENCE = 0x7A,
	RD_SPEED = 0x7B,
};

const float PACKET_TIME_OUT = 3.0f;

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

const std::string EMPTY_STRING = "";

#endif