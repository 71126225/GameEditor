#pragma once

#include "CodeUtility.h"

class CodeNetPacket : public CodeUtility
{
public:
	static void generate();
protected:
	// ������
	static void generateServer(const myVector<string>& gamePacketNameList, const myVector<PacketInfo>& packetInfoList, const myVector<PacketStruct>& structInfoList, int& packetVersion);
	static void generateServerGamePacketDefineFile(const myVector<PacketInfo>& packetList, const string& filePath);
	static void generateServerGamePacketRegisteFile(const myVector<PacketInfo>& packetList, const string& filePath, int packetVersion);
	static void generateServerCSPacketFileHeader(const PacketInfo& packetInfo, const string& filePath, const string& apiMacro);
	static void generateServerSCPacketFileHeader(const PacketInfo& packetInfo, const string& filePath, const string& apiMacro);
	static void generateServerCSPacketFileSource(const PacketInfo& packetInfo, const string& filePath);
	// �ͻ���
	static void generateClient(const myVector<string>& gamePacketNameList, const myVector<PacketInfo>& packetInfoList, const myVector<PacketStruct>& structInfoList, int packetVersion);
	static void generateClientGamePacketDefineFile(const myVector<PacketInfo>& packetList, const string& filePath);
	static void generateClientGamePacketRegisteFile(const myVector<PacketInfo>& packetList, const string& filePath, int packetVersion);
	static void generateClientCSPacketFileHeader(const PacketInfo& packetInfo, const string& filePath, const string& apiMacro);
	static void generateClientSCPacketFileHeader(const PacketInfo& packetInfo, const string& filePath, const string& apiMacro);
	static void generateClientSCPacketFileSource(const PacketInfo& packetInfo, const string& filePath);
	// ���������߼�
	static int findPacketVersion(const string& filePath);
	static void generateCppPacketMemberDeclare(const myVector<PacketMember>& memberList, myVector<string>& generateCodes);
	static void generateCppPacketReadWrite(const PacketInfo& packetInfo, myVector<string>& generateCodes, bool includeResetProperty);
	static void generateCppStruct(const PacketStruct& structInfo, const string& filePath);
};