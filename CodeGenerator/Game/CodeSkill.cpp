#include "CodeSkill.h"

void CodeSkill::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppHeaderPath = cppGamePath + "Character/Component/Skill/";

	myVector<string> list = findTargetHeaderFile(cppGamePath, 
		[](const string& fileName) 
		{
			return startWith(fileName, "PlayerSkill_") || 
				   startWith(fileName, "MonsterSkill_") || 
				   startWith(fileName, "EnvironmentSkill_"); 
		},
		[](const string& line) 
		{
			return findSubstr(line, " : public PlayerSkill") || 
				   findSubstr(line, " : public MonsterSkill") || 
				   findSubstr(line, " : public EnvironmentSkill");
		});
	// ����StringDefineSkill�ļ�
	generateStringDefineSkill(list, cppGameStringDefineFile);
}

void CodeSkill::generateStringDefineSkill(const myVector<string>& skillList, const string& stringDefineFile)
{
	// ����StringDefine.h���ض�����
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(stringDefineFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// Skill"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const string& item : skillList)
	{
		codeList.insert(++lineStart, stringDeclare(item));
	}
	writeFile(stringDefineFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}