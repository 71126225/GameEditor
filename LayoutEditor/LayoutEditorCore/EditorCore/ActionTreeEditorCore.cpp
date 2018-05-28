#include "txAction.h"
#include "txActionTree.h"
#include "txActionTreeManager.h"
#include "ActionTreeEditorCore.h"
#include "EditorCoreRoot.h"
#include "ECLog.h"
#include "Utility.h"

ActionTreeEditorCore::ActionTreeEditorCore()
:
mCurAction(NULL),
mCurActionTree(NULL),
mActionTreeModified(false)
{
	;
}

void ActionTreeEditorCore::loadActionTree(const std::string& tree)
{
	if (mActionTreeModified && mCurActionTree != NULL && mCurActionTree->getName() != tree)
	{
		askSaveActionTree("�Ƿ񱣴浱ǰ����Ϊ��?");
	}
	// ���ѡ����,Ȼ�������Ϊ��
	selectActionTree(EMPTY_STRING);
	selectAction(EMPTY_STRING);
	txActionTree* action = mActionTreeManager->getActionTree(tree);
	// ���ͼ�����Ϊ�����¼�
	mEditorCoreRoot->sendEvent(CE_LOAD_ACTION_TREE, tree);
	// ѡ�и���Ϊ��
	selectActionTree(action->getName());
	// ���Ҫ����֪ͨ�¼�
	setActionTreeModified(false, true);
}

void ActionTreeEditorCore::createActionTree(const std::string& name, const bool& createRoot)
{
	if (mActionTreeModified && mCurActionTree != NULL)
	{
		askSaveActionTree("�Ƿ񱣴浱ǰ����Ϊ��?");
	}
	selectActionTree(EMPTY_STRING);
	selectAction(EMPTY_STRING);
	txActionTree* actionTree = mActionTreeManager->createActionTree(name, createRoot);
	if (actionTree == NULL)
	{
		return;
	}
	// ���ͼ�����Ϊ�����¼�
	mEditorCoreRoot->sendEvent(CE_NEW_ACTION_TREE, actionTree->getName());
	// ѡ�и���Ϊ��
	selectActionTree(actionTree->getName());
	// ���Ҫ�����޸�״̬
	setActionTreeModified(true, true);
}

void ActionTreeEditorCore::deleteActionTree(const std::string& name)
{
	// ��Ϊname������,������Ҫ����һ����ʱ�ı���
	std::string treeName = name;
	bool ret = mActionTreeManager->destroyActionTree(treeName);
	if (ret)
	{
		selectActionTree(EMPTY_STRING);
		selectAction(EMPTY_STRING);
		mEditorCoreRoot->sendEvent(CE_DELETE_ACTION_TREE, treeName);
	}
}

void ActionTreeEditorCore::selectActionTree(const std::string& name)
{
	if (mActionTreeModified && mCurActionTree != NULL)
	{
		askSaveActionTree("�Ƿ񱣴浱ǰ����Ϊ��?");
	}
	mCurActionTree = mActionTreeManager->getActionTree(name);
	// ����ѡ����Ϊ�����¼�
	mEditorCoreRoot->sendEvent(CE_SELECT_ACTION_TREE, mCurActionTree != NULL ? mCurActionTree->getName() : EMPTY_STRING);
}

void ActionTreeEditorCore::addAction(const std::string& actionType, const std::string& actionName, const std::string& parentName)
{
	if (mCurActionTree == NULL)
	{
		return;
	}
	txAction* parent = mCurActionTree->getAction(parentName);
	if (parent == NULL && mCurActionTree->getRootAction())
	{
		EDITOR_CORE_ERROR("��Ϊ���Ѿ��и��ڵ���,�޷�����Ӹ��ڵ�,��ѡ��һ�����ڵ��������Ϊ!");
		return;
	}
	txAction* action = mCurActionTree->addAction(actionType, actionName, parent);
	if (action == NULL)
	{
		return;
	}
	// ���������Ϊ���¼�
	txVector<std::string> params;
	params.push_back(action->getName());
	params.push_back(parentName);
	mEditorCoreRoot->sendEvent(CE_ADD_ACTION, params);
	// �����޸�״̬
	setActionTreeModified(true, true);
}

void ActionTreeEditorCore::deleteAction(const std::string& actionName)
{
	if (mCurActionTree == NULL)
	{
		return;
	}
	clearActionSelection();
	// ��ΪactionName��������������,����Ϊ�˱������,��Ҫʹ��һ����ʱ����
	std::string name = actionName;
	bool ret = mCurActionTree->deleteAction(name);
	if (!ret)
	{
		return;
	}
	// ����ɾ����Ϊ���¼�
	mEditorCoreRoot->sendEvent(CE_DELETE_ACTION, name);
	// �����޸�״̬
	setActionTreeModified(true, true);
}

void ActionTreeEditorCore::setActionTreeModified(const bool& modified, const bool& force, const bool& sendEvt)
{
	if (mActionTreeModified != modified || force)
	{
		mActionTreeModified = modified;
		if (sendEvt)
		{
			// �����޸�״̬�ı���¼�
			txVector<std::string> params;
			params.push_back(mCurActionTree ? mCurActionTree->getName() : EMPTY_STRING);
			params.push_back(txStringUtility::boolToString(mActionTreeModified));
			mEditorCoreRoot->sendEvent(CE_ACTION_TREE_MODIFIED, params);
		}
	}
}

void ActionTreeEditorCore::saveActionTree()
{
	;
}

void ActionTreeEditorCore::selectAction(const std::string& actionName)
{
	// �����ظ�����ѡ����,��������ѭ���ĵݹ����
	if (mCurAction != NULL && mCurAction->getName() == actionName)
	{
		return;
	}
	clearActionSelection(true);
	if (mCurActionTree != NULL)
	{
		mCurAction = mCurActionTree->getAction(actionName);
	}
	mEditorCoreRoot->sendEvent(CE_ACTION_SELECTION_CHANGED);
}

void ActionTreeEditorCore::reselectAction()
{
	mEditorCoreRoot->sendEvent(CE_ACTION_SELECTION_CHANGED);
}

void ActionTreeEditorCore::clearActionSelection(const bool& sendEvt)
{
	mCurAction = NULL;
	if (sendEvt)
	{
		mEditorCoreRoot->sendEvent(CE_ACTION_SELECTION_CHANGED);
	}
}

bool ActionTreeEditorCore::askSaveActionTree(const std::string& info)
{
	MODAL_DIALOG_RESULT ret = mEditorCoreRoot->showModalYesNoCancel(info);
	// �����,�򱣴��ļ�
	if (ret == MDR_YES)
	{
		saveActionTree();
	}
	// �����,���ǵ�ǰ����Ϊδ�޸�״̬
	else if (ret == MDR_NO)
	{
		setActionTreeModified(false, false, false);
	}
	// ���ȡ���򲻴��ļ�
	else
	{
		return false;
	}
	return true;
}