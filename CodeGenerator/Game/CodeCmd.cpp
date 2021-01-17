#include "CodeCmd.h"

void CodeCmd::generateCmdCode(string filePath, string headerPath)
{
	string cmdFile;
	openTxtFile("Cmd.txt", cmdFile);
	if (cmdFile.length() == 0)
	{
		ERROR("δ���ļ�Cmd.txt");
		return;
	}
	myVector<string> cmdList;
	split(cmdFile.c_str(), "\r\n", cmdList);
	// ����StringDefineCmd�ļ�
	generateStringDefineCmd(cmdList, filePath);
	// ����CommandHeader.h�ļ�
	generateCppCmdTotalHeaderFile(cmdList, headerPath);
}

// CommandHeader.h�ļ�
void CodeCmd::generateCppCmdTotalHeaderFile(const myVector<string>& cmdList, string filePath)
{
	string str0;
	line(str0, "#ifndef _COMMAND_HEADER_H_");
	line(str0, "#define _COMMAND_HEADER_H_");
	line(str0, "");
	line(str0, "#include \"CommandHeaderBase.h\"");
	line(str0, "");
	uint count = cmdList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + cmdList[i] + ".h\"");
	}
	line(str0, "");
	line(str0, "#include \"StringDefine.h\"");
	line(str0, "");
	line(str0, "#endif", false);

	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "CommandHeader.h", str0);
}

// StringDefineCmd.h��StringDefineCmd.cpp
void CodeCmd::generateStringDefineCmd(const myVector<string>& cmdList, string filePath)
{
	// ͷ�ļ�
	string header;
	uint cmdCount = cmdList.size();
	FOR_I(cmdCount)
	{
		line(header, "DECLARE_STRING(" + cmdList[i] + ");");
	}

	header = ANSIToUTF8(header.c_str(), true);
	writeFile(filePath + "StringDefineCmd.h", header);

	// Դ�ļ�
	string source;
	line(source, "#include \"StringDefine.h\"");
	line(source, "#include \"CommandHeader.h\"");
	line(source, "");
	FOR_I(cmdCount)
	{
		line(source, "DEFINE_STRING(" + cmdList[i] + ");");
	}

	source = ANSIToUTF8(source.c_str(), true);
	writeFile(filePath + "StringDefineCmd.cpp", source);
}