#include "CodeCmd.h"

void CodeCmd::generate()
{
	string cppHeaderPath = cppGamePath + "CommandSystem/";

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
	generateStringDefineCmd(cmdList, cppStringDefinePath);
	// ����CommandHeader.h�ļ�
	generateCppCmdTotalHeaderFile(cmdList, cppHeaderPath);
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

	writeFile(filePath + "CommandHeader.h", ANSIToUTF8(str0.c_str(), true));
}

// StringDefineCmd.h��StringDefineCmd.cpp
void CodeCmd::generateStringDefineCmd(const myVector<string>& cmdList, string filePath)
{
	// ͷ�ļ�
	string header;
	uint cmdCount = cmdList.size();
	FOR_I(cmdCount)
	{
		line(header, stringDeclare(cmdList[i]));
	}

	writeFile(filePath + "StringDefineCmd.h", ANSIToUTF8(header.c_str(), true));

	// Դ�ļ�
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(cmdCount)
	{
		line(source, stringDefine(cmdList[i]));
	}

	writeFile(filePath + "StringDefineCmd.cpp", ANSIToUTF8(source.c_str(), true));
}