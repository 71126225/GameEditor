#include "CodeSkill.h"

void CodeSkill::generateSkillCode(string filePath, string headerPath)
{
	string skillFile;
	openTxtFile("Skill.txt", skillFile);
	if (skillFile.length() == 0)
	{
		ERROR("δ���ļ�Skill.txt");
		return;
	}
	myVector<string> skillList;
	split(skillFile.c_str(), "\r\n", skillList);
	// ����StringDefineSkill�ļ�
	generateStringDefineSkill(skillList, filePath);
	// ����CharacterSkillHeader.h�ļ�
	generateCppSkillTotalHeaderFile(skillList, headerPath);
}

// CharacterSkillHeader.h
void CodeSkill::generateCppSkillTotalHeaderFile(const myVector<string>& skillList, string filePath)
{
	string str0;
	line(str0, "#ifndef _CHARACTER_SKILL_HEADER_H_");
	line(str0, "#define _CHARACTER_SKILL_HEADER_H_");
	line(str0, "");
	uint count = skillList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + skillList[i] + ".h\"");
	}
	line(str0, "");
	line(str0, "#include \"StateInterfaceHeader.h\"");
	line(str0, "");
	line(str0, "#endif", false);
	validPath(filePath);
	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "CharacterSkillHeader.h", str0);
}

// StringDefineSkill.h��StringDefineSkill.cpp
void CodeSkill::generateStringDefineSkill(const myVector<string>& skillList, string filePath)
{
	// ͷ�ļ�
	string header;
	uint cmdCount = skillList.size();
	FOR_I(cmdCount)
	{
		line(header, "DECLARE_STRING(" + skillList[i] + ");");
	}
	validPath(filePath);
	header = ANSIToUTF8(header.c_str(), true);
	writeFile(filePath + "StringDefineSkill.h", header);

	// Դ�ļ�
	string source;
	line(source, "#include \"StringDefine.h\"");
	line(source, "#include \"CharacterSkillHeader.h\"");
	line(source, "");
	FOR_I(cmdCount)
	{
		line(source, "DEFINE_STRING(" + skillList[i] + ");");
	}
	source = ANSIToUTF8(source.c_str(), true);
	writeFile(filePath + "StringDefineSkill.cpp", source);
}