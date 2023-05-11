#include "CodeSceneTrigger.h"

void CodeSceneTrigger::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppHeaderPath = cppGamePath + "SceneTriggerManager/";

	string file = openTxtFile("SceneTrigger.txt");
	if (file.length() == 0)
	{
		ERROR("δ���ļ�SceneTrigger.txt");
		return;
	}
	myVector<string> lineList;
	split(file.c_str(), "\r\n", lineList);
	bool findTriggerFlag = false;
	int triggerStart = -1;
	int triggerEnd = -1;
	FOR_VECTOR(lineList)
	{
		if (lineList[i] == "Trigger")
		{
			findTriggerFlag = true;
			continue;
		}
		if (!findTriggerFlag)
		{
			continue;
		}
		removeAll(lineList[i], '\t');
		if (lineList[i] == "{")
		{
			triggerStart = i + 1;
		}
		else if (lineList[i] == "}")
		{
			triggerEnd = i - 1;
			break;
		}
	}
	END(lineList);

	bool findModifierFlag = false;
	int modifierStart = -1;
	int modifierEnd = -1;
	FOR_I(lineListCount)
	{
		if (lineList[i] == "Modifier")
		{
			findModifierFlag = true;
			continue;
		}
		if (!findModifierFlag)
		{
			continue;
		}
		removeAll(lineList[i], '\t');
		if (lineList[i] == "{")
		{
			modifierStart = i + 1;
		}
		else if (lineList[i] == "}")
		{
			modifierEnd = i - 1;
			break;
		}
	}
	int triggerCount = triggerEnd - triggerStart + 1;
	myVector<string> triggerList;
	FOR_I((uint)triggerCount)
	{
		triggerList.push_back(lineList[triggerStart + i]);
	}
	int modifierCount = modifierEnd - modifierStart + 1;
	myVector<string> modifierList;
	FOR_I((uint)modifierCount)
	{
		modifierList.push_back(lineList[modifierStart + i]);
	}
	// ����StringDefineSceneTrigger�ļ�
	generateStringDefine(triggerList, cppStringDefinePath);
	// ����SceneTriggerRegister�ļ�
	generateCppRegister(triggerList, modifierList, cppHeaderPath);
}

// SceneTriggerRegister�ļ�
void CodeSceneTrigger::generateCppRegister(const myVector<string>& triggerList, const myVector<string>& modifierList, const string& filePath)
{
	string str0;
	line(str0, "#include \"GameHeader.h\"");
	line(str0, "");
	line(str0, "void SceneTriggerRegister::registeAll()");
	line(str0, "{");
	// ������
	FOR_VECTOR_CONST(triggerList)
	{
		string enumName = nameToUpper(triggerList[i].substr(strlen("SceneTrigger")));
		enumName.erase(0, 1);
		line(str0, "\tGameBase::mSceneTriggerFactoryManager->addFactory<" + triggerList[i] + ">(SCENE_TRIGGER::" + enumName + ");");
	}
	line(str0, "");
	// ���������޸���
	FOR_VECTOR_CONST(modifierList)
	{
		line(str0, "\tGameBase::mTriggerModifierFactoryManager->addFactory<" + modifierList[i] + ">(" + intToString(getLastNumber(modifierList[i])) + ");");
	}
	line(str0, "}", false);

	writeFile(filePath + "SceneTriggerRegister.cpp", ANSIToUTF8(str0.c_str(), true));
}

// StringDefineSceneTrigger.h��StringDefineSceneTrigger.cpp
void CodeSceneTrigger::generateStringDefine(const myVector<string>& list, const string& filePath)
{
	// ͷ�ļ�
	string header;
	line(header, "#ifdef _STRING_DEFINE_SCENE_TRIGGER_H_");
	line(header, "#error \"����ͷ�ļ�,ֻ�ܱ�StringDefine.h������\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_SCENE_TRIGGER_H_");
	line(header, "");
	FOR_VECTOR_CONST(list)
	{
		line(header, stringDeclare(list[i]));
	}
	line(header, "");
	line(header, "#endif", false);
	writeFile(filePath + "StringDefineSceneTrigger.h", ANSIToUTF8(header.c_str(), true));
}