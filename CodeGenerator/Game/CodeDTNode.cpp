#include "CodeDTNode.h"

void CodeDTNode::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	myVector<string> files = findTargetHeaderFile(cppGamePath, "DT", [](const string& line) 
	{
		return findSubstr(line, " : public DTAction") || 
				findSubstr(line, " : public DTCondition") || 
				findSubstr(line, " : public DTControl") || 
				findSubstr(line, " : public DTDecorate");
	});
	// ����StringDefineDTNode�ļ�
	generateStringDefine(files, cppGameStringDefineFile);
	// ����DTNodeRegister.cpp�ļ�
	generateRegisterFile(files, cppGamePath + "/Character/Component/DecisionTree/");
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