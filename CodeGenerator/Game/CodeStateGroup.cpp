#include "CodeStateGroup.h"

void CodeStateGroup::generate()
{
	string stateFile;
	openTxtFile("StateGroup.txt", stateFile);
	if (stateFile.length() == 0)
	{
		ERROR("δ���ļ�StateGroup.txt");
		return;
	}
	myVector<string> stateList;
	split(stateFile.c_str(), "\r\n", stateList);
	// ����StringDefine�ļ�
	generateStringDefineStateGroup(stateList, cppStringDefinePath);
}

// StringDefineStateGroup.h��StringDefineStateGroup.cpp
void CodeStateGroup::generateStringDefineStateGroup(const myVector<string>& stateList, string filePath)
{
	// ͷ�ļ�
	string header;
	line(header, "#ifdef _STRING_DEFINE_STATE_GROUP_H_");
	line(header, "#error \"����ͷ�ļ�,ֻ�ܱ�StringDefine.h������\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_STATE_GROUP_H_");
	line(header, "");
	uint count = stateList.size();
	FOR_I(count)
	{
		line(header, stringDeclare(stateList[i]));
	}
	line(header, "");
	line(header, "#endif");
	writeFile(filePath + "StringDefineStateGroup.h", ANSIToUTF8(header.c_str(), true));

	// Դ�ļ�
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(count)
	{
		line(source, stringDefine(stateList[i]));
	}
	writeFile(filePath + "StringDefineStateGroup.cpp", ANSIToUTF8(source.c_str(), true));
}