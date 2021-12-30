#include "CodeComponent.h"

void CodeComponent::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppRegisterPath = cppGamePath + "/Component/";

	string stateFile;
	openTxtFile("Component.txt", stateFile);
	if (stateFile.length() == 0)
	{
		ERROR("δ���ļ�Component.txt");
		return;
	}

	myVector<string> stateList;
	split(stateFile.c_str(), "\r\n", stateList);
	// ����StringDefine�ļ�
	generateStringDefineComponent(stateList, cppStringDefinePath);
	// ComponentRegister.cpp
	generateComponentRegister(stateList, cppRegisterPath);
}

// StringDefineComponent.h��StringDefineComponent.cpp
void CodeComponent::generateStringDefineComponent(const myVector<string>& componentList, string filePath)
{
	// ͷ�ļ�
	string header;
	line(header, "#ifdef _STRING_DEFINE_COMPONENT_H_");
	line(header, "#error \"����ͷ�ļ�,ֻ�ܱ�StringDefine.h������\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_COMPONENT_H_");
	line(header, "");
	uint count = componentList.size();
	FOR_I(count)
	{
		line(header, stringDeclare(componentList[i]));
	}
	line(header, "");
	line(header, "#endif");
	writeFile(filePath + "StringDefineComponent.h", ANSIToUTF8(header.c_str(), true));
}

// ComponentRegister.cpp
void CodeComponent::generateComponentRegister(const myVector<string>& componentList, string filePath)
{
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	line(source, "#define COMPONENT_FACTORY(T) mGameComponentFactoryManager->addFactory<T>(NAME(T))");
	line(source, "");
	line(source, "void ComponentRegister::registeAll()");
	line(source, "{");
	FOR_VECTOR_CONST(componentList)
	{
		line(source, "\tCOMPONENT_FACTORY(" + componentList[i] + ");");
	}
	line(source, "}");
	writeFile(filePath + "ComponentRegister.cpp", ANSIToUTF8(source.c_str(), true));
}