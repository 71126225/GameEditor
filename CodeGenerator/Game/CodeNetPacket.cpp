#include "CodeNetPacket.h"

void CodeNetPacket::generate()
{
	string cppCSDeclarePath = cppGamePath + "Socket/PacketDeclareCS/";
	string cppSCDeclarePath = cppGamePath + "Socket/PacketDeclareSC/";
	string cppCSPacketPath = cppGamePath + "Socket/ClientServer/";
	string cppSCPacketPath = cppGamePath + "Socket/ServerClient/";
	string cppPacketDefinePath = cppGamePath + "Socket/";
	string csharpCSHeaderPath = csGamePath + "Socket/PacketHeaderCS/";
	string csharpSCHeaderPath = csGamePath + "Socket/PacketHeaderSC/";
	string csharpSCPacketPath = csGamePath + "Socket/ServerClient/";
	string csharpPacketDefinePath = csGamePath + "Socket/";

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
	string tempPacketName;
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
			info.mPacketName = lines[i - 1];
			packetInfoList.push_back(info);
			continue;
		}
		// ��Ա�����б���ʼ
		if (line == "{")
		{
			packetStart = true;
			tempPacketName = lines[i - 1];
			tempMemberList.clear();
			continue;
		}
		// ��Ա�����б����
		if (line == "}")
		{
			PacketInfo info;
			info.mPacketName = tempPacketName;
			info.mMemberList = tempMemberList;
			packetInfoList.push_back(info);
			packetStart = false;
			tempMemberList.clear();
			tempPacketName = "";
			continue;
		}
		if (packetStart)
		{
			tempMemberList.push_back(parseMemberLine(line));
		}
	}

	// ɾ�����ļ�
	deleteFolder(cppCSDeclarePath);
	deleteFolder(cppSCDeclarePath);
	// c#��ֻɾ�������ļ�,��ɾ��meta�ļ�
	myVector<string> csharpCSFileList;
	findFiles(csharpCSHeaderPath, csharpCSFileList, ".cs");
	FOR_VECTOR_CONST(csharpCSFileList)
	{
		deleteFile(csharpCSFileList[i]);
	}
	myVector<string> csharpSCFileList;
	findFiles(csharpSCHeaderPath, csharpSCFileList, ".cs");
	FOR_VECTOR_CONST(csharpSCFileList)
	{
		deleteFile(csharpSCFileList[i]);
	}

	myVector<string> packetList;
	FOR_VECTOR_CONST(packetInfoList)
	{
		// ���ɴ����ļ�
		// .h����
		generateCppCSPacketFileHeader(packetInfoList[i], cppCSPacketPath);
		generateCppCSPacketFileSource(packetInfoList[i], cppCSPacketPath);
		generateCppSCPacketFile(packetInfoList[i], cppSCPacketPath);
		// .cs����
		generateCSharpDecalreFile(packetInfoList[i], csharpCSHeaderPath, csharpSCHeaderPath);
		generateCSharpSCPacketFile(packetInfoList[i].mPacketName, csharpSCPacketPath);
		packetList.push_back(packetInfoList[i].mPacketName);
	}
	// c++
	generateCppPacketDefineFile(packetInfoList, cppPacketDefinePath);
	generateCppPacketRegisteFile(packetInfoList, cppPacketDefinePath);
	generateCppPacketTotalHeaderFile(packetInfoList, cppPacketDefinePath);
	generateStringDefinePacket(packetList, cppStringDefinePath);
	// c#
	generateCSharpPacketDefineFile(packetInfoList, csharpPacketDefinePath);
	generateCSharpPacketRegisteFile(packetInfoList, csharpPacketDefinePath);
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
	line(str, "#include \"ServerDefine.h\"");
	line(str, "");
	line(str, "enum class PACKET_TYPE : ushort");
	line(str, "{");
	line(str, "\tMIN,");
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
	line(str, "\tCS_MAX,");
	line(str, "");
	line(str, "\tSC_MIN = 6000,");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			line(str, "\t" + packetNameToUpper(packetList[i].mPacketName) + ",");
		}
	}
	line(str, "\tSC_MAX,");
	line(str, "};");
	line(str, "");
	line(str, "#endif", false);

	writeFile(filePath + "PacketDefine.h", ANSIToUTF8(str.c_str(), true));
}

