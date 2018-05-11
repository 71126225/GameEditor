#include <windows.h>

#include "EditorFrame.h"
#include "EditorApp.h"
#include "MemeryViewer.h"
#include "MemeryDetailWindow.h"
#include "MemeryTypeStatisticsWindow.h"

enum
{
	ID_TIMER,
};

BEGIN_EVENT_TABLE(EditorFrame, wxFrame)

EVT_TIMER(ID_TIMER, OnTimer)
EVT_CLOSE(OnCloseWindow)

END_EVENT_TABLE()

EditorFrame::EditorFrame(wxString title, wxSize size)
:
wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size),
mMemeryViewer(NULL),
mTimer(NULL),
mMemeryDetailWindow(NULL),
mMemeryTypeStatisticsWindow(NULL)
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
	mTimer = new wxTimer();
	mTimer->Start(1);
	mTimer->SetOwner(this, ID_TIMER);
}

void EditorFrame::destroy()
{
	if (mTimer != NULL)
	{
		delete mTimer;
		mTimer = NULL;
	}
	mAuiManager.UnInit();
	if (mMemeryDetailWindow != NULL)
	{
		delete mMemeryDetailWindow;
		mMemeryDetailWindow = NULL;
	}
	if (mMemeryTypeStatisticsWindow != NULL)
	{
		delete mMemeryTypeStatisticsWindow;
		mMemeryTypeStatisticsWindow = NULL;
	}
	if (mMemeryViewer != NULL)
	{
		delete mMemeryViewer;
		mMemeryViewer = NULL;
	}
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
	mAuiManager.Update();
}

void EditorFrame::CreateWindows()
{
	mMemeryDetailWindow = new MemeryDetailWindow(this, wxMINIMIZE_BOX | wxMAXIMIZE_BOX);
	mMemeryDetailWindow->init(mMemeryViewer);
	mAuiManager.AddPane(mMemeryDetailWindow, wxAuiPaneInfo().Name("MemeryDetailWindow").BestSize(wxSize(500, -1)).FloatingSize(400, 600).Caption("�ڴ���ϸ��Ϣ").Left().Dockable(true));
	mMemeryDetailWindow->Show();

	mMemeryTypeStatisticsWindow = new MemeryTypeStatisticsWindow(this, wxMINIMIZE_BOX | wxMAXIMIZE_BOX, wxSize(700, -1));
	mMemeryTypeStatisticsWindow->init(mMemeryViewer);
	mAuiManager.AddPane(mMemeryTypeStatisticsWindow, wxAuiPaneInfo().Name("MemeryTypeStatisticsWindow").FloatingSize(500, 600).Caption("�ڴ�����ͳ����Ϣ").Right().Dockable(true));
	mMemeryTypeStatisticsWindow->Show();

	mAuiManager.Update();
}

void EditorFrame::CreateStatuBar()
{
	CreateStatusBar(2);
	SetStatusText("camera pos : 0.0, 0.0, 0.0", 0);
	SetStatusText("camera dir : 0.0, 0.0, 0.0", 0);
}

void EditorFrame::UpdateStatus()
{
	;
}

void EditorFrame::CreateEditorCore()
{
	mMemeryViewer = new MemeryViewer();
}

void EditorFrame::RefreshAllResource()
{
	;
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
	if (mMemeryDetailWindow != NULL)
	{
		mMemeryDetailWindow->update(elapsedTime);
	}
	if (mMemeryTypeStatisticsWindow != NULL)
	{
		mMemeryTypeStatisticsWindow->update(elapsedTime);
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