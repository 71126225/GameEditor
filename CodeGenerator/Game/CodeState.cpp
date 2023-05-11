#include "CodeState.h"

void CodeState::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppHeaderPath = cppGamePath + "Character/Component/StateMachine/";

	string stateFile = openTxtFile("State.txt");
	if (stateFile.length() == 0)
	{
		ERROR("δ���ļ�State.txt");
		return;
	}
	myVector<string> stateList;
	split(stateFile.c_str(), "\r\n", stateList);
	// ����StringDefineState�ļ�
	generateStringDefineState(stateList, cppStringDefinePath);
	// ����StateRegister.cpp�ļ�
	generateStateRegister(stateList, cppHeaderPath);
}

// StringDefineState.h��StringDefineState.cpp
void CodeState::generateStringDefineState(const myVector<string>& stateList, const string& filePath)
{
	// ͷ�ļ�
	string header;
	line(header, "#ifdef _STRING_DEFINE_STATE_H_");
	line(header, "#error \"����ͷ�ļ�,ֻ�ܱ�StringDefine.h������\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_STATE_H_");
	line(header, "");
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
	line(header, "");
	line(header, "#endif", false);
	writeFile(filePath + "StringDefineState.h", ANSIToUTF8(header.c_str(), true));
}

void CodeState::generateStateRegister(const myVector<string>& stateList, const string& filePath)
{
	myVector<string> preCodeList;
	myVector<string> endCodeList;
	findCustomCode(filePath + "StateRegister.cpp", preCodeList, endCodeList);
	myVector<string> stateRegisteList;
	FOR_VECTOR_CONST(stateList)
	{
		if (startWith(stateList[i], "StateAction") || startWith(stateList[i], "StateBehaviour"))
		{
			continue;
		}
		stateRegisteList.push_back("STATE_FACTORY(" + stateList[i] + ", CHARACTER_STATE::" + nameToUpper(stateList[i], false) + ");");
	}

	// �����µ��ļ�
	string source;
	FOR_VECTOR(preCodeList)
	{
		line(source, preCodeList[i]);
	}
	END(preCodeList);
	FOR_VECTOR(stateRegisteList)
	{
		line(source, "\t" + stateRegisteList[i]);
	}
	END(stateRegisteList);
	line(source, "");
	FOR_VECTOR(endCodeList)
	{
		line(source, endCodeList[i]);
	}
	END(endCodeList);
	writeFile(filePath + "StateRegister.cpp", ANSIToUTF8(source.c_str(), true));
}

void CodeState::findCustomCode(const string& fullPath, myVector<string>& preCodeList, myVector<string>& endCodeList)
{
	myVector<string> fileLines;
	openTxtFileLines(fullPath, fileLines);
	int preCodeEnd = -1;
	int endCodeStart = -1;
	FOR_VECTOR(fileLines)
	{
		if (preCodeEnd < 0 && fileLines[i] == "\t// buff״̬")
		{
			preCodeEnd = i;
			continue;
		}
		if (endCodeStart < 0 && fileLines[i] == "\t// ��Ϊ״̬")
		{
			endCodeStart = i;
			break;
		}
	}
	END(fileLines);
	if (preCodeEnd < 0)
	{
		ERROR("״̬ע������ļ���������,// buff״̬�ؼ���δ�ҵ�,�ļ�����:" + intToString(fileLines.size()) + ":" + fullPath);
		return;
	}
	if (endCodeStart < 0)
	{
		ERROR("״̬ע������ļ���������,// ��Ϊ״̬�ؼ���δ�ҵ�,�ļ�����:" + intToString(fileLines.size()) + ":" + fullPath);
		return;
	}
	FOR_I(fileLinesCount)
	{
		if ((int)i <= preCodeEnd)
		{
			preCodeList.push_back(fileLines[i]);
		}
		else if ((int)i >= endCodeStart)
		{
			endCodeList.push_back(fileLines[i]);
		}
	}
}