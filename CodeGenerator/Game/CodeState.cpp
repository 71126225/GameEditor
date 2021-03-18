#include "CodeState.h"

void CodeState::generate()
{
	string cppHeaderPath = cppGamePath + "Character/Component/StateMachine/";

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
	generateStringDefineState(stateList, cppStringDefinePath);
	// ����StateHeader.h�ļ�
	generateCppStateTotalHeaderFile(stateList, cppHeaderPath);
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
	line(str0, "#endif", false);

	writeFile(filePath + "StateHeader.h", ANSIToUTF8(str0.c_str(), true));
}

// StringDefineState.h��StringDefineState.cpp
void CodeState::generateStringDefineState(const myVector<string>& stateList, string filePath)
{
	// ͷ�ļ�
	string header;
	uint count = stateList.size();
	FOR_I(count)
	{
		line(header, stringDeclare(stateList[i]));
	}
	line(header, "");
	FOR_I(count)
	{
		line(header, stringDeclare(stateList[i] + "Param"));
	}
	writeFile(filePath + "StringDefineState.h", ANSIToUTF8(header.c_str(), true));

	// Դ�ļ�
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(count)
	{
		line(source, stringDefine(stateList[i]));
	}
	line(source, "");
	FOR_I(count)
	{
		line(source, stringDefine(stateList[i] + "Param"));
	}
	writeFile(filePath + "StringDefineState.cpp", ANSIToUTF8(source.c_str(), true));
}