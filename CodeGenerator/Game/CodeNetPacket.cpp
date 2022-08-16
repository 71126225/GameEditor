#include "CodeNetPacket.h"

void CodeNetPacket::generate()
{
	string cppCSPacketPath = cppGamePath + "Socket/ClientServer/";
	string cppSCPacketPath = cppGamePath + "Socket/ServerClient/";
	string cppPacketDefinePath = cppGamePath + "Socket/";
	string csharpCSGamePath = csGamePath + "Socket/ClientServer/";
	string csharpCSHotfixPath = csHotfixGamePath + "Socket/ClientServer/";
	string csharpSCGamePath = csGamePath + "Socket/ServerClient/";
	string csharpSCHotfixPath = csHotfixGamePath + "Socket/ServerClient/";
	string csharpPacketDefinePath = csHotfixGamePath + "Socket/";

	// ����ģ���ļ�
	string csFileContent;
	openTxtFile("PacketCS.txt", csFileContent);
	if (csFileContent.length() == 0)
	{
		ERROR("δ�ҵ�Э���ļ�PacketCS.txt");
		return;
	}
	string scFileContent;
	openTxtFile("PacketSC.txt", scFileContent);
	if (scFileContent.length() == 0)
	{
		ERROR("δ�ҵ�Э���ļ�PacketSC.txt");
		return;
	}
	myVector<string> csLines;
	myVector<string> scLines;
	split(csFileContent.c_str(), "\r\n", csLines);
	split(scFileContent.c_str(), "\r\n", scLines);
	myVector<string> lines;
	lines.merge(csLines);
	lines.merge(scLines);
	bool packetStart = false;
	myVector<PacketInfo> packetInfoList;
	myVector<PacketMember> tempMemberList;
	int tempPacketNameLine;
	FOR_VECTOR_CONST(lines)
	{
		string line = lines[i];
		// ����ע��
		if (startWith(line, "//"))
		{
			continue;
		}
		// ����������ע��,��ȥ��
		int pos = -1;
		if (findString(line.c_str(), "//", &pos))
		{
			line = line.substr(0, pos);
		}
		// ȥ�������Ʊ��,�ֺ�
		removeAll(line, '\t', ';');
		// û�г�Ա��������Ϣ��
		if (line == "{}")
		{
			PacketInfo info;
			parsePacketName(lines[i - 1], info);
			info.mComment = lines[i - 2];
			packetInfoList.push_back(info);
			continue;
		}
		// ��Ա�����б���ʼ
		if (line == "{")
		{
			packetStart = true;
			tempPacketNameLine = i - 1;
			tempMemberList.clear();
			continue;
		}
		// ��Ա�����б����
		if (line == "}")
		{
			PacketInfo info;
			parsePacketName(lines[tempPacketNameLine], info);
			info.mMemberList = tempMemberList;
			info.mComment = lines[tempPacketNameLine - 1];
			packetInfoList.push_back(info);
			packetStart = false;
			tempMemberList.clear();
			tempPacketNameLine = -1;
			continue;
		}
		if (packetStart)
		{
			tempMemberList.push_back(parseMemberLine(line));
		}
	}

	// �򿪷���������Ϣע���ļ�,�ҵ���ǰ����Ϣ�汾��,Ȼ��汾������
	int packetVersion = findPacketVersion(cppPacketDefinePath + "PacketRegister.cpp") + 1;

	myVector<string> hotfixList;
	myVector<string> noHotfixList;
	myVector<string> packetList;
	FOR_VECTOR_CONST(packetInfoList)
	{
		const PacketInfo& packetInfo = packetInfoList[i];
		// ���ɴ����ļ�
		// .h.cpp����
		if (cppGamePath.length() > 0)
		{
			generateCppCSPacketFileHeader(packetInfo, cppCSPacketPath);
			generateCppCSPacketFileSource(packetInfo, cppCSPacketPath);
			generateCppSCPacketFileHeader(packetInfo, cppSCPacketPath);
		}
		if (csGamePath.length() > 0)
		{
			// .cs����
			generateCSharpPacketFile(packetInfo, csharpCSHotfixPath, csharpCSGamePath, csharpSCHotfixPath, csharpSCGamePath);
		}
		packetList.push_back(packetInfo.mPacketName);
		if (packetInfo.mHotFix)
		{
			hotfixList.push_back(packetInfo.mPacketName);
		}
		else
		{
			noHotfixList.push_back(packetInfo.mPacketName);
		}
	}
	// c++
	if (cppGamePath.length() > 0)
	{
		generateCppPacketDefineFile(packetInfoList, cppPacketDefinePath);
		generateCppPacketRegisteFile(packetInfoList, cppPacketDefinePath, packetVersion);
		generateCppPacketTotalHeaderFile(packetInfoList, cppPacketDefinePath);
		generateStringDefinePacket(packetList, cppStringDefinePath);

		// ɾ�����õ���Ϣ
		// c++ CS
		myVector<string> cppCSFiles;
		findFiles(cppCSPacketPath, cppCSFiles, NULL, 0);
		FOR_VECTOR(cppCSFiles)
		{
			if (!packetList.contains(getFileNameNoSuffix(cppCSFiles[i], true)))
			{
				deleteFile(cppCSFiles[i]);
			}
		}
		END(cppCSFiles);
		// c++ SC
		myVector<string> cppSCFiles;
		findFiles(cppSCPacketPath, cppSCFiles, NULL, 0);
		FOR_VECTOR(cppSCFiles)
		{
			if (!packetList.contains(getFileNameNoSuffix(cppSCFiles[i], true)))
			{
				deleteFile(cppSCFiles[i]);
			}
		}
		END(cppSCFiles);
	}

	// c#
	if (csGamePath.length() > 0)
	{
		generateCSharpPacketDefineFile(packetInfoList, csharpPacketDefinePath);
		generateCSharpPacketRegisteFile(packetInfoList, csharpPacketDefinePath, packetVersion);

		// c# CS���ȸ�
		myVector<string> csharpCSNoHotfixFiles;
		findFiles(csharpCSGamePath, csharpCSNoHotfixFiles, ".cs");
		FOR_VECTOR(csharpCSNoHotfixFiles)
		{
			const string& file = csharpCSNoHotfixFiles[i];
			if (!noHotfixList.contains(getFileNameNoSuffix(file, true)))
			{
				deleteFile(file);
				deleteFile(file + ".meta");
			}
		}
		END(csharpCSNoHotfixFiles);
		// c# CS�ȸ�
		myVector<string> csharpCSHotfixFiles;
		findFiles(csharpCSHotfixPath, csharpCSHotfixFiles, ".cs");
		FOR_VECTOR(csharpCSHotfixFiles)
		{
			const string& file = csharpCSHotfixFiles[i];
			if (!hotfixList.contains(getFileNameNoSuffix(file, true)))
			{
				deleteFile(file);
				deleteFile(file + ".meta");
			}
		}
		END(csharpCSHotfixFiles);
		// c# SC���ȸ�
		myVector<string> csharpSCNoHotfixFiles;
		findFiles(csharpSCGamePath, csharpSCNoHotfixFiles, ".cs");
		FOR_VECTOR(csharpSCNoHotfixFiles)
		{
			const string& file = csharpSCNoHotfixFiles[i];
			if (!noHotfixList.contains(getFileNameNoSuffix(file, true)))
			{
				deleteFile(file);
				deleteFile(file + ".meta");
			}
		}
		END(csharpSCNoHotfixFiles);
		// c# SC�ȸ�
		myVector<string> csharpSCHotfixFiles;
		findFiles(csharpSCHotfixPath, csharpSCHotfixFiles, ".cs");
		FOR_VECTOR(csharpSCHotfixFiles)
		{
			const string& file = csharpSCHotfixFiles[i];
			if (!hotfixList.contains(getFileNameNoSuffix(file, true)))
			{
				deleteFile(file);
				deleteFile(file + ".meta");
			}
		}
		END(csharpSCHotfixFiles);
	}
}

