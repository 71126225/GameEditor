#include "CodeComponent.h"

void CodeComponent::generate()
{
	string stateFile;
	openTxtFile("Component.txt", stateFile);
	if (stateFile.length() == 0)
	{
		ERROR("δ���ļ�Component.txt");
		return;
	}
	myVector<string> stateList;
	split(stateFile.c_str(), "\r\n", stateList);
	// ����StringDefine�ļ�
	generateStringDefineComponent(stateList, cppStringDefinePath);
}

// StringDefineComponent.h��StringDefineComponent.cpp
void CodeComponent::generateStringDefineComponent(const myVector<string>& stateList, string filePath)
{
	// ͷ�ļ�
	string header;
	uint cmdCount = stateList.size();
	FOR_I(cmdCount)
	{
		line(header, stringDeclare(stateList[i]));
	}

	header = ANSIToUTF8(header.c_str(), true);
	writeFile(filePath + "StringDefineComponent.h", header);

	// Դ�ļ�
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(cmdCount)
	{
		line(source, stringDefine(stateList[i]));
	}

	source = ANSIToUTF8(source.c_str(), true);
	writeFile(filePath + "StringDefineComponent.cpp", source);
}