#ifndef _EC_UNDO_MANAGER_H_
#define _EC_UNDO_MANAGER_H_

#include "txEngineDefine.h"
#include "EditorCoreBase.h"

class ECUndo;
class ECUndoManager : public EditorCoreBase
{
public:
	ECUndoManager()
		:
		mUndoing(false),
		mRedoing(false)
	{}
	virtual ~ECUndoManager(){ destroy(); }
	void init(){}
	void destroy();
	void addUndo(ECUndo* undoCollection);
	void undo();
	void redo();
	bool canUndo(){return mUndoBuffer.size() > 0;}
	bool canRedo(){return mRedoBuffer.size() > 0;}
	void clearRedo();
	void clearUndo();
	void checkUndoRedoEnable();
	template<typename T>
	T* createUndo()
	{
		T* undoCollection = TRACE_NEW(T, undoCollection);
		return undoCollection;
	}
protected:
	// beforeEnable�Ǹı�֮ǰ�Ŀɳ���״̬,nowEnable�Ǹİ�֮��Ŀɳ���״̬,force������״̬��û�иı䶼ǿ�Ʒ����¼�
	void undoChanged(const bool& beforeEnable, const bool& nowEnable, const bool& force = false);
	void redoChanged(const bool& beforeEnable, const bool& nowEnable, const bool& force = false);
protected:
	txVector<ECUndo*> mUndoBuffer;
	txVector<ECUndo*> mRedoBuffer;
	bool mUndoing;	// �Ƿ����ڳ���,�������,����ӳ�������ʱʵ���������������
	bool mRedoing;	// �Ƿ���������,�����������,������ӳ�������ʱ��Ҫ�������б����
};

#endif