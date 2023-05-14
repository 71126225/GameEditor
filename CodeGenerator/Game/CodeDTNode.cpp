#include "CodeDTNode.h"

void CodeDTNode::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	const string cppHeaderPath = cppGamePath + "/Character/Component/DecisionTree/";

	string file = openTxtFile("DTNode.txt");
	if (file.length() == 0)
	{
		ERROR("δ���ļ�DTNode.txt");
		return;
	}
	myVector<string> nodeLineList;
	split(file.c_str(), "\r\n", nodeLineList);
	// ����StringDefineDTNode�ļ�
	generateStringDefine(nodeLineList, cppGameStringDefineFile);
	// ����DTNodeRegister.cpp�ļ�
	generateRegisterFile(nodeLineList, cppHeaderPath);
}

void CodeDTNode::generateStringDefine(const myVector<string>& nodeList, const string& stringDefineFile)
{
	// ����StringDefine.h���ض�����
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(stringDefineFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// DTNode"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const string& item : nodeList)
	{
		codeList.insert(++lineStart, stringDeclare(item));
	}
	writeFile(stringDefineFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

// DTNodeRegister.h��DTNodeRegister.cpp
void CodeDTNode::generateRegisterFile(const myVector<string>& nodeList, const string& headerPath)
{
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	line(source, "void DTNodeRegister::registeAll()");
	line(source, "{");
	const uint count = nodeList.size();
	FOR_I(count)
	{
		line(source, "\tGameBase::mDTNodeFactoryManager->addFactory<" + nodeList[i] + ">(NAME(" + nodeList[i] + "));");
	}
	line(source, "}", false);

	writeFile(headerPath + "DTNodeRegister.cpp", ANSIToUTF8(source.c_str(), true));
}