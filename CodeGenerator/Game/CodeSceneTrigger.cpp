#include "CodeSceneTrigger.h"

void CodeSceneTrigger::generate()
{
	string cppHeaderPath = cppGamePath + "SceneTriggerManager/";

	string file;
	openTxtFile("SceneTrigger.txt", file);
	if (file.length() == 0)
	{
		ERROR("δ���ļ�SceneTrigger.txt");
		return;
	}
	myVector<string> list;
	split(file.c_str(), "\r\n", list);
	// ����StringDefineSceneTrigger�ļ�
	generateStringDefine(list, cppStringDefinePath);
	// ����SceneTriggerHeader.h�ļ�
	generateCppTotalHeaderFile(list, cppHeaderPath);
	// ����SceneTriggerRegister�ļ�
	generateCppRegister(list, cppHeaderPath);
}

// SceneTriggerHeader.h
void CodeSceneTrigger::generateCppTotalHeaderFile(const myVector<string>& list, string filePath)
{
	string str0;
	line(str0, "#ifndef _SCENE_TRIGGER_HEADER_H_");
	line(str0, "#define _SCENE_TRIGGER_HEADER_H_");
	line(str0, "");
	line(str0, "#include \"SceneTrigger.h\"");
	uint count = list.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + list[i] + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif", false);

	writeFile(filePath + "SceneTriggerHeader.h", ANSIToUTF8(str0.c_str(), true));
}

// SceneTriggerRegister�ļ�
void CodeSceneTrigger::generateCppRegister(const myVector<string>& list, string filePath)
{
	string str0;
	line(str0, "#include \"GameHeader.h\"");
	line(str0, "");
	line(str0, "#define ADD_TRIGGER(trigger, type) mSceneTriggerFactoryManager->addFactory<trigger>(type)");
	line(str0, "");
	line(str0, "void SceneTriggerRegister::registeAll()");
	line(str0, "{");
	uint count = list.size();
	FOR_I(count)
	{
		string enumName = nameToUpper(list[i].substr(strlen("SceneTrigger")));
		enumName.erase(0, 1);
		line(str0, "\tADD_TRIGGER(" + list[i] + ", SCENE_TRIGGER::" + enumName + ");");
	}
	line(str0, "}");

	writeFile(filePath + "SceneTriggerRegister.cpp", ANSIToUTF8(str0.c_str(), true));
}

// StringDefineSceneTrigger.h��StringDefineSceneTrigger.cpp
void CodeSceneTrigger::generateStringDefine(const myVector<string>& list, string filePath)
{
	// ͷ�ļ�
	string header;
	line(header, "#ifdef _STRING_DEFINE_SCENE_TRIGGER_H_");
	line(header, "#error \"����ͷ�ļ�,ֻ�ܱ�StringDefine.h������\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_SCENE_TRIGGER_H_");
	line(header, "");
	uint count = list.size();
	FOR_I(count)
	{
		line(header, stringDeclare(list[i]));
	}
	line(header, "");
	line(header, "#endif");
	writeFile(filePath + "StringDefineSceneTrigger.h", ANSIToUTF8(header.c_str(), true));

	// Դ�ļ�
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(count)
	{
		line(source, stringDefine(list[i]));
	}
	writeFile(filePath + "StringDefineSceneTrigger.cpp", ANSIToUTF8(source.c_str(), true));
}