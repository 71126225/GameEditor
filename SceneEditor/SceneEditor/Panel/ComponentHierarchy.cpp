#include "txUtility.h"
#include "PropertyHeader.h"
#include "txComponentHeader.h"

#include "EditorCoreRoot.h"
#include "ECEditor.h"
#include "GameLog.h"
#include "SceneEditorCore.h"
#include "ComponentEditorCore.h"

#include "ComponentHierarchy.h"
#include "EditorCommandHeader.h"

enum
{
	ID_TREE,
	ID_REFRESH_BUTTON,
};

BEGIN_EVENT_TABLE(ComponentHierarchy, EditorPanel)
EVT_TREE_ITEM_ACTIVATED(ID_TREE, ComponentHierarchy::OnItemSelected)
EVT_TREE_SEL_CHANGED(ID_TREE, ComponentHierarchy::OnItemSelected)
EVT_TREE_BEGIN_LABEL_EDIT(ID_TREE, ComponentHierarchy::OnItemBeginEdit)
EVT_TREE_END_LABEL_EDIT(ID_TREE, ComponentHierarchy::OnItemEndEdit)
EVT_TREE_KEY_DOWN(ID_TREE, ComponentHierarchy::OnItemKeyDown)
EVT_TREE_ITEM_RIGHT_CLICK(ID_TREE, ComponentHierarchy::OnItemRightClick)
EVT_TREE_STATE_IMAGE_CLICK(ID_TREE, ComponentHierarchy::OnItemStateClick)
EVT_TOOL(ID_REFRESH_BUTTON, ComponentHierarchy::OnRefreshButton)
END_EVENT_TABLE()

ComponentHierarchy::ComponentHierarchy(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
EditorPanel(parent, position, size, style, name),
mRootId(NULL),
mHandleEvent(true)
{
	setup();
}

ComponentHierarchy::~ComponentHierarchy()
{
	TRACE_DELETE(mImageList);
	TRACE_DELETE(mStateImageList);
}

void ComponentHierarchy::setup()
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	mAuiToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_LAYOUT);
	mAuiToolBar->AddTool(ID_REFRESH_BUTTON, "ˢ��", wxBITMAP(IDB_REFRESH));
	mAuiToolBar->Realize();
	bSizer2->Add(mAuiToolBar, 0, wxALL, 5);

	bSizer1->Add(bSizer2, 0, wxEXPAND, 5);

	mHierarchyTree = new wxTreeCtrl(this, ID_TREE, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS | wxTR_MULTIPLE);
	bSizer1->Add(mHierarchyTree, 1, wxALL | wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();
	this->Centre(wxBOTH);

	initTreeImage();
}

void ComponentHierarchy::initTreeImage()
{
	// ����ͼ��
	txMap<std::string, wxBitmap> bitmapList;
	bitmapList.insert(TOSTRING(txComponentAlpha), wxBITMAP(IDB_COMPONENT_ALPHA));
	bitmapList.insert(TOSTRING(txComponentAudio), wxBITMAP(IDB_COMPONENT_AUDIO));
	bitmapList.insert(TOSTRING(txComponentCollisionBox), wxBITMAP(IDB_COMPONENT_COLLISION_BOX));
	bitmapList.insert(TOSTRING(txComponentCollisionCapsule), wxBITMAP(IDB_COMPONENT_COLLISION_CAPSULE));
	bitmapList.insert(TOSTRING(txComponentCollisionCylinder), wxBITMAP(IDB_COMPONENT_COLLISION_CYLINDER));
	bitmapList.insert(TOSTRING(txComponentCollisionPlane), wxBITMAP(IDB_COMPONENT_COLLISION_PLANE));
	bitmapList.insert(TOSTRING(txComponentCollisionSphere), wxBITMAP(IDB_COMPONENT_COLLISION_SPHERE));
	bitmapList.insert(TOSTRING(txComponentDynamicMesh), wxBITMAP(IDB_COMPONENT_DYNAMIC_MESH));
	bitmapList.insert(TOSTRING(txComponentEffect), wxBITMAP(IDB_COMPONENT_EFFECT));
	bitmapList.insert(TOSTRING(txComponentHSL), wxBITMAP(IDB_COMPONENT_HSL));
	bitmapList.insert(TOSTRING(txComponentMove), wxBITMAP(IDB_COMPONENT_MOVE));
	bitmapList.insert(TOSTRING(txComponentPhysics), wxBITMAP(IDB_COMPONENT_PHYSICS));
	bitmapList.insert(TOSTRING(txComponentRotateAccelerateToTarget), wxBITMAP(IDB_COMPONENT_ROTATE_ACCELERATE_TO_TARGET));
	bitmapList.insert(TOSTRING(txComponentRotateSpeed), wxBITMAP(IDB_COMPONENT_ROTATE_SPEED));
	bitmapList.insert(TOSTRING(txComponentRotateToTarget), wxBITMAP(IDB_COMPONENT_ROTATE_TO_TARGET));
	bitmapList.insert(TOSTRING(txComponentScale), wxBITMAP(IDB_COMPONENT_SCALE));
	bitmapList.insert(TOSTRING(txComponentStaticMesh), wxBITMAP(IDB_COMPONENT_STATIC_MESH));
	bitmapList.insert(TOSTRING(txComponentTransform), wxBITMAP(IDB_COMPONENT_TRANSFORM));
	bitmapList.insert(TOSTRING(txComponentTrembling), wxBITMAP(IDB_COMPONENT_TREMBLING));
	bitmapList.insert(TOSTRING(txComponentTwinkle), wxBITMAP(IDB_COMPONENT_TWINKLE));

	mImageList = TRACE_NEW(wxImageList, mImageList, 16, 16);
	auto iter = bitmapList.begin();
	auto iterEnd = bitmapList.end();
	FOR_STL(bitmapList, int i = 0; iter != iterEnd; (++iter, ++i))
	{
		mImageList->Add(iter->second);
		mWindowIconIndexList.insert(iter->first, i);
	}
	END_FOR_STL(bitmapList);
	mHierarchyTree->SetImageList(mImageList);

	// ������ʾ״̬ͼ��
	mStateImageList = TRACE_NEW(wxImageList, mStateImageList, 16, 16);
	mStateImageList->Add(wxBITMAP(IDB_ENABLED_COMPONENT));
	mStateImageList->Add(wxBITMAP(IDB_DISABLED_COMPONENT));
	mHierarchyTree->SetStateImageList(mStateImageList);
}

