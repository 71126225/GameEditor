#ifndef _EDITOR_CORE_COMMAND_H_
#define _EDITOR_CORE_COMMAND_H_

#include "txCommand.h"
#include "txEngineBase.h"

#include "EditorCoreBase.h"
#include "CommonDefine.h"

class EditorCoreCommand : public txCommand, public EditorCoreBase
{
public:
	EditorCoreCommand(){}
};

#endif