#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_

#include "txEngineDefine.h"

const std::string ROLE_PATH = "Role/";
const std::string CAMERA_EFFECT_PATH = "CameraEffect/";

enum CORE_EVENT
{
	CE_EDITOR_ERROR,				// �༭���Ĵ�����Ϣ,����1�Ǵ�����Ϣ
	CE_EDITOR_DELETED,				// ɾ���˱༭��,����1�Ǳ༭������
	CE_HEIGHT_MAP_DONE,				// �����߶�ͼ�Ѿ��������,����1�ǳ�����,����2�Ǽ������õ�����
	CE_EDITOR_ADDED,				// ����˱༭��,����1�Ǳ༭�������
	CE_EDITOR_SELECTION_CHANGED,	// �༭��ѡ�����иı�,�޲���
	CE_EDITOR_COMPONENT_DELETED,	// �༭��ɾ�����,����1�Ǳ༭������,����2�����������
	CE_COMPONENT_PROPERTY_CHANGED,	// ������Ըı�,����1��������,����2�������������������,����3������ԭ����ֵ,����4���µ�����ֵ,����5�Ǳ༭����,����6�Ǹı����Ե�Դ
	CE_COMPONENT_PROPERTY_REFRESH,	// ������Ըı�,ֻ��֪ͨ�����иı�,���ǲ�ָ��������ʲô���Ըı���
	CE_COMPONENT_SELECTION_CHANGED,	// ���ѡ�����иı�,�޲���
	CE_COMPONENT_ADDED,				// �༭�������һ�����,����1�����ӵ���ߵ�����,����3��ӵ������,����4�������ڸ�����е�λ��
	CE_SCENE_OPEN,					// �򿪳���,����1Ϊ������
	CE_SCENE_NEW,					// �½�����,����1Ϊ������
	CE_SCENE_DESTROIED,				// �رճ���,����1Ϊ������
	CE_SCENE_MODIFIED,				// �����Ƿ��޸Ĺ�,����1Ϊ������,����2Ϊ�Ƿ��޸Ĺ�
	CE_SCENE_RENAMED,				// ����������,����1Ϊ�޸�ǰ������,����2Ϊ�޸ĺ������
	CE_UNDO_ENABLE_CHANGE,			// �Ƿ�ɳ���״̬�ı�,����1Ϊ�Ƿ�ɳ���
	CE_REDO_ENABLE_CHANGE,			// �Ƿ������״̬�ı�,����1Ϊ�Ƿ������
	CE_MAX,
};

enum MODAL_DIALOG_STYLE
{
	MDS_OK,
	MDS_YES,
	MDS_NO,
	MDS_CANCEL,
	MDS_MAX,
};

enum MODAL_DIALOG_RESULT
{
	MDR_OK,
	MDR_YES,
	MDR_NO,
	MDR_CANCEL,
	MDR_MAX,
};

struct RayCallBackUserData
{
	void* mRay;
	void* mScene;
};

typedef int(*ModalDialog)(const std::string& caption, const std::string& info, int style);

const int MAX_UNDO_COUNT = 128;

#endif