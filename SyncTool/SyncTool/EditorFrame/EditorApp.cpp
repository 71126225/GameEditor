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
	HWND hwnd = GetForegroundWindow();//ֱ�ӻ��ǰ�����ڵľ��         
	SendMessageA(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICON1)));
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
	std::string gymID = txUtility::getRegistryValue("Software\\����̤����ʿƼ���չ���޹�˾\\���ܶ���4.0", "REGISTE_CODE");
	if (gymID == "")
	{
		wxMessageBox("�Ҳ����豸��ע����Ϣ!");
		return false;
	}
	// ͨ���������г���
	if (argc == 3)
	{
		// ʹ���������ļ������ϴ�
		if (std::string(argv[1]) == "upload")
		{
			mBusyInfo = TRACE_NEW(EditorBusyInfo, mBusyInfo);
			mBusyInfo->hide();
			wxSize size(600, 400);
			wxPoint pos((1920 - size.x) / 2, (1080 - size.x) / 2);
			mMainFrame = new EditorFrame(this, "�ϴ��ļ�", pos, size);
			mMainFrame->init();
			mMainFrame->Show(true);
			mMainFrame->setAutoStartGame(false);
			mMainFrame->setAutoCloseUploaded(true);
			txVector<std::pair<std::string, std::string>> fileList;
			std::string str = gymID + "/" + txUtility::getDateTimeString();
			fileList.push_back(std::make_pair(std::string(argv[2]), str));
			mMainFrame->start(fileList);
		}
		return true;
	}
	else
	{
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
		mMainFrame = new EditorFrame(this, "�汾����", pos, size);
		mMainFrame->init();
		mMainFrame->Show(false);

		// �жϵ�ǰĿ¼���Ƿ�����Ҫ�ϴ����ļ�
		// ֻ�ϴ������������ļ�,�ļ�������ʽ20XX-XX-XX_XXXXX
		txVector<std::string> folders;
		FileUtility::findFolders("./", folders);
		FOR_STL(folders, int i = 0; i < folders.size(); )
		{
			// �Ƴ�./
			folders[i] = folders[i].substr(strlen("./"), folders[i].length() - strlen("./"));
			// �����������ļ��б���,������Ƴ�
			if (folders[i].length() > 11 && StringUtility::startWith(folders[i], "20") && folders[i][4] == '-' && folders[i][7] == '-' && folders[i][10] == '_')
			{
				++i;
			}
			else
			{
				folders.erase(folders.begin() + i, false);
			}
		}
		END_FOR_STL(folders);
		// ����������Ҫ�ϴ����ļ�
		mMainFrame->setAutoCloseUploaded(false);
		mMainFrame->setAutoStartGame(true);
		txVector<std::string> files;
		int count = folders.size();
		FOR_STL(folders, int i = 0; i < count; ++i)
		{
			FileUtility::findFiles(folders[i], files);
		}
		END_FOR_STL(folders);
		txVector<std::pair<std::string, std::string>> uploadFiles;
		int uploadCount = files.size();
		FOR_STL(files, int i = 0; i < uploadCount; ++i)
		{
			std::string path = gymID + "/" + StringUtility::getParentDir(files[i]);
			uploadFiles.push_back(std::make_pair(files[i], path));
		}
		END_FOR_STL(files);
		mMainFrame->start(uploadFiles);
		return true;
	}
}