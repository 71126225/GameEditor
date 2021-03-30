#include "CodeSceneTrigger.h"

void CodeSceneTrigger::generate()
{
	string cppHeaderPath = cppGamePath + "SceneTriggerManager/";

	string triggerFile;
	openTxtFile("SceneTrigger.txt", triggerFile);
	if (triggerFile.length() == 0)
	{
		ERROR("δ���ļ�SceneTrigger.txt");
		return;
	}
	myVector<string> triggerList;
	split(triggerFile.c_str(), "\r\n", triggerList);
	// ����StringDefineSceneTrigger�ļ�
	generateStringDefineTrigger(triggerList, cppStringDefinePath);
	// ����SceneTriggerHeader.h�ļ�
	generateCppTriggerTotalHeaderFile(triggerList, cppHeaderPath);
}

// SceneTriggerHeader.h
void CodeSceneTrigger::generateCppTriggerTotalHeaderFile(const myVector<string>& triggerList, string filePath)
{
	string str0;
	line(str0, "#ifndef _SCENE_TRIGGER_HEADER_H_");
	line(str0, "#define _SCENE_TRIGGER_HEADER_H_");
	line(str0, "");
	line(str0, "#include \"SceneTrigger.h\"");
	uint count = triggerList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + triggerList[i] + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif", false);

	writeFile(filePath + "SceneTriggerHeader.h", ANSIToUTF8(str0.c_str(), true));
}

// SceneTriggerRegister�ļ�
void CodeSceneTrigger::generateCppRegister(const myVector<string>& triggerList, string filePath)
{
	string str0;
	line(str0, "#include \"GameHeader.h\"");
	line(str0, "");
	line(str0, "#define ADD_TRIGGER(trigger, type) mSceneTriggerFactoryManager->addFactory<trigger>(type)");
	line(str0, "");
	line(str0, "void SceneTriggerRegister::registeAll()");
	line(str0, "{");
	uint count = triggerList.size();
	FOR_I(count)
	{
		string enumName = nameToUpper(triggerList[i].substr(strlen("SceneTrigger")));
		line(str0, "ADD_TRIGGER(" + triggerList[i] + ", SCENE_TRIGGER::" + enumName + ");");
	}
	line(str0, "}");

	writeFile(filePath + "SceneTriggerRegister.cpp", ANSIToUTF8(str0.c_str(), true));
}

// StringDefineSceneTrigger.h��StringDefineSceneTrigger.cpp
void CodeSceneTrigger::generateStringDefineTrigger(const myVector<string>& triggerList, string filePath)
{
	// ͷ�ļ�
	string header;
	line(header, "#ifdef _STRING_DEFINE_SCENE_TRIGGER_H_");
	line(header, "#error \"����ͷ�ļ�,ֻ�ܱ�StringDefine.h������\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_SCENE_TRIGGER_H_");
	line(header, "");
	uint count = triggerList.size();
	FOR_I(count)
	{
		line(header, stringDeclare(triggerList[i]));
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
		line(source, stringDefine(triggerList[i]));
	}
	writeFile(filePath + "StringDefineSceneTrigger.cpp", ANSIToUTF8(source.c_str(), true));
}