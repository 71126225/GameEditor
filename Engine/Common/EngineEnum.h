#ifndef _ENGINE_ENUM_H_
#define _ENGINE_ENUM_H_

// �������Ͷ���
enum TERRAIN_TYPE
{
	TT_GROUND,		// ����
	TT_WATER,		// ˮ��
};

// ��ײ�����Ͷ���
enum GEOM_TYPE
{
	GT_NONE,
	GT_BOX,
	GT_PLANE,
	GT_SPHERE,
	GT_CAPSULE,
	GT_CYLINDER,
};

enum ENGINE_EVENT
{
	EE_LOAD_SCENE_OBJECT,	// ���س����ļ��е�����,����Ϊ������
	EE_START_LOAD_SCENE,	// ��ʼ���س����ļ�,����Ϊ������
	EE_END_LOAD_SCENE,	// ���س����ļ�����,�޲���
	EE_SCENE_LOAD_PROGRESS,	// �����ļ����ؽ���,����Ϊ0~1�ĸ��������ַ���
	EE_CHARACTER_LOADED,	// ���ؽ�ɫ���,�޲���
	EE_CALCULATE_HEIGHT_MAP_DONE, // ���㳡���߶�ͼ���,����Ϊ�������ͼ�����ʹ�õ�ʱ��,��λΪ��
	EE_START_LOAD_DYNAMIC_ENTITY,	// ��ʼ���ض�̬ʵ��,����Ϊʵ����
	EE_DYNAMIC_ENTITY_MODEL_LOADED,	// ��̬ʵ���ģ���ļ��������,����1��ʵ����,����2��ģ���ļ���
	EE_DYNAMIC_ENTITY_SKELETON_LOADED,// ��̬ʵ��Ĺ����ļ��������,����1��ʵ����,����2�ǹ����ļ���
	EE_START_LOAD_ENTITY_ANIMATION,		// ��ʼ���ض���,����1��ʵ����,����2�Ƕ���������
	EE_ONE_ENTITY_ANIMATION_LOADED,	// һ�������������,����1��ʵ����,����2�Ƕ���������,����3�����������ļ���
	EE_ENTITY_ALL_ANIMATION_LOADED,	// ��̬ʵ��Ķ���ȫ���������,����1��ʵ����,����2�Ƕ�������
	EE_DYNAMIC_ENTITY_LOAD_PROGRESS_CHANGED, // ʵ��ļ��ؽ��ȸı�,����1��ʵ����,����2��ʵ��ģ���ļ���,����3�Ǽ��ؽ���
	EE_END_DYNAMIC_ENTITY_LOAD,	// ��̬ʵ��������,����Ϊʵ����
	EE_ENGINE_ERROR_INFO,	// ���������Ϣ,����1Ϊ������Ϣ����
	EE_WINDOW_SELECTED,			// ��ĳ�����ڱ�ѡ��,����1Ϊ������
	EE_WINDOW_UNSELECTED,		// ��ĳ������ȡ��ѡ��,����1Ϊ������
	EE_MOUSE_ENTER_WINDOW,		// ��������ĳ������,����1Ϊ������
	EE_MOUSE_LEAVE_WINDOW,		// ������뿪ĳ������,����1Ϊ������
	EE_WINDOW_PREFAB_CHANGED,	// ���ڸı��˹�����Ԥ��,����1Ϊ������,����2Ϊ�ı���Ԥ����
	EE_PREFAB_RESOURCE_CHANGED,	// Ԥ����Դ�иı�,�޲���
	EE_TEMPLATE_RESOURCE_CHANGED,// ģ����Դ�иı�,�޲���
	EE_ERROR_LOG,				// ��༭�㷢�ʹ�����Ϣ
	EE_MAX,
};

enum ENTITY_COPY_TYPE
{
	ECT_NONE,	// �޿���
	ECT_DEEP,	// ��ȿ���
	ECT_SIMPLE,	// ǳ����
	ECT_ANIM_SIMPLE,	// ��̬ģ�͵�ǳ����,ǳ����������Ϣ�Ͷ���,�������
};

enum PLAY_STATE
{
	PS_PLAY,
	PS_PAUSE,
	PS_STOP,
};

// ��Χ���������ཻ������
enum BOX_INTERSECT_FACE
{
	BIF_FRONT,	// ֻ�������
	BIF_BACK,	// ֻ��ⱳ��
	BIF_BOTH,	// �����涼���
};

enum BLEND_MODE
{
	BM_ADD,			// ֱ�ӽ�����ɫ���,����255��ǿ��Ϊ255
	BM_MULTI,		// ��ɫ���������
	BM_REPLACE,		// ��ɫ�滻
	BM_MAX,
};

