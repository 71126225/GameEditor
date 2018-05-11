#include "EditorApp.h"
#include "EditorFrame.h"
#include "EditorBusyInfo.h"
#include "resource.h"
#include "FileUtility.h"
#include "StringUtility.h"
#include "txUtility.h"
#include "EditorCore.h"

IMPLEMENT_APP(EditorApp)

BEGIN_EVENT_TABLE(EditorApp, wxApp)
EVT_KEY_DOWN(OnKeyDown)
EVT_KEY_UP(OnKeyUp)
EVT_MOUSEWHEEL(OnMouseWheel)

END_EVENT_TABLE()


EditorApp::EditorApp()
:
mBusyInfo(NULL),
mMainFrame(NULL)
{
	;
}

EditorApp::~EditorApp()
{
	mMainFrame = NULL;
}

void EditorApp::destroyBusyInfo()
{
	TRACE_DELETE(mBusyInfo);
}

void EditorApp::OnKeyDown(wxKeyEvent& event)
{
	event.Skip();
}

void EditorApp::OnKeyUp(wxKeyEvent& event)
{
	event.Skip();
}

void EditorApp::OnMouseWheel(wxMouseEvent& event)
{
	event.Skip();
}

bool EditorApp::OnInit()
{
	std::string GAME_NAME = "dndl4.4.exe";
	HWND hwnd = GetForegroundWindow();//ֱ�ӻ��ǰ�����ڵľ��         
	SendMessageA(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICON)));

	// �жϵ�ǰ�Ƿ��Ѿ������˸���Ϸ,����Ѿ�������ֱ���˳�
	txSet<std::string> processList;
	txUtility::getAllProcess(processList);  
	if (processList.find(GAME_NAME) != processList.end())
	{
		MessageBoxA(NULL, "��Ϸ�Ѿ�����!", "��ʾ", MB_OK);
		return false;
	}

	mBusyInfo = TRACE_NEW(EditorBusyInfo, mBusyInfo);
	mBusyInfo->hide();
	wxSize size(600, 400);
	wxPoint pos((1920 - size.x) / 2, (1080 - size.x) / 2);
	mMainFrame = new EditorFrame(this, GAME_NAME, "�汾���", pos, size);
	mMainFrame->init();
	mMainFrame->Show(true);
	// ����������Ҫ�ϴ����ļ�
	mMainFrame->startCheckVersion();
	return true;
}