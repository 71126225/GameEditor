#include "CodeState.h"

void CodeState::generate()
{
	string stateFile = openTxtFile("State.txt");
	if (stateFile.length() == 0)
	{
		ERROR("δ���ļ�State.txt");
		return;
	}
	myVector<string> stateList;
	split(stateFile.c_str(), "\r\n", stateList);
	// ����StringDefineState�ļ�
	generateStringDefineState(stateList, cppGameStringDefineFile);
	// ����StateRegister.cpp�ļ�
	generateStateRegister(stateList, cppGamePath + "Character/Component/StateMachine/StateRegister.cpp");
}

void CodeState::generateStringDefineState(const myVector<string>& stateList, const string& stringDefineFile)
{
	// ����StringDefine.h���ض�����
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(stringDefineFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// State"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const string& item : stateList)
	{
		codeList.insert(++lineStart, stringDeclare(item));
	}
	for (const string& item : stateList)
	{
		codeList.insert(++lineStart, stringDeclare(item + "Param"));
	}
	writeFile(stringDefineFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

void CodeState::generateStateRegister(const myVector<string>& stateList, const string& filePath)
{
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(filePath, codeList, lineStart,
		[](const string& codeLine) { return codeLine == "\t// buff״̬"; },
		[](const string& codeLine) { return codeLine == "\t// ��Ϊ״̬"; }))
	{
		return;
	}
	myVector<string> stateRegisteList;
	FOR_VECTOR_CONST(stateList)
	{
		if (startWith(stateList[i], "StateAction") || startWith(stateList[i], "StateBehaviour"))
		{
			continue;
		}
		codeList.insert(++lineStart, "\tSTATE_FACTORY(" + stateList[i] + ", CHARACTER_STATE::" + nameToUpper(stateList[i], false) + ");");
	}
	codeList.insert(++lineStart, "");

	// �����µ��ļ�
	writeFile(filePath, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}