// PacketHeader.h��PacketDeclareHeader.h�ļ�
void CodeNetPacket::generateCppPacketTotalHeaderFile(const myVector<PacketInfo>& packetList, string filePath)
{
	// PacketHeader.h
	string str0;
	line(str0, "#ifndef _PACKET_HEADER_H_");
	line(str0, "#define _PACKET_HEADER_H_");
	line(str0, "");
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			line(str0, "#include \"" + packetList[i].mPacketName + ".h\"");
		}
	}
	line(str0, "");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			line(str0, "#include \"" + packetList[i].mPacketName + ".h\"");
		}
	}
	line(str0, "#include \"StringDefine.h\"");
	line(str0, "");
	line(str0, "#endif", false);
	writeFile(filePath + "PacketHeader.h", ANSIToUTF8(str0.c_str(), true));
}

// PacketDefine.h�ļ�
void CodeNetPacket::generateCppPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath)
{
	string str;
	line(str, "#ifndef _PACKET_DEFINE_H_");
	line(str, "#define _PACKET_DEFINE_H_");
	line(str, "");
	line(str, "#include \"FrameDefine.h\"");
	line(str, "");
	line(str, "enum class PACKET_TYPE : ushort");
	line(str, "{");
	line(str, "\tMIN,");
	line(str, "");
	line(str, "\tGATE_CS_MIN = 1000,");
	line(str, "\tGATE_CS_HEART_BEAT,");
	line(str, "\tGATE_CS_SERVER_INFO,");
	line(str, "");
	line(str, "\tGATE_SC_MIN = 1200,");
	line(str, "\tGATE_SC_HEART_BEAT,");
	line(str, "");	
	line(str, "\tCS_MIN = 3000,");
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			line(str, "\t" + packetNameToUpper(packetList[i].mPacketName) + ",");
		}
	}
	line(str, "");
	line(str, "\tSC_MIN = 6000,");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			line(str, "\t" + packetNameToUpper(packetList[i].mPacketName) + ",");
		}
	}
	line(str, "};");
	line(str, "");
	line(str, "#endif", false);

	writeFile(filePath + "PacketDefine.h", ANSIToUTF8(str.c_str(), true));
}

