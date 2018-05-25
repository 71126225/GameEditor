#ifndef _COMMAND_EDITOR_ADD_COMPONENT_H_
#define _COMMAND_EDITOR_ADD_COMPONENT_H_

#include "EditorCoreCommand.h"

class txComponent;
class CommandEditorAddComponent : public EditorCoreCommand
{
public:
	virtual void reset()
	{
		mComponent = NULL;
		mType = EMPTY_STRING;
		mComponentName = EMPTY_STRING;
		mParentComponentName = EMPTY_STRING;
		mComponentPos = -1;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	txComponent* mComponent;	// Դ���,����ñ�����Ϊ��,����ӵ��������ȡ������,�����������󽫸�������������Կ������������
	std::string mType;
	std::string mComponentName;
	std::string mParentComponentName;
	int mComponentPos;
};

#endif