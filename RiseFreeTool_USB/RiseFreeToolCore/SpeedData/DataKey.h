#ifndef _DATA_KEY_H_
#define _DATA_KEY_H_

#include "CommonDefine.h"
#include "EditorCoreLog.h"

class DataKey
{
public:
	DataKey(unsigned char cmdID)
	{
		mKey = 0;
		mValueLength = 0;
		mValue = NULL;
		mCmdID = cmdID;
	}
	virtual ~DataKey()
	{
		TRACE_DELETE_ARRAY(mValue);
	}
	bool setData(unsigned char* data, const int& count)
	{
		if (count < getTotalLength())
		{
			return false;
		}
		int offset = 0;
		unsigned char lastKey = 0;
		BinaryUtility::read(lastKey, data, offset, true);
		// ���key����ȷ˵����һ�����ݲ����ɸ�key������
		if (mKey != lastKey)
		{
			return false;
		}
		BinaryUtility::read(mValueLength, data, offset, true);
		if (count - offset >= mValueLength)
		{
			mValue = TRACE_NEW_ARRAY(unsigned char, mValueLength, mValue);
			BinaryUtility::readBuffer(mValue, data, mValueLength, offset);
		}
		if (mValue != NULL && mValueLength >= getRealLength())
		{
			parseValue();
		}
		return true;
	}
	int getTotalLength() { return mValueLength + sizeof(mKey) + sizeof(mValueLength); }
	virtual void copy(DataKey* key)
	{
		if (mKey != key->mKey)
		{
			EDITOR_CORE_ERROR_DELAY("copy key error! key is different!");
			return;
		}
		if (mValueLength != key->mValueLength)
		{
			mValueLength = key->mValueLength;
			TRACE_DELETE_ARRAY(mValue);
			mValue = TRACE_NEW_ARRAY(unsigned char, mValueLength, mValue);
		}
		memcpy(mValue, key->mValue, mValueLength);
	}
	virtual int getRealLength() = 0;
	virtual void parseValue() = 0;
	virtual DataKey* createKey() = 0;
public:
	unsigned char mCmdID;		// ���������ID
	unsigned char mKey;
	unsigned char mValueLength;
	unsigned char* mValue;
};

// ���״̬����
class MachineKey : public DataKey
{
public:
	MachineKey(unsigned char cmdID)
		:DataKey(cmdID)
	{
		mKey = KEY_MACHINE;
	}
	virtual void copy(DataKey* other)
	{
		MachineKey* otherKey = static_cast<MachineKey*>(other);
		DataKey::copy(otherKey);
		mPower = otherKey->mPower;
		mRPM = otherKey->mRPM;
		mAngle = otherKey->mAngle;
		mKeyStatus = otherKey->mKeyStatus;
	}
	virtual void parseValue()
	{
		int offset = 0;
		BinaryUtility::read(mPower, mValue, offset, true);
		BinaryUtility::read(mRPM, mValue, offset, true);
		// ���λΪ0,�Ƕ�Ϊ��
		if ((mValue[offset] & 0x80) == 0)
		{
			BinaryUtility::read(mAngle, mValue, offset, true);
		}
		// ���λΪ1,�Ƕ�Ϊ��
		else
		{
			mValue[offset] &= 0x7f;
			BinaryUtility::read(mAngle, mValue, offset, true);
			mAngle = -mAngle;
		}
		BinaryUtility::read(mKeyStatus, mValue, offset, true);
	}
	bool isKeyDown(int index)
	{
		return (mKeyStatus & (0x00000001 << index)) != 0;
	}
	virtual int getRealLength(){ return sizeof(mPower)+sizeof(mRPM)+sizeof(mAngle)+sizeof(mKeyStatus); }
	virtual DataKey* createKey()
	{
		MachineKey* key = TRACE_NEW(MachineKey, key, mCmdID);
		return key;
	}
public:
	unsigned short mPower;		// ����
	unsigned short mRPM;		// ת��
	short mAngle;				// ���λΪ�����־λ,���λΪ1��ʾ����ת,Ϊ0��ʾ����ת,ʣ�µ�15λΪת���ĽǶ�
	unsigned char mKeyStatus;	// ��4λ��ʾ����״̬,Ϊ1��ʾ����,Ϊ0��ʾδ����
};

// �����������
class FrictionKey : public DataKey
{
public:
	FrictionKey(unsigned char cmdID)
		:DataKey(cmdID)
	{
		mKey = KEY_FRICTION;
	}
	virtual void copy(DataKey* other)
	{
		FrictionKey* otherKey = static_cast<FrictionKey*>(other);
		DataKey::copy(otherKey);
		mFriction = otherKey->mFriction;
	}
	virtual void parseValue()
	{
		int offset = 0;
		BinaryUtility::read(mFriction, mValue, offset, true);
	}
	virtual int getRealLength(){ return sizeof(mFriction); }
	virtual DataKey* createKey()
	{
		FrictionKey* key = TRACE_NEW(FrictionKey, key, mCmdID);
		return key;
	}
public:
	unsigned char mFriction;	// ���������С
};

#endif