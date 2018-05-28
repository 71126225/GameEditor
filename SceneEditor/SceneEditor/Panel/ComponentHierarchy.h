#ifndef _COMPONENT_HIERARCHY_H_
#define _COMPONENT_HIERARCHY_H_

#include "EditorPanel.h"

class txComponent;
class txComponentOwner;
class ComponentHierarchy : public EditorPanel
{
public:
	ComponentHierarchy(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~ComponentHierarchy();
	void setup();
	void showComponent(txComponentOwner* owner);
	// ����ָ����Ϊѡ��״̬,��������Ѿ���ѡ����,�����κβ���
	void setSelection(txVector<std::string>& itemLabel);
	void unSelectAll();
	void setHandleEvent(bool handleEvent) { mHandleEvent = handleEvent; }
	void setActiveIcon(txComponent* component);
	void refreshActiveIcon(txComponentOwner* owner);
	void addItem(txComponent* component, int pos = -1);
	void deleteItem(const std::string& label);
	void renameItem(const std::string& label, const std::string& newLabel);
	void moveItemPosition(const std::string& parentLabel, txComponent* component, int oldPosition, int newPosition);
	void refreshChildItem(txComponent* component);
	DECLARE_EVENT_TABLE()
	void OnItemSelected(wxTreeEvent& event);
	void OnItemBeginEdit(wxTreeEvent& event);
	void OnItemEndEdit(wxTreeEvent& event);
	void OnItemKeyDown(wxTreeEvent& event);
	void OnItemRightClick(wxTreeEvent& event);
	void OnRefreshButton(wxCommandEvent& event);
	void OnItemStateClick(wxTreeEvent& event);
protected:
	// ��ӵ�ָ����ID������
	void appendItem(wxTreeItemId parent, txComponent* window, int pos = -1);
	void initTreeImage();
	int getWindowIconIndex(std::string windowName);
	void collapseItem(wxTreeItemId item);
	void expandItem(wxTreeItemId item);
	void notifyItemDelete(wxTreeItemId item);	// ֪ͨ����ɾ��ĳһ��
protected:
	wxAuiToolBar*	mAuiToolBar;
	wxTreeCtrl*		mHierarchyTree;
	wxTreeItemId	mRootId;
	wxString		mPreString;
	wxImageList*	mImageList;
	wxImageList*	mStateImageList;
	txMap<std::string, int> mWindowIconIndexList;
	txMap<std::string, wxTreeItemId> mTreeItemMap;
	bool mHandleEvent;			// �Ƿ���wx�������¼�
};

#endif