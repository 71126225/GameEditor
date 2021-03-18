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
void CodeComponent::generateStringDefineComponent(const myVector<string>& componentList, string filePath)
{
	// ͷ�ļ�
	string header;
	uint count = componentList.size();
	FOR_I(count)
	{
		line(header, stringDeclare(componentList[i]));
	}

	writeFile(filePath + "StringDefineComponent.h", ANSIToUTF8(header.c_str(), true));

	// Դ�ļ�
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(count)
	{
		line(source, stringDefine(componentList[i]));
	}

	writeFile(filePath + "StringDefineComponent.cpp", ANSIToUTF8(source.c_str(), true));
}