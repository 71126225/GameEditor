#include "CodeSceneTrigger.h"

void CodeSceneTrigger::generate()
{
	string cppHeaderPath = cppGameCorePath + "SceneTriggerManager/";

	myVector<string> triggerList = findTargetHeaderFile(cppGameCorePath, 
		[](const string& fileName) { return startWith(fileName, "SceneTrigger"); },
		[](const string& line) { return findSubstr(line, " : public SceneTrigger"); });
	myVector<string> modifierList = findTargetHeaderFile(cppGameCorePath, 
		[](const string& fileName) { return startWith(fileName, "TriggerModifier"); },
		[](const string& line) { return findSubstr(line, " : public TriggerModifier"); });
	// ����StringDefineSceneTrigger�ļ�
	CodeUtility::generateStringDefine(triggerList, "// SceneTrigger", "GameCoreStringDefine", cppGameCoreStringDefineHeaderFile, cppGameCoreStringDefineSourceFile);
	// ����SceneTriggerRegister�ļ�
	generateCppRegister(triggerList, modifierList, cppHeaderPath);
}

// SceneTriggerRegister�ļ�
void CodeSceneTrigger::generateCppRegister(const myVector<string>& triggerList, const myVector<string>& modifierList, const string& filePath)
{
	string str0;
	line(str0, "#include \"GameCoreHeader.h\"");
	line(str0, "");
	line(str0, "void SceneTriggerRegister::registeAll()");
	line(str0, "{");
	// ������
	FOR_VECTOR_CONST(triggerList)
	{
		string enumName = nameToUpper(triggerList[i].substr(strlen("SceneTrigger")));
		enumName.erase(0, 1);
		line(str0, "\tGameCoreBase::mSceneTriggerFactoryManager->addFactory<" + triggerList[i] + ">(SCENE_TRIGGER::" + enumName + ");");
	}
	line(str0, "");
	// ���������޸���
	FOR_VECTOR_CONST(modifierList)
	{
		line(str0, "\tGameCoreBase::mTriggerModifierFactoryManager->addFactory<" + modifierList[i] + ">(" + intToString(getLastNumber(modifierList[i])) + ");");
	}
	line(str0, "}", false);

	writeFile(filePath + "SceneTriggerRegister.cpp", ANSIToUTF8(str0.c_str(), true));
}

void CodeSceneTrigger::generateStringDefine(const myVector<string>& list, const string& stringDefineFile)
{
	// ����StringDefine.h���ض�����
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(stringDefineFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// "); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const string& item : list)
	{
		codeList.insert(++lineStart, stringDeclare(item));
	}
	writeFile(stringDefineFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}