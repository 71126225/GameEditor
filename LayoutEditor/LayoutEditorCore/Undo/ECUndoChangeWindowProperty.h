#ifndef _EC_UNDO_CHANGE_WINDOW_PROPERTY_H_
#define _EC_UNDO_CHANGE_WINDOW_PROPERTY_H_

#include "ECUndo.h"

// �������Ըı�
class ECUndoChangeWindowProperty : public ECUndo
{
public:
	ECUndoChangeWindowProperty(){}
	virtual ~ECUndoChangeWindowProperty() {}
	virtual void undo();
public:
	std::string mWindowName;		// ������
	std::string mPropertyName;		// ������
	std::string mPropertyValue;		// ԭ��������ֵ
};

#endif