#ifndef _EDITOR_FRAME_H_
#define _EDITOR_FRAME_H_

#include "txEngineEventHandler.h"
#include "txCommandReceiver.h"

#include "CoreEventHandler.h"
#include "EditorCoreBase.h"
#include "CommonDefine.h"

#include "EditorUtility.h"

class txEngineRoot;
class txCommandSystem;
class PropertyWindow;
class WindowResource;
class RenderCanvas;
class LayoutHierarchy;
class InnerChildHierarchy;
class LayoutResource;
class LoadedLayout;
class PrefabResource;
class WindowTemplate;
class KeyFrameGraph;
class KeyFrameResource;
class ComponentPropertyPanel;
class ComponentViewer;
class ActionTreeGraph;
class ActionTreeResource;
class ActionPropertyPanel;
class EditorCoreRoot;
class ConnectWindowDialog;
class SymmetryWindowDialog;
class LayoutManager;
class Layout;
class Window;
const std::string P_FRAME_LAYOUT_PATH = P_MEDIA_PATH + "FrameLayout/";
const std::string FRAME_LAYOUT_SUFFIX = ".FrameLayout";
const int MAX_RESOLUTION_COUNT = 10;

class EditorFrame : public wxFrame, public txEngineEventHandler, public CoreEventHandler, public txCommandReceiver, public EditorCoreBase
{
public:
	EditorFrame(const wxString& title, const wxSize& size);
	void init();
	void destroy();
	void setup();
	virtual ~EditorFrame();
	virtual void registerEngineEventHandler();
	virtual void unregisterEngineEventHandler();
	virtual void registerEditorCoreEventHandler();
	virtual void unregisterEditorCoreEventHandler();
	virtual void onEditorCoreEvent(const CORE_EVENT& type, const txVector<std::string>& params);
	virtual void onEngineEvent(const ENGINE_EVENT& event, const txVector<std::string>& paramList);
	void Update(const float& elapsedTime);
	void Render();
	void OpenFile(const std::string& fileName);	// ��һ���ļ�
	void DestroyLayout(const std::string& layoutName);
	void CloseLayout();
	void enableUndo(bool enable);
	void enableRedo(bool enable);
	txMap<wxWindow*, std::pair<int, int> >& getWindowIDList() { return mWindowIDList; }
	wxAuiManager& getAuiManager() { return mAuiManager; }
	wxWindow* getPanel(const std::string& name);
	void logError(const std::string& info);
	void logInfo(const std::string& info);
	static int showModalDialog(const std::string& caption, const std::string& info, const int& style);

	wxMenu* getFileMenu() { return mFileMenu; }
	wxMenu* getEditMenu() { return mEditMenu; }
	wxMenu* getToolMenu() { return mToolMenu; }
	wxMenu* getWindowMenu() { return mWindowMenu; }
	wxMenu* getLayoutMenu() { return mLayoutMenu; }
	wxMenu* getResolutionMenu() { return mResolutionMenu; }
	wxMenu* getWindowPosition() { return mWindowPosition; }

	wxAuiToolBar* getFileToolBar() {return mFileToolBar; }
	wxAuiToolBar* getEditToolBar() {return mEditToolBar; }
	wxAuiToolBar* getShowHideToolBar() {return mShowHideToolBar; }
	wxAuiToolBar* getWindowToolBar() {return mWindowToolBar; }

	EditorCoreRoot* getEditorCoreRoot() const { return mEditorCoreRoot; }
	LayoutHierarchy* getLayoutHierarchy() { return mLayoutHierarchy; }
	InnerChildHierarchy* getInnerChildHierarchy() { return mInnerChildHierarchy; }
	PropertyWindow* getPropertyWindow() { return mPropertyWindow; }
	WindowResource* getWindowResource() { return mWindowResource; }
	RenderCanvas* getRenderCanvas() { return mRenderCanvas; }
	LayoutResource* getLayoutResource() { return mLayoutResource; }
	LoadedLayout* getLoadedLayout() { return mLoadedLayout; }
	PrefabResource* getPrefabResource() { return mPrefabResource; }
	WindowTemplate* getWindowTemplate() { return mWindowTemplate; }
	KeyFrameGraph* getKeyFrameGraph() { return mKeyFrameGraph; }
	KeyFrameResource* getKeyFrameResource() { return mKeyFrameResource; }
	ComponentViewer* getComponentViewer() { return mComponentViewer; }
	ComponentPropertyPanel* getComponentPropertyPanel() { return mComponentPropertyPanel; }
	ActionTreeResource* getActionTreeResource() { return mActionTreeResource; }
	ActionTreeGraph* getActionTreeGraph() { return mActionTreeGraph; }
	ActionPropertyPanel* getActionPropertyPanel() { return mActionPropertyPanel; }
	
