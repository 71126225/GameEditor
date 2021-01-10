#include "CodeState.h"

void CodeState::generateStateCode(string filePath, string headerPath)
{
	string stateFile;
	openTxtFile("State.txt", stateFile);
	if (stateFile.length() == 0)
	{
		ERROR("δ���ļ�State.txt");
		return;
	}
	myVector<string> stateList;
	split(stateFile.c_str(), "\r\n", stateList);
	// ����StringDefineState�ļ�
	generateStringDefineState(stateList, filePath);
	// ����StateHeader.h�ļ�
	generateCppStateTotalHeaderFile(stateList, headerPath);
}

// StateHeader.h
void CodeState::generateCppStateTotalHeaderFile(const myVector<string>& stateList, string filePath)
{
	string str0;
	line(str0, "#ifndef _STATE_HEADER_H_");
	line(str0, "#define _STATE_HEADER_H_");
	line(str0, "");
	uint count = stateList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + stateList[i] + ".h\"");
	}
	line(str0, "");
	line(str0, "#include \"StateInterfaceHeader.h\"");
	line(str0, "");
	line(str0, "#endif", false);

	validPath(filePath);
	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "StateHeader.h", str0);
}

// StringDefineState.h��StringDefineState.cpp
void CodeState::generateStringDefineState(const myVector<string>& stateList, string filePath)
{
	// ͷ�ļ�
	string header;
	uint cmdCount = stateList.size();
	FOR_I(cmdCount)
	{
		line(header, "DECLARE_STRING(" + stateList[i] + ");");
	}

	validPath(filePath);
	header = ANSIToUTF8(header.c_str(), true);
	writeFile(filePath + "StringDefineState.h", header);

	// Դ�ļ�
	string source;
	line(source, "#include \"StringDefine.h\"");
	line(source, "#include \"StateHeader.h\"");
	line(source, "");
	FOR_I(cmdCount)
	{
		line(source, "DEFINE_STRING(" + stateList[i] + ");");
	}

	source = ANSIToUTF8(source.c_str(), true);
	writeFile(filePath + "StringDefineState.cpp", source);
}