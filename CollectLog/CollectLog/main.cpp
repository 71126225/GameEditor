#include "FileUtility.h"
#include "StringUtility.h"
#include "EditorCoreUtility.h"

void getAllProcess(std::set<std::string>& processList)
{
	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//���ϵͳ���վ�� 
	PROCESSENTRY32* info = new PROCESSENTRY32;
	info->dwSize = sizeof(PROCESSENTRY32);
	//����һ�� Process32First �������ӿ����л�ȡ�����б� 
	Process32First(handle, info);
	//�ظ����� Process32Next��ֱ���������� FALSE Ϊֹ 
	while (Process32Next(handle, info))
	{
		processList.insert(info->szExeFile);
	}
	delete info;
}

bool isGameRunning(const std::string& name)
{
	// �жϵ�ǰ�Ƿ��Ѿ������˸���Ϸ,����Ѿ�������ֱ���˳�
	std::set<std::string> processList;
	getAllProcess(processList);
	if (processList.find(name) != processList.end())
	{
		return true;
	}
	return false;
}

bool hasFile(const std::string& fileName)
{
	FILE* pFile = NULL;
	fopen_s(&pFile, fileName.c_str(), "rb");
	if (pFile == NULL)
	{
		return false;
	}
	fclose(pFile);
	return true;
}

int getDayCount(std::string dateStr)
{
	std::string dayStr = dateStr.substr(0, dateStr.find_first_of(' '));
	std::string year = dayStr.substr(0, dayStr.find_first_of('-'));
	dayStr = dayStr.substr(dayStr.find_first_of('-') + 1, dayStr.length() - dayStr.find_first_of('-') - 1);
	std::string month = dayStr.substr(0, dayStr.find_first_of('-'));
	dayStr = dayStr.substr(dayStr.find_first_of('-') + 1, dayStr.length() - dayStr.find_first_of('-') - 1);
	std::string day = dayStr;
	int dayCount = StringUtility::stringToInt(year) * 356 + StringUtility::stringToInt(month) * 30 + StringUtility::stringToInt(day);
	return dayCount;
}

void copyFile(const std::string& src, const std::string& dest)
{
	if (!FileUtility::isFileExist(StringUtility::getParentDir(dest)))
	{
		FileUtility::createFolder(StringUtility::getParentDir(dest));
	}
	CopyFileA(src.c_str(), dest.c_str(), true);
	DeleteFileA(src.c_str());
}

std::string getDateString()
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char fileName[1024];
	sprintf_s(fileName, 1024, "%d-%d-%d %dʱ%d��%d��", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	return fileName;
}

int WINAPI WinMain(HINSTANCE   hInstance,              // Instance
	HINSTANCE   hPrevInstance,              // Previous Instance
	LPSTR       lpCmdLine,              // Command Line Parameters
	int     nCmdShow)               // Window Show State
{
	std::string gameName = "���ܶ���4.4";
	std::string logDestPath = "Log/";
	std::string logName = "output_log.txt";
	int maxDayCount = 7;
	// ɾ��7��ǰ����־
	int curDayCount = getDayCount(getDateString());
	std::vector<std::string> fileList;
	std::vector<std::string> patterns;
	patterns.push_back(".txt");
	FileUtility::findFiles("./" + logDestPath, fileList, patterns);
	int count = fileList.size();
	for (int i = 0; i < count; ++i)
	{
		std::string dayString = StringUtility::getFileNameNoSuffix(fileList[i]);
		if (curDayCount - getDayCount(dayString) >= maxDayCount)
		{
			DeleteFileA(fileList[i].c_str());
		}
	}
	while (true)
	{
		Sleep(100);
		if (isGameRunning(gameName + ".exe"))
		{
			continue;
		}
		// �����Ϸû������,���Ҵ�����־�ļ�,�ͽ���־�ļ��������ⲿ�ļ���
		if (hasFile(logName))
		{
			std::string fileName = getDateString();
			copyFile(logName, logDestPath + fileName + ".txt");
			std::cout << "����ȡ��־ : " << fileName << std::endl;
		}
	}
	return 0;
}