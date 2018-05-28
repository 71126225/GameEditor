#ifndef _LAYOUT_HIERARCHY_H_
#define _LAYOUT_HIERARCHY_H_

#include "EditorPanel.h"

class Layout;
class Window;
class LayoutHierarchy : public EditorPanel
{
public:
	LayoutHierarchy(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	~LayoutHierarchy();
	void setup();

	// ��ʾ���ֵĲ㼶��ͼ
	void displayLayout(::Layout* layout);

	// ����ָ����Ϊѡ��״̬,��������Ѿ���ѡ����,�����κβ���
	void setSelection(txVector<std::string>& itemLabel);
	void unSelectAll();
	void setHandleEvent(bool handleEvent) { mHandleEvent = handleEvent; }
	void setVisibleIcon(Window* window);
	void addItem(Window* window, int pos = -1);
	void deleteItem(const std::string& label);
	void renameItem(const std::string& label, const std::string& newLabel);
	void moveItemPosition(const std::string& parentLabel, Window* window, int oldPosition, int newPosition);
	void refreshChildItem(Window* window);
	void resetParentItem(Window* window);	// ˢ��window�ĸ��ڵ�

	DECLARE_EVENT_TABLE()
	void OnItemSelected(wxTreeEvent& event);
	void OnItemBeginEdit(wxTreeEvent& event);
	void OnItemEndEdit(wxTreeEvent& event);
	void OnItemKeyDown(wxTreeEvent& event);
	void OnItemRightClick(wxTreeEvent& event);
	void OnMenuCollapse(wxCommandEvent& event);
	void OnMenuExpand(wxCommandEvent& event);
	void OnMenuCreatePrefab(wxCommandEvent& event);
	void OnMenuCreateTemplate(wxCommandEvent& event);
	void OnMenuCopy(wxCommandEvent& event);
	void OnMenuCopyWithChildren(wxCommandEvent& event);
	void OnMenuCut(wxCommandEvent& event);
	void OnMenuPaste(wxCommandEvent& event);
	void OnMenuLockExpand(wxCommandEvent& event);
	void OnRefreshButton(wxCommandEvent& event);
	void OnSearch(wxCommandEvent& event);   // ����
	void OnSearchListSelect(wxListEvent& event);
	void OnItemStateClick(wxTreeEvent& event);
protected:
	// ��ӵ�ָ����ID������
	void appendItem(wxTreeItemId parent, Window* window, int pos = -1);
	void initTreeImage();
	int getWindowIconIndex(std::string windowName);
	void collapseItem(wxTreeItemId item);
	void expandItem(wxTreeItemId item);
	void notifyItemDelete(wxTreeItemId item);	// ֪ͨ����ɾ��ĳһ��
	void showMenu(const wxPoint& pt);			// �Ҽ��˵�
	void showSearch(const std::string& value);	// ��ʾ��������
protected:
	wxAuiToolBar*	mAuiToolBar;
	wxTreeCtrl*		mHierarchyTree;
	wxListCtrl*		mSearchResultList;
	wxTextCtrl*		mSearchTextCtrl;
	wxTreeItemId	mRootId;
	wxString		mPreString;
	wxImageList*	mImageList;
	wxImageList*	mStateImageList;
	txMap<std::string, int> mWindowIconIndexList;
	txSet<wxTreeItemId> mLockExpandItemList;		// ʼ��������չ������
	txMap<std::string, wxTreeItemId> mTreeItemMap;
	bool mHandleEvent;			// �Ƿ���wx�������¼�
};

#endif