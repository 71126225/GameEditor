#include "CodeObjectItem.h"

void CodeObjectItem::generate()
{
	myVector<string> list = findTargetHeaderFile(cppGameCorePath, [](const string& fileName) { return startWith(fileName, "ObjectItem"); },
															  [](const string& line) { return findSubstr(line, " : public ObjectItem"); });
	// ����StringDefine�ļ�
	CodeUtility::generateStringDefine(list, "// ObjectItem", "GameCoreStringDefine", cppGameCoreStringDefineHeaderFile, cppGameCoreStringDefineSourceFile);
}