// PacketRegister.cpp�ļ�
void CodeNetPacket::generateCppPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath)
{
	string str;
	line(str, "#include \"GameHeader.h\"");
	line(str, "");
	line(str, "#define PACKET_FACTORY(packet, type) mPacketFactoryManager->addFactory<packet>(PACKET_TYPE::type, NAME(packet));");
	line(str, "");
	line(str, "void PacketRegister::registeAll()");
	line(str, "{");
	line(str, "\tuint preCount = mPacketFactoryManager->getFactoryCount();");
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			line(str, "\tPACKET_FACTORY(" + packetList[i].mPacketName + ", " + packetNameToUpper(packetList[i].mPacketName) + ");");
		}
	}
	line(str, "\tmPacketFactoryManager->checkRegisteCount(preCount, (int)PACKET_TYPE::CS_MAX - (int)PACKET_TYPE::CS_MIN - 1, \"CS\");");
	line(str, "");
	line(str, "\tpreCount = mPacketFactoryManager->getFactoryCount();");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			line(str, "\tPACKET_FACTORY(" + packetList[i].mPacketName + ", " + packetNameToUpper(packetList[i].mPacketName) + ");");
		}
	}
	line(str, "\tmPacketFactoryManager->checkRegisteCount(preCount, (int)PACKET_TYPE::SC_MAX - (int)PACKET_TYPE::SC_MIN - 1, \"SC\");");
	line(str, "};", false);

	writeFile(filePath + "PacketRegister.cpp", ANSIToUTF8(str.c_str(), true));
}

// StringDefinePacket.h��StringDefinePacket.cpp
void CodeNetPacket::generateStringDefinePacket(const myVector<string>& packetList, string filePath)
{
	// ͷ�ļ�
	string header;
	uint cmdCount = packetList.size();
	FOR_I(cmdCount)
	{
		line(header, stringDeclare(packetList[i]));
	}
	writeFile(filePath + "StringDefinePacket.h", ANSIToUTF8(header.c_str(), true));

	// Դ�ļ�
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(cmdCount)
	{
		line(source, stringDefine(packetList[i]));
	}
	writeFile(filePath + "StringDefinePacket.cpp", ANSIToUTF8(source.c_str(), true));
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
	if (!isFileExist(headerFullPath))
	{
		string header;
		string marco = nameToUpper(packetName.substr(2));
		line(header, "#ifndef _CS" + marco + "_H_");
		line(header, "#define _CS" + marco + "_H_");
		line(header, "");
		line(header, "#include \"Packet.h\"");
		line(header, "");
		line(header, "class " + packetName + " : public Packet");
		line(header, "{");
		line(header, "public:");
		const auto& memberList = packetInfo.mMemberList;
		FOR_VECTOR_CONST(memberList)
		{
			line(header, "\t" + cppMemberDeclareString(memberList[i]));
		}
		line(header, "public:");
		if (memberListCount == 0)
		{
			line(header, "\tvoid fillParams() override{}");
		}
		else
		{
			line(header, "\tvoid fillParams() override");
			line(header, "\t{");
			FOR_I(memberListCount)
			{
				line(header, "\t\t" + cppPushParamString(memberList[i]));
			}
			line(header, "\t}");
		}
		line(header, "\tvoid execute() override;");
		line(header, "\tvoid debugInfo(char* buffer, int size) override");
		line(header, "\t{");
		line(header, "\t\tPACKET_DEBUG(\"\");");
		line(header, "\t}");
		line(header, "};");
		line(header, "");
		line(header, "#endif", false);

		writeFile(headerFullPath, ANSIToUTF8(header.c_str(), true));
	}
	else
	{
		updateOldFormatPackteFile(packetInfo, headerFullPath);
	}
}

