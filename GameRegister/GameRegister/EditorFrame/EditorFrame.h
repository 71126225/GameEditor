#ifndef _EDITOR_FRAME_H_
#define _EDITOR_FRAME_H_

#include <map>

#include "CTEventHandler.h"
#include "txCommandReceiver.h"
#include "EditorUtility.h"

class txCommandSystem;
class EditorCore;
class MainPanel;
class EditorFrame : public wxFrame, public CTEventHandler, public txCommandReceiver
{
public:
	EditorFrame(wxString title, wxSize size);
	void init();
	void destroy();
	void setup();
	virtual ~EditorFrame();
	// ˢ��ȫ���Ŀ�ѡ�в˵��Ϳ�ѡ�й��߰�ť��ѡ��״̬
	void RefreshAllMenuToolCheckState();
	void Update(float elapsedTime);
	void Render();
	void KeyProcess();
	virtual WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
	virtual void registerEditorCoreEventHandler();
	virtual void unregisterEditorCoreEventHandler();
	virtual void onEditorCoreEvent(const CORE_EVENT_TYPE& type, const std::vector<std::string>& params);
	void showPanel(wxWindow* panel, bool show);
	std::map<wxWindow*, int>& getWindowIDList() { return mWindowIDList; }
	wxAuiManager& getAuiManager() { return mAuiManager; }
	wxWindow* getWindow(const std::string& name);
	wxAuiToolBar* getWindowToolBar() { return mWindowToolBar; }
	
	DECLARE_EVENT_TABLE()
	void OnTimer(wxTimerEvent& event);
	void OnExit(wxCommandEvent& event);			// ��Ӧ�˵����˳��¼�
	void OnCloseWindow(wxCloseEvent& event);	// ��Ӧ���򷢳��Ĺر��¼�
	void OnPanelShow(wxCommandEvent& event);
protected:
	void CreateMenu();
	void CreateToolBar();
	void CreateWindows();
	void CreateStatuBar();
	void UpdateStatus();
	void CreateEditorCore();
	void RefreshAllResource();
protected:
	wxTimer* mTimer;
	wxMenu* mFileMenu;
	wxAuiToolBar* mWindowToolBar;
	wxAuiManager mAuiManager;
	txCommandSystem* mCommandSystem;
	std::map<wxWindow*, int> mWindowIDList;
	std::map<std::string, wxWindow*> mWindowList;
	EditorCore* mEditorCore;
	MainPanel* mMainPanel;
};

#endif