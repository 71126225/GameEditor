#include "CodeComponent.h"

void CodeComponent::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	const string cppRegisterPath = cppGamePath + "/Component/";

	string stateFile = openTxtFile("Component.txt");
	if (stateFile.length() == 0)
	{
		ERROR("δ���ļ�Component.txt");
		return;
	}

	myVector<string> stateList;
	split(stateFile.c_str(), "\r\n", stateList);
	// ����StringDefine�ļ�
	generateStringDefineComponent(stateList, cppGameStringDefineFile);
	// ComponentRegister.cpp
	generateComponentRegister(stateList, cppRegisterPath);
}

void CodeComponent::generateStringDefineComponent(const myVector<string>& componentList, const string& stringDefineFile)
{
	// ����StringDefine.h���ض�����
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(stringDefineFile, codeList, lineStart,
		[](const string& codeLine) { return findSubstr(codeLine, "// Component"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const string& item : componentList)
	{
		codeList.insert(++lineStart, stringDeclare(item));
	}
	writeFile(stringDefineFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

// ComponentRegister.cpp
void CodeComponent::generateComponentRegister(const myVector<string>& componentList, const string& filePath)
{
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	line(source, "void ComponentRegister::registeAll()");
	line(source, "{");
	FOR_VECTOR_CONST(componentList)
	{
		line(source, "\tFrameBase::mGameComponentFactoryManager->addFactory<" + componentList[i] + ">(NAME(" + componentList[i] + "));");
	}
	line(source, "}", false);
	writeFile(filePath + "ComponentRegister.cpp", ANSIToUTF8(source.c_str(), true));
}