void CodeNetPacket::updateOldFormatPackteFile(const PacketInfo& packetInfo, string fullPath)
{
	const string& packetName = packetInfo.mPacketName;
	// ���ļ�,�������ļ�
	string headerFile;
	openTxtFile(fullPath, headerFile);
	headerFile = UTF8ToANSI(headerFile.c_str(), true);
	myVector<string> headerLines;
	split(headerFile.c_str(), "\r\n", headerLines, false);
	// �ҵ���һ��{
	int bracketIndex = headerLines.find("{");
	if (bracketIndex < 0)
	{
		ERROR("������Ϣ���ļ���������,�Ҳ���{");
		return;
	}
	if (bracketIndex + 1 >= (int)headerLines.size())
	{
		ERROR("������Ϣ���ļ���������,{���������һ��");
		return;
	}
	const auto& memberList = packetInfo.mMemberList;
	// �����һ���Ǻ�,��˵���Ǿɸ�ʽ���ļ�,��Ҫ���滻
	if (findSubstr(headerLines[bracketIndex + 1], packetName + "_Declare;"))
	{
		myVector<string> newLines;
		newLines.push_back("public:");
		FOR_VECTOR_CONST(memberList)
		{
			newLines.push_back("\t" + cppMemberDeclareString(memberList[i]));
		}
		// �����һ��Ӧ����public:,�������޷�ʶ��ĸ�ʽ
		if (headerLines[bracketIndex + 2] != "public:")
		{
			ERROR("������Ϣ���ļ���������,�����һ��Ӧ����public:");
			return;
		}
		newLines.push_back("public:");
		if (memberListCount == 0)
		{
			newLines.push_back("\tvoid fillParams() override{}");
		}
		else
		{
			newLines.push_back("\tvoid fillParams() override");
			newLines.push_back("\t{");
			FOR_I(memberListCount)
			{
				newLines.push_back("\t\t" + cppPushParamString(memberList[i]));
			}
			newLines.push_back("\t}");
		}
		headerLines.erase(bracketIndex + 2);
		headerLines.erase(bracketIndex + 1);
		// ���¼ӵ��в��뵽ԭ�����ļ���
		FOR_VECTOR(newLines)
		{
			headerLines.insert(headerLines.begin() + bracketIndex + 1 + i, newLines[i]);
		}
		END(newLines);
	}
	// �¸�ʽ���ļ�,Ҳ����û�������ĺ�,�滻����������ע��
	else
	{
		// �ҵ���һ��public:
		int firstPublicIndex = headerLines.find("public:");
		int secondPublicIndex = headerLines.find("public:", firstPublicIndex + 1);
		if (firstPublicIndex < 0 || secondPublicIndex < 0)
		{
			ERROR("������Ϣ���ļ���������,public:������С��2��");
			return;
		}
		// �Ƴ�����public:֮��ɵı�������
		uint oldVariableCount = secondPublicIndex - firstPublicIndex - 1;
		FOR_I(oldVariableCount)
		{
			headerLines.erase(secondPublicIndex - 1 - i);
		}
		FOR_VECTOR_CONST(memberList)
		{
			headerLines.insert(firstPublicIndex + 1 + i, "\t" + cppMemberDeclareString(memberList[i]));
		}
		secondPublicIndex += memberList.size() - oldVariableCount;
		// �ҵ�fillParams()
		int fillParamIndex = -1;
		FOR_VECTOR(headerLines)
		{
			if (findSubstr(headerLines[i], "void fillParams()"))
			{
				fillParamIndex = i;
				break;
			}
		}
		END(headerLines);
		if (fillParamIndex < 0)
		{
			ERROR("������Ϣ���ļ���������,�Ҳ���fillParams����");
			return;
		}
		// �������б���ע��
		if (headerLines[fillParamIndex + 1] == "\t{")
		{
			int endBracketIndex = headerLines.find("\t}", fillParamIndex + 2);
			// �Ƴ�������ע��ı���
			FOR_I(oldVariableCount)
			{
				headerLines.erase(fillParamIndex + 2 + oldVariableCount - 1 - i);
			}
			// ���û���µı���,���Ƴ�������,�������źϲ���������������
			if (memberListCount == 0)
			{
				headerLines.erase(fillParamIndex + 2);
				headerLines.erase(fillParamIndex + 1);
				headerLines[fillParamIndex] = "\t\tvoid fillParams() override{}";
			}
			else
			{
				// ����µı���
				FOR_I(memberListCount)
				{
					headerLines.insert(fillParamIndex + 2 + i, "\t\t" + cppPushParamString(memberList[i]));
				}
			}
		}
		// ������û�б���ע��
		else
		{
			// ����µı�������ҲΪ0,��������,����0ʱ�Ŵ���
			if (memberListCount > 0)
			{
				headerLines[fillParamIndex] += "\t\tvoid fillParams() override";
				headerLines.insert(fillParamIndex + 1, "\t{");
				// ����µı���
				FOR_I(memberListCount)
				{
					headerLines.insert(fillParamIndex + 2 + i, "\t\t" + cppPushParamString(memberList[i]));
				}
				headerLines.insert(fillParamIndex + 2 + memberListCount, "\t}");
			}
		}
	}
	// �����µ��ļ�
	string newFile;
	FOR_VECTOR(headerLines)
	{
		newFile += headerLines[i];
		if (i != headerLinesCount - 1)
		{
			newFile += "\r\n";
		}
	}
	END(headerLines);
	writeFile(fullPath, ANSIToUTF8(newFile.c_str(), true));
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
		line(source, "\tCharacterPlayer* player = (CharacterPlayer*)getCharacter();");
		line(source, "\tif (player == NULL)");
		line(source, "\t{");
		line(source, "\t\treturn;");
		line(source, "\t}");
		line(source, "}", false);

		writeFile(sourceFullPath, ANSIToUTF8(source.c_str(), true));
	}
}

