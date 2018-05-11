#ifndef _EDITOR_FRAME_H_
#define _EDITOR_FRAME_H_

#include "EventHandler.h"
#include "CommandReceiver.h"
#include "EditorUtility.h"
#include "EditorCoreBase.h"

class RiseFreeToolCore;
class SpeedDataPanel;
class EditorFrame : public wxFrame, public EventHandler, public CommandReceiver, public EditorCoreBase
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
	virtual void onEditorCoreEvent(const CORE_EVENT_TYPE& type, const txVector<std::string>& params);
	void showPanel(wxWindow* panel, bool show);
	txMap<wxWindow*, int>& getWindowIDList() { return mWindowIDList; }
	wxAuiManager& getAuiManager() { return mAuiManager; }
	wxWindow* getWindow(std::string name);
	SpeedDataPanel* getSpeedDataPanel() { return mSpeedDataPanel; }
	RiseFreeToolCore* getRiseFreeToolCore() { return mRiseFreeToolCore; }
	wxAuiToolBar* getWindowToolBar() { return mWindowToolBar; }
	DECLARE_EVENT_TABLE()
	void OnTimer(wxTimerEvent& event);
	void OnExit(wxCommandEvent& event);			// ��Ӧ�˵����˳��¼�
	void OnCloseWindow(wxCloseEvent& event);	// ��Ӧ���򷢳��Ĺر��¼�
	void OnSpeedDataPanel(wxCommandEvent& event);
	void OnConfigDevicePanel(wxCommandEvent& event);
	void OnConfigCardPanel(wxCommandEvent& event);
	void OnDeviceListPanel(wxCommandEvent& event);
	void OnCardInfoPanel(wxCommandEvent& event);
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
	SpeedDataPanel* mSpeedDataPanel;
	RiseFreeToolCore* mRiseFreeToolCore;
	txMap<wxWindow*, int> mWindowIDList;
	txMap<std::string, wxWindow*> mWindowList;
};

#endif