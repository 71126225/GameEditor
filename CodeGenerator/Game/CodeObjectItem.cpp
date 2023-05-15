#include "CodeObjectItem.h"

void CodeObjectItem::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	myVector<string> list = findTargetHeaderFile(cppGamePath, [](const string& fileName) { return startWith(fileName, "ObjectItem"); },
															  [](const string& line) { return findSubstr(line, " : public ObjectItem"); });
	// ����StringDefine�ļ�
	generateStringDefineObjectItem(list, cppGameStringDefineFile);
}

void CodeObjectItem::generateStringDefineObjectItem(const myVector<string>& objectItemList, const string& stringDefineFile)
{
	// ����StringDefine.h���ض�����
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(stringDefineFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// ObjectItem"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const string& item : objectItemList)
	{
		codeList.insert(++lineStart, stringDeclare(item));
	}
	writeFile(stringDefineFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}