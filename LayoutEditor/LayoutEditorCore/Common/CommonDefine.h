#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_

#include "txMathUtility.h"

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

enum CORE_EVENT
{
	CE_LAYOUT_DESTROIED,					// ���ֱ�����,����1Ϊ������
	CE_LAYOUT_CLOSED,						// �رղ���,����1Ϊ��������
	CE_SELECTION_CHANGED,					// �л���ѡ����
	CE_WINDOW_RENAMED,						// ���� 1ԭ�������� 2������
	CE_LAYOUT_RENAMED,						// ����������,����1�ǲ���ԭ��������,����2ʱ�����µ�����
	CE_WINDOW_DELETED,						// ���� 1 ����������,2��������
	CE_WINDOW_ADDED,						// ���� 1 ����������,2��������,����3����ӵĴ����ڸ������е�˳��λ��
	CE_LAYOUT_ADDED,						// ���� 1 Layout����
	CE_WINDOW_PROPERTY_CHANGED,				// �������Ըı�,����1��������,����2���޸�ǰ������ֵ,����3���޸ĺ������ֵ,����4�Ǵ�����,����5��˭ȥ�޸ĵĴ��ڵ�����(��ַ)
	CE_LAYOUT_MODIFIED,						// �����޸�״̬�ı�,����1�ǲ�����,����2Ϊ�Ƿ��޸�
	CE_LAYOUT_VISIBLE_CHANGED,				// ���ֵ���ʾ״̬�ı�,����1Ϊ��������,����2Ϊ�޸��Ժ����ʾ״̬
	CE_WINDOW_POS_CHANGED,					// �����ڲ����е�λ�øı�,����1�Ǵ�����,����2�Ǹ�������,����3�Ǹı�֮ǰ�Ĵ���λ���±�,����4�Ǹı�֮��Ĵ���λ���±�
	CE_WINDOW_PREFAB_CHANGED,				// ���ڸı��˹�����Ԥ��,����1Ϊ������,����2Ϊ�ı���Ԥ����
	CE_UNDO_ENABLE_CHANGE,					// �Ƿ�ɳ���״̬�ı�,����1Ϊ�Ƿ�ɳ���
	CE_REDO_ENABLE_CHANGE,					// �Ƿ������״̬�ı�,����1Ϊ�Ƿ������
	CE_PREFAB_RESOURCE_CHANGED,				// ����Ԥ����Դ�иı�,�޲���
	CE_TEMPLATE_RESOURCE_CHANGED,			// ����ģ����Դ�иı�,�޲���
	CE_WINDOW_CUTED,						// ���ڼ��в���ճ��,����1�Ǽ��еĴ�����
	CE_TREMBLE_MODIFIED,					// �𶯹ؼ�֡�޸�״̬�ı�,����1������,����2���Ƿ��޸�
	CE_LOAD_TREMBLING,						// ���عؼ�֡��,����1������
	CE_NEW_TREMBLING,						// �½��ؼ�֡��,����1������
	CE_ADD_NODE,							// ���һ���ؼ�֡�ڵ�,����1�ǽڵ���
	CE_DELETE_NODE,							// ɾ��һ���ؼ�֡�ڵ�,����1�ǽڵ���
	CE_SELECT_TREMBLING_NODE,				// ѡ��һ���ؼ�֡�ڵ�,����1�ǽڵ���
	CE_SELECT_KEY_FRAME,					// ѡ��һ���ؼ�֡,����1�ǹؼ�֡�����ַ
	CE_ADD_KEY_FRAME,						// ���һ���ؼ�֡,����1�ǹؼ�֡���±�
	CE_DELETE_KEY_FRAME,					// ɾ��һ���ؼ�֡,����1�ǹؼ�֡���±�
	CE_WINDOW_COMPONENT_PROPERTY_CHANGED,	// ������������Ըı�,����1�������,����2��������,����3���޸�ǰ������ֵ,����4���޸ĺ������ֵ,����5�Ǵ�����,����6��˭ȥ�޸ĵĴ��ڵ�����(��ַ)
	CE_COMPONENT_SELECTION_CHANGED,			// ���ѡ����ı�
	CE_LOAD_ACTION_TREE,					// ����һ����Ϊ��,����1����Ϊ��������
	CE_NEW_ACTION_TREE,						// �½�һ����Ϊ��,����1����Ϊ��������
	CE_DELETE_ACTION_TREE,					// ɾ��һ����Ϊ��,����1����Ϊ��������
	CE_SELECT_ACTION_TREE,					// ѡ��һ����Ϊ��,����1����Ϊ������
	CE_ACTION_SELECTION_CHANGED,			// ��Ϊѡ����ı�
	CE_ADD_ACTION,							// ���һ����Ϊ,����1����Ϊ������,����2�Ǹ��ڵ������
	CE_DELETE_ACTION,						// ɾ��һ����Ϊ,����1����Ϊ������
	CE_ACTION_TREE_MODIFIED,				// ��Ϊ���Ƿ��޸�,����1����Ϊ������,����2���Ƿ��޸�
	CE_ERROR_LOG,							// �༭���������㷢���Ĵ�����Ϣ,����1�Ǵ�����Ϣ����
	CE_INFO_LOG,							// �༭���������㷢������ʾ��Ϣ,����1����ʾ��Ϣ����
	CE_MAX,
};

typedef int(*ModalDialog)(const std::string& caption, const std::string& info, const int& style);

const int MAX_UNDO_COUNT = 100;

#endif