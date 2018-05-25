#include "txEngineRoot.h"

#include "LayoutManager.h"
#include "Layout.h"
#include "WindowProperty.h"
#include "WindowPrefabManager.h"
#include "WindowTemplateManager.h"
#include "WindowHeader.h"

#include "LayoutEditorCore.h"
#include "EditorCommandHeader.h"
#include "ECLog.h"

#include "LayoutHierarchy.h"
#include "EditorApp.h"
#include "EditorFrame.h"

enum
{
	ID_TREE,
	ID_MENU_COLLAPSE,
	ID_MENU_EXPAND,
	ID_MENU_CREATE_PREFAB,
	ID_MENU_CREATE_TEMPLATE,
	ID_MENU_COPY,
	ID_MENU_COPY_WITH_CHILDREN,
	ID_MENU_CUT,
	ID_MENU_PASTE,
	ID_MENU_LOCK_EXPAND,
	ID_REFRESH_BUTTON,
	ID_SEARCH,
	ID_SEARCH_RESULT_LIST,
};

BEGIN_EVENT_TABLE(LayoutHierarchy, EditorPanel)
EVT_TREE_ITEM_ACTIVATED(ID_TREE, LayoutHierarchy::OnItemSelected)
EVT_TREE_SEL_CHANGED(ID_TREE, LayoutHierarchy::OnItemSelected)
EVT_TREE_BEGIN_LABEL_EDIT(ID_TREE, LayoutHierarchy::OnItemBeginEdit)
EVT_TREE_END_LABEL_EDIT(ID_TREE, LayoutHierarchy::OnItemEndEdit)
EVT_TREE_KEY_DOWN(ID_TREE, LayoutHierarchy::OnItemKeyDown)
EVT_TREE_ITEM_RIGHT_CLICK(ID_TREE, LayoutHierarchy::OnItemRightClick)
EVT_MENU(ID_MENU_COLLAPSE, LayoutHierarchy::OnMenuCollapse)
EVT_MENU(ID_MENU_EXPAND, LayoutHierarchy::OnMenuExpand)
EVT_MENU(ID_MENU_CREATE_PREFAB, LayoutHierarchy::OnMenuCreatePrefab)
EVT_MENU(ID_MENU_CREATE_TEMPLATE, LayoutHierarchy::OnMenuCreateTemplate)
EVT_MENU(ID_MENU_COPY, LayoutHierarchy::OnMenuCopy)
EVT_MENU(ID_MENU_COPY_WITH_CHILDREN, LayoutHierarchy::OnMenuCopyWithChildren)
EVT_MENU(ID_MENU_CUT, LayoutHierarchy::OnMenuCut)
EVT_MENU(ID_MENU_PASTE, LayoutHierarchy::OnMenuPaste)
EVT_MENU(ID_MENU_LOCK_EXPAND, LayoutHierarchy::OnMenuLockExpand)
EVT_LIST_ITEM_SELECTED(ID_SEARCH_RESULT_LIST, LayoutHierarchy::OnSearchListSelect)
EVT_TEXT(ID_SEARCH, LayoutHierarchy::OnSearch)
EVT_TREE_STATE_IMAGE_CLICK(ID_TREE, LayoutHierarchy::OnItemStateClick)
EVT_BUTTON(ID_REFRESH_BUTTON, LayoutHierarchy::OnRefreshButton)
END_EVENT_TABLE()

