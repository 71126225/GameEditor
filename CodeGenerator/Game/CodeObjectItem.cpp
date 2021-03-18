#include "CodeObjectItem.h"

void CodeObjectItem::generate()
{
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
	uint count = objectItemList.size();
	FOR_I(count)
	{
		line(header, stringDeclare(objectItemList[i]));
	}
	writeFile(filePath + "StringDefineObjectItem.h", ANSIToUTF8(header.c_str(), true));

	// Դ�ļ�
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(count)
	{
		line(source, stringDefine(objectItemList[i]));
	}
	writeFile(filePath + "StringDefineObjectItem.cpp", ANSIToUTF8(source.c_str(), true));
}