// ������Ⱦ�ĶԳƷ�ʽ
enum SYMMETRY_MODE
{
	SM_NONE,					// �����Գƴ���
	SM_LEFT_COPY_TO_RIGHT,		// ����ߵ�ֱ�Ӹ��Ƶ��ұ�
	SM_LEFT_MIRROR_TO_RIGHT,	// ����ߵľ����Ƶ��ұ�
	SM_HORI_INVERSE,			// ˮƽ�����Ϸ�תͼƬ
	SM_VERT_INVERSE,			// ��ֱ�����Ϸ�תͼƬ
	SM_HORI_VERT_MIRROR,		// ˮƽ����ֱ������Գ�
	SM_BOTTOM_COPY_TO_TOP,		// �±ߵ�ֱ�Ӹ��Ƶ��ϱ�
	SM_BOTTOM_MIRROR_TO_TOP,	// �±ߵľ����Ƶ��ϱ�
};

enum OBJECT_TYPE
{
	OT_ENTITY,
	OT_DIRECTIONAL_LIGHT,
	OT_BILLBOARD,
	OT_EMITTER,
	OT_MAX,
};

// ������Ե�����
enum PROPERTY_TYPE
{
	PT_BOOL,
	PT_INT,
	PT_STRING,
	PT_VECTOR2,
	PT_VECTOR3,
	PT_VECTOR4,
	PT_FLOAT,
	PT_ENUM,
	PT_TEXTURE,
	PT_DIM,
	PT_POINT,
};

// ��Ϊִ�е�״̬
enum ACTION_EXECUTE_STATE
{
	AES_NOT_EXECUTE,		// ��δִ��
	AES_EXECUTING,			// ����ִ��
	AES_PAUSE,				// ��ִͣ��
	AES_EXECUTE_DONE,		// ִ�����
};

// windowsϵͳ
enum WINDOWS_SYSTEM
{
	WS_NONE,
	WS_WIN_XP,
	WS_WIN_VISTA,
	WS_WIN_7,
	WS_WIN_8,
	WS_WIN_10,
};

enum POLYGON_MODE
{
	PM_LINES = GL_LINES,
	PM_TRIANGLES = GL_TRIANGLES,
};

// �����������ת����
enum CAMERA_LINKER_SWITCH
{
	CLS_NONE,
	CLS_LINEAR,
	CLS_CIRCLE,
	CLS_AROUND_TARGET,
};

// ��갴ť
enum MOUSE_BUTTON_DEFINE
{
	MBD_NONE,
	MBD_LEFT,
	MBD_RIGHT,
	MBD_MIDDLE,
};

enum TRANS_SPACE
{
	TS_LOCAL,
	TS_WORLD,
};

// ����״̬
enum KEY_PUSH_STATE
{
	KPS_KEEP_UP, // �����ſ�
	KPS_KEEP_DOWN, // ��������
	KPS_CURRENT_UP, // ��ǰ�ſ�
	KPS_CURRENT_DOWN, // ��ǰ����
};

// �������Ͷ���
enum WINDOW_TYPE
{
	WT_PREFAB,
	WT_WINDOW,
	WT_BUTTON,
	WT_EDITBOX,
	WT_CHECKBOX,
	WT_NUMBER_WINDOW,
	WT_PROGRESS_WINDOW,
	WT_TEXTURE_WINDOW,
	WT_TEXTURE_ANIM_WINDOW,
	WT_TEXT_WINDOW,
	WT_VIDEO_WINDOW,
	WT_MAX,
};

// �����¼�,��Ҫ�����ڽű���
enum WINDOW_EVENT
{
	WE_CLICKED,	// ���ڱ����
	WE_RETURN,	// �ڴ��ڻ�ý���ʱ�����˻س���
	WE_TABLE,	// �ڴ��ڻ�ý���ʱ������table��
	WE_SELECT,	// ����Ϊѡ��״̬
	WE_UNSELECT,// ����Ϊ��ѡ��״̬
	WE_MOUSE_ENTER,	// �����봰��
	WE_MOUSE_LEAVE,	// ����뿪����
};

// ��ť״̬
enum BUTTON_STATE
{
	BS_NORMAL,
	BS_HOVER,
	BS_PUSHED,
	BS_DISABLED,
	BS_MAX_STATE,
};

// UIͣ������
enum WINDOW_DOCKING_POSITION
{
	WDP_MIN = -1,
	WDP_LEFT,
	WDP_RIGHT,
	WDP_CENTER,
	WDP_MAX,
};

// ���ڴ�С���Զ����㷽ʽ
enum AUTO_SIZE_TYPE
{
	AST_NONE,			// ���Զ������С
	AST_AUTO_HEIGHT,	// �Զ�����߶�
	AST_AUTO_WIDTH,		// �Զ�������
};

// ����֡����ѭ����ʽ
enum LOOP_MODE
{
	LM_NONE,			// ��ѭ��
	LM_NORMAL,			// ͷ-β,ͷ-βѭ��
	LM_PINGPONG,		// ͷ-β,β-ͷѭ��
};

#endif