#include <windows.h>
#include <Dbt.h>

#include "ConfigToolCore.h"
#include "CTEventSystem.h"
#include "txMemeryTrace.h"
#include "SpeedDataManager.h"

#include "EditorFrame.h"
#include "EditorApp.h"
#include "SpeedDataPanel.h"
#include "HeartRatePanel.h"
#include "ConfigDevicePanel.h"
#include "ConfigCardPanel.h"
#include "DeviceListPanel.h"
#include "CardInfoPanel.h"
#include "EditorCommandHeader.h"

enum
{
	ID_TIMER,
	ID_MENU_EXIT,
	ID_TOOL_SPEED_DATA_PANEL,
	ID_TOOL_HEART_RATE_PANEL,
	ID_TOOL_CONFIG_CARD_PANEL,
	ID_TOOL_CONFIG_DEVICE_PANEL,
	ID_TOOL_DEVICE_LIST_PANEL,
	ID_TOOL_CARD_INFO_PANEL,
};

BEGIN_EVENT_TABLE(EditorFrame, wxFrame)

EVT_TIMER(ID_TIMER, OnTimer)
EVT_CLOSE(OnCloseWindow)
EVT_MENU(ID_MENU_EXIT, OnExit)
EVT_TOOL(ID_TOOL_SPEED_DATA_PANEL, OnSpeedDataPanel)
EVT_TOOL(ID_TOOL_HEART_RATE_PANEL, OnHeartRatePanel)
EVT_TOOL(ID_TOOL_CONFIG_DEVICE_PANEL, OnConfigDevicePanel)
EVT_TOOL(ID_TOOL_CONFIG_CARD_PANEL, OnConfigCardPanel)
EVT_TOOL(ID_TOOL_DEVICE_LIST_PANEL, OnDeviceListPanel)
EVT_TOOL(ID_TOOL_CARD_INFO_PANEL, OnCardInfoPanel)

END_EVENT_TABLE()

EditorFrame::EditorFrame(wxString title, wxSize size)
:
wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size),
txCommandReceiver("EditorFrame"),
mTimer(NULL),
mFileMenu(NULL),
mConfigDevicePanel(NULL),
mConfigCardPanel(NULL),
mSpeedDataPanel(NULL),
mHeartRatePanel(NULL),
mDeviceListPanel(NULL),
mCardInfoPanel(NULL),
mConfigToolCore(NULL)
{
	;
}

EditorFrame::~EditorFrame()
{
	;
}

void EditorFrame::init()
{
	setup();
	mTimer = TRACE_NEW(wxTimer, mTimer);
	mTimer->Start(1);
	mTimer->SetOwner(this, ID_TIMER);
}

void EditorFrame::destroy()
{
	TRACE_DELETE(mTimer);
	unregisterEditorCoreEventHandler();
	TRACE_DELETE(mConfigToolCore);
	mAuiManager.UnInit();
}

void EditorFrame::setup()
{
	mAuiManager.SetManagedWindow(this);
	// �����༭����
	CreateEditorCore();
	// �����˵���
	CreateMenu();
	// ����������
	CreateToolBar();
	// ���������Ӵ���
	CreateWindows();
	// �����ײ���״̬��ʾ��
	CreateStatuBar();
	// �������ˢ��һ��ȫ���ؼ���ѡ��״̬
	RefreshAllMenuToolCheckState();
	// ˢ��ȫ����Դ�б�
	RefreshAllResource();
}

void EditorFrame::CreateMenu()
{
	wxMenuBar* menuBar = new wxMenuBar();

	// �ļ��˵�
	mFileMenu = new wxMenu();
	mFileMenu->Append(ID_MENU_EXIT, wxT("�˳�"));
	menuBar->Append(mFileMenu, "�ļ�");

	SetMenuBar(menuBar);
}

