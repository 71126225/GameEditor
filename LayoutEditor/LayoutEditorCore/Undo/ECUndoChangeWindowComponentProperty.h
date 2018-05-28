#ifndef _EC_UNDO_CHANGE_WINDOW_COMPONENT_PROPERTY_H_
#define _EC_UNDO_CHANGE_WINDOW_COMPONENT_PROPERTY_H_

#include "ECUndo.h"

// �������Ըı�
class ECUndoChangeWindowComponentProperty : public ECUndo
{
public:
	ECUndoChangeWindowComponentProperty(){}
	virtual ~ECUndoChangeWindowComponentProperty() {}
	virtual void undo();
public:
	std::string mWindowName;		// ������
	std::string mComponentName;		// �����
	std::string mPropertyName;		// ������
	std::string mPropertyValue;		// ԭ��������ֵ
};

#endif