#include "CodeDTNode.h"

void CodeDTNode::generate()
{
	myVector<string> files = findTargetHeaderFile(cppGameCorePath,
		[](const string& fileName) { return startWith(fileName, "DT"); }, 
		[](const string& line)
	{
		return findSubstr(line, " : public DTAction") || 
				findSubstr(line, " : public DTCondition") || 
				findSubstr(line, " : public DTControl") || 
				findSubstr(line, " : public DTDecorate");
	});
	// ����StringDefineDTNode�ļ�
	generateStringDefine(files, cppGameCoreStringDefineFile);
	// ����DTNodeRegister.cpp�ļ�
	generateRegisterFile(files, cppGameCorePath + "/Character/Component/DecisionTree/");
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
	line(source, "#include \"GameCoreHeader.h\"");
	line(source, "");
	line(source, "void DTNodeRegister::registeAll()");
	line(source, "{");
	const uint count = nodeList.size();
	FOR_I(count)
	{
		line(source, "\tGameCoreBase::mDTNodeFactoryManager->addFactory<" + nodeList[i] + ">(GAME_NAME(" + nodeList[i] + "));");
	}
	line(source, "}", false);

	writeFile(headerPath + "DTNodeRegister.cpp", ANSIToUTF8(source.c_str(), true));
}