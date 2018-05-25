#ifndef _PREFAB_RESOURCE_H_
#define _PREFAB_RESOURCE_H_

#include "EditorPanel.h"

class WindowPrefabManager;
class PrefabResource : public EditorPanel
{
public:
	PrefabResource(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	~PrefabResource();
	void Setup();
	void refreshList(); // ˢ���б�

	DECLARE_EVENT_TABLE()
	void OnRefresh(wxCommandEvent& event);		// ��Ӧˢ�°�ť�ĵ���¼�
	void OnDoubleClick(wxCommandEvent& event);
	void OnRightUp(wxMouseEvent& event);
	void OnRightDown(wxMouseEvent& event);
	void OnMenuOpenFile(wxCommandEvent& event);
	void OnMenuOpenDir(wxCommandEvent& event);
	void OnMenuEditPrefab(wxCommandEvent& event);
protected:
	void ShowMenu(const wxPoint& pt);	// �Ҽ��˵�
protected:
	wxAuiToolBar* mToolBar;
	wxListBox* mResourceList;
};

#endif