void ComponentHierarchy::showComponent(txComponentOwner* owner)
{
	mHierarchyTree->DeleteAllItems();
	mTreeItemMap.clear();
	if (owner == NULL)
	{
		return;
	}
	mRootId = mHierarchyTree->AddRoot(owner->getName());
	mTreeItemMap.insert(owner->getName(), mRootId);
	// ��һ�������,Ȼ��ݹ���ʾ�������
	txVector<txComponent*>& componentList = owner->getRootComponentList();
	int componentCount = componentList.size();
	FOR_STL(componentList, int i = 0; i < componentCount; ++i)
	{
		appendItem(mRootId, componentList[i]);
	}
	END_FOR_STL(componentList);
	expandItem(mRootId);
}

void ComponentHierarchy::setSelection(txVector<std::string>& itemLabel)
{
	// �Ƚ�ֹ��Ӧwx�¼�,������ܻ��������ѭ��������������Ԥ֪�Ĵ���
	setHandleEvent(false);

	mHierarchyTree->UnselectAll();
	int itemCount = itemLabel.size();
	FOR_STL(itemLabel, int i = 0; i < itemCount; ++i)
	{
		txMap<std::string, wxTreeItemId>::iterator iter = mTreeItemMap.find(itemLabel[i]);
		if (iter != mTreeItemMap.end())
		{
			mHierarchyTree->SelectItem(iter->second, true);
		}
	}
	END_FOR_STL(itemLabel);

	setHandleEvent(true);
}

void ComponentHierarchy::unSelectAll()
{
	mHierarchyTree->UnselectAll();
}

void ComponentHierarchy::setActiveIcon(txComponent* component)
{
	if (component == NULL)
	{
		return;
	}
	auto iterItem = mTreeItemMap.find(component->getName());
	if (iterItem != mTreeItemMap.end())
	{
		int stateIconIndex = component->isComponentActive() ? 0 : 1;
		if (stateIconIndex != mHierarchyTree->GetItemState(iterItem->second))
		{
			mHierarchyTree->SetItemState(iterItem->second, stateIconIndex);
		}
	}
}

void ComponentHierarchy::refreshActiveIcon(txComponentOwner* owner)
{
	auto& componentList = owner->getAllComponent();
	auto iterComponent = componentList.begin();
	auto iterComponentEnd = componentList.end();
	FOR_STL(componentList, ; iterComponent != iterComponentEnd; ++iterComponent)
	{
		setActiveIcon(iterComponent->second);
	}
	END_FOR_STL(componentList);
}

void ComponentHierarchy::addItem(txComponent* component, int pos)
{
	if (component == NULL)
	{
		return;
	}
	wxTreeItemId parentID = mRootId;
	if (component->getParentComponent() != NULL)
	{
		auto iterItem = mTreeItemMap.find(component->getParentComponent()->getName());
		if (iterItem != mTreeItemMap.end())
		{
			parentID = iterItem->second;
		}
		else
		{
			return;
		}
	}
	appendItem(parentID, component, pos);
	expandItem(parentID);
}