// PacketRegister.cpp�ļ�
void CodeNetPacket::generateCppPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath, int packetVersion)
{
	string str;
	line(str, "#include \"GameHeader.h\"");
	line(str, "");
	line(str, "#define PACKET_FACTORY(packet, type) mPacketFactoryManager->addFactory<packet>(type, NAME(packet));");
	line(str, "");
	line(str, "int PacketRegister::PACKET_VERSION = " + intToString(packetVersion) + ";");
	line(str, "void PacketRegister::registeAll()");
	line(str, "{");
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			line(str, "\tPACKET_FACTORY(" + packetList[i].mPacketName + ", PACKET_TYPE::" + packetNameToUpper(packetList[i].mPacketName) + ");");
		}
	}
	line(str, "");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			line(str, "\tPACKET_FACTORY(" + packetList[i].mPacketName + ", PACKET_TYPE::" + packetNameToUpper(packetList[i].mPacketName) + ");");
		}
	}
	line(str, "};", false);

	writeFile(filePath + "PacketRegister.cpp", ANSIToUTF8(str.c_str(), true));
}

// StringDefinePacket.h��StringDefinePacket.cpp
void CodeNetPacket::generateStringDefinePacket(const myVector<string>& packetList, string filePath)
{
	// ͷ�ļ�
	string header;
	line(header, "#ifdef _STRING_DEFINE_PACKET_H_");
	line(header, "#error \"����ͷ�ļ�,ֻ�ܱ�StringDefine.h������\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_PACKET_H_");
	line(header, "");
	uint count = packetList.size();
	FOR_I(count)
	{
		line(header, stringDeclare(packetList[i]));
	}
	line(header, "");
	line(header, "#endif");
	writeFile(filePath + "StringDefinePacket.h", ANSIToUTF8(header.c_str(), true));
}

