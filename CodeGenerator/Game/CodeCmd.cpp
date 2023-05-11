#include "CodeCmd.h"

void CodeCmd::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppHeaderPath = cppGamePath + "CommandSystem/";

	string cmdFile = openTxtFile("Cmd.txt");
	if (cmdFile.length() == 0)
	{
		ERROR("δ���ļ�Cmd.txt");
		return;
	}
	myVector<string> cmdList;
	split(cmdFile.c_str(), "\r\n", cmdList);
	// ����StringDefineCmd�ļ�
	generateStringDefineCmd(cmdList, cppStringDefinePath);
}

// StringDefineCmd.h��StringDefineCmd.cpp
void CodeCmd::generateStringDefineCmd(const myVector<string>& cmdList, const string& filePath)
{
	// ͷ�ļ�
	string header;
	line(header, "#ifdef _STRING_DEFINE_CMD_H_");
	line(header, "#error \"����ͷ�ļ�,ֻ�ܱ�StringDefine.h������\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_CMD_H_");
	line(header, "");
	uint count = cmdList.size();
	FOR_I(count)
	{
		line(header, stringDeclare(cmdList[i]));
	}
	line(header, "");
	line(header, "#endif", false);
	writeFile(filePath + "StringDefineCmd.h", ANSIToUTF8(header.c_str(), true));
}