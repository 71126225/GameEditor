#ifndef _EC_UNDO_DELETE_COMPONENT_H_
#define _EC_UNDO_DELETE_COMPONENT_H_

#include "ECUndo.h"

// ����ɾ�����,����������
class txComponent;
class ECUndoDeleteComponent : public ECUndo
{
public:
	ECUndoDeleteComponent()
		:
		mComponent(NULL),
		mComponentPos(-1)
	{}
	void setComponent(txComponent* component);
	virtual void undo();
public:
	txComponent* mComponent;
	std::string mEditorName;
	std::string mParentComponentName;
	int mComponentPos;
};

#endif