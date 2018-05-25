#ifndef _EC_ROOT_H_
#define _EC_ROOT_H_

#include "txSingleton.h"
#include "txEngineBase.h"

#include "CommonDefine.h"
#include "EditorCoreBase.h"

class txEngineRoot;
class CoreEventSystem;
class ECEditorManager;
class ECSceneManager;
class ECUndoManager;
class SceneEditorCore;
class ComponentEditorCore;
class EditorCoreRoot : public txSingleton<EditorCoreRoot>, public EditorCoreBase, public txEngineBase
{
public:
	EditorCoreRoot();
	virtual ~EditorCoreRoot(){ destroy(); }
	void init(const HWND& hwnd, const int& colourBits, const int& width, const int& height);
	void preUpdate(float elapsedTime);
	void update(float elapsedTime);
	void lateUpdate(float elapsedTime);
	void render();
	void destroy();
	void notifyInitDone(){ mInitFlag = true; }
	void setModalDialog(ModalDialog dialog, int style[MDS_MAX], int result[MDR_MAX]);
	void logError(const std::string& info);
	// ����ʹ��EditorCoreRoot��sendEvent�����EventSystem��pushEvent
	void sendEvent(const CORE_EVENT& type, const std::string& param, const bool& sendImmediately = true);
	void sendEvent(const CORE_EVENT& type, const txVector<std::string>& params = txVector<std::string>(), bool sendImmediately = true);
	void sendDelayEvent(const CORE_EVENT& type, const std::string& param){ sendEvent(type, param, false); }
	void sendDelayEvent(const CORE_EVENT& type, const txVector<std::string>& params = txVector<std::string>()){ sendEvent(type, params, false); }
	MODAL_DIALOG_RESULT showModalYesNoCancel(const std::string& info);
	MODAL_DIALOG_RESULT showModalYesNo(const std::string& info);

	CoreEventSystem* getCoreEventSystem() { return mCoreEventSystem; }
	txEngineRoot* getEngineRoot() { return mEngineRoot; }
	ECEditorManager* getEditorManager() { return mEditorManager; }
	ECSceneManager* getEditorSceneManager() { return mEditorSceneManager; }
	ECUndoManager* getUndoManager() { return mUndoManager; }
	SceneEditorCore* getSceneEditorCore() { return mSceneEditorCore; }
	ComponentEditorCore* getComponentEditorCore() { return mComponentEditorCore; }
protected:
	void checkErrorInfoBuffer();
	MODAL_DIALOG_RESULT showModal(const std::string& info, const int& flag);
protected:
	txEngineRoot* mEngineRoot;							// ����
	CoreEventSystem* mCoreEventSystem;					// �¼�ϵͳ
	ECEditorManager* mEditorManager;					// �༭�������
	ECSceneManager* mEditorSceneManager;				// ����������
	ECUndoManager* mUndoManager;						// ����������
	SceneEditorCore* mSceneEditorCore;
	ComponentEditorCore* mComponentEditorCore;
	txVector<std::string> mErrorInfoBuffer;				// ������Ϣ������,�����¼�ϵͳ��ʼ�����֮ǰʹ��,���ڱ����¼�ϵͳ��ʼ�����֮ǰ�������Ĵ�����Ϣ
	bool mInitFlag;										// ��ʼ�����,Ϊfalse��ʾ��δ��ʼ�����
	ModalDialog mModalDialog;							// ����ѯ�ʵ�ģʽ�Ի���
	int mDialogStyle[MDS_MAX];							// �Ի�����
	int mDialogResult[MDR_MAX];							// �Ի��򷵻ؽ��
};

#endif