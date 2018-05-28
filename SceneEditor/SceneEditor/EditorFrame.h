#ifndef _EDITOR_FRAME_H_
#define _EDITOR_FRAME_H_

#include "txEngineEventHandler.h"
#include "txCommandReceiver.h"

#include "CoreEventHandler.h"
#include "EditorCoreBase.h"

#include "EditorUtility.h"

class EditorCoreRoot;
class PropertyWindow;
class RenderCanvas;
class SceneHierarchy;
class ComponentResource;
class ComponentHierarchy;
class txEngineRoot;
class txCommandSystem;

class EditorFrame : public wxFrame, public CoreEventHandler, public txEngineEventHandler, public txCommandReceiver, public EditorCoreBase
{
public:
	EditorFrame(const wxString& title, const wxSize& size);
	void init();
	void destroy();
	void setup();
	virtual ~EditorFrame();
	virtual void registerEngineEventHandler();
	virtual void unregisterEngineEventHandler();
	virtual void registerEditorCoreEvent();
	virtual void unregisterEditorCoreEvent();
	void Update(float elapsedTime);
	void Render();
	void KeyProcess();
	void enableUndo(const bool& enable);
	void enableRedo(const bool& enable);
	wxWindow* getWindow(const std::string& name);
	txMap<wxWindow*, std::pair<int, int> >& getWindowIDList() { return mWindowIDList; }
	wxMenu* getWindowMenu() { return mWindowMenu; }
	wxAuiToolBar* getWindowToolBar() { return mWindowToolBar; }

	PropertyWindow* getPropertyWindow() { return mPropertyWindow; }
	RenderCanvas* getRenderCanvas() { return mRenderCanvas; }
	EditorCoreRoot* getEditorCoreRoot() { return mEditorCoreRoot; }
	SceneHierarchy* getSceneHierarchy() { return mSceneHierarchy; }
	ComponentResource* getComponentResource() { return mComponentResource; }
	ComponentHierarchy* getComponentHierarchy() { return mComponentHierarchy; }

	virtual void onEditorEvent(const CORE_EVENT& type, const txVector<std::string>& params);
	virtual void onEngineEvent(const ENGINE_EVENT& event, const txVector<std::string>& paramList);
	
	DECLARE_EVENT_TABLE()
	void OnTimer(wxTimerEvent& event);
	void OnNewAdvanceScene(wxCommandEvent& event);
	void OnOpenScene(wxCommandEvent& event);
	void OnOpenAdvanceScene(wxCommandEvent& event);
	void OnSaveAdvanceScene(wxCommandEvent& event);
	void OnTransSceneFile(wxCommandEvent& event);
	void OnTransSkinMeshFile(wxCommandEvent& event);
	void OnCloseFile(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event); // ��Ӧ�˵����˳��¼�
	void OnCloseWindow(wxCloseEvent& event);	// ��Ӧ���򷢳��Ĺر��¼�
	void OnUndo(wxCommandEvent& event);
	void OnRedo(wxCommandEvent& event);
	void OnCopy(wxCommandEvent& event);
	void OnCut(wxCommandEvent& event);
	void OnPaste(wxCommandEvent& event);
	void OnNewEditor(wxCommandEvent& event);
	void OnShowEditorGrid(wxCommandEvent& event);
	void OnShowEditorAxis(wxCommandEvent& event);
	void OnShowSky(wxCommandEvent& event);
	void OnShowSceneGrid(wxCommandEvent& event);
	void OnShowScene(wxCommandEvent& event);
	void OnResetCamera(wxCommandEvent& event);
	void OnSceneHierarchyPanel(wxCommandEvent& event);
	void OnPropertyPanel(wxCommandEvent& event);
	void OnRenderCanvas(wxCommandEvent& event);
	void OnComponentResourcePanel(wxCommandEvent& event);
	void OnComponentHierarchyPanel(wxCommandEvent& event);
	void OnLoadLayout(wxCommandEvent& event);
	void OnSaveLayout(wxCommandEvent& event);
	void OnCalculateHeightMap(wxCommandEvent& event);
	void OnShowHeightMap(wxCommandEvent& event);
	void OnSaveHeightMap(wxCommandEvent& event);
	void OnSaveRoadPoint(wxCommandEvent& event);
	void OnLoadHeightMap(wxCommandEvent& event);
	void OnCameraMoveSpeed(wxCommandEvent& event);
	void OnCameraRotateSpeed(wxCommandEvent& event);
	void OnSceneX(wxCommandEvent& event);
	void OnSceneZ(wxCommandEvent& event);
	void OnSceneMoveSpeed(wxCommandEvent& event);
	void OnCameraVerticalDown(wxCommandEvent& event);
	void OnBackgroundColour(wxColourPickerEvent& event);
protected:
	void createMenu();
	void createToolBar();
	void createRenderWindow();
	void createWindows();
	void createStatuBar();
	void updateStatus();
	void createEditorCore();
	void refreshAllResource();
	void showPanel(wxWindow* panel, const bool& show);
	void saveRoadPoint(const std::string& fullPath, txMap<int, txVector<VECTOR3> >& roadPoint);
	// ˢ��ȫ���Ŀ�ѡ�в˵��Ϳ�ѡ�й��߰�ť��ѡ��״̬
	void refreshAllMenuToolCheckState();
protected:
	wxTimer* mTimer;
	// ����˵��ʹ��ڵ�ָ��,����ʹ��
	wxMenu* mFileMenu;
	wxMenu* mEditMenu;
	wxMenu* mToolMenu;
	wxMenu* mWindowMenu;
	wxMenu* mLayoutMenu;
	wxAuiToolBar* mEditToolBar;
	wxAuiToolBar* mShowHideToolBar;
	wxAuiToolBar* mWindowToolBar;
	wxAuiToolBar* mCameraToolBar;
	wxAuiToolBar* mSceneToolBar;
	SceneHierarchy* mSceneHierarchy;
	PropertyWindow* mPropertyWindow;
	RenderCanvas* mRenderCanvas;
	ComponentResource* mComponentResource;
	ComponentHierarchy* mComponentHierarchy;
	wxAuiManager mAuiManager;
	
	EditorCoreRoot* mEditorCoreRoot;
	wxTextCtrl* mSceneMoveText;

	txMap<wxWindow*, std::pair<int, int> > mWindowIDList;
	txMap<std::string, wxWindow*> mWindowList;
};

#endif