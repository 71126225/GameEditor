#ifndef _GAME_DEFINE_H_
#define _GAME_DEFINE_H_

#include "Utility.h"

struct PacketMember
{
	bool mIsArray;
	myVector<string> mArrayLengthMacro;	// �������ֻ�����
	string mTypeName;
	string mMemberName;
};

struct PacketInfo
{
	myVector<PacketMember> mMemberList;
	string mPacketName;					// ��Ϣ����
	string mComment;					// ��Ϣע��
	bool mShowInfo;						// �Ƿ���ʾ������Ϣ
	bool mServerExecuteInMain;			// ���������յ�CS��Ϣ�Ƿ������߳�ִ��
	bool mHotFix;						// ��Ϣ�ڿͻ����Ƿ���ȸ�
	bool mUDP;							// �Ƿ�ͨ��UDP����
};

enum class SQLITE_OWNER : byte
{
	NONE,				// �����ڿͻ��˻��߷�����,�����������
	BOTH,				// �ͻ��˺ͷ����������õ�
	CLIENT_ONLY,		// ���ͻ�����
	SERVER_ONLY,		// ����������
};

struct SQLiteMember
{
	SQLITE_OWNER mOwner;
	string mTypeName;
	string mMemberName;
	string mComment;
};

struct SQLiteInfo
{
	myVector<SQLiteMember> mMemberList;
	SQLITE_OWNER mOwner;
	string mSQLiteName;
	string mComment;
	bool mHotFix;
};

struct MySQLMember
{
	string mTypeName;
	string mMemberName;
	string mComment;
	bool mUTF8;
};

struct MySQLInfo
{
	myVector<MySQLMember> mMemberList;	// ����ID�ֶε��ֶ���Ϣ�б�
	myVector<string> mIndexList;		// �����б�
	string mMySQLClassName;
	string mMySQLTableName;
	string mComment;
};

#endif