// SCPacket.h�ļ�
void CodeNetPacket::generateCppSCPacketFile(const PacketInfo& packetInfo, string filePath)
{
	const string& packetName = packetInfo.mPacketName;
	if (!startWith(packetName, "SC"))
	{
		return;
	}

	// SCPacket.h
	string headerFullPath = filePath + packetName + ".h";
	if (!isFileExist(headerFullPath))
	{
		string header;
		string marco = nameToUpper(packetName.substr(2));
		line(header, "#ifndef _SC" + marco + "_H_");
		line(header, "#define _SC" + marco + "_H_");
		line(header, "");
		line(header, "#include \"Packet.h\"");
		line(header, "");
		line(header, "class " + packetName + " : public Packet");
		line(header, "{");
		line(header, "public:");
		const auto& memberList = packetInfo.mMemberList;
		FOR_VECTOR_CONST(memberList)
		{
			line(header, "\t" + cppMemberDeclareString(memberList[i]));
		}
		line(header, "public:");
		if (memberListCount == 0)
		{
			line(header, "\tvoid fillParams() override{}");
		}
		else
		{
			line(header, "\tvoid fillParams() override");
			line(header, "\t{");
			FOR_I(memberListCount)
			{
				line(header, "\t\t" + cppPushParamString(memberList[i]));
			}
			line(header, "\t}");
		}
		line(header, "\tvoid debugInfo(char* buffer, int size) override");
		line(header, "\t{");
		line(header, "\t\tPACKET_DEBUG(\"\")");
		line(header, "\t}");
		line(header, "};");
		line(header, "");
		line(header, "#endif", false);

		writeFile(headerFullPath, ANSIToUTF8(header.c_str(), true));
	}
	else
	{
		updateOldFormatPackteFile(packetInfo, headerFullPath);
	}
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
	line(str, "\tpublic static ushort CS_MIN = " + intToString(csPacketNumber) + ";");
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
	line(str, "\tpublic static ushort CS_MAX = " + intToString(csPacketNumber) + ";");
	line(str, "");
	int scPacketNumber = 6000;
	line(str, "\tpublic static ushort SC_MIN = " + intToString(scPacketNumber) + ";");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			++scPacketNumber;
			line(str, "\tpublic static ushort " + packetNameToUpper(packetList[i].mPacketName) + " = " + intToString(scPacketNumber) + ";");
		}
	}
	++scPacketNumber;
	line(str, "\tpublic static ushort SC_MAX = " + intToString(scPacketNumber) + ";");
	line(str, "}", false);

	writeFile(filePath + "PacketDefine.cs", ANSIToUTF8(str.c_str(), true));
}

