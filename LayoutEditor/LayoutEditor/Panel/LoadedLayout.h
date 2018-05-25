#ifndef _LOADED_LAYOUT_H_
#define _LOADED_LAYOUT_H_

#include "EditorPanel.h"

class LoadedLayout : public EditorPanel
{
public:
	LoadedLayout(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	~LoadedLayout();
	void Setup();
	void refreshList(); // ˢ���б�
	void setItemImage(const std::string& itemName, bool visible); // true��ʾ�л�Ϊ��ʾ��ͼ��,false��ʾ�л�Ϊ���ص�ͼ��

	DECLARE_EVENT_TABLE()
	void OnRefresh(wxCommandEvent& event);		// ��Ӧˢ�°�ť�ĵ���¼�
	void OnItemActivated(wxListEvent& event);
	void OnItemRightClick(wxListEvent& event);
	void OnItemKeyDown(wxListEvent& event);
protected:
	wxAuiToolBar* mToolBar;
	wxListCtrl* mResourceList;
	wxImageList* mImageList;
};

#endif