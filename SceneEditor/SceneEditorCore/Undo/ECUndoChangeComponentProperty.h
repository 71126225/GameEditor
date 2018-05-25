#ifndef _EC_UNDO_CHANGE_COMPONENT_PROPERTY_H_
#define _EC_UNDO_CHANGE_COMPONENT_PROPERTY_H_

#include "ECUndo.h"

// �������Ըı�
class ECUndoChangeComponentProperty : public ECUndo
{
public:
	virtual void undo();
public:
	std::string mEditorName;		// �༭����
	std::string mComponentName;		// �����
	std::string mPropertyName;		// ������
	std::string mPropertyValue;		// ԭ��������ֵ
};

#endif