// CSPacket.h��CSPacket.cpp
void CodeNetPacket::generateCppCSPacketFileHeader(const PacketInfo& packetInfo, string filePath)
{
	const string& packetName = packetInfo.mPacketName;
	if (!startWith(packetName, "CS"))
	{
		return;
	}

	// CSPacket.h
	string headerFullPath = filePath + packetName + ".h";
	myVector<string> includeList;
	myVector<string> customList;
	findCppIncludeCustomCode(packetName, headerFullPath, includeList, customList);
	string header;
	string marco = nameToUpper(packetName.substr(2));
	line(header, "#ifndef _CS" + marco + "_H_");
	line(header, "#define _CS" + marco + "_H_");
	line(header, "");
	// #include����
	FOR_VECTOR(includeList)
	{
		line(header, includeList[i]);
	}
	END(includeList);
	line(header, "");
	line(header, packetInfo.mComment);
	line(header, "class " + packetName + " : public Packet");
	line(header, "{");
	line(header, "public:");
	const auto& memberList = packetInfo.mMemberList;
	FOR_VECTOR_CONST(memberList)
	{
		line(header, "\t" + cppMemberDeclareString(memberList[i]));
	}
	line(header, "public:");
	line(header, "\tvoid fillParams() override");
	line(header, "\t{");
	line(header, "\t\tmShowInfo = " + boolToString(packetInfo.mShowInfo) + ";");
	line(header, "\t\tmExecuteInMain = " + boolToString(packetInfo.mServerExecuteInMain) + ";");
	FOR_I(memberListCount)
	{
		line(header, "\t\t" + cppPushParamString(memberList[i]));
	}
	line(header, "\t}");
	line(header, "\tvoid execute() override;");
	// �Զ�����벿��
	FOR_VECTOR(customList)
	{
		line(header, customList[i]);
	}
	END(customList);
	line(header, "};");
	line(header, "");
	line(header, "#endif", false);

	writeFile(headerFullPath, ANSIToUTF8(header.c_str(), true));
}

void CodeNetPacket::findCppIncludeCustomCode(const string& packetName, const string& fullPath, myVector<string>& includeList, myVector<string>& customList)
{
	if (isFileExist(fullPath))
	{
		myVector<string> fileLines;
		openTxtFileLines(fullPath, fileLines);
		uint lineCount = fileLines.size();
		int customStart = -1;
		int customEnd = -1;
		bool isCSPacket = startWith(packetName, "CS");

		// #include����
		FOR_I(lineCount)
		{
			if (startWith(fileLines[i], "#include \""))
			{
				includeList.push_back(fileLines[i]);
			}
		}

		// �����Զ���������ʼ
		if (isCSPacket)
		{
			FOR_I(lineCount)
			{
				// CS��Ϣ��execute�������������β֮�����Զ�����벿��
				if (fileLines[i] == "\tvoid execute() override;")
				{
					customStart = i + 1;
					break;
				}
			}
		}
		else
		{
			bool findFillParams = false;
			FOR_I(lineCount)
			{
				// SC��Ϣ��fillParams������β�����β֮�����Զ�����벿��
				if (fileLines[i] == "\tvoid fillParams() override")
				{
					findFillParams = true;
					continue;
				}
				if (findFillParams && fileLines[i] == "\t}")
				{
					customStart = i + 1;
					break;
				}
			}
		}

		// �����Զ���������ֹ
		if (customStart >= 0)
		{
			FOR_I(lineCount)
			{
				if (fileLines[lineCount - i - 1] == "};")
				{
					customEnd = lineCount - i - 2;
					break;
				}
			}
		}
		// ���Զ�����벿�ַ����б�
		if (customStart >= 0 && customEnd >= 0 && customStart <= customEnd)
		{
			uint customLineCount = customEnd - customStart + 1;
			FOR_I(customLineCount)
			{
				customList.push_back(fileLines[customStart + i]);
			}
		}
	}
	else
	{
		includeList.push_back("#include \"Packet.h\"");
		customList.push_back("\tvoid debugInfo(Array<512>& buffer) override");
		customList.push_back("\t{");
		customList.push_back("\t\tdebug(buffer, "");");
		customList.push_back("\t}");
	}
}

