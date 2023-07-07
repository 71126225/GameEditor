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
	CodeUtility::generateStringDefine(files, "// DTNode", "GameCoreStringDefine", cppGameCoreStringDefineHeaderFile, cppGameCoreStringDefineSourceFile);
	// ����DTNodeRegister.cpp�ļ�
	generateRegisterFile(files, cppGameCorePath + "/Character/Component/DecisionTree/");
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
		line(source, "\tGameCoreBase::mDTNodeFactoryManager->addFactory<" + nodeList[i] + ">(GAME_CORE_NAME(" + nodeList[i] + "));");
	}
	line(source, "}", false);

	writeFile(headerPath + "DTNodeRegister.cpp", ANSIToUTF8(source.c_str(), true));
}