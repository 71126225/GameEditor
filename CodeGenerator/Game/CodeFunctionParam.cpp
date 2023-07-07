#include "CodeFunctionParam.h"

void CodeFunctionParam::generate()
{
	// Game
	myVector<string> listGame = findTargetHeaderFile(cppGamePath, 
		[](const string& fileName) { return startWith(fileName, "Param"); },
		[](const string& line) { return findSubstr(line, " : public FunctionParam"); });
	// ����StringDefineFunctionParam�ļ�
	CodeUtility::generateStringDefine(listGame, "// FcuntionParam", "GameStringDefine", cppGameStringDefineHeaderFile, cppGameStringDefineSourceFile);
	// ����FunctionParamRegister�ļ�
	generateCppGameRegister(listGame, cppGamePath + "FunctionParam/");

	// GameCore
	myVector<string> listGameCore = findTargetHeaderFile(cppGameCorePath, 
		[](const string& fileName) { return startWith(fileName, "Param"); },
		[](const string& line) { return findSubstr(line, " : public FunctionParam"); });
	// ����StringDefineFunctionParam�ļ�
	CodeUtility::generateStringDefine(listGameCore, "// FcuntionParam", "GameCoreStringDefine", cppGameCoreStringDefineHeaderFile, cppGameCoreStringDefineSourceFile);
	// ����FunctionParamRegister�ļ�
	generateCppGameCoreRegister(listGameCore, cppGameCorePath + "FunctionParam/");
}

// FunctionParamRegister�ļ�
void CodeFunctionParam::generateCppGameRegister(const myVector<string>& paramList, const string& filePath)
{
	string str0;
	line(str0, "#include \"GameHeader.h\"");
	line(str0, "");
	line(str0, "void GameFunctionParamRegister::registeAll()");
	line(str0, "{");
	for (const string& item : paramList)
	{
		line(str0, "\tGameCoreBase::mFunctionParamFactoryManager->addFactory<" + item + ">(GAME_NAME(" + item + "));");
	}
	line(str0, "}", false);

	writeFile(filePath + "GameFunctionParamRegister.cpp", ANSIToUTF8(str0.c_str(), true));
}

void CodeFunctionParam::generateCppGameCoreRegister(const myVector<string>& paramList, const string& filePath)
{
	string str0;
	line(str0, "#include \"GameCoreHeader.h\"");
	line(str0, "");
	line(str0, "void GameCoreFunctionParamRegister::registeAll()");
	line(str0, "{");
	for (const string& item : paramList)
	{
		line(str0, "\tGameCoreBase::mFunctionParamFactoryManager->addFactory<" + item + ">(GAME_CORE_NAME(" + item + "));");
	}
	line(str0, "}", false);

	writeFile(filePath + "GameCoreFunctionParamRegister.cpp", ANSIToUTF8(str0.c_str(), true));
}