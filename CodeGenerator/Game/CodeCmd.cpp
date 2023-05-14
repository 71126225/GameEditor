#include "CodeCmd.h"

void CodeCmd::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cmdFile = openTxtFile("Cmd.txt");
	if (cmdFile.length() == 0)
	{
		ERROR("δ���ļ�Cmd.txt");
		return;
	}
	// �˴���ֱ��ɨ������ļ�
	myVector<string> cmdList;
	split(cmdFile.c_str(), "\r\n", cmdList);
	generateStringDefineCmd(cmdList, cppGameStringDefineFile);
}

void CodeCmd::generateStringDefineCmd(const myVector<string>& cmdList, const string& stringDefineFile)
{
	// ����StringDefine.h���ض�����
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(stringDefineFile, codeList, lineStart,
		[](const string& codeLine) { return findSubstr(codeLine, "// Cmd"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const string& item : cmdList)
	{
		codeList.insert(++lineStart, stringDeclare(item));
	}
	writeFile(stringDefineFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}