int CodeNetPacket::findPacketVersion(const string& filePath)
{
	int packetVersion = 0;
	string fileContent;
	openTxtFile(filePath, fileContent);
	myVector<string> lines;
	split(fileContent.c_str(), "\r\n", lines);
	FOR_VECTOR(lines)
	{
		const string& line = lines[i];
		int index = 0;
		if (findString(line.c_str(), "PACKET_VERSION = ", &index))
		{
			int pos0 = (int)line.find_first_of('=', index);
			int pos1 = (int)line.find_first_of(';', index);
			packetVersion = stringToInt(line.substr(pos0 + 1, pos1 - pos0 - 1));
			break;
		}
	}
	END(lines);
	return packetVersion;
}

void CodeNetPacket::generateCppCSPacketFileSource(const PacketInfo& packetInfo, string filePath)
{
	const string& packetName = packetInfo.mPacketName;
	if (!startWith(packetName, "CS"))
	{
		return;
	}

	// CSPacket.cpp
	string sourceFullPath = filePath + packetName + ".cpp";
	if (!isFileExist(sourceFullPath))
	{
		string source;
		line(source, "#include \"GameHeader.h\"");
		line(source, "");
		line(source, "void " + packetName + "::execute()");
		line(source, "{");
		line(source, "\tCharacterPlayer* player = getCharacter();");
		line(source, "\tif (player == NULL)");
		line(source, "\t{");
		line(source, "\t\treturn;");
		line(source, "\t}");
		line(source, "}", false);

		writeFile(sourceFullPath, ANSIToUTF8(source.c_str(), true));
	}
}

// SCPacket.h�ļ�
void CodeNetPacket::generateCppSCPacketFileHeader(const PacketInfo& packetInfo, string filePath)
{
	const string& packetName = packetInfo.mPacketName;
	if (!startWith(packetName, "SC"))
	{
		return;
	}

	// SCPacket.h
	string headerFullPath = filePath + packetName + ".h";
	myVector<string> includeList;
	myVector<string> customList;
	findCppIncludeCustomCode(packetName, headerFullPath, includeList, customList);

	string header;
	string marco = nameToUpper(packetName.substr(2));
	line(header, "#ifndef _SC" + marco + "_H_");
	line(header, "#define _SC" + marco + "_H_");
	line(header, "");
	// #include����
	FOR_VECTOR(includeList)
	{
		line(header, includeList[i]);
	}
	END(includeList);
	line(header, "");
	line(header, packetInfo.mComment);
	line(header, "class " + packetName + " : public Packet");
	line(header, "{");
	line(header, "public:");
	const auto& memberList = packetInfo.mMemberList;
	FOR_VECTOR_CONST(memberList)
	{
		line(header, "\t" + cppMemberDeclareString(memberList[i]));
	}
	line(header, "public:");
	line(header, "\tvoid fillParams() override");
	line(header, "\t{");
	line(header, "\t\tmShowInfo = " + boolToString(packetInfo.mShowInfo) + ";");
	FOR_I(memberListCount)
	{
		line(header, "\t\t" + cppPushParamString(memberList[i]));
	}
	line(header, "\t}");
	// �Զ�����벿��
	FOR_VECTOR(customList)
	{
		line(header, customList[i]);
	}
	END(customList);
	line(header, "};");
	line(header, "");
	line(header, "#endif", false);

	writeFile(headerFullPath, ANSIToUTF8(header.c_str(), true));
}

// PacketDefine.cs�ļ�
void CodeNetPacket::generateCSharpPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath)
{
	string str;
	line(str, "using System;");
	line(str, "");
	line(str, "public class PACKET_TYPE");
	line(str, "{");
	line(str, "\tpublic static ushort MIN = 0;");
	line(str, "");
	int csPacketNumber = 3000;
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			++csPacketNumber;
			line(str, "\tpublic static ushort " + packetNameToUpper(packetList[i].mPacketName) + " = " + intToString(csPacketNumber) + ";");
		}
	}
	++csPacketNumber;
	line(str, "");
	int scPacketNumber = 6000;
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			++scPacketNumber;
			line(str, "\tpublic static ushort " + packetNameToUpper(packetList[i].mPacketName) + " = " + intToString(scPacketNumber) + ";");
		}
	}
	++scPacketNumber;
	line(str, "}", false);

	writeFile(filePath + "PacketDefine.cs", ANSIToUTF8(str.c_str(), true));
}

