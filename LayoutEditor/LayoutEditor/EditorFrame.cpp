#include "txEngineRoot.h"
#include "txRenderWindow.h"
#include "txEngineEventSystem.h"
#include "txProperty.h"
#include "txRenderWindow.h"

#include "Layout.h"
#include "WindowHeader.h"
#include "LayoutUtility.h"
#include "WindowProperty.h"
#include "LayoutManager.h"

#include "EditorCoreRoot.h"
#include "CoreEventSystem.h"
#include "ECUndoManager.h"
#include "WindowHeader.h"
#include "LayoutEditorCore.h"

#include "EditorCommandHeader.h"
#include "EditorFrame.h"
#include "PropertyWindow.h"
#include "WindowResource.h"
#include "RenderCanvas.h"
#include "EditorApp.h"
#include "EditorBusyInfo.h"
#include "LayoutHierarchy.h"
#include "InnerChildHierarchy.h"
#include "LayoutResource.h"
#include "LoadedLayout.h"
#include "PrefabResource.h"
#include "WindowTemplate.h"
#include "KeyFrameGraph.h"
#include "KeyFrameResource.h"
#include "ConnectWindowDialog.h"
#include "ComponentPropertyPanel.h"
#include "ComponentViewer.h"
#include "ActionTreeResource.h"
#include "ActionTreeGraph.h"
#include "SymmetryWindowDialog.h"
#include "ActionPropertyPanel.h"

enum
{
	ID_TIMER,
	ID_MENU_NEW_FILE,
	ID_MENU_SAVE_FILE,
	ID_MENU_CLOSE_FILE,
	ID_MENU_EXIT,
	ID_MENU_UNDO,
	ID_MENU_REDO,
	ID_MENU_COPY,
	ID_MENU_CUT,
	ID_MENU_PASTE,
	ID_MENU_PICKUP_USED_RESOURCES,
	ID_MENU_PANEL_PROPERTY,
	ID_MENU_PANEL_WINDOW_RESOURCE,
	ID_MENU_PANEL_PREFAB_RESOURCE,
	ID_MENU_PANEL_LAYOUT_RESOURCE,
	ID_MENU_PANEL_LOADED_LAYOUT,
	ID_MENU_PANEL_LAYOUT_HIERARCHY,
	ID_MENU_PANEL_INNER_CHILD_HIERARCHY,
	ID_MENU_PANEL_WINDOW_TEMPLATE,
	ID_MENU_PANEL_KEY_FRAME_GRAPH,
	ID_MENU_PANEL_KEY_FRAME_RESOURCE,
	ID_MENU_PANEL_RENDER,
	ID_MENU_PANEL_COMPONENT_VIEWER,
	ID_MENU_PANEL_COMPONENT_PROPERTY,
	ID_MENU_PANEL_ACTION_TREE_RESOURCE,
	ID_MENU_PANEL_ACTION_TREE_GRAPH,
	ID_MENU_PANEL_ACTION_PROPERTY,
	ID_MENU_RESOLUTION_0,
	ID_MENU_LOAD_PERSPECTIVE = ID_MENU_RESOLUTION_0 + MAX_RESOLUTION_COUNT,
	ID_MENU_SAVE_PERSPECTIVE,
	ID_MENU_WINDOW_POSITION_CENTER,
	ID_MENU_WINDOW_POSITION_LEFT_TOP,
	ID_MENU_WINDOW_POSITION_LEFT_DOWN,
	ID_MENU_WINDOW_POSITION_RIGHT_TOP,
	ID_MENU_WINDOW_POSITION_RIGHT_DOWN,
	ID_MENU_WINDOW_POSITION_TOP_CENTER,
	ID_MENU_WINDOW_POSITION_BOTTOM_CENTER,
	ID_MENU_WINDOW_POSITION_LEFT_CENTER,
	ID_MENU_WINDOW_POSITION_RIGHT_CENTER,
	ID_MENU_WINDOW_POSITION_FILL,
	ID_TOOL_NEW_FILE,
	ID_TOOL_SAVE_FILE,
	ID_TOOL_UNDO,
	ID_TOOL_REDO,
	ID_TOOL_COPY,
	ID_TOOL_CUT,
	ID_TOOL_PASTE,
	ID_TOOL_PANEL_PROPERTY,
	ID_TOOL_PANEL_WINDOW_RESOURCE,
	ID_TOOL_PANEL_PREFAB_RESOURCE,
	ID_TOOL_PANEL_LAYOUT_RESOURCE,
	ID_TOOL_PANEL_LOADED_LAYOUT,
	ID_TOOL_PANEL_LAYOUT_HIERARCHY,
	ID_TOOL_PANEL_INNER_CHILD_HIERARCHY,
	ID_TOOL_PANEL_WINDOW_TEMPLATE,
	ID_TOOL_PANEL_KEY_FRAME_GRAPH,
	ID_TOOL_PANEL_KEY_FRAME_RESOURCE,
	ID_TOOL_PANEL_RENDER,
	ID_TOOL_PANEL_COMPONENT_VIEWER,
	ID_TOOL_PANEL_COMPONENT_PROPERTY,
	ID_TOOL_PANEL_ACTION_TREE_RESOURCE,
	ID_TOOL_PANEL_ACTION_TREE_GRAPH,
	ID_TOOL_PANEL_ACTION_PROPERTY,
	ID_TOOL_SHOW_WIREFRAME,
	ID_TOOL_SHOW_SELECTWINDOW_COORDINATE,
	ID_TOOL_SHOW_RENDERWINDOW_COORDINATE,
	ID_TOOL_WINDOW_CENTER,
	ID_TOOL_WINDOW_LEFT_TOP,
	ID_TOOL_WINDOW_LEFT_DOWN,
	ID_TOOL_WINDOW_LEFT_CENTER,
	ID_TOOL_WINDOW_RIGHT_TOP,
	ID_TOOL_WINDOW_RIGHT_DOWN,
	ID_TOOL_WINDOW_RIGHT_CENTER,
	ID_TOOL_WINDOW_TOP_CENTER,
	ID_TOOL_WINDOW_BOTTOM_CENTER,
	ID_TOOL_WINDOW_FILL,
	ID_TOOL_WINDOW_POS_UP,
	ID_TOOL_WINDOW_POS_DOWN,
	ID_TOOL_WINDOW_POS_TOP,
	ID_TOOL_WINDOW_POS_BOTTOM,
	ID_TOOL_SCREEN_DENSITY_TEXT,
	ID_TOOL_DOCK_RENDER_WINDOW,
	ID_TOOL_CONNECT_WINDOW,
	ID_TOOL_SYMMETRY_WINDOW,
	ID_TOOL_DIRECT_VERTICES,
	ID_TOOL_BACKGROUND_COLOUR,
	ID_TOOL_SELECT_INVISIBLE,
	ID_TOOL_SELECT_UN_HANDLE_INPUT,
	ID_TOOL_NUMBER_RECT,
};

BEGIN_EVENT_TABLE(EditorFrame, wxFrame)

EVT_TIMER(ID_TIMER, OnTimer)
EVT_CLOSE(OnCloseWindow)

