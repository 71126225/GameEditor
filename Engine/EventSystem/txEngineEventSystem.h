#ifndef _TX_ENGINE_EVENT_SYSTEM_H_
#define _TX_ENGINE_EVENT_SYSTEM_H_

#include "txEventSystemBase.h"

// ������Ҫֱ��ʹ��txEventSystem��pushEvent,Ӧ��ʹ��txEngineRoot��sendEvent
class txEventHandler;
class txEngineEventSystem : public txEventSystemBase<ENGINE_EVENT, txEngineEventHandler>
{
public:
	txEngineEventSystem();
protected:
};

#endif