// PacketRegister.cs�ļ�
void CodeNetPacket::generateCSharpPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath, int packetVersion)
{
	string str;
	line(str, "using System;");
	line(str, "");
	line(str, "public class PacketRegister : GB");
	line(str, "{");
	line(str, "\tpublic static int PACKET_VERSION = " + intToString(packetVersion) + ";");
	line(str, "\tpublic static void registeAll()");
	line(str, "\t{");
	myVector<PacketInfo> udpCSList;
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (!startWith(packetList[i].mPacketName, "CS"))
		{
			continue;
		}
		if (!packetList[i].mUDP)
		{
			line(str, "\t\tregistePacket(typeof(" + packetList[i].mPacketName + "), PACKET_TYPE." + packetNameToUpper(packetList[i].mPacketName) + ");");
		}
		else
		{
			udpCSList.push_back(packetList[i]);
		}
	}
	line(str, "");
	myVector<PacketInfo> udpSCList;
	FOR_I(packetCount)
	{
		if (!startWith(packetList[i].mPacketName, "SC"))
		{
			continue;
		}
		if (!packetList[i].mUDP)
		{
			if (packetList[i].mClientExecuteInMain)
			{
				line(str, "\t\tregistePacket(typeof(" + packetList[i].mPacketName + "), PACKET_TYPE." + packetNameToUpper(packetList[i].mPacketName) + ");");
			}
			else
			{
				line(str, "\t\tregistePacket(typeof(" + packetList[i].mPacketName + "), PACKET_TYPE." + packetNameToUpper(packetList[i].mPacketName) + ", false);");
			}
		}
		else
		{
			udpSCList.push_back(packetList[i]);
		}
	}
	if (udpCSList.size() > 0)
	{
		line(str, "");
		FOR_VECTOR(udpCSList)
		{
			line(str, "\t\tregisteUDP(PACKET_TYPE." + packetNameToUpper(udpCSList[i].mPacketName) + ", \"" + udpCSList[i].mPacketName + "\");");
		}
		END(udpCSList);
	}
	if (udpSCList.size() > 0)
	{
		line(str, "");
		FOR_VECTOR(udpSCList)
		{
			line(str, "\t\tregisteUDP(PACKET_TYPE." + packetNameToUpper(udpSCList[i].mPacketName) + ", \"" + udpSCList[i].mPacketName + "\");");
		}
		END(udpSCList);
	}
	line(str, "\t}");
	line(str, "\tprotected static void registePacket(Type classType, ushort type, bool executeInMain = true)");
	line(str, "\t{");
	line(str, "\t\tif (!executeInMain)");
	line(str, "\t\t{");
	line(str, "\t\t\tmNetManager.getConnectTCP().addExecuteThreadPacket(type);");
	line(str, "\t\t}");
	line(str, "\t\tmNetPacketTypeManager.registePacket(classType, type);");
	line(str, "\t}");
	line(str, "\tprotected static void registeUDP(ushort type, string packetName)");
	line(str, "\t{");
	line(str, "\t\tmNetPacketTypeManager.registeUDPPacketName(type, packetName);");
	line(str, "\t}");
	line(str, "}", false);

	writeFile(filePath + "PacketRegister.cs", ANSIToUTF8(str.c_str(), true));
}

