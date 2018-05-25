#ifndef _EC_UNDO_WINDOW_RENAME_H_
#define _EC_UNDO_WINDOW_RENAME_H_

#include "ECUndo.h"

// ����������
class ECUndoWindowRename : public ECUndo
{
public:
	ECUndoWindowRename(){}
	virtual ~ECUndoWindowRename(){}
	void setName(const std::string& name) { mName = name; }
	void setWindowName(const std::string& name) { mWindowName = name; }
	virtual void undo();
protected:
	std::string mName;			// ����ǰ�Ĵ�����
	std::string mWindowName;	// ������
};

#endif