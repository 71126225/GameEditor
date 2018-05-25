#ifndef _WINDOW_TEMPLATE_H_
#define _WINDOW_TEMPLATE_H_

#include "EditorPanel.h"

class WindowTemplateManager;
class WindowTemplate : public EditorPanel
{
public:
	WindowTemplate(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	~WindowTemplate(){}
	void Setup();
	void refreshList(); // ˢ���б�

	DECLARE_EVENT_TABLE()
	void OnRefresh(wxCommandEvent& event);		// ��Ӧˢ�°�ť�ĵ���¼�
	void OnDoubleClick(wxListEvent& event);
	void OnContexMenu(wxContextMenuEvent& event);
	void OnOpenFile(wxCommandEvent& event);
	void OnOpenDir(wxCommandEvent& event);
protected:
	void ShowMenu(const wxPoint& pt);	// �Ҽ��˵�
protected:
	wxAuiToolBar* mToolBar;
	wxListCtrl* mTemplateList;
	wxString mSelectText;
};

#endif