void EditorFrame::CreateToolBar()
{
	mWindowToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORIZONTAL);
	mWindowToolBar->SetToolBitmapSize(wxSize(16, 16));
	mWindowToolBar->AddTool(ID_TOOL_SPEED_DATA_PANEL, wxT("�ٶ����ݴ���"), wxBITMAP(IDB_SPEED_ICON), wxT("��ʾ�ٶ����ݴ���"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_HEART_RATE_PANEL, wxT("�������ݴ���"), wxBITMAP(IDB_HEART_RATE_ICON), wxT("�������ݴ���"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_CONFIG_DEVICE_PANEL, wxT("�����豸����"), wxBITMAP(IDB_CONFIG_DEVICE_ICON), wxT("��ʾ�����豸����"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_CONFIG_CARD_PANEL, wxT("���ÿ�Ƭ����"), wxBITMAP(IDB_CONFIG_CARD_ICON), wxT("��ʾ���ÿ�Ƭ����"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_DEVICE_LIST_PANEL, wxT("�豸�б���"), wxBITMAP(IDB_DEVICE_LIST_ICON), wxT("��ʾ�豸�б���"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_CARD_INFO_PANEL, wxT("ˢ����Ϣ����"), wxBITMAP(IDB_CARD_INFO_ICON), wxT("��ʾˢ����Ϣ����"), wxITEM_CHECK);
	mWindowToolBar->Realize();
	mAuiManager.AddPane(mWindowToolBar, wxAuiPaneInfo().Name(wxT("WindowToolBar")).Caption(wxT("���ڹ�����")).ToolbarPane().Top());
	
	mAuiManager.Update();
}

void EditorFrame::CreateWindows()
{
	mSpeedDataPanel = new SpeedDataPanel(this, wxPoint(-1, -1), wxSize(400, -1), wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxMINIMIZE | wxMAXIMIZE, wxT("SpeedDataPanel"));
	mAuiManager.AddPane(mSpeedDataPanel, wxAuiPaneInfo().Name(mSpeedDataPanel->GetName()).BestSize(wxSize(-1, -1)).Caption(wxT("�ٶ�����")).Center().Dockable(true));
	mSpeedDataPanel->Show();

	mHeartRatePanel = new HeartRatePanel(this, wxPoint(-1, -1), wxSize(400, -1), wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxMINIMIZE | wxMAXIMIZE, wxT("HeartRatePanel"));
	mAuiManager.AddPane(mHeartRatePanel, wxAuiPaneInfo().Name(mHeartRatePanel->GetName()).BestSize(wxSize(-1, -1)).Caption(wxT("��������")).Center().Dockable(true));
	mHeartRatePanel->Show();

	mDeviceListPanel = new DeviceListPanel(this, wxPoint(-1, -1), wxSize(400, -1), wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxMINIMIZE | wxMAXIMIZE, wxT("DeviceListPanel"));
	mAuiManager.AddPane(mDeviceListPanel, wxAuiPaneInfo().Name(mDeviceListPanel->GetName()).BestSize(wxSize(-1, -1)).Caption(wxT("�豸�б�")).Left().Dockable(true));
	mDeviceListPanel->Show();

	mConfigDevicePanel = new ConfigDevicePanel(this, wxPoint(-1, -1), wxSize(400, -1), wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxMINIMIZE | wxMAXIMIZE, wxT("ConfigDevicePanel"));
	mAuiManager.AddPane(mConfigDevicePanel, wxAuiPaneInfo().Name(mConfigDevicePanel->GetName()).BestSize(wxSize(-1, -1)).Caption(wxT("�����豸")).Left().Dockable(true));
	mConfigDevicePanel->Show();

	mConfigCardPanel = new ConfigCardPanel(this, wxPoint(-1, -1), wxSize(400, -1), wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxMINIMIZE | wxMAXIMIZE, wxT("ConfigCardPanel"));
	mAuiManager.AddPane(mConfigCardPanel, wxAuiPaneInfo().Name(mConfigCardPanel->GetName()).BestSize(wxSize(-1, -1)).Caption(wxT("���ÿ�Ƭ")).Left().Dockable(true));
	mConfigCardPanel->Show();

	mCardInfoPanel = new CardInfoPanel(this, wxPoint(-1, -1), wxSize(400, -1), wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxMINIMIZE | wxMAXIMIZE, wxT("CardInfoPanel"));
	mAuiManager.AddPane(mCardInfoPanel, wxAuiPaneInfo().Name(mCardInfoPanel->GetName()).BestSize(wxSize(-1, -1)).Caption(wxT("ˢ����Ϣ")).Left().Dockable(true));
	mCardInfoPanel->Show();

	mAuiManager.Update();

	mWindowIDList.insert(std::make_pair(mSpeedDataPanel, ID_TOOL_SPEED_DATA_PANEL));
	mWindowIDList.insert(std::make_pair(mHeartRatePanel, ID_TOOL_HEART_RATE_PANEL));
	mWindowIDList.insert(std::make_pair(mConfigDevicePanel, ID_TOOL_CONFIG_DEVICE_PANEL));
	mWindowIDList.insert(std::make_pair(mConfigCardPanel, ID_TOOL_CONFIG_CARD_PANEL));
	mWindowIDList.insert(std::make_pair(mDeviceListPanel, ID_TOOL_DEVICE_LIST_PANEL));
	mWindowIDList.insert(std::make_pair(mCardInfoPanel, ID_TOOL_CARD_INFO_PANEL));

	mWindowList.insert(std::make_pair(mSpeedDataPanel->GetName().ToStdString(), mSpeedDataPanel));
	mWindowList.insert(std::make_pair(mHeartRatePanel->GetName().ToStdString(), mHeartRatePanel));
	mWindowList.insert(std::make_pair(mConfigDevicePanel->GetName().ToStdString(), mConfigDevicePanel));
	mWindowList.insert(std::make_pair(mConfigCardPanel->GetName().ToStdString(), mConfigCardPanel));
	mWindowList.insert(std::make_pair(mDeviceListPanel->GetName().ToStdString(), mDeviceListPanel));
	mWindowList.insert(std::make_pair(mCardInfoPanel->GetName().ToStdString(), mCardInfoPanel));
}

void EditorFrame::CreateStatuBar()
{
	;
}

void EditorFrame::UpdateStatus()
{
	;
}

void EditorFrame::CreateEditorCore()
{
	mConfigToolCore = TRACE_NEW(ConfigToolCore, mConfigToolCore);
	mConfigToolCore->init();
	mCommandSystem = mConfigToolCore->getCommandSystem();
	registerEditorCoreEventHandler();
	mConfigToolCore->notifyInitDone();
}

void EditorFrame::RefreshAllResource()
{
	mConfigToolCore->checkDevice();
	mHeartRatePanel->refreshTime();
	mSpeedDataPanel->refresh();
}

void EditorFrame::RefreshAllMenuToolCheckState()
{
	std::map<wxWindow*, int>::iterator iterWindow = mWindowIDList.begin();
	std::map<wxWindow*, int>::iterator iterWindowEnd = mWindowIDList.end();
	for (; iterWindow != iterWindowEnd; ++iterWindow)
	{
		bool check = iterWindow->first->IsShown();
		mWindowToolBar->ToggleTool(iterWindow->second, check);
	}
}

void EditorFrame::OnTimer(wxTimerEvent& event)
{
	static unsigned long lastTime = timeGetTime();
	unsigned long curTime = timeGetTime();
	float elapsedTime = (curTime - lastTime) / 1000.0f;
	Update(elapsedTime);
	lastTime = curTime;
	Render();
}

void EditorFrame::Update(float elapsedTime)
{
	if (mConfigToolCore == NULL)
	{
		return;
	}
	mConfigToolCore->update(elapsedTime);
	SpeedDataManager* speedDataManager = mConfigToolCore->getSpeedDataManager();
	// �����Ҫ��������,��ӹ��ߺ����л������,���Ҵ�������ҳ����ʾ����
	if (speedDataManager->isSpeedDataDirty())
	{
		mSpeedDataPanel->showData(speedDataManager->getSpeedDataList());
		speedDataManager->setSpeedDataDirty(false);
	}
	if (speedDataManager->isHeartRateDataDirty())
	{
		mHeartRatePanel->showData(speedDataManager->getHeartRateDataList());
		speedDataManager->setHeartRateDataDirty(false);
	}
	if (speedDataManager->isCardInfoDirty())
	{
		mCardInfoPanel->setCardInfo(speedDataManager->getCardData());
		speedDataManager->setCardInfoDirty(false);
	}
	KeyProcess();
	UpdateStatus();
}

void EditorFrame::Render()
{
	;
}

void EditorFrame::KeyProcess()
{
	;
}

WXLRESULT EditorFrame::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
	if (message == WM_DEVICECHANGE)
	{
		// �Ƴ��豸,����豸,�豸�仯(ʵ�ʲ���ֵһֱ�����豸�仯)
		if (wParam == DBT_DEVICEREMOVECOMPLETE || wParam == DBT_DEVICEARRIVAL || wParam == DBT_DEVNODES_CHANGED)
		{
			mConfigToolCore->checkDevice();
		}
	}
	return wxFrame::MSWWindowProc(message, wParam, lParam);
}

void EditorFrame::registerEditorCoreEventHandler()
{
	mConfigToolCore->getEventSystem()->registerAllEvent(this);
}

void EditorFrame::unregisterEditorCoreEventHandler()
{
	mConfigToolCore->getEventSystem()->unregisterAllEvent(this);
}

void EditorFrame::onEditorCoreEvent(const CORE_EVENT_TYPE& type, std::vector<std::string>& params)
{
	if (type == CET_ERROR_LOG)
	{
		EditorUtility::logError(params[0]);
	}
	else if (type == CET_INFO_LOG)
	{
		EditorUtility::logInfo(params[0]);
	}
	else if (type == CET_DEVICE_CHANGED)
	{
		CommandEditorFrameNotifyDeviceChanged cmd(COMMAND_PARAM);
		mCommandSystem->pushCommand(&cmd, this);
	}
	else if (type == CET_RECEIVE_DATA)
	{
		char* data = (char*)txUtility::stringToInt(params[0]);
		int dataCount = txUtility::stringToInt(params[1]);
		mDeviceListPanel->setReceiveData(data, dataCount);
	}
	else if (type == CET_SEND_DATA)
	{
		char* data = (char*)txUtility::stringToInt(params[0]);
		int dataCount = txUtility::stringToInt(params[1]);
		mDeviceListPanel->setSendData(data, dataCount);
	}
	else if (type == CET_HEART_RATE_TIME)
	{
		mHeartRatePanel->refreshTime();
	}
}

wxWindow* EditorFrame::getWindow(std::string name)
{
	std::map<std::string, wxWindow*>::iterator iter = mWindowList.find(name);
	if (iter != mWindowList.end())
	{
		return iter->second;
	}
	return NULL;
}

void EditorFrame::showPanel(wxWindow* panel, bool show)
{
	if (mAuiManager.GetPane(panel).IsShown() != show)
	{
		mAuiManager.GetPane(panel).Show(show);
		mAuiManager.Update();
	}
}

void EditorFrame::OnSpeedDataPanel(wxCommandEvent& event)
{
	showPanel(mSpeedDataPanel, event.IsChecked());
}

void EditorFrame::OnHeartRatePanel(wxCommandEvent& event)
{
	showPanel(mHeartRatePanel, event.IsChecked());
}

void EditorFrame::OnConfigDevicePanel(wxCommandEvent& event)
{
	showPanel(mConfigDevicePanel, event.IsChecked());
}

void EditorFrame::OnConfigCardPanel(wxCommandEvent& event)
{
	showPanel(mConfigCardPanel, event.IsChecked());
}

void EditorFrame::OnDeviceListPanel(wxCommandEvent& event)
{
	showPanel(mDeviceListPanel, event.IsChecked());
}

void EditorFrame::OnCardInfoPanel(wxCommandEvent& event)
{
	showPanel(mCardInfoPanel, event.IsChecked());
}

void EditorFrame::OnExit(wxCommandEvent& event)
{
	// �����رմ��ڵ��¼�
	Close(true);
}

void EditorFrame::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
{
	// �����Լ�������
	destroy();
	// ���ٴ���
	Destroy();
}