void ComponentHierarchy::deleteItem(const std::string& label)
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

void ComponentHierarchy::renameItem(const std::string& label, const std::string& newLabel)
{
	wxTreeItemId itemID;
	auto iterItem = mTreeItemMap.find(label);
	if (iterItem != mTreeItemMap.end())
	{
		itemID = iterItem->second;
	}
	else
	{
		GAME_ERROR("error : can not find window item in tree! component name : %s", label.c_str());
		return;
	}
	mHierarchyTree->SetItemText(itemID, newLabel);
	// ɾ���ɵ���,�����µ���
	mTreeItemMap.erase(iterItem);
	mTreeItemMap.insert(newLabel, itemID);
}

void ComponentHierarchy::moveItemPosition(const std::string& parentLabel, txComponent* component, int oldPosition, int newPosition)
{
	if (component == NULL)
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
		GAME_ERROR("error : can not find parent item in tree! parent label : %s", parentLabel.c_str());
		return;
	}

	// �����ƶ���������ɾ������Ӷ������������ѡ��״̬�ı�,����Ϊ�˱�֤��ǰ�ƶ������һֱ��ѡ��״̬,��Ҫ����ѡ���¼�
	setHandleEvent(false);
	// ɾ���ɵ���,����ɵ���������չ���б���,����Ҫ���б���ɾ��
	wxTreeItemIdValue cookie;
	wxTreeItemId childItem = mHierarchyTree->GetFirstChild(parentItemID, cookie);
	// ����������,����չ�����е������
	for (int i = 0; childItem.IsOk(); ++i)
	{
		if (i == oldPosition)
		{
			if (component->getName() != mHierarchyTree->GetItemText(childItem))
			{
				GAME_ERROR("error : component position did not match! component name : %s, old pos : %d", component->getName().c_str(), oldPosition);
				return;
			}
			else
			{
				deleteItem(component->getName());
			}
			break;
		}
		childItem = mHierarchyTree->GetNextChild(parentItemID, cookie);
	}
	// ����µ���
	appendItem(parentItemID, component, newPosition);
	setHandleEvent(true);
}

void ComponentHierarchy::refreshChildItem(txComponent* component)
{
	if (component == NULL)
	{
		return;
	}
	// ɾ��������µ����������,������������
	wxTreeItemId itemID;
	auto iterItem = mTreeItemMap.find(component->getName());
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
	// ����������,����չ�����е������
	for (int i = 0; childItem.IsOk(); ++i)
	{
		notifyItemDelete(childItem);
		childItem = mHierarchyTree->GetNextChild(itemID, cookie);
	}
	mHierarchyTree->DeleteChildren(itemID);

	txVector<txComponent*>& childList = component->getChildComponentList();
	int childCount = childList.size();
	FOR_STL(childList, int i = 0; i < childCount; ++i)
	{
		appendItem(itemID, childList[i]);
	}
	END_FOR_STL(childList);
	expandItem(itemID);
}

void ComponentHierarchy::OnItemSelected(wxTreeEvent& event)
{
	// ���������wx���¼���ֱ�ӷ���
	if (!mHandleEvent)
	{
		return;
	}

	setHandleEvent(false);

	if (mSceneEditorCore->getEditorSelections().size() > 0)
	{
		ECEditor* editor = mSceneEditorCore->getEditorSelections()[0];
		txVector<txComponent*> selectWindowList;
		wxArrayTreeItemIds seletionIDList;
		int count = mHierarchyTree->GetSelections(seletionIDList);
		for (int i = 0; i < count; ++i)
		{
			wxTreeItemId selectedItem = seletionIDList[i];
			if (selectedItem != mRootId)
			{
				std::string secletString = mHierarchyTree->GetItemText(selectedItem).ToStdString();
				txComponent* component = editor->getComponent(secletString);
				if (component != NULL)
				{
					selectWindowList.push_back(component);
				}
			}
		}
		mComponentEditorCore->setComponentSelection(selectWindowList);
	}

	setHandleEvent(true);
}

void ComponentHierarchy::OnItemBeginEdit(wxTreeEvent& event)
{
	mPreString = mHierarchyTree->GetItemText(event.GetItem()).ToStdString();
}

void ComponentHierarchy::OnItemEndEdit(wxTreeEvent& event)
{
	std::string newName = event.GetLabel().ToStdString();
	if (newName == EMPTY_STRING)
	{
		event.Veto();
	}
	else
	{
		if (mSceneEditorCore->getEditorSelections().size() > 0)
		{
			ECEditor* editor = mSceneEditorCore->getEditorSelections()[0];
			txComponent* component = editor->getComponent(mPreString.ToStdString());
			// ֻ�ܸ����������
			if (component != NULL)
			{
				bool result = false;
				CommandEditorRenameComponent* cmdRename = NEW_CMD(cmdRename);
				cmdRename->mComponentName = component->getName();
				cmdRename->mNewName = newName;
				cmdRename->setResult(&result);
				mCommandSystem->pushCommand(cmdRename, editor);
				if (!result)
				{
					event.Veto();
				}
			}
		}
	}
}

