#ifndef _TX_EVENT_SYSTEM_BASE_H_
#define _TX_EVENT_SYSTEM_BASE_H_

#include "CommonDefine.h"
#include "txEventHandler.h"
#include "EditorCoreLog.h"
#include "txThreadLock.h"

#ifndef ADD_PARAM_COUNT
#define ADD_PARAM_COUNT(t, c) mEventParamCountList.insert(t, c);
#endif

template<typename T, class H>
class txEventSystemBase
{
public:
	txEventSystemBase(){}
	virtual ~txEventSystemBase(){}
	void update(const float& elapsedTime)
	{
		LOCK(mEventLock);
		int bufferSize = mEventBuffer.size();
		if (bufferSize == 0)
		{
			UNLOCK(mEventLock);
			return;
		}
		// Ϊ�˱����ڶ��߳��г�������,�����¼�������û������¼�,���ٴα�����,����һ���¼��б�,Ȼ���ٷ����¼�
		auto eventList = mEventBuffer;
		mEventBuffer.clear();
		UNLOCK(mEventLock);
		// ���¼���������¼����ͳ�ȥ
		for (int i = 0; i < bufferSize; ++i)
		{
			pushEvent(eventList[i].first, eventList[i].second, true);
		}
	}
	void unRegisterEventHandler(T event, H* handler)
	{
		auto iter = mEventHandlerList.find(event);
		if (iter != mEventHandlerList.end())
		{
			auto iterHandler = iter->second.find(handler);
			if (iterHandler != iter->second.end())
			{
				iter->second.erase(iterHandler);
			}
		}
	}
	void unregisterAllEvent(H* handler)
	{
		auto iter = mEventParamCountList.begin();
		auto iterEnd = mEventParamCountList.end();
		for (; iter != iterEnd; ++iter)
		{
			unRegisterEventHandler(iter->first, handler);
		}
	}
	void registerAllEvent(H* handler)
	{
		auto iter = mEventParamCountList.begin();
		auto iterEnd = mEventParamCountList.end();
		for (; iter != iterEnd; ++iter)
		{
			registerEventHandler(iter->first, handler);
		}
	}
	void registerEventHandler(const T& event, H* handler)
	{
		auto iter = mEventHandlerList.find(event);
		if (iter != mEventHandlerList.end())
		{
			iter->second.insert(handler);
		}
		else
		{
			txSet<H*> handlerList;
			handlerList.insert(handler);
			mEventHandlerList.insert(event, handlerList);
		}
	}
	void pushEvent(const T& event, const txVector<std::string>& paramList, const bool& sendImmediately = true)
	{
		// ������������¼�,������¼��б�,���¼�����ÿ���¼�������
		if (sendImmediately)
		{
			auto iter = mEventHandlerList.find(event);
			auto iterEventDefine = mEventParamCountList.find(event);
			if (iterEventDefine == mEventParamCountList.end())
			{
				return;
			}
			int eventParamCount = iterEventDefine->second;
			if (iter != mEventHandlerList.end())
			{
				auto iterHandler = iter->second.begin();
				auto iterHandlerEnd = iter->second.end();
				for (; iterHandler != iterHandlerEnd; ++iterHandler)
				{
					if (eventParamCount == paramList.size())
					{
						(*iterHandler)->notifyEvent(event, paramList);
					}
					else
					{
						EDITOR_CORE_ERROR("error : param list size error! should have param count : %d, cur param count : %d. event : %d", eventParamCount, paramList.size(), (int)event);
					}
				}
			}
		}
		// ��������������¼�,���¼������¼���������
		else
		{
			LOCK(mEventLock);
			mEventBuffer.push_back(std::pair<T, txVector<std::string> >(event, paramList));
			UNLOCK(mEventLock);
		}
	}
protected:
	txMap<T, txSet<H*> > mEventHandlerList;
	txVector<std::pair<T, txVector<std::string> > > mEventBuffer;
	txMap<T, int> mEventParamCountList; // ÿ���¼��Ĳ����б���
	txThreadLock mEventLock;
};

#endif