EVT_MENU(ID_MENU_NEW_FILE, OnNewFile)
EVT_MENU(ID_MENU_SAVE_FILE, OnSaveFile)
EVT_MENU(ID_MENU_CLOSE_FILE, OnCloseFile)
EVT_MENU(ID_MENU_EXIT, OnExit)
EVT_MENU(ID_MENU_UNDO, OnUndo)
EVT_MENU(ID_MENU_REDO, OnRedo)
EVT_MENU(ID_MENU_COPY, OnCopy)
EVT_MENU(ID_MENU_CUT, OnCut)
EVT_MENU(ID_MENU_PASTE, OnPaste)
EVT_MENU(ID_MENU_PICKUP_USED_RESOURCES, OnPickupUsedResources)
EVT_MENU(ID_MENU_PANEL_PROPERTY, OnPanelShow)
EVT_MENU(ID_MENU_PANEL_WINDOW_RESOURCE, OnPanelShow)
EVT_MENU(ID_MENU_PANEL_PREFAB_RESOURCE, OnPanelShow)
EVT_MENU(ID_MENU_PANEL_LAYOUT_RESOURCE, OnPanelShow)
EVT_MENU(ID_MENU_PANEL_LOADED_LAYOUT, OnPanelShow)
EVT_MENU(ID_MENU_PANEL_LAYOUT_HIERARCHY, OnPanelShow)
EVT_MENU(ID_MENU_PANEL_WINDOW_TEMPLATE, OnPanelShow)
EVT_MENU(ID_MENU_PANEL_RENDER, OnPanelShow)
EVT_MENU(ID_MENU_PANEL_INNER_CHILD_HIERARCHY, OnPanelShow)
EVT_MENU(ID_MENU_PANEL_KEY_FRAME_GRAPH, OnPanelShow)
EVT_MENU(ID_MENU_PANEL_KEY_FRAME_RESOURCE, OnPanelShow)
EVT_MENU(ID_MENU_PANEL_COMPONENT_PROPERTY, OnPanelShow)
EVT_MENU(ID_MENU_PANEL_COMPONENT_VIEWER, OnPanelShow)
EVT_MENU(ID_MENU_PANEL_ACTION_TREE_GRAPH, OnPanelShow)
EVT_MENU(ID_MENU_PANEL_ACTION_TREE_RESOURCE, OnPanelShow)
EVT_MENU(ID_MENU_PANEL_ACTION_PROPERTY, OnPanelShow)
EVT_MENU(ID_MENU_LOAD_PERSPECTIVE, OnLoadPerspective)
EVT_MENU(ID_MENU_SAVE_PERSPECTIVE, OnSavePerspective)
EVT_MENU(ID_MENU_WINDOW_POSITION_CENTER, OnWindowPositinCenter)
EVT_MENU(ID_MENU_WINDOW_POSITION_LEFT_TOP, OnWindowPositionLeftTop)
EVT_MENU(ID_MENU_WINDOW_POSITION_LEFT_DOWN, OnwindowPositionLeftDown)
EVT_MENU(ID_MENU_WINDOW_POSITION_RIGHT_TOP, OnWindowPositionRightTop)
EVT_MENU(ID_MENU_WINDOW_POSITION_RIGHT_DOWN, OnwindowPositionRightDown)
EVT_MENU(ID_MENU_WINDOW_POSITION_TOP_CENTER, OnWindowPositionTopCenter)
EVT_MENU(ID_MENU_WINDOW_POSITION_BOTTOM_CENTER, OnWindowPositionBottomCenter)
EVT_MENU(ID_MENU_WINDOW_POSITION_LEFT_CENTER, OnWindowPositionLeftCenter)
EVT_MENU(ID_MENU_WINDOW_POSITION_RIGHT_CENTER, OnWindowPositionRightCenter)
EVT_MENU(ID_MENU_WINDOW_POSITION_FILL, OnWindowPositionFill)
EVT_TOOL(ID_TOOL_NEW_FILE, OnNewFile)
EVT_TOOL(ID_TOOL_SAVE_FILE, OnSaveFile)
EVT_TOOL(ID_TOOL_UNDO, OnUndo)
EVT_TOOL(ID_TOOL_REDO, OnRedo)
EVT_TOOL(ID_TOOL_COPY, OnCopy)
EVT_TOOL(ID_TOOL_CUT, OnCut)
EVT_TOOL(ID_TOOL_PASTE, OnPaste)
EVT_TOOL(ID_TOOL_PANEL_PROPERTY, OnPanelShow)
EVT_TOOL(ID_TOOL_PANEL_WINDOW_RESOURCE, OnPanelShow)
EVT_TOOL(ID_TOOL_PANEL_PREFAB_RESOURCE, OnPanelShow)
EVT_TOOL(ID_TOOL_PANEL_LAYOUT_RESOURCE, OnPanelShow)
EVT_TOOL(ID_TOOL_PANEL_LOADED_LAYOUT, OnPanelShow)
EVT_TOOL(ID_TOOL_PANEL_LAYOUT_HIERARCHY, OnPanelShow)
EVT_TOOL(ID_TOOL_PANEL_WINDOW_TEMPLATE, OnPanelShow)
EVT_TOOL(ID_TOOL_PANEL_RENDER, OnPanelShow)
EVT_TOOL(ID_TOOL_PANEL_INNER_CHILD_HIERARCHY, OnPanelShow)
EVT_TOOL(ID_TOOL_PANEL_KEY_FRAME_GRAPH, OnPanelShow)
EVT_TOOL(ID_TOOL_PANEL_KEY_FRAME_RESOURCE, OnPanelShow)
EVT_TOOL(ID_TOOL_PANEL_COMPONENT_PROPERTY, OnPanelShow)
EVT_TOOL(ID_TOOL_PANEL_COMPONENT_VIEWER, OnPanelShow)
EVT_TOOL(ID_TOOL_PANEL_ACTION_TREE_GRAPH, OnPanelShow)
EVT_TOOL(ID_TOOL_PANEL_ACTION_TREE_RESOURCE, OnPanelShow)
EVT_TOOL(ID_TOOL_PANEL_ACTION_PROPERTY, OnPanelShow)
EVT_TOOL(ID_TOOL_SHOW_WIREFRAME, OnWireframeVisible)
EVT_TOOL(ID_TOOL_SHOW_RENDERWINDOW_COORDINATE, OnShowRenderWindowCoordinate)
EVT_TOOL(ID_TOOL_SHOW_SELECTWINDOW_COORDINATE, OnShowWindowCoordinate)
EVT_TOOL(ID_TOOL_WINDOW_CENTER, OnWindowPositinCenter)
EVT_TOOL(ID_TOOL_WINDOW_LEFT_TOP, OnWindowPositionLeftTop)
EVT_TOOL(ID_TOOL_WINDOW_LEFT_DOWN, OnwindowPositionLeftDown)
EVT_TOOL(ID_TOOL_WINDOW_RIGHT_TOP, OnWindowPositionRightTop)
EVT_TOOL(ID_TOOL_WINDOW_RIGHT_DOWN, OnwindowPositionRightDown)
EVT_TOOL(ID_TOOL_WINDOW_TOP_CENTER, OnWindowPositionTopCenter)
EVT_TOOL(ID_TOOL_WINDOW_BOTTOM_CENTER, OnWindowPositionBottomCenter)
EVT_TOOL(ID_TOOL_WINDOW_LEFT_CENTER, OnWindowPositionLeftCenter)
EVT_TOOL(ID_TOOL_WINDOW_RIGHT_CENTER, OnWindowPositionRightCenter)
EVT_TOOL(ID_TOOL_WINDOW_FILL, OnWindowPositionFill)
EVT_TOOL(ID_TOOL_WINDOW_POS_UP, OnWindowPosMoveUp)
EVT_TOOL(ID_TOOL_WINDOW_POS_DOWN, OnWindowPosMoveDown)
EVT_TOOL(ID_TOOL_WINDOW_POS_TOP, OnWindowPosMoveTop)
EVT_TOOL(ID_TOOL_WINDOW_POS_BOTTOM, OnWindowPosMoveBottom)
EVT_TEXT(ID_TOOL_SCREEN_DENSITY_TEXT, OnScreenDensity)
EVT_TOOL(ID_TOOL_DOCK_RENDER_WINDOW, OnDockRenderWindow)
EVT_TOOL(ID_TOOL_CONNECT_WINDOW, OnConnectWindow)
EVT_TOOL(ID_TOOL_SYMMETRY_WINDOW, OnSymmetryWindow)
EVT_TOOL(ID_TOOL_DIRECT_VERTICES, OnDirectVertices)
EVT_COLOURPICKER_CHANGED(ID_TOOL_BACKGROUND_COLOUR, OnBackgroundColour)
EVT_TOOL(ID_TOOL_SELECT_INVISIBLE, OnSelectInvisible)
EVT_TOOL(ID_TOOL_SELECT_UN_HANDLE_INPUT, OnSelectUnHandleInput)
EVT_TOOL(ID_TOOL_NUMBER_RECT, OnShowNumberRect)

END_EVENT_TABLE()

EditorFrame::EditorFrame(const wxString& title, const wxSize& size)
:
wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size),
txCommandReceiver("EditorFrame"),
mPropertyWindow(NULL),
mWindowResource(NULL),
mRenderCanvas(NULL),
mEditorCoreRoot(NULL),
mAppNormalName(title.ToStdString()),
mResolutionMenu(NULL),
mWindowPosition(NULL),
mWindowTemplate(NULL),
mKeyFrameGraph(NULL),
mInnerChildHierarchy(NULL)
{
	// ���ֱ���ֻ���ǵ�ǰ��ʾ�������ֱ���,1920*1080
	mResolutionSize.push_back(std::make_pair(wxSize(1920, 1080), wxSize(16, 9)));
	mResolutionSize.push_back(std::make_pair(wxSize(1600, 900), wxSize(16, 9)));
	mResolutionSize.push_back(std::make_pair(wxSize(1280, 720), wxSize(16, 9)));
	mResolutionSize.push_back(std::make_pair(wxSize(1024, 768), wxSize(4, 3)));
	mResolutionSize.push_back(std::make_pair(wxSize(1024, 576), wxSize(16, 9)));
	mResolutionSize.push_back(std::make_pair(wxSize(800, 600), wxSize(4, 3)));
}

EditorFrame::~EditorFrame()
{
	wxGetApp().destroyBusyInfo();
}

void EditorFrame::init()
{
	setup();
	mTimer = TRACE_NEW(wxTimer, mTimer);
	mTimer->Start(20);
	mTimer->SetOwner(this, ID_TIMER);
}

void EditorFrame::destroy()
{
	if (mLayoutEditorCore->isLayoutModified())
	{
		// �����޷�ȡ��,ֻ��ѡ�񱣴���߲�����,���Ҹ���ʾ��д���༭������
		int ret = wxMessageBox(wxT("�ļ�δ����,�Ƿ񱣴��ļ�"), wxT("��ʾ"), wxYES_NO);
		if (ret == wxYES)
		{
			mLayoutEditorCore->saveLayout();
		}
	}

	TRACE_DELETE(mTimer);
	unregisterEngineEventHandler();
	unregisterEditorCoreEventHandler();
	TRACE_DELETE(mEditorCoreRoot);
	mAuiManager.UnInit();
}

void EditorFrame::registerEngineEventHandler()
{
	mEngineEventSystem->registeAllEvent(this);
}

void EditorFrame::unregisterEngineEventHandler()
{
	mEngineEventSystem->unregisteAllEvent(this);
}

void EditorFrame::registerEditorCoreEventHandler()
{
	mCoreEventSystem->registeAllEvent(this);
}

void EditorFrame::unregisterEditorCoreEventHandler()
{
	mCoreEventSystem->unregisteAllEvent(this);
}

void EditorFrame::setup()
{
	mAuiManager.SetManagedWindow(this);
	// �����˵���
	createMenu();
	// ����������
	createToolBar();
	// ������Ⱦ����,��Ϊ�ڴ����༭����ʱ��Ҫһ����Ⱦ���ھ��
	createRenderWindow();
	// �����༭����
	createEditorCore();
	// ���������Ӵ���
	createWindows();
	// �����ײ���״̬��ʾ��
	createStatuBar();
	// �������ˢ��һ��ȫ���ؼ���ѡ��״̬
	refreshAllMenuToolCheckState();
	// ˢ��ȫ����Դ�б�
	refreshAllResource();
	// ����Ĭ�ϱ༭������
	loadPerspective("default");
}

