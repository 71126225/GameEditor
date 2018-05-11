#include "EditorFrame.h"
#include "EditorApp.h"
#include "MainViewerPanel.h"
#include "HistoryDatePanel.h"
#include "DataBase.h"

enum
{
	ID_TIMER,
	ID_TOOL_HISTROY_DATE_PANEL,
	ID_TOOL_MAIN_VIEWER_PANEL,
};

BEGIN_EVENT_TABLE(EditorFrame, wxFrame)

EVT_TIMER(ID_TIMER, OnTimer)
EVT_CLOSE(OnCloseWindow)
EVT_TOOL(ID_TOOL_HISTROY_DATE_PANEL, OnPanelShow)
EVT_TOOL(ID_TOOL_MAIN_VIEWER_PANEL, OnPanelShow)

END_EVENT_TABLE()

EditorFrame::EditorFrame(wxString title, wxSize size)
:
wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size),
mTimer(NULL),
mDataBase(NULL),
mAppNormalName(title)
{
	;
}

void EditorFrame::init()
{
	setup();
	mTimer = new wxTimer();
	mTimer->Start(1);
	mTimer->SetOwner(this, ID_TIMER);
}

void EditorFrame::destroy()
{
	TRACE_DELETE(mDataBase);
	TRACE_DELETE(mTimer);
	mAuiManager.UnInit();
}

void EditorFrame::setup()
{
	// �����༭����
	CreateEditorCore();

	mAuiManager.SetManagedWindow(this);
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
	menuBar->Append(mFileMenu, "�ļ�");

	SetMenuBar(menuBar);
}

void EditorFrame::CreateToolBar()
{
	mWindowToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORIZONTAL);
	mWindowToolBar->SetToolBitmapSize(wxSize(16, 16));
	mWindowToolBar->AddTool(ID_TOOL_HISTROY_DATE_PANEL, wxT("���ڴ���"), wxBITMAP(IDB_PANEL_DATE), wxT("��ʾ���ڴ���"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_MAIN_VIEWER_PANEL, wxT("���ݴ���"), wxBITMAP(IDB_PANEL_INFO), wxT("��ʾ���ݴ���"), wxITEM_CHECK);
	mWindowToolBar->Realize();
	mAuiManager.AddPane(mWindowToolBar, wxAuiPaneInfo().Name(wxT("WindowToolBar")).Caption(wxT("���ڹ�����")).ToolbarPane().Top());
	mAuiManager.Update();
}

void EditorFrame::CreateWindows()
{
	mMainViewerPanel = new MainViewerPanel(this, wxMINIMIZE_BOX | wxMAXIMIZE_BOX);
	mAuiManager.AddPane(mMainViewerPanel, wxAuiPaneInfo().Name("MainViewerPanel").BestSize(wxSize(500, -1)).FloatingSize(400, 600).Caption("��������").Center());
	mMainViewerPanel->Show();
	mWindowIDList.insert(std::make_pair(mMainViewerPanel, ID_TOOL_MAIN_VIEWER_PANEL));

	mHistoryDatePanel = new HistoryDatePanel(this, wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxMINIMIZE | wxMAXIMIZE);
	mAuiManager.AddPane(mHistoryDatePanel, wxAuiPaneInfo().Name("HistoryDatePanel").BestSize(wxSize(300, -1)).FloatingSize(400, 600).Caption("��ʷ����").Left());
	mHistoryDatePanel->Show();
	mWindowIDList.insert(std::make_pair(mHistoryDatePanel, ID_TOOL_HISTROY_DATE_PANEL));

	mAuiManager.Update();
}

void EditorFrame::CreateStatuBar()
{
	std::map<wxWindow*, int>::iterator iterWindow = mWindowIDList.begin();
	std::map<wxWindow*, int>::iterator iterWindowEnd = mWindowIDList.end();
	for (; iterWindow != iterWindowEnd; ++iterWindow)
	{
		bool check = iterWindow->first->IsShown();
		mWindowToolBar->ToggleTool(iterWindow->second, check);
	}
}

void EditorFrame::UpdateStatus()
{
	;
}

void EditorFrame::CreateEditorCore()
{
	mDataBase = TRACE_NEW(DataBase, mDataBase);
	mDataBase->init(true);
}

void EditorFrame::RefreshAllResource()
{
	mHistoryDatePanel->showHistoryDate();
}

void EditorFrame::RefreshAllMenuToolCheckState()
{
	;
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

void EditorFrame::errorMessage(const std::string& str)
{
	wxMessageBox(str);
}

void EditorFrame::notifyWindowShow(wxWindow* window, const bool& show)
{
	std::map<wxWindow*, int>::const_iterator iterWindow = mWindowIDList.find(window);
	if (iterWindow == mWindowIDList.end())
	{
		return;
	}
	mWindowToolBar->ToggleTool(iterWindow->second, show);
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

void EditorFrame::OnPanelShow(wxCommandEvent& event)
{
	int eventID = event.GetId();
	std::map<wxWindow*, int>::iterator iterPanel = mWindowIDList.begin();
	std::map<wxWindow*, int>::iterator iterPanelEnd = mWindowIDList.end();
	for (; iterPanel != iterPanelEnd; ++iterPanel)
	{
		if (iterPanel->second == eventID)
		{
			if (mAuiManager.GetPane(iterPanel->first).IsShown() != event.IsChecked())
			{
				mAuiManager.GetPane(iterPanel->first).Show(event.IsChecked());
				mAuiManager.Update();
			}
			break;
		}
	}
}