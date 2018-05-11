#ifndef _PAYLOAD_H_
#define _PAYLOAD_H_

#include "DataKey.h"

class Payload
{
public:
	Payload()
	{
		mID = 0;
	}
	virtual ~Payload(){ clear(); }
	void clear()
	{
		mID = 0;
		TRACE_DELETE(mDataKey);
	}
	void setData(unsigned char* data, const int& count)
	{
		int offset = 0;
		BinaryUtility::read(mID, data, offset, true);
		parseData(data + sizeof(mID), count - sizeof(mID));
	}
	virtual void parseData(unsigned char* data, const int& count) = 0;
public:
	unsigned char mID;
	DataKey* mDataKey;
};

// �˶���������
class FitPayload : public Payload
{
public:
	virtual void parseData(unsigned char* data, const int& count)
	{
		mDataKey = TRACE_NEW(MachineKey, mDataKey, mID);
		if (!mDataKey->setData(data, count))
		{
			TRACE_DELETE(mDataKey);
		}
	}
};

// ������������
class FrictionPayload : public Payload
{
public:
	virtual void parseData(unsigned char* data, const int& count)
	{
		mDataKey = TRACE_NEW(FrictionKey, mDataKey, mID);
		if (mDataKey->setData(data, count))
		{
			TRACE_DELETE(mDataKey);
		}
	}
};

#endif