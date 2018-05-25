#include "Utility.h"

#include "ECUndoManager.h"
#include "ECUndo.h"
#include "CommonDefine.h"
#include "EditorCoreRoot.h"

void ECUndoManager::destroy()
{
	clearUndo();
	clearRedo();
}

void ECUndoManager::addUndo(ECUndo* undoCollection)
{
	// ���ڽ��г�������,������Ӧ������������
	if (mUndoing)
	{
		bool beforeRedoEnable = canRedo();
		mRedoBuffer.push_back(undoCollection);
		redoChanged(beforeRedoEnable, canRedo());
	}
	// û�����ڳ���,����볷������
	else
	{
		bool beforeUndoEnable = canUndo();
		// �������ĳ��������Ѿ��������������,��ɾ�����ȼ���ĳ�������
		if (mUndoBuffer.size() >= MAX_UNDO_COUNT)
		{
			TRACE_DELETE(*mUndoBuffer.begin());
			mUndoBuffer.erase(mUndoBuffer.begin());
		}
		mUndoBuffer.push_back(undoCollection);
		undoChanged(beforeUndoEnable, canUndo());

		// �������������ʱ��ӳ�������,����Ҫ��������б�
		if (!mRedoing)
		{
			clearRedo();
		}
	}
}

void ECUndoManager::undo()
{
	mUndoing = true;
	if (canUndo())
	{
		// ִ���˳����������ɾ���ò���
		bool beforeUndoEnable = canUndo();
		mUndoBuffer[mUndoBuffer.size() - 1]->undo();
		TRACE_DELETE(*(mUndoBuffer.begin() + mUndoBuffer.size() - 1));
		mUndoBuffer.erase(mUndoBuffer.begin() + mUndoBuffer.size() - 1);
		undoChanged(beforeUndoEnable, canUndo());
	}
	mUndoing = false;
}

void ECUndoManager::redo()
{
	mRedoing = true;
	if (canRedo())
	{
		// ִ���������������ɾ���ò���
		bool beforeRedoEnable = canRedo();
		mRedoBuffer[mRedoBuffer.size() - 1]->undo();
		TRACE_DELETE(*(mRedoBuffer.begin() + mRedoBuffer.size() - 1));
		mRedoBuffer.erase(mRedoBuffer.begin() + mRedoBuffer.size() - 1);
		redoChanged(beforeRedoEnable, canRedo());
	}
	mRedoing = false;
}

void ECUndoManager::clearRedo()
{
	bool beforeRedoEnable = canRedo();
	int redoCount = mRedoBuffer.size();
	FOR_STL(mRedoBuffer, int i = 0; i < redoCount; ++i)
	{
		TRACE_DELETE(mRedoBuffer[i]);
	}
	END_FOR_STL(mRedoBuffer);
	mRedoBuffer.clear();
	redoChanged(beforeRedoEnable, canRedo());
}

void ECUndoManager::clearUndo()
{
	bool beforeUndoEnable = canUndo();
	int undoCount = mUndoBuffer.size();
	FOR_STL(mUndoBuffer, int i = 0; i < undoCount; ++i)
	{
		TRACE_DELETE(mUndoBuffer[i]);
	}
	END_FOR_STL(mUndoBuffer);
	mUndoBuffer.clear();
	undoChanged(beforeUndoEnable, canUndo());
}

void ECUndoManager::checkUndoRedoEnable()
{
	undoChanged(false, canUndo(), true);
	redoChanged(false, canRedo(), true);
}

void ECUndoManager::undoChanged(const bool& beforeEnable, const bool& nowEnable, const bool& force)
{
	if (beforeEnable != nowEnable || force)
	{
		txVector<std::string> params;
		params.push_back(txStringUtility::boolToString(nowEnable));
		mEditorCoreRoot->sendEvent(CE_UNDO_ENABLE_CHANGE, params);
	}
}

void ECUndoManager::redoChanged(const bool& beforeEnable, const bool& nowEnable, const bool& force)
{
	if (beforeEnable != nowEnable || force)
	{
		txVector<std::string> params;
		params.push_back(txStringUtility::boolToString(nowEnable));
		mEditorCoreRoot->sendEvent(CE_REDO_ENABLE_CHANGE, params);
	}
}