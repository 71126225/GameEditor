#ifndef _EDITOR_FRAME_H_
#define _EDITOR_FRAME_H_

#include "CTEventHandler.h"
#include "txCommandReceiver.h"
#include "EditorUtility.h"

class ConfigToolCore;
class SpeedDataPanel;
class WirelessDevicePanel;
class SetupDevicePanel;
class SpeedDataPanel;
class HeartRatePanel;
class txCommandSystem;
class EditorPanel;
class EditorFrame : public wxFrame, public CTEventHandler, public txCommandReceiver
{
public:
	EditorFrame(const wxString& title, const wxSize& size);
	void init();
	void destroy();
	void setup();
	virtual ~EditorFrame();
	void Update(float elapsedTime);
	void Render();
	void KeyProcess();
	virtual WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
	virtual void registerEditorCoreEventHandler();
	virtual void unregisterEditorCoreEventHandler();
	virtual void onEditorCoreEvent(const CORE_EVENT& type, txVector<std::string>& params);
	
	//get
	//-------------------------------------------------------------------------------------------------------------------------
	txMap<EditorPanel*, int>& getWindowIDList() { return mWindowIDList; }
	wxAuiManager& getAuiManager() { return mAuiManager; }
	EditorPanel* getPanel(const std::string& name);
	int getPanelID(const std::string& name);
	WirelessDevicePanel* getWirelessDevicePanel() { return mWirelessDevicePanel; }
	SetupDevicePanel* getSetupDevicePanel() { return mSetupDevicePanel; }
	HeartRatePanel* getHeartRatePanel() { return mHeartRatePanel; }
	ConfigToolCore* getConfigTool() { return mConfigToolCore; }
	wxAuiToolBar* getWindowToolBar() { return mWindowToolBar; }
	
	DECLARE_EVENT_TABLE()
	void OnTimer(wxTimerEvent& event);
	void OnExit(wxCommandEvent& event);			// ��Ӧ�˵����˳��¼�
	void OnCloseWindow(wxCloseEvent& event);	// ��Ӧ���򷢳��Ĺر��¼�
	void OnWirelessDevicePanel(wxCommandEvent& event);
	void OnSetupDevicePanel(wxCommandEvent& event);
	void OnSpeedDataPanel(wxCommandEvent& event);
	void OnHeartRatePanel(wxCommandEvent& event);
protected:
	void CreateMenu();
	void CreateToolBar();
	void CreateWindows();
	void CreateStatuBar();
	void UpdateStatus();
	void CreateEditorCore();
	void RefreshAllResource();
	void hideAllPanel();
	void showPanel(wxWindow* panel, bool show, bool update = true);
	// ˢ��ȫ���Ŀ�ѡ�в˵��Ϳ�ѡ�й��߰�ť��ѡ��״̬
	void RefreshAllMenuToolCheckState();
protected:
	wxTimer* mTimer;
	wxMenu* mFileMenu;
	wxAuiToolBar* mWindowToolBar;
	wxAuiManager mAuiManager;
	WirelessDevicePanel* mWirelessDevicePanel;
	SetupDevicePanel* mSetupDevicePanel;
	ConfigToolCore* mConfigToolCore;
	SpeedDataPanel* mSpeedDataPanel;
	HeartRatePanel* mHeartRatePanel;
	txMap<EditorPanel*, int> mWindowIDList;
	txMap<std::string, EditorPanel*> mWindowList;
};

#endif