// PacketRegister.cs�ļ�
void CodeNetPacket::generateCSharpPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath)
{
	string str;
	line(str, "using System;");
	line(str, "");
	line(str, "public class PacketRegister : GameBase");
	line(str, "{");
	line(str, "\tpublic static void registeAll()");
	line(str, "\t{");
	line(str, "\t\tint preCount = mSocketFactory.getPacketTypeCount();");
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			line(str, "\t\tregistePacket(Typeof<" + packetList[i].mPacketName + ">(), PACKET_TYPE." + packetNameToUpper(packetList[i].mPacketName) + ");");
		}
	}
	line(str, "\t\tmSocketFactory.checkRegisteCount(PACKET_TYPE.CS_MAX - PACKET_TYPE.CS_MIN - 1, preCount, \"CS\");");
	line(str, "");
	line(str, "\t\tpreCount = mSocketFactory.getPacketTypeCount();");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			line(str, "\t\tregistePacket(Typeof<" + packetList[i].mPacketName + ">(), PACKET_TYPE." + packetNameToUpper(packetList[i].mPacketName) + ");");
		}
	}
	line(str, "\t\tmSocketFactory.checkRegisteCount(PACKET_TYPE.SC_MAX - PACKET_TYPE.SC_MIN - 1, preCount, \"SC\");");
	line(str, "\t}");
	line(str, "\tprotected static void registePacket(Type classType, ushort type)");
	line(str, "\t{");
	line(str, "\t\tmSocketFactory.registePacket(classType, type);");
	line(str, "\t\tmSocketFactoryThread.registePacket(classType, type);");
	line(str, "\t}");
	line(str, "}", false);

	writeFile(filePath + "PacketRegister.cs", ANSIToUTF8(str.c_str(), true));
}

// Packet_Declare.cs�ļ�
void CodeNetPacket::generateCSharpDecalreFile(const PacketInfo& packetInfo, string csFilePath, string scFilePath)
{
	const int prefixLength = 2;
	if (packetInfo.mPacketName.substr(0, prefixLength) != "CS" && packetInfo.mPacketName.substr(0, prefixLength) != "SC")
	{
		ERROR("����ǰ׺����");
		return;
	}
	string file;
	line(file, "using System;");
	line(file, "");
	line(file, "public partial class " + packetInfo.mPacketName + " : SocketPacket");
	line(file, "{");
	uint memberCount = packetInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		line(file, "\t" + cSharpMemberDeclareString(packetInfo.mMemberList[i]));
	}
	if (memberCount > 0)
	{
		line(file, "\tprotected override void fillParams()");
		line(file, "\t{");
		FOR_I(memberCount)
		{
			line(file, "\t\t" + cSharpPushParamString(packetInfo.mMemberList[i]));
		}
		line(file, "\t}");
	}
	line(file, "}", false);

	if (startWith(packetInfo.mPacketName, "CS"))
	{
		writeFile(csFilePath + packetInfo.mPacketName + "_Declare.cs", ANSIToUTF8(file.c_str(), true));
	}
	else if (startWith(packetInfo.mPacketName, "SC"))
	{
		writeFile(scFilePath + packetInfo.mPacketName + "_Declare.cs", ANSIToUTF8(file.c_str(), true));
	}
}

void CodeNetPacket::generateCSharpSCPacketFile(const string& packetName, string filePath)
{
	if (!startWith(packetName, "SC"))
	{
		return;
	}
	string fullPath = filePath + packetName + ".cs";
	if (!isFileExist(fullPath))
	{
		string file;
		line(file, "using System;");
		line(file, "");
		line(file, "public partial class " + packetName + " : SocketPacket");
		line(file, "{");
		line(file, "\tpublic override void execute()");
		line(file, "\t{");
		line(file, "\t}");
		line(file, "}", false);

		writeFile(fullPath, ANSIToUTF8(file.c_str(), true));
	}
}