void EditorFrame::createMenu()
{
	wxMenuBar* menuBar = new wxMenuBar();

	// �ļ��˵�
	mFileMenu = new wxMenu();
	mFileMenu->Append(ID_MENU_NEW_FILE, wxT("�½��ļ�\tCtrl-N"));
	mFileMenu->Append(ID_MENU_SAVE_FILE, wxT("�����ļ�\tCtrl-S"));
	mFileMenu->Append(ID_MENU_CLOSE_FILE, wxT("�ر��ļ�"));
	mFileMenu->Append(ID_MENU_EXIT, wxT("�˳�"));
	menuBar->Append(mFileMenu, wxT("�ļ�"));

	// �༭�˵�
	mEditMenu = new wxMenu();
	mEditMenu->Append(ID_MENU_UNDO, wxT("����\tCtrl-Z"));
	mEditMenu->Append(ID_MENU_REDO, wxT("����\tCtrl-Y"));
	menuBar->Append(mEditMenu, wxT("�༭"));

	// ���߲˵�
	mToolMenu = new wxMenu();
	mToolMenu->Append(ID_MENU_PICKUP_USED_RESOURCES, wxT("��ȡ������ʹ����Դ"));
	menuBar->Append(mToolMenu, wxT("����"));

	// ���ڲ˵�
	mWindowMenu = new wxMenu();
	mWindowMenu->Append(ID_MENU_PANEL_PROPERTY, wxT("���Դ���"), wxT("��ʾ���Դ���"), wxITEM_CHECK);
	mWindowMenu->Append(ID_MENU_PANEL_WINDOW_RESOURCE, wxT("�������ʹ���"), wxT("��ʾ�������ʹ���"), wxITEM_CHECK);
	mWindowMenu->Append(ID_MENU_PANEL_PREFAB_RESOURCE, wxT("����Ԥ�贰��"), wxT("��ʾ����Ԥ�贰��"), wxITEM_CHECK);
	mWindowMenu->Append(ID_MENU_PANEL_LAYOUT_RESOURCE, wxT("������Դ����"), wxT("��ʾ������Դ����"), wxITEM_CHECK);
	mWindowMenu->Append(ID_MENU_PANEL_LOADED_LAYOUT, wxT("�Ѽ��ز��ִ���"), wxT("��ʾ�Ѽ��ز��ִ���"), wxITEM_CHECK);
	mWindowMenu->Append(ID_MENU_PANEL_LAYOUT_HIERARCHY, wxT("������ͼ����"), wxT("��ʾ������ͼ����"), wxITEM_CHECK);
	mWindowMenu->Append(ID_MENU_PANEL_INNER_CHILD_HIERARCHY, wxT("�ڲ��Ӵ�����ͼ����"), wxT("��ʾ�ڲ��Ӵ�����ͼ����"), wxITEM_CHECK);
	mWindowMenu->Append(ID_MENU_PANEL_WINDOW_TEMPLATE, wxT("����ģ�崰��"), wxT("��ʾ����ģ�崰��"), wxITEM_CHECK);
	mWindowMenu->Append(ID_MENU_PANEL_KEY_FRAME_GRAPH, wxT("�ؼ�֡�༭����"), wxT("��ʾ�ؼ�֡�༭����"), wxITEM_CHECK);
	mWindowMenu->Append(ID_MENU_PANEL_KEY_FRAME_RESOURCE, wxT("�ؼ�֡��Դ����"), wxT("��ʾ�ؼ�֡��Դ����"), wxITEM_CHECK);
	mWindowMenu->Append(ID_MENU_PANEL_RENDER, wxT("��Ⱦ����"), wxT("��ʾ��Ⱦ����"), wxITEM_CHECK);
	mWindowMenu->Append(ID_MENU_PANEL_COMPONENT_VIEWER, wxT("�����������"), wxT("��ʾ�����������"), wxITEM_CHECK);
	mWindowMenu->Append(ID_MENU_PANEL_COMPONENT_PROPERTY, wxT("������Դ���"), wxT("��ʾ������Դ���"), wxITEM_CHECK);
	mWindowMenu->Append(ID_MENU_PANEL_ACTION_TREE_GRAPH, wxT("��Ϊ���༭����"), wxT("��ʾ��Ϊ���༭����"), wxITEM_CHECK);
	mWindowMenu->Append(ID_MENU_PANEL_ACTION_TREE_RESOURCE, wxT("��Ϊ����Դ����"), wxT("��ʾ��Ϊ����Դ����"), wxITEM_CHECK);
	mWindowMenu->Append(ID_MENU_PANEL_ACTION_PROPERTY, wxT("��Ϊ���Դ���"), wxT("��ʾ��Ϊ���Դ���"), wxITEM_CHECK);
	menuBar->Append(mWindowMenu, wxT("����"));

	// ���ֲ˵�
	mLayoutMenu = new wxMenu(); 
	wxMenu* subLoadMenu = new wxMenu();
	wxMenuItem* menuLoadItem = new wxMenuItem(mLayoutMenu, ID_MENU_LOAD_PERSPECTIVE, wxT("���ؽ��沼��"), wxT("���ؽ��沼��"), wxITEM_NORMAL, subLoadMenu);
	mLayoutMenu->Append(menuLoadItem);
	//��������FrameLayout�ļ�
	loadAllPerspective(subLoadMenu);
	mLayoutMenu->Append(ID_MENU_SAVE_PERSPECTIVE, wxT("������沼��"));
	
	menuBar->Append(mLayoutMenu, wxT("����"));

	// �ֱ��ʲ˵�
	mResolutionMenu = new wxMenu();
	createResolutionMenu(mResolutionMenu);
	menuBar->Append(mResolutionMenu, wxT("�ֱ���"));

	// ����λ��
	mWindowPosition = new wxMenu();
	mWindowPosition->Append(ID_MENU_WINDOW_POSITION_CENTER, wxT("����"));
	mWindowPosition->Append(ID_MENU_WINDOW_POSITION_LEFT_TOP, wxT("���Ͻ�"));
	mWindowPosition->Append(ID_MENU_WINDOW_POSITION_LEFT_DOWN, wxT("���½�"));
	mWindowPosition->Append(ID_MENU_WINDOW_POSITION_RIGHT_TOP, wxT("���Ͻ�"));
	mWindowPosition->Append(ID_MENU_WINDOW_POSITION_RIGHT_DOWN, wxT("���½�"));
	mWindowPosition->Append(ID_MENU_WINDOW_POSITION_TOP_CENTER, wxT("�Ͼ���"));
	mWindowPosition->Append(ID_MENU_WINDOW_POSITION_BOTTOM_CENTER, wxT("�¾���"));
	mWindowPosition->Append(ID_MENU_WINDOW_POSITION_LEFT_CENTER, wxT("�����"));
	mWindowPosition->Append(ID_MENU_WINDOW_POSITION_RIGHT_CENTER, wxT("�Ҿ���"));
	mWindowPosition->Append(ID_MENU_WINDOW_POSITION_FILL, wxT("����"));
	menuBar->Append(mWindowPosition, wxT("����λ��"));

	SetMenuBar(menuBar);
}