void ComponentHierarchy::OnItemKeyDown(wxTreeEvent& event)
{
	if (mSceneEditorCore->getEditorSelections().size() == 0)
	{
		return;
	}
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

		ECEditor* editor = mSceneEditorCore->getEditorSelections()[0];
		// �ٸ����б��е�����ɾ�����,�Ӻ���ǰɾ��
		int itemCount = itemTextList.size();
		FOR_STL(itemTextList, int i = itemCount - 1; i >= 0; --i)
		{
			if (itemTextList[i] != editor->getName())
			{
				CommandEditorDeleteComponent* cmdDelete = NEW_CMD(cmdDelete);
				cmdDelete->mComponentName = itemTextList[i];
				mCommandSystem->pushCommand(cmdDelete, editor);
			}
		}
		END_FOR_STL(itemTextList);
	}
}

void ComponentHierarchy::OnItemRightClick(wxTreeEvent& event)
{
	event.Skip();
}

void ComponentHierarchy::OnRefreshButton(wxCommandEvent& event)
{
	if (mSceneEditorCore->getEditorSelections().size() > 0)
	{
		showComponent(mSceneEditorCore->getEditorSelections()[0]);
	}
	else
	{
		showComponent(NULL);
	}
	// ˢ��ʱȡ���������ѡ����
	mComponentEditorCore->clearComponentSelections();
}

void ComponentHierarchy::OnItemStateClick(wxTreeEvent& event)
{
	if (mSceneEditorCore->getEditorSelections().size() == 0)
	{
		return;
	}
	ECEditor* editor = mSceneEditorCore->getEditorSelections()[0];
	wxTreeItemId itemId = event.GetItem();
	wxString itemText = mHierarchyTree->GetItemText(itemId);
	txComponent* component = editor->getComponent(itemText.ToStdString());
	if (component != NULL)
	{
		CommandEditorSetProperty* cmdProperty = NEW_CMD(cmdProperty);
		cmdProperty->mComponentName = component->getName();
		cmdProperty->mPropertyName = txProperty::getPropertyName<txComponentPropertyActive>();
		cmdProperty->mPropertyValue = txStringUtility::boolToString(!component->isComponentActive());
		cmdProperty->mChangeOperator = this;
		mCommandSystem->pushCommand(cmdProperty, editor);
	}
}

void ComponentHierarchy::appendItem(wxTreeItemId parent, txComponent* component, int pos)
{
	if (component == NULL)
	{
		return;
	}
	// �Ȱ��Լ���ӵ��ڵ���,Ȼ���ٰ��Լ��������Ӵ�����ӵ��Լ��Ľڵ���
	int iconIndex = getWindowIconIndex(component->getType());
	wxTreeItemId windowId = NULL;
	if (txMath::isInRange(pos, 0, (int)mHierarchyTree->GetChildrenCount(parent, false) - 1))
	{
		windowId = mHierarchyTree->InsertItem(parent, pos, component->getName(), iconIndex);
	}
	else
	{
		windowId = mHierarchyTree->AppendItem(parent, component->getName(), iconIndex);
	}
	int stateIconIndex = component->isComponentActive() ? 0 : 1;
	mHierarchyTree->SetItemState(windowId, stateIconIndex);
	mTreeItemMap.insert(component->getName(), windowId);
	txVector<txComponent*>& childList = component->getChildComponentList();
	int childWindowNumber = childList.size();
	FOR_STL(childList, int i = 0; i < childWindowNumber; ++i)
	{
		appendItem(windowId, childList[i]);
	}
	END_FOR_STL(childList);
}

int ComponentHierarchy::getWindowIconIndex(std::string windowName)
{
	auto iter = mWindowIconIndexList.find(windowName);
	if (iter != mWindowIconIndexList.end())
	{
		return iter->second;
	}
	return -1;
}

void ComponentHierarchy::collapseItem(wxTreeItemId item)
{
	if (!item.IsOk())
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

void ComponentHierarchy::expandItem(wxTreeItemId item)
{
	if (!item.IsOk())
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

void ComponentHierarchy::notifyItemDelete(wxTreeItemId item)
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
		GAME_ERROR("error : can not find item in tree item map! item label : %s", itemLabel.c_str());
		return;
	}
	mTreeItemMap.erase(iterTree);
}