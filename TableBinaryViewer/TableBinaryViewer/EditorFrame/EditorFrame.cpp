#include "EditorFrame.h"
#include "EditorApp.h"
#include "MainViewerWindow.h"
#include "TableBinaryResourceWindow.h"
#include "TableTxtResourceWindow.h"
#include "SupportedTableWindow.h"
#include "DataBase.h"

enum
{
	ID_TIMER,
	ID_MENU_SAVE,
	ID_TOOL_SAVE,
	ID_EDIT_TABLE
};

BEGIN_EVENT_TABLE(EditorFrame, wxFrame)

EVT_TIMER(ID_TIMER, OnTimer)
EVT_CLOSE(OnCloseWindow)
EVT_MENU(ID_MENU_SAVE, OnSave)
EVT_TOOL(ID_TOOL_SAVE, OnSave)
EVT_TOOL(ID_EDIT_TABLE, OnEditTable)

END_EVENT_TABLE()

EditorFrame::EditorFrame(wxString title, wxSize size)
:
wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size),
mTimer(NULL),
mDataBase(NULL),
mMainViewerWindow(NULL),
mTableBinaryResourceWindow(NULL),
mTableTxtResourceWindow(NULL),
mSupportedTableWindow(NULL),
mAppNormalName(title),
mDataModified(false)
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
	if (isModified())
	{
		int ret = wxMessageBox(wxT("�Ƿ񱣴浱ǰ���?"), "��ʾ", wxCANCEL | wxYES | wxNO);
		if (ret == wxYES)
		{
			saveData();
		}
		else if (ret == wxNO)
		{
			;
		}
		else if (ret == wxCANCEL)
		{
			return;
		}
	}
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
	mFileMenu->Append(ID_MENU_SAVE, wxT("������\tCtrl-S"));
	menuBar->Append(mFileMenu, "�ļ�");

	SetMenuBar(menuBar);
}

void EditorFrame::CreateToolBar()
{
	mFileToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORIZONTAL);
	mFileToolBar->SetToolBitmapSize(wxSize(16, 16));
	mFileToolBar->AddTool(ID_TOOL_SAVE, wxT("������"), wxBITMAP(IDB_SAVE), wxT("������"));
	mFileToolBar->AddTool(ID_EDIT_TABLE, wxT("��Ʊ��"), wxBITMAP(IDB_EDITTABLE), wxT("��Ʊ��"));
	mFileToolBar->Realize();
	mAuiManager.AddPane(mFileToolBar, wxAuiPaneInfo().Name(wxT("FileToolBar")).Caption(wxT("�ļ�������")).ToolbarPane().Top());
	mAuiManager.Update();
}

void EditorFrame::CreateWindows()
{
	mMainViewerWindow = new MainViewerWindow(this, wxMINIMIZE_BOX | wxMAXIMIZE_BOX);
	mAuiManager.AddPane(mMainViewerWindow, wxAuiPaneInfo().Name("MainViewerWindow").BestSize(wxSize(500, -1)).FloatingSize(400, 600).Caption("��������").Center());
	mMainViewerWindow->Show();

	mTableBinaryResourceWindow = new TableBinaryResourceWindow(this, wxPoint(-1, -1), wxSize(400, -1), wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxMINIMIZE | wxMAXIMIZE, wxT("TableBinaryResourceWindow"));
	mAuiManager.AddPane(mTableBinaryResourceWindow, wxAuiPaneInfo().Name("TableBinaryResourceWindow").BestSize(wxSize(300, -1)).FloatingSize(400, 600).Caption("�����������б�").Right());
	mTableBinaryResourceWindow->Show();

	mTableTxtResourceWindow = new TableTxtResourceWindow(this, wxPoint(-1, -1), wxSize(400, -1), wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxMINIMIZE | wxMAXIMIZE, wxT("TableTxtResourceWindow"));
	mAuiManager.AddPane(mTableTxtResourceWindow, wxAuiPaneInfo().Name("TableTxtResourceWindow").BestSize(wxSize(300, -1)).FloatingSize(400, 600).Caption("�ı������б�").Right());
	mTableTxtResourceWindow->Show();

	mSupportedTableWindow = new SupportedTableWindow(this, wxPoint(-1, -1), wxSize(400, -1), wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxMINIMIZE | wxMAXIMIZE, wxT("SupportedTableWindow"));
	mAuiManager.AddPane(mSupportedTableWindow, wxAuiPaneInfo().Name("SupportedTableWindow").BestSize(wxSize(300, -1)).FloatingSize(400, 600).Caption("֧�ֵı���ʽ").Left());
	mSupportedTableWindow->Show();

	mAuiManager.Update();
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
	mDataBase = TRACE_NEW(DataBase, mDataBase);
	mDataBase->setErrorCallback(EditorFrame::errorMessage);
	mDataBase->init(false);
}

void EditorFrame::RefreshAllResource()
{
	mTableBinaryResourceWindow->refresh();
	mTableTxtResourceWindow->refresh();
	mSupportedTableWindow->refresh();
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

void EditorFrame::OnSave(wxCommandEvent& event)
{
	saveData();
}

void EditorFrame::OnEditTable(wxCommandEvent& event)
{
	if (mTableName != "")
	{
		mSupportedTableWindow->editTable(const_cast<DataTemplate*>(mDataBase->getDataTemplate(mTableName)));
	}
}

void EditorFrame::notifyOpenTable(std::string tableName)
{
	mTableName = tableName;
	refreshTitle();
}

void EditorFrame::setDataModified(bool modified)
{
	mDataModified = modified;
	refreshTitle();
}

void EditorFrame::saveData()
{
	mDataBase->writeTemplateConfig(mTableName);
	mDataBase->writeBinaryFile(mTableName);
	setDataModified(false);
}

void EditorFrame::refreshTitle()
{
	if (mDataModified)
	{
		SetTitle(mAppNormalName + " - " + mTableName + " *");
	}
	else
	{
		SetTitle(mAppNormalName + " - " + mTableName);
	}
}