	DECLARE_EVENT_TABLE()
	void OnTimer(wxTimerEvent& event);
	void OnNewFile(wxCommandEvent& event);
	void OnSaveFile(wxCommandEvent& event);
	void OnCloseFile(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event); // ��Ӧ�˵����˳��¼�
	void OnCloseWindow(wxCloseEvent& event);	// ��Ӧ���򷢳��Ĺر��¼�
	void OnUndo(wxCommandEvent& event);
	void OnRedo(wxCommandEvent& event);
	void OnCopy(wxCommandEvent& event);
	void OnCut(wxCommandEvent& event);
	void OnPaste(wxCommandEvent& event);
	void OnPickupUsedResources(wxCommandEvent& event);
	void OnPanelShow(wxCommandEvent& event);
	void OnLoadPerspective(wxCommandEvent& event);
	void OnSavePerspective(wxCommandEvent& event);
	void OnResolution(wxCommandEvent& event);
	void OnWireframeVisible(wxCommandEvent& event);
	void OnWindowPositinCenter(wxCommandEvent& event);			// ���ھ���
	void OnWindowPositionLeftTop(wxCommandEvent& event);		// �������Ͻ�
	void OnwindowPositionLeftDown(wxCommandEvent& event);		// �������½�
	void OnWindowPositionRightTop(wxCommandEvent& event);		// �������Ͻ�
	void OnwindowPositionRightDown(wxCommandEvent& event);		// �������½�
	void OnWindowPositionTopCenter(wxCommandEvent& event);		// ���Ͼ���
	void OnWindowPositionBottomCenter(wxCommandEvent& event);	// ���¾���
	void OnWindowPositionLeftCenter(wxCommandEvent& event);		// �����
	void OnWindowPositionRightCenter(wxCommandEvent& event);	// �Ҿ���
	void OnWindowPositionFill(wxCommandEvent& event);			// ����
	void OnShowRenderWindowCoordinate(wxCommandEvent& event);	// ��ʾ��Ⱦ��������ϵ
	void OnShowWindowCoordinate(wxCommandEvent& event);			// ��ʾѡ�д�������ϵ
	void OnWindowPosMoveUp(wxCommandEvent& event);				// ��������һ��λ��
	void OnWindowPosMoveDown(wxCommandEvent& event);			// ��������һ��λ��
	void OnWindowPosMoveTop(wxCommandEvent& event);				// �������Ƶ����
	void OnWindowPosMoveBottom(wxCommandEvent& event);			// �������Ƶ���ײ�
	void OnScreenDensity(wxCommandEvent& event);
	void OnDockRenderWindow(wxCommandEvent& event);
	void OnConnectWindow(wxCommandEvent& event);
	void OnSymmetryWindow(wxCommandEvent& event);
	void OnDirectVertices(wxCommandEvent& event);
	void OnBackgroundColour(wxColourPickerEvent& event);
	void OnSelectInvisible(wxCommandEvent& event);
	void OnSelectUnHandleInput(wxCommandEvent& event);
	void OnShowNumberRect(wxCommandEvent& event);
protected:
	void createMenu();
	void createToolBar();
	void createRenderWindow();
	void createWindows();
	void createStatuBar(){}
	void updateStatus(){}
	void createEditorCore();
	void refreshAllResource();
	void refreshAllMenuToolCheckState();	// ˢ��ȫ���Ŀ�ѡ�в˵��Ϳ�ѡ�й��߰�ť��ѡ��״̬
	void loadAllPerspective(wxMenu* parentMenu);
	void createResolutionMenu(wxMenu* parentMenu);
	void loadPerspective(const std::string& name);
protected:
	wxAuiManager mAuiManager;
	wxTimer* mTimer;
	// ����˵��ʹ��ڵ�ָ��,����ʹ��
	wxMenu* mFileMenu;
	wxMenu* mEditMenu;
	wxMenu* mToolMenu;
	wxMenu* mWindowMenu;
	wxMenu* mLayoutMenu;
	wxMenu* mResolutionMenu;  //�ֱ��ʲ˵�
	wxMenu* mWindowPosition;
	wxAuiToolBar* mFileToolBar;
	wxAuiToolBar* mEditToolBar;
	wxAuiToolBar* mShowHideToolBar;
	wxAuiToolBar* mWindowToolBar;
	wxAuiToolBar* mSelectToolBar;
	PrefabResource* mPrefabResource;
	LayoutResource* mLayoutResource;
	LoadedLayout* mLoadedLayout;
	LayoutHierarchy* mLayoutHierarchy;
	InnerChildHierarchy* mInnerChildHierarchy;
	PropertyWindow* mPropertyWindow;
	WindowResource* mWindowResource;
	RenderCanvas* mRenderCanvas;
	WindowTemplate* mWindowTemplate;
	KeyFrameGraph* mKeyFrameGraph;
	KeyFrameResource* mKeyFrameResource;
	ComponentViewer* mComponentViewer;
	ComponentPropertyPanel* mComponentPropertyPanel;
	ActionTreeResource* mActionTreeResource;
	ActionTreeGraph* mActionTreeGraph;
	ActionPropertyPanel* mActionPropertyPanel;
	ConnectWindowDialog* mConnectWindowDialog;
	SymmetryWindowDialog* mSymmetryWindowDialog;
	EditorCoreRoot* mEditorCoreRoot;
	std::string mAppNormalName;
	// first�Ƿֱ���,second�ǿ�߱�
	txVector<std::pair<wxSize, wxSize> > mResolutionSize;
	txMap<wxWindow*, std::pair<int, int> > mWindowIDList;
	txMap<std::string, wxWindow*> mWindowList;
};

#endif