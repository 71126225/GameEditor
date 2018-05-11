#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_

#include "EditorCoreUtility.h"
#include "EngineDefine.h"
#include "txSTLBase.h"
#include "txVector.h"
#include "txMap.h"
#include "txSet.h"
#include "txMemeryTrace.h"
#include "txLog.h"

const float TIME_OUT = 0.5f;

enum SOCKET_PACKET
{
	SP_MAX,
};

enum LIB_CURL_PACKET
{
	LCP_MAX,
};

enum HTTP_METHOD
{
	HM_GET,
	HM_POST,
};

enum CORE_EVENT_TYPE
{
	CET_ERROR_LOG,
	CET_INFO_LOG,
	CET_START_DOWNLOAD_VERSION,		// �������ذ汾���ļ�
	CET_START_DOWNLOAD_LIST_FILE,	// ���������б��ļ�
	CET_NEW_VERSION,		// ����1Ϊ�Ƿ����°汾���Ը���
	CET_START_DOWNLOAD,		// ����1Ϊ�ļ���
	CET_FINISH_DOWNLOAD,	// ����1Ϊ�ļ���
	CET_START_GENERATE_LOCAL_FILE,	// ����1�ļ�������
	CET_GENERATING_LOCAL_FILE,		// ����1�ļ�������, ����2�Ѿ����ҵ��ļ�����
	CET_FINISH_GENERATE_LOCAL_FILE,
	CET_UPDATING_FILE,		// ���ڸ��������ļ�
	CET_UPDATING_PROGRESS,	// �����ļ��Ľ���
	CET_UPDATE_DONE,
	CET_CANCEL_UPDATE,
	CET_NOTHING_UPDATE,
	CET_START_UPLOAD,		// ����1���ļ���
	CET_UPLOADING_FILE,		// ����1���ļ���
	CET_FINISH_UPLOAD,		// ����1���ļ���,����2���Ƿ�ɹ�
	CET_ALL_UPLOADED,
	CET_STOP_PROGRAM,		// �˳�����
	CET_MAX,
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

const int BIG_FILE_SIZE = 1024 * 1024 * 16;		// ���ļ���С,�ļ���С���ڵ��ڸô�Сʱ��Ϊ�Ǵ��ļ�
const std::string GAME_NAME = "dndl";			// �̶�����Ϸ����,��ʹ�汾����Ҳ�����޸�
const std::string REMOTE_URL = "http://app1.taxingtianji.com/gameResource/dndl/";
const std::string UPLOAD_URL = "http://app1.taxingtianji.com/wechat/gamelogtxt/php_info.php";	// Ҫ���������ϴ�������־�ĵ�ַ
//const std::string UPLOAD_URL = "http://localhost/php_info.php";								// ���ز��Եĵ�ַ

#endif