LayoutHierarchy::LayoutHierarchy(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
EditorPanel(parent, position, size, style, name),
mRootId(NULL),
mHandleEvent(true)
{
	setup();
}

LayoutHierarchy::~LayoutHierarchy()
{
	TRACE_DELETE(mImageList);
	TRACE_DELETE(mStateImageList);
}

void LayoutHierarchy::setup()
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	wxBitmapButton* refreshButton = new wxBitmapButton(this, ID_REFRESH_BUTTON, wxBITMAP(IDB_REFRESH));
	bSizer2->Add(refreshButton, 0, wxALL, 5);

	wxStaticText* staticText = new wxStaticText(this, wxID_ANY, wxT("����"), wxDefaultPosition, wxDefaultSize, 0);
	staticText->Wrap(-1);
	bSizer2->Add(staticText, 0, wxALL, 5);

	mSearchTextCtrl = new wxTextCtrl(this, ID_SEARCH, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(mSearchTextCtrl, 2, wxALL | wxEXPAND, 5);
	bSizer1->Add(bSizer2, 0, wxEXPAND, 5);

	// �����б�Ĭ������
	mSearchResultList = new wxListCtrl(this, ID_SEARCH_RESULT_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_NO_HEADER);
	bSizer1->Add(mSearchResultList, 1, wxEXPAND, 5);
	mSearchResultList->Hide();

	mSearchResultList->InsertColumn(0, wxT("��һ"), 0, this->GetSize().GetWidth());

	mHierarchyTree = new wxTreeCtrl(this, ID_TREE, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS | wxTR_MULTIPLE);
	bSizer1->Add(mHierarchyTree, 1, wxALL | wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();
	this->Centre(wxBOTH);

	initTreeImage();
}

void LayoutHierarchy::initTreeImage()
{
	// ����ͼ��
	txMap<std::string, wxBitmap> windowIconList;
	windowIconList.insert("Button", wxBITMAP(IDB_BUTTON));
	windowIconList.insert("CheckBox", wxBITMAP(IDB_CHECKBOX));
	windowIconList.insert("EditBox", wxBITMAP(IDB_EDITBOX));
	windowIconList.insert("EffectRingWindow", wxBITMAP(IDB_EFFECTRINGWINDOW));
	windowIconList.insert("NumberWindow", wxBITMAP(IDB_NUMBERWINDOW));
	windowIconList.insert("ProgressWindow", wxBITMAP(IDB_PROGRESSWINDOW));
	windowIconList.insert("TextureAnimWindow", wxBITMAP(IDB_TEXTUREANIMWINDOW));
	windowIconList.insert("TextureWindow", wxBITMAP(IDB_TEXTUREWINDOW));
	windowIconList.insert("TextWindow", wxBITMAP(IDB_TEXTWINDOW));
	windowIconList.insert("Window", wxBITMAP(IDB_WINDOW));
	windowIconList.insert("VideoWindow", wxBITMAP(IDB_VIDEO_WINDOW));
	mImageList = TRACE_NEW(wxImageList, mImageList, 16, 16);
	auto iter = windowIconList.begin();
	auto iterEnd = windowIconList.end();
	FOR_STL(windowIconList, int i = 0; iter != iterEnd; (++iter, ++i))
	{
		mImageList->Add(iter->second);
		mWindowIconIndexList.insert(iter->first, i);
	}
	END_FOR_STL(windowIconList);
	mHierarchyTree->SetImageList(mImageList);

	// ������ʾ״̬ͼ��
	mStateImageList = TRACE_NEW(wxImageList, mStateImageList, 16, 16);
	mStateImageList->Add(wxBITMAP(IDB_WINDOW_SHOW));
	mStateImageList->Add(wxBITMAP(IDB_WINDOW_HIDE));
	mHierarchyTree->SetStateImageList(mStateImageList);
}

void LayoutHierarchy::displayLayout(::Layout* layout)
{
	mHierarchyTree->DeleteAllItems();
	mSearchResultList->DeleteAllItems();
	mLockExpandItemList.clear();
	mTreeItemMap.clear();
	if (layout == NULL)
	{
		return;
	}
	mRootId = mHierarchyTree->AddRoot(layout->getName());
	mTreeItemMap.insert(layout->getName(), mRootId);
	Window* rootWindow = layout->getRootWindow();
	if (rootWindow != NULL)
	{
		appendItem(mRootId, rootWindow);
	}
	expandItem(mRootId);
}

void LayoutHierarchy::setSelection(txVector<std::string>& itemLabel)
{
	setHandleEvent(false);
	mHierarchyTree->UnselectAll();
	int itemCount = itemLabel.size();
	FOR_STL(itemLabel, int i = 0; i < itemCount; ++i)
	{
		auto iter = mTreeItemMap.find(itemLabel[i]);
		if (iter != mTreeItemMap.end())
		{
			mHierarchyTree->SelectItem(iter->second, true);
		}
	}
	END_FOR_STL(itemLabel);
	setHandleEvent(true);
}

void LayoutHierarchy::unSelectAll()
{
	mHierarchyTree->UnselectAll();
}

void LayoutHierarchy::setVisibleIcon(Window* window)
{
	if (window == NULL)
	{
		return;
	}
	auto iterItem = mTreeItemMap.find(window->getName());
	if (iterItem != mTreeItemMap.end())
	{
		int stateIconIndex = window->getWindowVisible() ? 0 : 1;
		if (stateIconIndex != mHierarchyTree->GetItemState(iterItem->second))
		{
			mHierarchyTree->SetItemState(iterItem->second, stateIconIndex);
		}
	}
}

void LayoutHierarchy::addItem(Window* window, int pos)
{
	if (window == NULL)
	{
		return;
	}
	wxTreeItemId parentID = mRootId;
	if (window->getParent() != NULL)
	{
		auto iterItem = mTreeItemMap.find(window->getParent()->getName());
		if (iterItem != mTreeItemMap.end())
		{
			parentID = iterItem->second;
		}
		else
		{
			return;
		}
	}

	appendItem(parentID, window, pos);
	expandItem(parentID);
}

void LayoutHierarchy::deleteItem(const std::string& label)
{
	wxTreeItemId itemID;
	auto iterItem = mTreeItemMap.find(label);
	if (iterItem != mTreeItemMap.end())
	{
		itemID = iterItem->second;
	}
	else
	{
		return;
	}
	notifyItemDelete(itemID);
	mHierarchyTree->Delete(itemID);
}

void LayoutHierarchy::renameItem(const std::string& label, const std::string& newLabel)
{
	wxTreeItemId itemID;
	auto iterItem = mTreeItemMap.find(label);
	if (iterItem != mTreeItemMap.end())
	{
		itemID = iterItem->second;
	}
	else
	{
		EDITOR_CORE_ERROR("error : can not find window item in tree! window name : %s", label.c_str());
		return;
	}
	mHierarchyTree->SetItemText(itemID, newLabel);
	// ɾ���ɵ���,�����µ���
	mTreeItemMap.erase(iterItem);
	mTreeItemMap.insert(newLabel, itemID);
}

void LayoutHierarchy::moveItemPosition(const std::string& parentLabel, Window* window, int oldPosition, int newPosition)
{
	if (window == NULL)
	{
		return;
	}
	wxTreeItemId parentItemID;
	auto iterItem = mTreeItemMap.find(parentLabel);
	if (iterItem != mTreeItemMap.end())
	{
		parentItemID = iterItem->second;
	}
	else
	{
		EDITOR_CORE_ERROR("error : can not find parent item in tree! parent label : %s", parentLabel.c_str());
		return;
	}

	// �����ƶ��ڵ������ɾ������Ӷ�������ڵ��ѡ��״̬�ı�,����Ϊ�˱�֤��ǰ�ƶ��Ĵ���һֱ��ѡ��״̬,��Ҫ����ѡ���¼�
	setHandleEvent(false);

	// ɾ���ɵ���,����ɵ���������չ���б���,����Ҫ���б���ɾ��
	wxTreeItemIdValue cookie;
	wxTreeItemId childItem = mHierarchyTree->GetFirstChild(parentItemID, cookie);
	// ������ӽڵ�,����չ�����е��ӽڵ�
	for (int i = 0; childItem.IsOk(); ++i)
	{
		if (i == oldPosition)
		{
			if (window->getName() != mHierarchyTree->GetItemText(childItem))
			{
				EDITOR_CORE_ERROR("error : window position did not match! window name : %s, old pos : %d", window->getName().c_str(), oldPosition);
				return;
			}
			else
			{
				deleteItem(window->getName());
			}
			break;
		}
		childItem = mHierarchyTree->GetNextChild(parentItemID, cookie);
	}
	// ����µ���
	appendItem(parentItemID, window, newPosition);

	setHandleEvent(true);
}

void LayoutHierarchy::refreshChildItem(Window* window)
{
	if (window == NULL)
	{
		return;
	}
	// ɾ���ô����µ������ӽڵ�,��������ӽڵ�
	wxTreeItemId itemID;
	auto iterItem = mTreeItemMap.find(window->getName());
	if (iterItem != mTreeItemMap.end())
	{
		itemID = iterItem->second;
	}
	else
	{
		return;
	}
	wxTreeItemIdValue cookie;
	wxTreeItemId childItem = mHierarchyTree->GetFirstChild(itemID, cookie);
	// ������ӽڵ�,����չ�����е��ӽڵ�
	for (int i = 0; childItem.IsOk(); ++i)
	{
		notifyItemDelete(childItem);
		childItem = mHierarchyTree->GetNextChild(itemID, cookie);
	}
	mHierarchyTree->DeleteChildren(itemID);

	txVector<Window*>& childList = window->getChildren();
	int childCount = childList.size();
	FOR_STL(childList, int i = 0; i < childCount; ++i)
	{
		appendItem(itemID, childList[i]);
	}
	END_FOR_STL(childList);
	expandItem(itemID);
}

void LayoutHierarchy::resetParentItem(Window* window)
{
	if (window == NULL || window->getParent() == NULL)
	{
		return;
	}
	wxTreeItemId parentItem;
	auto iterParentItem = mTreeItemMap.find(window->getParent()->getName());
	if (iterParentItem != mTreeItemMap.end())
	{
		parentItem = iterParentItem->second;
	}
	else
	{
		return;
	}
	// ��ɾ���ɵĽڵ�
	deleteItem(window->getName());
	// �����µĽڵ�
	appendItem(parentItem, window, window->getParent()->getChildPos(window));
	expandItem(parentItem);
}

void LayoutHierarchy::OnItemSelected(wxTreeEvent& event)
{
	// ���������wx���¼���ֱ�ӷ���
	if (!mHandleEvent)
	{
		return;
	}

	setHandleEvent(false);

	::Layout* layout = mLayoutEditorCore->getCurLayout();
	
	txVector<Window*> selectWindowList;
	wxArrayTreeItemIds seletionIDList;
	int count = mHierarchyTree->GetSelections(seletionIDList);
	for (int i = 0; i < count; ++i)
	{
		wxTreeItemId selectedItem = seletionIDList[i];
		// ����ѡ�в���
		if (selectedItem != mRootId)
		{
			std::string secletString = mHierarchyTree->GetItemText(selectedItem).ToStdString();
			Window* windFind = layout->getWindow(secletString);
			if (windFind != NULL)
			{
				selectWindowList.push_back(windFind);
			}
		}
	}
	mLayoutEditorCore->setSelection(selectWindowList);

	setHandleEvent(true);
}

void LayoutHierarchy::OnItemBeginEdit(wxTreeEvent& event)
{
	mPreString = mHierarchyTree->GetItemText(event.GetItem()).ToStdString();
}

void LayoutHierarchy::OnItemEndEdit(wxTreeEvent& event)
{
	std::string newName = event.GetLabel().ToStdString();
	if (newName == EMPTY_STRING)
	{
		event.Veto();
	}
	else
	{
		::Layout* layout = mLayoutEditorCore->getCurLayout();
		Window* windFind = layout->getWindow(mPreString.ToStdString());

		// �ж�ѡ�����layout����window
		if (NULL != windFind)
		{
			bool result = false;
			CommandWindowRename* cmdRename = NEW_CMD(cmdRename);
			cmdRename->mNewName = newName;
			cmdRename->setResult(&result);
			mCommandSystem->pushCommand(cmdRename, windFind);
			if (!result)
			{
				event.Veto();
			}
		}
		else
		{
			bool succsee = mLayoutEditorCore->renameLayout(newName);
			if (!succsee)
			{
				event.Veto();
			}
		}
	}
}

void LayoutHierarchy::OnItemKeyDown(wxTreeEvent& event)
{
	int keyCode = event.GetKeyEvent().GetRawKeyCode();
	if (VK_DELETE == keyCode)
	{
		// ������ѡ�����ı�����һ���б���
		wxArrayTreeItemIds seletionIDList;
		int count = mHierarchyTree->GetSelections(seletionIDList);
		txVector<std::string> itemTextList;
		for (int i = 0; i < count; ++i)
		{
			std::string text = mHierarchyTree->GetItemText(seletionIDList[i]).ToStdString();
			itemTextList.push_back(text);
		}

		// �ٸ����б��е�����ɾ������
		int itemCount = itemTextList.size();
		FOR_STL(itemTextList, int i = 0; i < itemCount; ++i)
		{
			// ���ɾ�����ڵ�,��رյ�ǰ�����ļ�
			if (itemTextList[i] == mLayoutEditorCore->getCurLayout()->getName())
			{
				mEditorFrame->CloseLayout();
				break;
			}
			// ɾ����Ӧ�Ĵ���
			else
			{
				mLayoutEditorCore->deleteWindow(itemTextList[i]);
			}
		}
		END_FOR_STL(itemTextList);
	}
}

void LayoutHierarchy::OnItemRightClick(wxTreeEvent& event)
{
	wxTreeItemId itemId = event.GetItem();
	wxCHECK_RET(itemId.IsOk(), "should have a valid item");
	// ����Ҽ�����
	wxPoint clientpt = event.GetPoint();
	// ��ʾ�˵�
	showMenu(clientpt);
	event.Skip();
}

void LayoutHierarchy::OnMenuCollapse(wxCommandEvent& event)
{
	// �ݹ齫�����ӽڵ㶼�۵�
	collapseItem(mHierarchyTree->GetFocusedItem());
}

void LayoutHierarchy::OnMenuExpand(wxCommandEvent& event)
{
	// �ݹ齫�����ӽڵ㶼չ��
	expandItem(mHierarchyTree->GetFocusedItem());
}

void LayoutHierarchy::OnMenuCreatePrefab(wxCommandEvent& event)
{
	std::string windowName = mHierarchyTree->GetItemText(mHierarchyTree->GetFocusedItem()).ToStdString();
	mLayoutEditorCore->savePrefab(windowName);
}

void LayoutHierarchy::OnMenuCreateTemplate(wxCommandEvent& event)
{
	std::string windowName = mHierarchyTree->GetItemText(mHierarchyTree->GetFocusedItem()).ToStdString();
	mLayoutEditorCore->saveTemplate(windowName);
}

void LayoutHierarchy::OnMenuCopy(wxCommandEvent& event)
{
	std::string windowName = mHierarchyTree->GetItemText(mHierarchyTree->GetFocusedItem()).ToStdString();
	mLayoutEditorCore->copyWindow(windowName);
}

void LayoutHierarchy::OnMenuCopyWithChildren(wxCommandEvent& event)
{
	std::string windowName = mHierarchyTree->GetItemText(mHierarchyTree->GetFocusedItem()).ToStdString();
	mLayoutEditorCore->copyWindow(windowName, true);
}

void LayoutHierarchy::OnMenuCut(wxCommandEvent& event)
{
	std::string windowName = mHierarchyTree->GetItemText(mHierarchyTree->GetFocusedItem()).ToStdString();
	mLayoutEditorCore->copyWindow(windowName, false, true);
}

void LayoutHierarchy::OnMenuPaste(wxCommandEvent& event)
{
	std::string windowName = mHierarchyTree->GetItemText(mHierarchyTree->GetFocusedItem()).ToStdString();
	mLayoutEditorCore->pasteWindow(windowName);
}

void LayoutHierarchy::OnMenuLockExpand(wxCommandEvent& event)
{
	wxTreeItemId item = mHierarchyTree->GetFocusedItem();
	auto iterLock = mLockExpandItemList.find(item);
	if (iterLock == mLockExpandItemList.end())
	{
		// �ı���ɫ����Ϊ��ɫ,���Ҽ��������б���
		mHierarchyTree->SetItemTextColour(item, wxColour(127, 127, 127, 255));
		mLockExpandItemList.insert(mHierarchyTree->GetFocusedItem());
	}
	else
	{
		// �ı���ɫ����Ϊ��ɫ,���Ҵ������б���ɾ��
		mHierarchyTree->SetItemTextColour(item, wxColour(0, 0, 0, 255));
		mLockExpandItemList.erase(iterLock);
	}
}

void LayoutHierarchy::OnRefreshButton(wxCommandEvent& event)
{
	displayLayout(mLayoutEditorCore->getCurLayout());
}

void LayoutHierarchy::OnSearch(wxCommandEvent& event)
{
	wxString value = mSearchTextCtrl->GetValue();
	showSearch(value.ToStdString());
}

void LayoutHierarchy::OnSearchListSelect(wxListEvent& event)
{
	::Layout* layout = mLayoutEditorCore->getCurLayout();
	txVector<Window*> selectWindowList;
	long item = mSearchResultList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	while (item != -1)
	{
		std::string secletString = mSearchResultList->GetItemText(item).ToStdString();
		Window* windFind = layout->getWindow(secletString);
		if (windFind != NULL)
		{
			selectWindowList.push_back(windFind);
		}
		item = mSearchResultList->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	}
	mLayoutEditorCore->setSelection(selectWindowList);
}

void LayoutHierarchy::OnItemStateClick(wxTreeEvent& event)
{
	::Layout* layout = mLayoutEditorCore->getCurLayout();
	if (layout != NULL)
	{
		wxTreeItemId itemId = event.GetItem();
		wxString itemText = mHierarchyTree->GetItemText(itemId);
		Window* window = layout->getWindow(itemText.ToStdString());
		if (window != NULL)
		{
			bool value = !window->getWindowVisible();
			CommandWindowSetProperty* cmdProperty = NEW_CMD(cmdProperty);
			cmdProperty->mPropertyName = txProperty::getPropertyName<WindowPropertyVisible>();
			cmdProperty->mPropertyValue = txStringUtility::boolToString(value);
			cmdProperty->mOperator = this;
			mCommandSystem->pushCommand(cmdProperty, window);
		}
	}
}

void LayoutHierarchy::appendItem(wxTreeItemId parent, Window* window, int pos)
{
	if (window == NULL)
	{
		return;
	}
	// �Ȱ��Լ���ӵ��ڵ���,Ȼ���ٰ��Լ��������Ӵ�����ӵ��Լ��Ľڵ���
	int iconIndex = getWindowIconIndex(window->getTypeName());
	wxTreeItemId windowId = NULL;
	if (pos >= 0 && pos < (int)mHierarchyTree->GetChildrenCount(parent, false))
	{
		windowId = mHierarchyTree->InsertItem(parent, pos, window->getName(), iconIndex);
	}
	else
	{
		windowId = mHierarchyTree->AppendItem(parent, window->getName(), iconIndex);
	}
	int stateIconIndex = window->getWindowVisible() ? 0 : 1;
	mHierarchyTree->SetItemState(windowId, stateIconIndex);
	mTreeItemMap.insert(window->getName(), windowId);
	txVector<Window*>& childList = window->getChildren();
	int childWindowNumber = childList.size();
	FOR_STL(childList, int i = 0; i < childWindowNumber; ++i)
	{
		appendItem(windowId, childList[i]);
	}
	END_FOR_STL(childList);
}

int LayoutHierarchy::getWindowIconIndex(std::string windowName)
{
	auto iter = mWindowIconIndexList.find(windowName);
	if (iter != mWindowIconIndexList.end())
	{
		return iter->second;
	}
	return -1;
}

void LayoutHierarchy::collapseItem(wxTreeItemId item)
{
	if (!item.IsOk())
	{
		return;
	}
	// ���������չ������,���ٱ����۵�
	if (mLockExpandItemList.find(item) != mLockExpandItemList.end())
	{
		return;
	}
	wxTreeItemIdValue cookie;
	wxTreeItemId childItem = mHierarchyTree->GetFirstChild(item, cookie);
	// ������ӽڵ�,�����۵����е��ӽڵ�
	while (childItem.IsOk())
	{
		collapseItem(childItem);
		childItem = mHierarchyTree->GetNextChild(item, cookie);
	}
	// �۵��������ӽڵ��,�۵��Լ�
	mHierarchyTree->Collapse(item);
}

void LayoutHierarchy::expandItem(wxTreeItemId item)
{
	if (!item.IsOk())
	{
		return;
	}
	// ���������չ������,����չ��
	if (mLockExpandItemList.find(item) != mLockExpandItemList.end())
	{
		return;
	}
	wxTreeItemIdValue cookie;
	wxTreeItemId childItem = mHierarchyTree->GetFirstChild(item, cookie);
	// ������ӽڵ�,����չ�����е��ӽڵ�
	while (childItem.IsOk())
	{
		expandItem(childItem);
		childItem = mHierarchyTree->GetNextChild(item, cookie);
	}
	// չ���������ӽڵ��,չ���Լ�
	mHierarchyTree->Expand(item);
}

void LayoutHierarchy::notifyItemDelete(wxTreeItemId item)
{
	wxTreeItemIdValue cookie;
	wxTreeItemId childItem = mHierarchyTree->GetFirstChild(item, cookie);
	// ������ӽڵ�,���ȱ������е��ӽڵ�
	while (childItem.IsOk())
	{
		notifyItemDelete(childItem);
		childItem = mHierarchyTree->GetNextChild(item, cookie);
	}
	// ���Լ����б���ɾ��
	std::string itemLabel = mHierarchyTree->GetItemText(item).ToStdString();
	auto iterTree = mTreeItemMap.find(itemLabel);
	if (iterTree == mTreeItemMap.end())
	{
		EDITOR_CORE_ERROR("error : can not find item in tree item map! item label : %s", itemLabel.c_str());
		return;
	}
	mTreeItemMap.erase(iterTree);
	// ���ɾ������ʱ����չ����,����Ҫ���б���ɾ��
	auto iterLock = mLockExpandItemList.find(item);
	if (iterLock != mLockExpandItemList.end())
	{
		mLockExpandItemList.erase(iterLock);
	}
}

void LayoutHierarchy::showMenu(const wxPoint& pt)
{
	wxMenu menu;
	menu.Append(ID_MENU_COLLAPSE, wxT("�۵�"));
	menu.Append(ID_MENU_EXPAND, wxT("չ��"));
	menu.Append(ID_MENU_CREATE_PREFAB, wxT("����Ԥ��"));
	menu.Append(ID_MENU_CREATE_TEMPLATE, wxT("����ģ��"));
	menu.Append(ID_MENU_COPY, wxT("����"));
	menu.Append(ID_MENU_COPY_WITH_CHILDREN, wxT("����(���Ӵ���)"));
	menu.Append(ID_MENU_CUT, wxT("����"));
	menu.Append(ID_MENU_PASTE, wxT("ճ��"));
	menu.Append(ID_MENU_LOCK_EXPAND, wxT("����չ��"));
	PopupMenu(&menu, pt);// ��ʾ�����˵�  
}

void LayoutHierarchy::showSearch(const std::string& value)
{
	if (value == EMPTY_STRING)
	{
		mHierarchyTree->Show(true);
		mSearchResultList->Hide();
		return; 
	}

	mSearchResultList->DeleteAllItems();
	mHierarchyTree->Hide();
	mSearchResultList->Show(true);
	mEditorFrame->getAuiManager().Update();

	// ��õ�ǰGUI����
	::Layout* curLayout = mLayoutEditorCore->getCurLayout();
	if (NULL != curLayout)
	{
		auto& windowList = curLayout->getWindowList();
		auto retList = txStringUtility::findSubstr(windowList, value, false);
		int listSize = retList.size();
		FOR_STL(retList, int i = 0; i < listSize; ++i)
		{
			mSearchResultList->InsertItem(mSearchResultList->GetItemCount(), wxString(retList[i].c_str()));
		}
		END_FOR_STL(retList);
	}
}