void EditorFrame::createToolBar()
{
	// �ļ�������
	mFileToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORIZONTAL);
	mFileToolBar->SetToolBitmapSize(wxSize(16, 16));
	mFileToolBar->AddTool(ID_TOOL_NEW_FILE, wxT("�½��ļ�"), wxBITMAP(IDB_NEW), wxT("�½��ļ�"));
	mFileToolBar->AddTool(ID_TOOL_SAVE_FILE, wxT("�����ļ�"), wxBITMAP(IDB_SAVE), wxT("�����ļ�"));
	mFileToolBar->Realize();
	mAuiManager.AddPane(mFileToolBar, wxAuiPaneInfo().Name(wxT("FileToolBar")).Caption(wxT("�ļ�������")).ToolbarPane().Top());

	mEditToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORIZONTAL);
	mEditToolBar->SetToolBitmapSize(wxSize(16, 16));
	mEditToolBar->AddTool(ID_TOOL_UNDO, wxT("����"), wxBITMAP(IDB_UNDO), wxT("����"));
	mEditToolBar->AddTool(ID_TOOL_REDO, wxT("����"), wxBITMAP(IDB_REDO), wxT("����"));

	mEditToolBar->AddTool(ID_TOOL_WINDOW_CENTER, wxT("����"), wxBITMAP(IDB_WINDOW_CENTER), wxT("���ھ���"));
	mEditToolBar->AddTool(ID_TOOL_WINDOW_LEFT_TOP, wxT("����"), wxBITMAP(IDB_WINDOW_LEFT_TOP), wxT("�����ƶ������������Ͻ�"));
	mEditToolBar->AddTool(ID_TOOL_WINDOW_LEFT_DOWN, wxT("����"), wxBITMAP(IDB_WINDOW_LEFT_DWON), wxT("�����ƶ������������½�"));
	mEditToolBar->AddTool(ID_TOOL_WINDOW_LEFT_CENTER, wxT("����"), wxBITMAP(IDB_WINDOW_LEFT_CENTER), wxT("�����ƶ�������������м�"));
	mEditToolBar->AddTool(ID_TOOL_WINDOW_RIGHT_TOP, wxT("����"), wxBITMAP(IDB_WINDOW_RIGHT_TOP), wxT("�����ƶ������������Ͻ�"));
	mEditToolBar->AddTool(ID_TOOL_WINDOW_RIGHT_DOWN, wxT("����"), wxBITMAP(IDB_WINDOW_RIGHT_DWON), wxT("�����ƶ������������½�"));
	mEditToolBar->AddTool(ID_TOOL_WINDOW_RIGHT_CENTER, wxT("����"), wxBITMAP(IDB_WINDOW_RIGHT_CENTER), wxT("�����ƶ����������ұ��м�"));
	mEditToolBar->AddTool(ID_TOOL_WINDOW_TOP_CENTER, wxT("����"), wxBITMAP(IDB_WINDOW_TOP_CENTER), wxT("�����ƶ����������ϱ��м�"));
	mEditToolBar->AddTool(ID_TOOL_WINDOW_BOTTOM_CENTER, wxT("����"), wxBITMAP(IDB_WINDOW_DWON_CENTER), wxT("�����ƶ����������±��м�"));
	mEditToolBar->AddTool(ID_TOOL_WINDOW_FILL, wxT("����"), wxBITMAP(IDB_WINDOW_FILL_FULL), wxT("������������������"));

	mEditToolBar->AddTool(ID_TOOL_WINDOW_POS_UP, wxT("����"), wxBITMAP(IDB_WINDOW_POS_UP), wxT("��������һ��λ��"));
	mEditToolBar->AddTool(ID_TOOL_WINDOW_POS_DOWN, wxT("����"), wxBITMAP(IDB_WINDOW_POS_DOWN), wxT("��������һ��λ��"));
	mEditToolBar->AddTool(ID_TOOL_WINDOW_POS_TOP, wxT("�Ƶ�����"), wxBITMAP(IDB_WINDOW_POS_TOP), wxT("�����Ƶ����"));
	mEditToolBar->AddTool(ID_TOOL_WINDOW_POS_BOTTOM, wxT("�Ƶ��ײ�"), wxBITMAP(IDB_WINDOW_POS_BOTTOM), wxT("�����Ƶ���ײ�"));
	mEditToolBar->AddTool(ID_TOOL_CONNECT_WINDOW, wxT("������ת����"), wxBITMAP(IDB_CONNECT_WINDOW), wxT("��ת����ʹ�ô�������ѡ�е���������"));
	mEditToolBar->AddTool(ID_TOOL_SYMMETRY_WINDOW, wxT("���ڶԳ�"), wxBITMAP(IDB_SYMMETRY_WINDOW), wxT("���ô��ڹ�������һ�����ڶԳ�"));

	mEditToolBar->AddLabel(wxID_ANY, "��Ļ�ܶ�:");
	wxTextCtrl* densityText = new wxTextCtrl(mEditToolBar, ID_TOOL_SCREEN_DENSITY_TEXT);
	mEditToolBar->AddControl(densityText);

	mEditToolBar->AddTool(ID_TOOL_DOCK_RENDER_WINDOW, wxT("ͣ����Ⱦ����"), wxBITMAP(IDB_DOCK_RENDER_WINDOW), wxT("ͣ����Ⱦ����"));
	mEditToolBar->AddTool(ID_TOOL_DIRECT_VERTICES, wxT("�Ƿ�ֱ����Ⱦ����Ļ��"), wxBITMAP(IDB_DIRECT_VERTICES), wxT("�Ƿ�ֱ����Ⱦ����Ļ��"), wxITEM_CHECK);
	// ���øù��߰�ť,ֻ����ͶӰ��ʽ��Ⱦ
	mEditToolBar->EnableTool(ID_TOOL_DIRECT_VERTICES, false);

	mEditToolBar->Realize();
	mAuiManager.AddPane(mEditToolBar, wxAuiPaneInfo().Name(wxT("EditToolBar")).Caption(wxT("�༭������")).ToolbarPane().Top());

	mShowHideToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORIZONTAL);
	mShowHideToolBar->SetToolBitmapSize(wxSize(16, 16));
	mShowHideToolBar->AddTool(ID_TOOL_SHOW_WIREFRAME, wxT("��ʾ�߿�"), wxBITMAP(IDB_WIREFRAME_VISIBLE), wxT("��ʾ���ڰ�Χ�߿�"), wxITEM_CHECK);
	mShowHideToolBar->AddTool(ID_TOOL_SHOW_SELECTWINDOW_COORDINATE, wxT("��ʾ��ǰѡ�д�������ϵ"), wxBITMAP(IDB_WINDOW_COORDINATE), wxT("��ʾ��ǰѡ�д�������ϵ"), wxITEM_CHECK);
	mShowHideToolBar->AddTool(ID_TOOL_SHOW_RENDERWINDOW_COORDINATE, wxT("��ʾ��Ⱦ��������ϵ"), wxBITMAP(IDB_RENDERWINDOW_COORDINATE), wxT("��ʾ��Ⱦ��������ϵ"), wxITEM_CHECK);
	wxColourPickerCtrl* backgroundColour = new wxColourPickerCtrl(mShowHideToolBar, ID_TOOL_BACKGROUND_COLOUR, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
	mShowHideToolBar->AddControl(backgroundColour);
	mShowHideToolBar->Realize();
	mAuiManager.AddPane(mShowHideToolBar, wxAuiPaneInfo().Name(wxT("ShowToolBar")).Caption(wxT("��ʾ������")).ToolbarPane().Top());

	mWindowToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORIZONTAL);
	mWindowToolBar->SetToolBitmapSize(wxSize(16, 16));
	mWindowToolBar->AddTool(ID_TOOL_PANEL_PROPERTY, wxT("���Դ���"), wxBITMAP(IDB_PANEL_PROPERTY), wxT("��ʾ���Դ���"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_PANEL_WINDOW_RESOURCE, wxT("�������ʹ���"), wxBITMAP(IDB_PANEL_WINDOW_RESOURCE), wxT("��ʾ�������ʹ���"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_PANEL_PREFAB_RESOURCE, wxT("����Ԥ�贰��"), wxBITMAP(IDB_PANEL_PREFAB_RESOURCE), wxT("��ʾ����Ԥ�贰��"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_PANEL_LAYOUT_RESOURCE, wxT("�����ļ�����"), wxBITMAP(IDB_PANEL_LAYOUT_RESOURCE), wxT("��ʾ�����ļ�����"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_PANEL_LOADED_LAYOUT, wxT("�Ѽ��ز��ִ���"), wxBITMAP(IDB_PANEL_LOADED_LAYOUT), wxT("��ʾ�Ѽ��ز��ִ���"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_PANEL_LAYOUT_HIERARCHY, wxT("������ͼ����"), wxBITMAP(IDB_PANEL_LAYOUT_HIERARCHY), wxT("��ʾ������ͼ����"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_PANEL_INNER_CHILD_HIERARCHY, wxT("�ڲ��Ӵ�����ͼ����"), wxBITMAP(IDB_PANEL_INNER_CHILD), wxT("��ʾ�ڲ��Ӵ�����ͼ����"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_PANEL_WINDOW_TEMPLATE, wxT("����ģ�崰��"), wxBITMAP(IDB_PANEL_WINDOW_TEMPLATE), wxT("��ʾ����ģ�崰��"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_PANEL_RENDER, wxT("��Ⱦ����"), wxBITMAP(IDB_PANEL_RENDER), wxT("��ʾ��Ⱦ����"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_PANEL_KEY_FRAME_GRAPH, wxT("�ؼ�֡�༭����"), wxBITMAP(IDB_PANEL_KEY_FRAME_GRAPH), wxT("��ʾ�ؼ�֡�༭����"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_PANEL_KEY_FRAME_RESOURCE, wxT("�ؼ�֡��Դ����"), wxBITMAP(IDB_PANEL_KEY_FRAME_RESOURCE), wxT("��ʾ�ؼ�֡��Դ����"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_PANEL_COMPONENT_VIEWER, wxT("�����������"), wxBITMAP(IDB_PANEL_COMPONENT_VIEWER), wxT("��ʾ�����������"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_PANEL_COMPONENT_PROPERTY, wxT("������Դ���"), wxBITMAP(IDB_PANEL_COMPONENT_PROPERTY), wxT("��ʾ������Դ���"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_PANEL_ACTION_TREE_GRAPH, wxT("��Ϊ���༭����"), wxBITMAP(IDB_PANEL_ACTION_TREE_GRAPH), wxT("��ʾ��Ϊ���༭����"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_PANEL_ACTION_TREE_RESOURCE, wxT("��Ϊ����Դ����"), wxBITMAP(IDB_PANEL_ACTION_TREE_RESOURCE), wxT("��ʾ��Ϊ����Դ����"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_PANEL_ACTION_PROPERTY, wxT("��Ϊ���Դ���"), wxBITMAP(IDB_PANEL_ACTION_TREE_PROPERTY), wxT("��ʾ��Ϊ���Դ���"), wxITEM_CHECK);
	mWindowToolBar->Realize();
	mAuiManager.AddPane(mWindowToolBar, wxAuiPaneInfo().Name(wxT("WindowToolBar")).Caption(wxT("���ڹ�����")).ToolbarPane().Top());
	
	mSelectToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORIZONTAL);
	mSelectToolBar->SetToolBitmapSize(wxSize(16, 16));
	mSelectToolBar->AddTool(ID_TOOL_SELECT_INVISIBLE, wxT("����ѡ�����ش���"), wxBITMAP(IDB_SELECT_INVISIBLE), wxT("����ѡ�����ش���"), wxITEM_CHECK);
	mSelectToolBar->AddTool(ID_TOOL_SELECT_UN_HANDLE_INPUT, wxT("����ѡ�в����������¼�����"), wxBITMAP(IDB_SELECT_UN_HANDLE_INPUT), wxT("����ѡ�в����������¼�����"), wxITEM_CHECK);
	mSelectToolBar->AddTool(ID_TOOL_NUMBER_RECT, wxT("��ʾ���ֱ߿�"), wxBITMAP(IDB_NUMBER_RECT), wxT("��ʾ���ֱ߿�"), wxITEM_CHECK);
	mSelectToolBar->Realize();
	mAuiManager.AddPane(mSelectToolBar, wxAuiPaneInfo().Name(wxT("SelectToolBar")).Caption(wxT("ѡ�񹤾���")).ToolbarPane().Top());

	mAuiManager.Update();
}

void EditorFrame::createRenderWindow()
{
	mRenderCanvas = new RenderCanvas(this, wxPoint(-1, -1), wxSize(-1, -1), wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxMINIMIZE | wxMAXIMIZE, wxT("RenderCanvas"));
	mAuiManager.AddPane(mRenderCanvas, wxAuiPaneInfo().Name(mRenderCanvas->GetName()).Center());
	mRenderCanvas->Show();

	mWindowIDList.insert(mRenderCanvas, std::make_pair(ID_MENU_PANEL_RENDER, ID_TOOL_PANEL_RENDER));
	mWindowList.insert(mRenderCanvas->GetName().ToStdString(), mRenderCanvas);
}

#define NEW_PANEL(type, panel, title, menuID, toolID, size, floatingSize, dock)																										\
	panel = new type(this, wxDefaultPosition, size, wxMINIMIZE | wxMAXIMIZE, wxT(TOSTRING(type)));																		\
	mAuiManager.AddPane(panel, wxAuiPaneInfo().Name(panel->GetName()).BestSize(wxSize(-1, -1)).FloatingSize(floatingSize).Caption(wxT(title)).##dock##().Dockable(true));	\
	mWindowIDList.insert(panel, std::make_pair(menuID, toolID));																						\
	mWindowList.insert(panel->GetName().ToStdString(), panel);

#define NEW_PANEL_LEFT(type, title, menuID, toolID) NEW_PANEL(type, m##type, title, menuID, toolID, wxSize(400, -1), wxSize(300, 800), Left)
#define NEW_PANEL_RIGHT(type, title, menuID, toolID) NEW_PANEL(type, m##type, title, menuID, toolID, wxSize(400, -1), wxSize(300, 800), Right)
#define NEW_PANEL_BOTTOM(type, title, menuID, toolID) NEW_PANEL(type, m##type, title, menuID, toolID, wxSize(-1, 400), wxSize(1200, 400), Bottom)

void EditorFrame::createWindows()
{
	// ��ߴ���
	NEW_PANEL_LEFT(LayoutHierarchy, "������ͼ", ID_MENU_PANEL_LAYOUT_HIERARCHY, ID_TOOL_PANEL_LAYOUT_HIERARCHY);
	NEW_PANEL_LEFT(InnerChildHierarchy, "�ڲ��Ӵ�����ͼ", ID_MENU_PANEL_INNER_CHILD_HIERARCHY, ID_TOOL_PANEL_INNER_CHILD_HIERARCHY);
	NEW_PANEL_LEFT(PropertyWindow, "��������", ID_MENU_PANEL_PROPERTY, ID_TOOL_PANEL_PROPERTY);
	NEW_PANEL_LEFT(ComponentPropertyPanel, "�������", ID_MENU_PANEL_COMPONENT_PROPERTY, ID_TOOL_PANEL_COMPONENT_PROPERTY);
	NEW_PANEL_LEFT(ActionPropertyPanel, "��Ϊ����", ID_MENU_PANEL_ACTION_PROPERTY, ID_TOOL_PANEL_ACTION_PROPERTY);

	// �ұߴ���
	NEW_PANEL_RIGHT(WindowResource, "��������", ID_MENU_PANEL_WINDOW_RESOURCE, ID_TOOL_PANEL_WINDOW_RESOURCE);
	NEW_PANEL_RIGHT(LayoutResource, "�����ļ�", ID_MENU_PANEL_LAYOUT_RESOURCE, ID_TOOL_PANEL_LAYOUT_RESOURCE);
	NEW_PANEL_RIGHT(LoadedLayout, "�Ѽ��ز���", ID_MENU_PANEL_LOADED_LAYOUT, ID_TOOL_PANEL_LOADED_LAYOUT);
	NEW_PANEL_RIGHT(PrefabResource, "����Ԥ��", ID_MENU_PANEL_PREFAB_RESOURCE, ID_TOOL_PANEL_PREFAB_RESOURCE);
	NEW_PANEL_RIGHT(WindowTemplate, "����ģ��", ID_MENU_PANEL_WINDOW_TEMPLATE, ID_TOOL_PANEL_WINDOW_TEMPLATE);
	NEW_PANEL_RIGHT(KeyFrameResource, "�ؼ�֡��Դ", ID_MENU_PANEL_KEY_FRAME_RESOURCE, ID_TOOL_PANEL_KEY_FRAME_RESOURCE);
	NEW_PANEL_RIGHT(ComponentViewer, "�������", ID_MENU_PANEL_COMPONENT_VIEWER, ID_TOOL_PANEL_COMPONENT_VIEWER);
	NEW_PANEL_RIGHT(ActionTreeResource, "��Ϊ����Դ", ID_MENU_PANEL_ACTION_TREE_RESOURCE, ID_TOOL_PANEL_ACTION_TREE_RESOURCE);

	// �ײ�����
	NEW_PANEL_BOTTOM(KeyFrameGraph, "�ؼ�֡�༭", ID_MENU_PANEL_KEY_FRAME_GRAPH, ID_TOOL_PANEL_KEY_FRAME_GRAPH);
	NEW_PANEL_BOTTOM(ActionTreeGraph, "��Ϊ���༭", ID_MENU_PANEL_ACTION_TREE_GRAPH, ID_TOOL_PANEL_ACTION_TREE_GRAPH);

	mConnectWindowDialog = new ConnectWindowDialog(this, wxT("��������"), wxDefaultPosition, wxSize(550, 250), wxDEFAULT_DIALOG_STYLE, wxT("ConnectWindowDialog"));
	mConnectWindowDialog->Show(false);

	mSymmetryWindowDialog = new SymmetryWindowDialog(this, wxT("���ڶԳ�"), wxDefaultPosition, wxSize(650, 250), wxDEFAULT_DIALOG_STYLE, wxT("SymmetryWindowDialog"));
	mSymmetryWindowDialog->Show(false);

	// ���д��ڶ�������ɺ�֪ͨEditorBase
	EditorBase::notifyConstructDone();

	auto iterPanel = mWindowList.begin();
	auto iterPanelEnd = mWindowList.end();
	FOR_STL(mWindowList, ; iterPanel != iterPanelEnd; ++iterPanel)
	{
		iterPanel->second->Show();
	}
	END_FOR_STL(mWindowList);

	mAuiManager.Update();
}

void EditorFrame::createEditorCore()
{
	mEditorCoreRoot = TRACE_NEW(EditorCoreRoot, mEditorCoreRoot);
	mEditorCoreRoot->init(mRenderCanvas->GetHWND(), 32, mRenderCanvas->GetScreenRect().GetWidth(), mRenderCanvas->GetScreenRect().GetHeight());
	// ����ģʽ�Ի�����,�Ի������б�,���ؽ���б�
	int dialogStyle[MDS_MAX];
	dialogStyle[MDS_OK] = wxOK;
	dialogStyle[MDS_YES] = wxYES;
	dialogStyle[MDS_NO] = wxNO;
	dialogStyle[MDS_CANCEL] = wxCANCEL;
	mEditorCoreRoot->setModalDialog(showModalDialog, dialogStyle, dialogStyle);

	// ע�������¼�������
	registerEngineEventHandler();
	// ע�����¼������ߺ�֪ͨ�����ʼ�����
	mEngineRoot->notifyInitDone();

	// ע��༭�����¼�������
	registerEditorCoreEventHandler();
	// ע�����¼������ߺ�֪ͨ�༭�����Ѿ���ʼ�����
	mEditorCoreRoot->notifyInitDone();
}

void EditorFrame::refreshAllResource()
{
	mWindowResource->refreshList();
	mLayoutResource->refreshList();
	mKeyFrameResource->refreshList();
	mActionTreeResource->refreshList();
	mActionTreeResource->refreshActionTypeList();
}

void EditorFrame::refreshAllMenuToolCheckState()
{
	// ��ѡ�в˵���ѡ��״̬
	auto iterWindow = mWindowIDList.begin();
	auto iterWindowEnd = mWindowIDList.end();
	FOR_STL(mWindowIDList, ; iterWindow != iterWindowEnd; ++iterWindow)
	{
		bool check = iterWindow->first->IsShown();
		mWindowMenu->Check(iterWindow->second.first, check);
		mWindowToolBar->ToggleTool(iterWindow->second.second, check);
	}
	END_FOR_STL(mWindowIDList);

	mShowHideToolBar->ToggleTool(ID_TOOL_SHOW_WIREFRAME, mLayoutEditorCore->getWireframeVisible());
	mShowHideToolBar->ToggleTool(ID_TOOL_SHOW_RENDERWINDOW_COORDINATE, mLayoutEditorCore->getRenderWindowCoordinateVisible());
	mShowHideToolBar->ToggleTool(ID_TOOL_SHOW_SELECTWINDOW_COORDINATE, mLayoutEditorCore->getSelectionWindowCoordinateVisible());

	mFileToolBar->EnableTool(ID_TOOL_SAVE_FILE, false);

	mEditorCoreRoot->getUndoManager()->checkUndoRedoEnable();

	wxTextCtrl* densityText = static_cast<wxTextCtrl*>(mEditToolBar->FindControl(ID_TOOL_SCREEN_DENSITY_TEXT));
	densityText->SetValue(txStringUtility::floatToString(mRenderWindow->getScreenDensity(), 2));

	mEditToolBar->ToggleTool(ID_TOOL_DIRECT_VERTICES, mLayoutManager->getDirectVertices());

	mSelectToolBar->ToggleTool(ID_TOOL_SELECT_INVISIBLE, mLayoutManager->getForceVisible());
	mSelectToolBar->ToggleTool(ID_TOOL_SELECT_UN_HANDLE_INPUT, mLayoutManager->getForceHandInput());
	mSelectToolBar->ToggleTool(ID_TOOL_NUMBER_RECT, NumberWindow::getShowNumberRect());

	// ������ɫ
	wxColourPickerCtrl* colourPicker = static_cast<wxColourPickerCtrl*>(mShowHideToolBar->FindControl(ID_TOOL_BACKGROUND_COLOUR));
	const VECTOR4& backgroundColour = mRenderWindow->getBackgroundColour();
	wxColour colour(backgroundColour.r * 255, backgroundColour.g * 255, backgroundColour.b * 255, backgroundColour.a * 255);
	colourPicker->SetColour(colour);
}

void EditorFrame::OnTimer(wxTimerEvent& event)
{
	static unsigned long lastTime = timeGetTime();
	unsigned long curTime = timeGetTime();
	float elapsedTime = (curTime - lastTime) / 1000.0f;
	txMath::clamp(elapsedTime, 0.001f, 0.3f);
	Update(elapsedTime);
	lastTime = curTime;
	Render();
}

void EditorFrame::Update(const float& elapsedTime)
{
	updateStatus();
	if (mEditorCoreRoot != NULL)
	{
		mEditorCoreRoot->preUpdate(elapsedTime);
		mEditorCoreRoot->update(elapsedTime);
		mEditorCoreRoot->lateUpdate(elapsedTime);
	}
}

void EditorFrame::Render()
{
	if (mEditorCoreRoot)
	{
		mEditorCoreRoot->render();
		mKeyFrameGraph->refreshGraph();
		mActionTreeGraph->refreshGraph();
	}
}

void EditorFrame::OpenFile(const std::string& fileName)
{
	mLayoutEditorCore->loadLayout(fileName);
}

void EditorFrame::DestroyLayout(const std::string& layoutName)
{
	mLayoutEditorCore->destroyLayout(layoutName);
}

void EditorFrame::CloseLayout()
{
	mLayoutEditorCore->closeLayout();
}

void EditorFrame::enableUndo(bool enable)
{
	mEditToolBar->EnableTool(ID_TOOL_UNDO, enable);
	mEditMenu->Enable(ID_MENU_UNDO, enable);
	mAuiManager.Update();
}

void EditorFrame::enableRedo(bool enable)
{
	mEditToolBar->EnableTool(ID_TOOL_REDO, enable);
	mEditMenu->Enable(ID_MENU_REDO, enable);
	mAuiManager.Update();
}

void EditorFrame::OnNewFile(wxCommandEvent& event)
{
	mLayoutEditorCore->createLayout();
}

void EditorFrame::OnSaveFile(wxCommandEvent& event)
{
	mLayoutEditorCore->saveLayout();
}

void EditorFrame::OnCloseFile(wxCommandEvent& event)
{
	CloseLayout();
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

void EditorFrame::OnUndo(wxCommandEvent& event)
{
	mEditorCoreRoot->getUndoManager()->undo();
}

void EditorFrame::OnRedo(wxCommandEvent& event)
{
	mEditorCoreRoot->getUndoManager()->redo();
}

void EditorFrame::OnCopy(wxCommandEvent& event)
{
	;
}

void EditorFrame::OnCut(wxCommandEvent& event)
{
	;
}

void EditorFrame::OnPaste(wxCommandEvent& event)
{
	;
}

void EditorFrame::OnPickupUsedResources(wxCommandEvent& event)
{
	mLayoutEditorCore->pickupAllResources(P_MEDIA_PATH + "UsedResource");
}

wxWindow* EditorFrame::getPanel(const std::string& name)
{
	auto iter = mWindowList.find(name);
	if (iter != mWindowList.end())
	{
		return iter->second;
	}
	return NULL;
}

void EditorFrame::logError(const std::string& info)
{
	wxMessageBox(info, wxT("����"));
}

void EditorFrame::logInfo(const std::string& info)
{
	wxMessageBox(info, wxT("��ʾ"));
}

int EditorFrame::showModalDialog(const std::string& caption, const std::string& info, const int& style)
{
	return wxMessageBox(info, caption, style);
}

void EditorFrame::OnPanelShow(wxCommandEvent& event)
{
	int eventID = event.GetId();
	auto iterPanel = mWindowIDList.begin();
	auto iterPanelEnd = mWindowIDList.end();
	FOR_STL(mWindowIDList, ; iterPanel != iterPanelEnd; ++iterPanel)
	{
		if (iterPanel->second.first == eventID || iterPanel->second.second == eventID)
		{
			if (mAuiManager.GetPane(iterPanel->first).IsShown() != event.IsChecked())
			{
				mAuiManager.GetPane(iterPanel->first).Show(event.IsChecked());
				mAuiManager.Update();
			}
			break;
		}
	}
	END_FOR_STL(mWindowIDList);
}

void EditorFrame::OnLoadPerspective(wxCommandEvent& event)
{	
	loadPerspective(mLayoutMenu->GetLabel(event.GetId()).ToStdString());
}

void EditorFrame::OnSavePerspective(wxCommandEvent& event)
{
	// �����ļ�
	wxFileDialog dialog(this, wxT("���沼��"), P_FRAME_LAYOUT_PATH, "", "*" + FRAME_LAYOUT_SUFFIX, wxFD_SAVE);
	if (dialog.ShowModal() != wxID_OK)
	{
		return;
	}
	// ��ȡ�ļ�·��
	std::string fileName = P_FRAME_LAYOUT_PATH + dialog.GetFilename().ToStdString();
	// ��ȡ��������
	wxString saveString = mAuiManager.SavePerspective();
	std::string saveStdString = mAuiManager.SavePerspective().ToStdString();
	// �������ݵ������ļ�
	txFileUtility::writeFile(fileName, saveStdString.length(), saveStdString.c_str());
	// ˢ�²��ֲ˵��б�
	wxMenuItem* testMenuItem = mLayoutMenu->FindItem(ID_MENU_LOAD_PERSPECTIVE);
	if (NULL != testMenuItem)
	{
		loadAllPerspective(testMenuItem->GetSubMenu());
	}
}

void EditorFrame::OnResolution(wxCommandEvent& event)
{
	int index = event.GetId() - ID_MENU_RESOLUTION_0;
	if (index >= 0 && index < (int)mResolutionSize.size())
	{
		// 16�Ǵ������ұ߿�Ŀ��,36�Ǵ������±߿�ĸ߶�
		mAuiManager.GetPane(mRenderCanvas).Floatable(true).Float().Dockable(false).FloatingSize(mResolutionSize[index].first + wxSize(16, 36));
		mAuiManager.Update();

		// ��Ҫ����ȷ�ϴ��ڿ��
		wxSize size = mResolutionSize[index].first;
		mEditorCoreRoot->resize(size.x, size.y);

		// �������Ϊ���ֱ���,����Ҫ֪ͨ��Ⱦ�����ڽ��������Ϣʱ�������λ��
		mRenderCanvas->notifyAdjustMousePosition(index == 0);
	}
}

void EditorFrame::OnWireframeVisible(wxCommandEvent& event)
{
	mLayoutEditorCore->setWireframeVisible(event.IsChecked());
}

void EditorFrame::OnWindowPositinCenter(wxCommandEvent& event)
{
	txVector<Window*>& selectiond = mLayoutEditorCore->getSelections();
	int selectionSize = selectiond.size();
	FOR_STL(selectiond, int i = 0; i < selectionSize; ++i)
	{
		Window* window = selectiond[i];
		const VECTOR2& finalSize = selectiond[i]->getFinalSize();
		txPoint centerPosition(txDim(0.5f, (int)(finalSize.x * -1.0f / 2.0f)), txDim(0.5f, (int)(finalSize.y * -1.0f / 2.0f)));
		CommandWindowSetProperty* cmdProeprty = NEW_CMD(cmdProeprty);
		cmdProeprty->mPropertyName = txProperty::getPropertyName<WindowPropertyPosition>();
		cmdProeprty->mPropertyValue = txStringUtility::pointToString(centerPosition);
		cmdProeprty->mOperator = this;
		mCommandSystem->pushCommand(cmdProeprty, window);
	}
	END_FOR_STL(selectiond);
}

void EditorFrame::OnWindowPositionLeftTop(wxCommandEvent& event)
{
	txVector<Window*>& selectiond = mLayoutEditorCore->getSelections();
	int selectionSize = selectiond.size();
	FOR_STL(selectiond, int i = 0; i < selectionSize; ++i)
	{
		Window* window = selectiond[i];
		const VECTOR2& finalSize = selectiond[i]->getFinalSize();
		txPoint position(txDim::ZERO, txDim(1.0f, (int)(finalSize.y * -1.0f)));
		CommandWindowSetProperty* cmdProeprty = NEW_CMD(cmdProeprty);
		cmdProeprty->mPropertyName = txProperty::getPropertyName<WindowPropertyPosition>();
		cmdProeprty->mPropertyValue = txStringUtility::pointToString(position);
		cmdProeprty->mOperator = this;
		mCommandSystem->pushCommand(cmdProeprty, window);
	}
	END_FOR_STL(selectiond);
}

void EditorFrame::OnwindowPositionLeftDown(wxCommandEvent& event)
{
	txVector<Window*>& selectiond = mLayoutEditorCore->getSelections();
	int selectionSize = selectiond.size();
	FOR_STL(selectiond, int i = 0; i < selectionSize; ++i)
	{
		Window* window = selectiond[i];
		const VECTOR2& finalSize = selectiond[i]->getFinalSize();
		CommandWindowSetProperty* cmdProeprty = NEW_CMD(cmdProeprty);
		cmdProeprty->mPropertyName = txProperty::getPropertyName<WindowPropertyPosition>();
		cmdProeprty->mPropertyValue = txStringUtility::pointToString(txPoint::ZERO);
		cmdProeprty->mOperator = this;
		mCommandSystem->pushCommand(cmdProeprty, window);
	}
	END_FOR_STL(selectiond);
}

void EditorFrame::OnWindowPositionRightTop(wxCommandEvent& event)
{
	txVector<Window*>& selectiond = mLayoutEditorCore->getSelections();
	int selectionSize = selectiond.size();
	FOR_STL(selectiond, int i = 0; i < selectionSize; ++i)
	{
		Window* window = selectiond[i];
		const VECTOR2& finalSize = selectiond[i]->getFinalSize();
		txPoint position(txDim(1.0f, (int)(finalSize.x * -1.0f)), txDim(1.0f, (int)(finalSize.y * -1.0f)));
		CommandWindowSetProperty* cmdProeprty = NEW_CMD(cmdProeprty);
		cmdProeprty->mPropertyName = txProperty::getPropertyName<WindowPropertyPosition>();
		cmdProeprty->mPropertyValue = txStringUtility::pointToString(position);
		cmdProeprty->mOperator = this;
		mCommandSystem->pushCommand(cmdProeprty, window);
	}
	END_FOR_STL(selectiond);
}

void EditorFrame::OnwindowPositionRightDown(wxCommandEvent& event)
{
	txVector<Window*>& selectiond = mLayoutEditorCore->getSelections();
	int selectionSize = selectiond.size();
	FOR_STL(selectiond, int i = 0; i < selectionSize; ++i)
	{
		Window* window = selectiond[i];
		const VECTOR2& finalSize = selectiond[i]->getFinalSize();
		txPoint position(txDim(1.0f, (int)(finalSize.x * -1.0f)), txDim::ZERO);
		CommandWindowSetProperty* cmdProeprty = NEW_CMD(cmdProeprty);
		cmdProeprty->mPropertyName = txProperty::getPropertyName<WindowPropertyPosition>();
		cmdProeprty->mPropertyValue = txStringUtility::pointToString(position);
		cmdProeprty->mOperator = this;
		mCommandSystem->pushCommand(cmdProeprty, window);
	}
	END_FOR_STL(selectiond);
}

void EditorFrame::OnWindowPositionTopCenter(wxCommandEvent& event)
{
	txVector<Window*>& selectiond = mLayoutEditorCore->getSelections();
	int selectionSize = selectiond.size();
	FOR_STL(selectiond, int i = 0; i < selectionSize; ++i)
	{
		Window* window = selectiond[i];
		const VECTOR2& finalSize = selectiond[i]->getFinalSize();
		txPoint position(txDim(0.5f, (int)(finalSize.x * -1.0f / 2.0f)), txDim(1.0f, (int)(finalSize.y * -1.0f)));
		CommandWindowSetProperty* cmdProeprty = NEW_CMD(cmdProeprty);
		cmdProeprty->mPropertyName = txProperty::getPropertyName<WindowPropertyPosition>();
		cmdProeprty->mPropertyValue = txStringUtility::pointToString(position);
		cmdProeprty->mOperator = this;
		mCommandSystem->pushCommand(cmdProeprty, window);
	}
	END_FOR_STL(selectiond);
}

void EditorFrame::OnWindowPositionBottomCenter(wxCommandEvent& event)
{
	txVector<Window*>& selectiond = mLayoutEditorCore->getSelections();
	int selectionSize = selectiond.size();
	FOR_STL(selectiond, int i = 0; i < selectionSize; ++i)
	{
		Window* window = selectiond[i];
		const VECTOR2& finalSize = selectiond[i]->getFinalSize();
		txPoint position(txDim(0.5f, (int)(finalSize.x * -1.0f / 2.0f)), txDim::ZERO);
		CommandWindowSetProperty* cmdProeprty = NEW_CMD(cmdProeprty);
		cmdProeprty->mPropertyName = txProperty::getPropertyName<WindowPropertyPosition>();
		cmdProeprty->mPropertyValue = txStringUtility::pointToString(position);
		cmdProeprty->mOperator = this;
		mCommandSystem->pushCommand(cmdProeprty, window);
	}
	END_FOR_STL(selectiond);
}
void EditorFrame::OnWindowPositionLeftCenter(wxCommandEvent& event)
{
	txVector<Window*>& selectiond = mLayoutEditorCore->getSelections();
	int selectionSize = selectiond.size();
	FOR_STL(selectiond, int i = 0; i < selectionSize; ++i)
	{
		Window* window = selectiond[i];
		const VECTOR2& finalSize = selectiond[i]->getFinalSize();
		txPoint position(txDim::ZERO, txDim(0.5f, (int)(finalSize.y * -1.0f / 2.0f)));
		CommandWindowSetProperty* cmdProeprty = NEW_CMD(cmdProeprty);
		cmdProeprty->mPropertyName = txProperty::getPropertyName<WindowPropertyPosition>();
		cmdProeprty->mPropertyValue = txStringUtility::pointToString(position);
		cmdProeprty->mOperator = this;
		mCommandSystem->pushCommand(cmdProeprty, window);
	}
	END_FOR_STL(selectiond);
}

void EditorFrame::OnWindowPositionRightCenter(wxCommandEvent& event)
{
	txVector<Window*>& selectiond = mLayoutEditorCore->getSelections();
	int selectionSize = selectiond.size();
	FOR_STL(selectiond, int i = 0; i < selectionSize; ++i)
	{
		Window* window = selectiond[i];
		const VECTOR2& finalSize = selectiond[i]->getFinalSize();
		txPoint position(txDim(1.0f, (int)(finalSize.x * -1.0f)), txDim(0.5f, (int)(finalSize.y * -1.0f / 2.0f)));
		CommandWindowSetProperty* cmdProeprty = NEW_CMD(cmdProeprty);
		cmdProeprty->mPropertyName = txProperty::getPropertyName<WindowPropertyPosition>();
		cmdProeprty->mPropertyValue = txStringUtility::pointToString(position);
		cmdProeprty->mOperator = this;
		mCommandSystem->pushCommand(cmdProeprty, window);
	}
	END_FOR_STL(selectiond);
}

void EditorFrame::OnWindowPositionFill(wxCommandEvent& event)
{
	txVector<Window*>& selectiond = mLayoutEditorCore->getSelections();
	int selectionSize = selectiond.size();
	FOR_STL(selectiond, int i = 0; i < selectionSize; ++i)
	{
		Window* window = selectiond[i];

		CommandWindowSetProperty* cmdPosition = NEW_CMD(cmdPosition);
		cmdPosition->mPropertyName = txProperty::getPropertyName<WindowPropertyPosition>();
		cmdPosition->mPropertyValue = txStringUtility::pointToString(txPoint::ZERO);
		cmdPosition->mOperator = this;
		mCommandSystem->pushCommand(cmdPosition, window);

		CommandWindowSetProperty* cmdSize = NEW_CMD(cmdSize);
		cmdSize->mPropertyName = txProperty::getPropertyName<WindowPropertySize>();
		cmdSize->mPropertyValue = txStringUtility::pointToString(txPoint::IDENTITY);
		cmdSize->mOperator = this;
		mCommandSystem->pushCommand(cmdSize, window);
	}
	END_FOR_STL(selectiond);
}

void EditorFrame::OnShowRenderWindowCoordinate(wxCommandEvent& event)
{
	mLayoutEditorCore->setRenderWindowCoordinateVisible(event.IsChecked());
}

void EditorFrame::OnShowWindowCoordinate(wxCommandEvent& event)
{
	mLayoutEditorCore->setSelectionWindowCoordinateVisible(event.IsChecked());
}

void EditorFrame::OnWindowPosMoveUp(wxCommandEvent& event)
{
	if (mLayoutEditorCore->getSelections().size() > 0)
	{
		Window* window = mLayoutEditorCore->getSelections()[0];
		if (window->getParent() != NULL)
		{
			mLayoutEditorCore->moveWindowPos(window, window->getParent()->getChildPos(window) - 1);
		}
	}
}

void EditorFrame::OnWindowPosMoveDown(wxCommandEvent& event)
{
	if (mLayoutEditorCore->getSelections().size() > 0)
	{
		Window* window = mLayoutEditorCore->getSelections()[0];
		if (window->getParent() != NULL)
		{
			mLayoutEditorCore->moveWindowPos(window, window->getParent()->getChildPos(window) + 1);
		}
	}
}

void EditorFrame::OnWindowPosMoveTop(wxCommandEvent& event)
{
	if (mLayoutEditorCore->getSelections().size() > 0)
	{
		Window* window = mLayoutEditorCore->getSelections()[0];
		if (window->getParent() != NULL)
		{
			mLayoutEditorCore->moveWindowPos(window, 0);
		}
	}
}

void EditorFrame::OnWindowPosMoveBottom(wxCommandEvent& event)
{
	if (mLayoutEditorCore->getSelections().size() > 0)
	{
		Window* window = mLayoutEditorCore->getSelections()[0];
		if (window->getParent() != NULL)
		{
			mLayoutEditorCore->moveWindowPos(window, window->getParent()->getChildren().size() - 1);
		}
	}
}

void EditorFrame::OnScreenDensity(wxCommandEvent& event)
{
	wxTextCtrl* densityText = static_cast<wxTextCtrl*>(mEditToolBar->FindControl(ID_TOOL_SCREEN_DENSITY_TEXT));
	wxString value = densityText->GetValue();
	mLayoutEditorCore->setScreenDensity(txStringUtility::stringToFloat(value.ToStdString()));
}

void EditorFrame::OnDockRenderWindow(wxCommandEvent& event)
{
	mAuiManager.GetPane(mRenderCanvas).Dock();
	mAuiManager.Update();
}

void EditorFrame::OnConnectWindow(wxCommandEvent& event)
{
	mConnectWindowDialog->Show(!mConnectWindowDialog->IsShown());
}

void EditorFrame::OnSymmetryWindow(wxCommandEvent& event)
{
	mSymmetryWindowDialog->Show(!mSymmetryWindowDialog->IsShown());
}

void EditorFrame::OnDirectVertices(wxCommandEvent& event)
{
	mLayoutManager->setDirectVertices(event.IsChecked());
}

void EditorFrame::OnBackgroundColour(wxColourPickerEvent& event)
{
	wxColour colour = event.GetColour();
	mRenderWindow->setBackgroundColour(VECTOR4(colour.Red() / 255.0f, colour.Green() / 255.0f, colour.Blue() / 255.0f, colour.Alpha() / 255.0f));
}

void EditorFrame::OnSelectInvisible(wxCommandEvent& event)
{
	mLayoutManager->setForceVisible(event.IsChecked());
}

void EditorFrame::OnSelectUnHandleInput(wxCommandEvent& event)
{
	mLayoutManager->setForceHandInput(event.IsChecked());
}

void EditorFrame::OnShowNumberRect(wxCommandEvent& event)
{
	NumberWindow::setShowNumberRect(event.IsChecked());
	TextWindow::setShowTextRect(event.IsChecked());
}

void EditorFrame::onEditorCoreEvent(const CORE_EVENT& type, const txVector<std::string>& params)
{
	if (type == CE_LAYOUT_DESTROIED)
	{
		CommandEditorFrameNotifyLayoutDestroied* cmd = NEW_CMD(cmd);
		cmd->mLayoutName = params[0];
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_LAYOUT_CLOSED)
	{
		CommandEditorFrameNotifyLayoutClosed* cmd = NEW_CMD(cmd);
		cmd->mLayoutName = params[0];
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_WINDOW_ADDED)
	{
		CommandEditorFrameNotifyWindowAdded* cmd = NEW_CMD(cmd);
		cmd->mParentName = params[0];
		cmd->mName = params[1];
		cmd->mWindowPos = txStringUtility::stringToInt(params[2]);
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_WINDOW_DELETED)
	{
		CommandEditorFrameNotifyWindowDelete* cmd = NEW_CMD(cmd);
		cmd->mParentName = params[0];
		cmd->mName = params[1];
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_SELECTION_CHANGED)
	{
		CommandEditorFrameNotifySelectionChanged* cmd = NEW_CMD(cmd);
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_WINDOW_RENAMED)
	{
		CommandEditorFrameNotifyWindowRename* cmd = NEW_CMD(cmd);
		cmd->mOldName = params[0];
		cmd->mNewName = params[1];
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_LAYOUT_RENAMED)
	{
		CommandEditorFrameNotifyLayoutRename* cmd = NEW_CMD(cmd);
		cmd->mOldName = params[0];
		cmd->mNewName = params[1];
		mCommandSystem->pushCommand(cmd, this);
		
		// �������ñ�����
		SetTitle(mAppNormalName + " - " + mLayoutEditorCore->getCurLayout()->getName() + " *");
	}
	else if (type == CE_LAYOUT_ADDED)
	{
		CommandEditorFrameNotifyLayoutAdded* cmd = NEW_CMD(cmd);
		cmd->mName = params[0];
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_WINDOW_PROPERTY_CHANGED)
	{
		CommandEditorFrameNotifyWindowPropertyChanged* cmd = NEW_CMD(cmd);
		cmd->mPropertyName = params[0];
		cmd->mOldPropertyValue = params[1];
		cmd->mPropertyValue = params[2];
		cmd->mWindowName = params[3];
		cmd->mChanger = (void*)(txStringUtility::stringToInt(params[4]));
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_LAYOUT_MODIFIED)
	{
		// ���ô��ڱ�������,����޸���,�����*��,���û���޸�,�򲻼�*��
		std::string layoutName = params[0];
		bool modified = txStringUtility::stringToBool(params[1]);
		if (modified)
		{
			SetTitle(mAppNormalName + " - " + layoutName + " *");
		}
		else
		{
			SetTitle(mAppNormalName + " - " + layoutName);
		}

		mFileToolBar->EnableTool(ID_TOOL_SAVE_FILE, modified);
	}
	else if (type == CE_LAYOUT_VISIBLE_CHANGED)
	{
		CommandEditorFrameNotifyLayoutVisibleChanged* cmd = NEW_CMD(cmd);
		cmd->mLayoutName = params[0];
		cmd->mVisible = txStringUtility::stringToBool(params[1]);
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_WINDOW_POS_CHANGED)
	{
		CommandEditorFrameNotifyWindowPosChanged* cmd = NEW_CMD(cmd);
		cmd->mName = params[0];
		cmd->mParentName = params[1];
		cmd->mOldPosition = txStringUtility::stringToInt(params[2]);
		cmd->mNewPosition = txStringUtility::stringToInt(params[3]);
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_WINDOW_PREFAB_CHANGED)
	{
		CommandEditorFrameNotifyWindowPrefabChanged* cmd = NEW_CMD(cmd);
		cmd->mWindowName = params[0];
		cmd->mPrefabName = params[1];
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_UNDO_ENABLE_CHANGE)
	{
		CommandEditorFrameNotifyUndoEnableChanged* cmd = NEW_CMD(cmd);
		cmd->mEnabled = txStringUtility::stringToBool(params[0]);
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_REDO_ENABLE_CHANGE)
	{
		CommandEditorFrameNotifyRedoEnableChanged* cmd = NEW_CMD(cmd);
		cmd->mEnabled = txStringUtility::stringToBool(params[0]);
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_ERROR_LOG)
	{
		logError(params[0]);
	}
	else if (type == CE_INFO_LOG)
	{
		logInfo(params[0]);
	}
	else if (type == CE_PREFAB_RESOURCE_CHANGED)
	{
		CommandEditorFrameNotifyPrefabResourceChanged* cmd = NEW_CMD(cmd);
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_TEMPLATE_RESOURCE_CHANGED)
	{
		CommandEditorFrameNotifyTemplateResourceChanged* cmd = NEW_CMD(cmd);
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_WINDOW_CUTED)
	{
		CommandEditorFrameNotifyWindowCuted* cmd = NEW_CMD(cmd);
		cmd->mCutWindowName = params[0];
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_TREMBLE_MODIFIED)
	{
		const std::string& tremblingName = params[0];
		bool modified = txStringUtility::stringToBool(params[1]);
		std::string caption = std::string("�ؼ�֡�༭-") + tremblingName + (modified ? " *" : EMPTY_STRING);
		mAuiManager.GetPane(mKeyFrameGraph).Caption(caption);
		mKeyFrameResource->notifyTremblingModified(modified);
		mAuiManager.Update();
	}
	else if (type == CE_LOAD_TREMBLING)
	{
		mKeyFrameResource->notifyTremblingActived();
	}
	else if (type == CE_NEW_TREMBLING)
	{
		mKeyFrameResource->notifyNewTrembling(params[0]);
	}
	else if (type == CE_ADD_NODE)
	{
		mKeyFrameResource->notifyAddDimension(params[0]);
	}
	else if (type == CE_DELETE_NODE)
	{
		mKeyFrameResource->notifyDeleteDimension(params[0]);
	}
	else if (type == CE_SELECT_TREMBLING_NODE)
	{
		mKeyFrameResource->notifyDimensionSelected();
	}
	else if (type == CE_SELECT_KEY_FRAME)
	{
		mKeyFrameResource->notifyKeyFrameSelected();
	}
	else if (type == CE_ADD_KEY_FRAME)
	{
		mKeyFrameResource->notifyAddKeyFrame(txStringUtility::stringToInt(params[0]));
	}
	else if (type == CE_DELETE_KEY_FRAME)
	{
		mKeyFrameResource->notifyDeleteKeyFrame(txStringUtility::stringToInt(params[0]));
	}
	else if (type == CE_WINDOW_COMPONENT_PROPERTY_CHANGED)
	{
		CommandEditorFrameNotifyComponentPropertyChanged* cmd = NEW_CMD(cmd);
		cmd->mComponentName = params[0];
		cmd->mPropertyName = params[1];
		cmd->mOldPropertyValue = params[2];
		cmd->mPropertyValue = params[3];
		cmd->mWindowName = params[4];
		cmd->mChanger = (void*)txStringUtility::stringToInt(params[5]);
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_COMPONENT_SELECTION_CHANGED)
	{
		CommandEditorFrameNotifyComponentSelectionChanged* cmd = NEW_CMD(cmd);
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_LOAD_ACTION_TREE)
	{
		mActionTreeResource->notifyActionTreeActived();
	}
	else if (type == CE_NEW_ACTION_TREE)
	{
		mActionTreeResource->notifyActionTreeAdded(params[0]);
	}
	else if (type == CE_DELETE_ACTION_TREE)
	{
		mActionTreeResource->notifyActionTreeDeleted(params[0]);
	}
	else if (type == CE_SELECT_ACTION_TREE)
	{
		mActionTreeResource->notifyActionTreeActived();
	}
	else if (type == CE_ACTION_SELECTION_CHANGED)
	{
		CommandEditorFrameNotifyActionSelectionChanged* cmd = NEW_CMD(cmd);
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_ADD_ACTION)
	{
		mActionTreeResource->notifyActionAdded(params[0], params[1]);
	}
	else if (type == CE_DELETE_ACTION)
	{
		mActionTreeResource->notifyActionDeleted(params[0]);
	}
	else if (type == CE_ACTION_TREE_MODIFIED)
	{
		const std::string& treeName = params[0];
		bool modified = txStringUtility::stringToBool(params[1]);
		std::string caption = std::string("��Ϊ���༭-") + treeName + (modified ? " *" : EMPTY_STRING);
		mAuiManager.GetPane(mActionTreeGraph).Caption(caption);
		mActionTreeResource->notifyActionTreeModified(modified);
		mAuiManager.Update();
	}
}

void EditorFrame::onEngineEvent(const ENGINE_EVENT& event, const txVector<std::string>& paramList)
{
	if (event == EE_ENGINE_ERROR_INFO)
	{
		logError(paramList[0]);
	}
}

void EditorFrame::loadAllPerspective(wxMenu* parentMenu)
{
	if (NULL == parentMenu)
	{
		return;
	}
	// �����
	auto& list = parentMenu->GetMenuItems();
	auto itr = list.begin();
	while (itr != list.end())
	{
		parentMenu->Delete(*(itr++));
	}

	// ������в����ļ�
	txVector<std::string> fileList;
	txFileUtility::findFiles(P_FRAME_LAYOUT_PATH, fileList, FRAME_LAYOUT_SUFFIX);
	int fileCount = fileList.size();
	FOR_STL(fileList, int i = 0; i < fileCount; ++i)
	{
		// ��ӵ��Ӳ˵�
		std::string& menuName = txStringUtility::getFileNameNoSuffix(fileList[i]);
		wxMenuItem* tempItem = new wxMenuItem(parentMenu, wxID_ANY, wxString(menuName.c_str()), wxEmptyString, wxITEM_NORMAL);
		parentMenu->Append(tempItem);
		this->Connect(tempItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(EditorFrame::OnLoadPerspective));
	}
	END_FOR_STL(fileList);
}

void EditorFrame::createResolutionMenu(wxMenu* parentMenu)
{
	int resolutionCount = mResolutionSize.size();
	FOR_STL(mResolutionSize, int i = 0; i < resolutionCount; ++i)
	{
		std::string resolutionString = txStringUtility::intToString(mResolutionSize[i].first.x) + "*" + txStringUtility::intToString(mResolutionSize[i].first.y);
		std::string aspectString = txStringUtility::intToString(mResolutionSize[i].second.x) + ":" + txStringUtility::intToString(mResolutionSize[i].second.y);
		wxMenuItem* resolutionItem = new wxMenuItem(parentMenu, ID_MENU_RESOLUTION_0 + i, wxString(resolutionString + " " + aspectString), wxEmptyString, wxITEM_NORMAL);
		parentMenu->Append(resolutionItem);
		this->Connect(resolutionItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(EditorFrame::OnResolution));
	}
	END_FOR_STL(mResolutionSize);
}

void EditorFrame::loadPerspective(const std::string& name)
{
	// ���ز���
	mAuiManager.LoadPerspective(txFileUtility::openTxtFile(P_FRAME_LAYOUT_PATH + name + FRAME_LAYOUT_SUFFIX));
}