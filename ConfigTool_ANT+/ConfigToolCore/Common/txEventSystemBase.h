#ifndef _TX_EVENT_SYSTEM_BASE_H_
#define _TX_EVENT_SYSTEM_BASE_H_

#include "txEventHandler.h"
#include "ThreadLock.h"
#include "Utility.h"

#ifndef ADD_PARAM_COUNT
#define ADD_PARAM_COUNT(t, c) mEventParamCountList.insert(t, c);
#endif

template<typename T, class H>
class txEventSystemBase
{
public:
	txEventSystemBase(){}
	virtual ~txEventSystemBase(){}
	virtual void update(float elapsedTime)
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
		FOR_STL(eventList, int i = 0; i < bufferSize; ++i)
		{
			pushEvent(eventList[i].first, eventList[i].second, true);
		}
		END_FOR_STL(eventList);
	}
	void unregisteEvent(const T& event, H* handler)
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
	void unregisteAllEvent(H* handler)
	{
		auto iter = mEventParamCountList.begin();
		auto iterEnd = mEventParamCountList.end();
		FOR_STL(mEventParamCountList, ; iter != iterEnd; ++iter)
		{
			unregisteEvent(iter->first, handler);
		}
		END_FOR_STL(mEventParamCountList);
	}
	void registeAllEvent(H* handler)
	{
		auto iter = mEventParamCountList.begin();
		auto iterEnd = mEventParamCountList.end();
		FOR_STL(mEventParamCountList, ; iter != iterEnd; ++iter)
		{
			registeEvent(iter->first, handler);
		}
		END_FOR_STL(mEventParamCountList);
	}
	void registeEvent(const T& event, H* handler)
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
	void pushEvent(const T& event, txVector<std::string>& paramList = txVector<std::string>(), const bool& sendImmediately = true)
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
				FOR_STL(iter->second, ; iterHandler != iterHandlerEnd; ++iterHandler)
				{
					if (eventParamCount == (int)paramList.size())
					{
						(*iterHandler)->notifyEvent(event, paramList);
					}
					else
					{
						TOOL_CORE_ERROR("error : param list size error! should have param count : %d, cur param count : %d", eventParamCount, paramList.size());
					}
				}
				END_FOR_STL(iter->second);
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
	ThreadLock mEventLock;
};

#endif