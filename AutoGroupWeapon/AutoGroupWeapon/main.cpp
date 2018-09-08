#include <iostream>
#include "Utility.h"

#define ITOA(i) Utility::intToString(i)
#define ATOI(s) Utility::stringToInt(s)

std::string getSection(std::string& str, const std::string& key)
{
	if (str == "")
	{
		return "";
	}
	int pos = str.find_first_of(key);
	std::string section;
	if (pos != -1)
	{
		section = str.substr(0, pos);
		str = str.substr(pos + key.length(), str.length() - pos - key.length());
	}
	else
	{
		section = str;
	}
	return section;
}

void main()
{
	std::cout << "����(1)" << std::endl;
	std::cout << "����(2)" << std::endl;
	std::cout << "����(3)" << std::endl;
	int type = 0;
	std::cin >> type;
	if (type == 1)
	{
		std::cout << "��������ID:";
		int weaponID = 0;
		std::cin >> weaponID;
		std::vector<std::string> fileList;
		Utility::findFiles("./", fileList, ".png", false);
		// ����ÿ���ļ�����Ϣ
		int fileCount = fileList.size();
		for (int i = 0; i < fileCount; ++i)
		{
			std::string fileName = Utility::getFileName(fileList[i]);
			int direction = ATOI(getSection(fileName, "_"));
			std::string action = getSection(fileName, "_");
			int index = ATOI(getSection(fileName, "_"));
			// ������Ŀ��Ŀ¼
			std::string destFolder = "weapon_id" + ITOA(weaponID) + "_dir" + ITOA(direction) + "_" + action;
			std::string destFileName = destFolder + "_" + ITOA(index) + ".png";
			Utility::copyFile(fileList[i], destFolder + "/" + destFileName);
		}
	}
	else if (type == 2)
	{
		std::cout << "����ְҵID:";
		int occupation = 0;
		std::cin >> occupation;
		std::cout << "�����·�ID:";
		int humanID = 0;
		std::cin >> humanID;

		std::vector<std::string> fileList;
		Utility::findFiles("./", fileList, ".png", false);
		// ����ÿ���ļ�����Ϣ
		int fileCount = fileList.size();
		for (int i = 0; i < fileCount; ++i)
		{
			std::string fileName = Utility::getFileName(fileList[i]);
			int direction = ATOI(getSection(fileName, "_"));
			std::string action = getSection(fileName, "_");
			int index = ATOI(getSection(fileName, "_"));
			// ������Ŀ��Ŀ¼
			std::string destFolder = "human_occup" + ITOA(occupation) + "_dir" + ITOA(direction) + "_cloth" + ITOA(humanID) + "_" + action;
			std::string destFileName = destFolder + "_" + ITOA(index) + ".png";
			Utility::copyFile(fileList[i], destFolder + "/" + destFileName);
		}
	}
	else if (type == 3)
	{
		std::cout << "�������ID:";
		int monsterID = 0;
		std::cin >> monsterID;

		std::vector<std::string> fileList;
		Utility::findFiles("./", fileList, ".png", false);
		// ����ÿ���ļ�����Ϣ
		int fileCount = fileList.size();
		for (int i = 0; i < fileCount; ++i)
		{
			std::string fileName = Utility::getFileName(fileList[i]);
			int direction = ATOI(getSection(fileName, "_"));
			std::string action = getSection(fileName, "_");
			int index = ATOI(getSection(fileName, "_"));
			// ������Ŀ��Ŀ¼
			std::string destFolder = "monster_id" + ITOA(monsterID) + "_dir" + ITOA(direction) + "_" + action;
			std::string destFileName = destFolder + "_" + ITOA(index) + ".png";
			Utility::copyFile(fileList[i], destFolder + "/" + destFileName);
		}
	}
}