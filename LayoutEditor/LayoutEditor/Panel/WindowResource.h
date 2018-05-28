#ifndef _WINDOW_RESOURCE_H_
#define _WINDOW_RESOURCE_H_

#include "EditorPanel.h"

class WindowResource : public EditorPanel
{
public:
	WindowResource(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	~WindowResource();
	void Setup();
	void refreshList(); // ˢ���б�

	DECLARE_EVENT_TABLE()
	void OnRefresh(wxCommandEvent& event);		// ��Ӧˢ�°�ť�ĵ���¼�
	void OnDoubleClick(wxListEvent& event);
protected:
	wxAuiToolBar* mToolBar;
	wxListCtrl* mResourceList;
	wxImageList* mImageList;
	txMap<WINDOW_TYPE, int> mWindowIconIndexList;
};

#endif