void CodeNetPacket::generateCSharpPacketFile(const PacketInfo& packetInfo, const string& csFileHotfixPath, const string& csFileGamePath, const string& scFileHotfixPath, const string& scFileGamePath)
{
	string packetName = packetInfo.mPacketName;
	if (!startWith(packetName, "CS") && !startWith(packetName, "SC"))
	{
		return;
	}

	myVector<string> usingList;
	myVector<string> customList;
	string fullPath;
	if (startWith(packetName, "CS"))
	{
		fullPath = packetInfo.mHotFix ? csFileHotfixPath + packetName + ".cs" : csFileGamePath + packetName + ".cs";
	}
	else if (startWith(packetName, "SC"))
	{
		fullPath = packetInfo.mHotFix ? scFileHotfixPath + packetName + ".cs" : scFileGamePath + packetName + ".cs";
	}

	findCSharpUsingListCustomCode(packetName, fullPath, usingList, customList);

	string file;
	// using����
	FOR_VECTOR(usingList)
	{
		line(file, usingList[i]);
	}
	END(usingList);
	line(file, "");
	// �̶���ʽ����
	line(file, packetInfo.mComment);
	line(file, "public class " + packetName + " : NetPacketFrame");
	line(file, "{");
	uint memberCount = packetInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		line(file, "\t" + cSharpMemberDeclareString(packetInfo.mMemberList[i]));
	}
	if (memberCount > 0)
	{
		// ����
		line(file, "\tpublic " + packetName + "()");
		line(file, "\t{");
		FOR_I(memberCount)
		{
			line(file, "\t\t" + cSharpMemberContructString(packetInfo.mMemberList[i], packetInfo.mHotFix));
		}
		line(file, "\t}");
		// fillParams
		line(file, "\tprotected override void fillParams()");
		line(file, "\t{");
		FOR_I(memberCount)
		{
			line(file, "\t\t" + cSharpPushParamString(packetInfo.mMemberList[i]));
		}
		line(file, "\t}");
	}
	// �Զ�����벿��
	FOR_VECTOR(customList)
	{
		line(file, customList[i]);
	}
	END(customList);
	line(file, "}", false);

	writeFile(fullPath, ANSIToUTF8(file.c_str(), true));
}

void CodeNetPacket::findCSharpUsingListCustomCode(const string& packetName, const string& fullPath, myVector<string>& usingList, myVector<string>& customList)
{
	if (isFileExist(fullPath))
	{
		myVector<string> fileLists;
		openTxtFileLines(fullPath, fileLists);
		// �����ռ䲿��
		uint lines = fileLists.size();
		FOR_I(lines)
		{
			if (!startWith(fileLists[i], "using "))
			{
				break;
			}
			usingList.push_back(fileLists[i]);
		}

		// �Զ��庯���岿��,��fillParams���������������֮�䶼���Զ��庯������
		int customStart = -1;
		int customEnd = -1;
		bool findFillParams = false;
		FOR_I(lines)
		{
			if (fileLists[i] == "\tprotected override void fillParams()")
			{
				findFillParams = true;
				continue;
			}
			if (findFillParams && fileLists[i] == "\t}")
			{
				customStart = i + 1;
				break;
			}
		}
		FOR_I(lines)
		{
			if (fileLists[lines - i - 1] == "}")
			{
				customEnd = lines - i - 2;
				break;
			}
		}

		// ���û��fillParams�ĺ�������,��˵��û�в���,������ȫ�������Զ������
		if (customStart < 0 || customEnd < 0)
		{
			FOR_I(lines)
			{
				if (startWith(fileLists[i], "public class "))
				{
					if (fileLists[i + 1] == "{}")
					{
						break;
					}
					else if (fileLists[i + 1] == "{")
					{
						customStart = i + 2;
						continue;
					}
				}
				if (customStart >= 0 && fileLists[i] == "}")
				{
					customEnd = i - 1;
					break;
				}
			}
		}
		// ���ҵ����Զ�����뱣�浽�б���
		if (customStart >= 0 && customEnd >= 0 && customStart <= customEnd)
		{
			int customCount = customEnd - customStart + 1;
			FOR_I((uint)customCount)
			{
				customList.push_back(fileLists[customStart + i]);
			}
		}
	}
	else
	{
		usingList.push_back("using System;");
		if (startWith(packetName, "SC"))
		{
			customList.push_back("\tpublic override void execute()");
			customList.push_back("\t{}");
		}
	}
}