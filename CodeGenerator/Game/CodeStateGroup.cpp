#include "CodeStateGroup.h"

void CodeStateGroup::generate()
{
	myVector<string> list = findTargetHeaderFile(cppGameCorePath, [](const string& fileName) { return startWith(fileName, "StateGroup"); },
															  [](const string& line) { return findSubstr(line, " : public StateGroup"); });
	// ����StringDefine�ļ�
	generateStringDefineStateGroup(list, cppGameCoreStringDefineFile);
}

void CodeStateGroup::generateStringDefineStateGroup(const myVector<string>& stateList, const string& stringDefineFile)
{
	// ����StringDefine.h���ض�����
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(stringDefineFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// StateGroup"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const string& item : stateList)
	{
		codeList.insert(++lineStart, stringDeclare(item));
	}
	writeFile(stringDefineFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}