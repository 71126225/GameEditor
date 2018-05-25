#ifndef _EDITOR_CORE_ROOT_H_
#define _EDITOR_CORE_ROOT_H_

#include "txSingleton.h"
#include "txEngineBase.h"
#include "CommonDefine.h"
#include "EditorCoreBase.h"
#include "txEngineEventHandler.h"

class txEngineRoot;
class txCamera;
class CoreEventSystem;
class ECEditorManager;
class ECUndoManager;
class TremblingEditorCore;
class LayoutEditorCore;
class ActionTreeEditorCore;
class ComponentEditorCore;
class EditorCoreRoot : public txSingleton<EditorCoreRoot>, public EditorCoreBase, public txEngineBase, public txEngineEventHandler
{
public:
	EditorCoreRoot();
	virtual ~EditorCoreRoot();
	void init(const HWND& hwnd, const int& colourBits, const int& width, const int& height);
	void preUpdate(const float& elapsedTime);
	void update(const float& elapsedTime);
	void lateUpdate(const float& elapsedTime);
	void render();
	void destroy();
	void resize(const int& width, const int& height);
	void notifyInitDone() { mInitFlag = true; }
	void setModalDialog(ModalDialog dialog, int style[MDS_MAX], int result[MDR_MAX]);
	// ����ʹ��ECRoot��sendEvent�����EventSystem��pushEvent
	void sendEvent(const CORE_EVENT& type, const std::string& param, const bool& sendImmediately = true);
	void sendEvent(const CORE_EVENT& type, const txVector<std::string>& params = txVector<std::string>(), bool sendImmediately = true);
	void sendDelayEvent(const CORE_EVENT& type, const std::string& param);
	void sendDelayEvent(const CORE_EVENT& type, const txVector<std::string>& params = txVector<std::string>());
	void logError(const std::string& info);
	void logInfo(const std::string& info);
	CoreEventSystem* getCoreEventSystem()			{ return mCoreEventSystem; }
	ECUndoManager* getUndoManager()					{ return mUndoManager; }
	txCamera* getCamera()							{ return mMainCamera; }
	TremblingEditorCore* getTremblingEditorCore()	{ return mTremblingEditorCore; }
	LayoutEditorCore* getLayoutEditorCore()			{ return mLayoutEditorCore; }
	ActionTreeEditorCore* getActionTreeEditorCore()	{ return mActionTreeEditorCore; }
	ComponentEditorCore* getComponentEditorCore()	{ return mComponentEditorCore; }
	virtual void registerEngineEventHandler();
	virtual void unregisterEngineEventHandler();
	virtual void onEngineEvent(const ENGINE_EVENT& event, const txVector<std::string>& paramList);
	MODAL_DIALOG_RESULT showModalYesNoCancel(const std::string& info);
	MODAL_DIALOG_RESULT showModalYesNo(const std::string& info);
protected:
	void initCamera();
	void checkErrorInfoBuffer();
	MODAL_DIALOG_RESULT showModal(const std::string& info, const int& flag);
protected:
	txEngineRoot*				mEngineRoot;			// ����
	CoreEventSystem*			mCoreEventSystem;		// �༭�����¼�ϵͳ
	ECUndoManager*				mUndoManager;			// �༭���ĳ���ϵͳ
	TremblingEditorCore*		mTremblingEditorCore;
	LayoutEditorCore*			mLayoutEditorCore;
	ActionTreeEditorCore*		mActionTreeEditorCore;
	ComponentEditorCore*		mComponentEditorCore;
	txCamera*					mMainCamera;			// ���ڿ����ӿ�
	txVector<std::string>		mErrorInfoBuffer;		// ������Ϣ����,���ڱ����¼�ϵͳ����֮ǰ�����Ĵ�����Ϣ
	bool						mInitFlag;				// ��ʼ�����,Ϊfalse��ʾ��δ��ʼ�����,���ⲿ���б��
	ModalDialog					mModalDialog;			// ����ѯ�ʵ�ģʽ�Ի���
	int							mDialogStyle[MDS_MAX];	// �Ի�����
	int							mDialogResult[MDR_MAX];	// �Ի��򷵻ؽ��
};

#endif