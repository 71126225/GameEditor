#include "CodeObjectItem.h"

void CodeObjectItem::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string stateFile;
	openTxtFile("ObjectItem.txt", stateFile);
	if (stateFile.length() == 0)
	{
		ERROR("δ���ļ�ObjectItem.txt");
		return;
	}
	myVector<string> stateList;
	split(stateFile.c_str(), "\r\n", stateList);
	// ����StringDefine�ļ�
	generateStringDefineObjectItem(stateList, cppStringDefinePath);
}

// StringDefineObjectItem.h��StringDefineObjectItem.cpp
void CodeObjectItem::generateStringDefineObjectItem(const myVector<string>& objectItemList, string filePath)
{
	// ͷ�ļ�
	string header;
	line(header, "#ifdef _STRING_DEFINE_OBJECT_ITEM_H_");
	line(header, "#error \"����ͷ�ļ�,ֻ�ܱ�StringDefine.h������\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_OBJECT_ITEM_H_");
	line(header, "");
	uint count = objectItemList.size();
	FOR_I(count)
	{
		line(header, stringDeclare(objectItemList[i]));
	}
	line(header, "");
	line(header, "#endif");
	writeFile(filePath + "StringDefineObjectItem.h", ANSIToUTF8(header.c_str(), true));

	// Դ�ļ�
	string source;
	line(source, "#include \"GameHeader.h\"");
	writeFile(filePath + "StringDefineObjectItem.cpp", ANSIToUTF8(source.c_str(), true));
}