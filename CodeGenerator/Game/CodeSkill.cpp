#include "CodeSkill.h"

void CodeSkill::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppHeaderPath = cppGamePath + "Character/Component/Skill/";

	string skillFile = openTxtFile("Skill.txt");
	if (skillFile.length() == 0)
	{
		ERROR("δ���ļ�Skill.txt");
		return;
	}
	myVector<string> skillList;
	split(skillFile.c_str(), "\r\n", skillList);
	// ����StringDefineSkill�ļ�
	generateStringDefineSkill(skillList, cppStringDefinePath);
}

// StringDefineSkill.h��StringDefineSkill.cpp
void CodeSkill::generateStringDefineSkill(const myVector<string>& skillList, const string& filePath)
{
	// ͷ�ļ�
	string header;
	line(header, "#ifdef _STRING_DEFINE_SKILL_H_");
	line(header, "#error \"����ͷ�ļ�,ֻ�ܱ�StringDefine.h������\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_SKILL_H_");
	line(header, "");
	uint count = skillList.size();
	FOR_I(count)
	{
		line(header, stringDeclare(skillList[i]));
	}
	line(header, "");
	line(header, "#endif", false);
	writeFile(filePath + "StringDefineSkill.h", ANSIToUTF8(header.c_str(), true));
}