#include <iostream>
#include <windows.h>
#include <shellapi.h>
#include <vector>
#include <string>
#include <set>
#include <tlhelp32.h>
#include "resource.h"

void findFiles(const char* pathName, std::vector<std::string>& files, const std::vector<std::string>& patterns, bool recursive);
void leftToRight(std::string& str);
void getAllProcess(std::set<std::string>& processList);

int main(int argc, char* argv[])
{
	HWND hwnd = GetForegroundWindow();//ֱ�ӻ��ǰ�����ڵľ��         
	SendMessageA(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICON)));

	// �ҵ���ǰ����Ϸ����
	std::string curFullPathWithFileName = argv[0];
	std::string curPath = curFullPathWithFileName.substr(0, curFullPathWithFileName.find_last_of('\\'));
	std::vector<std::string> files;
	std::vector<std::string> patterns;
	patterns.push_back(".exe");
	findFiles(curPath.c_str(), files, patterns, false);
	int fileCount = files.size();
	// ת��/Ϊ\\,�����Ƴ��������Լ�
	int selfIndex = -1;
	for (int i = 0; i < fileCount; ++i)
	{
		leftToRight(files[i]);
		if (files[i] == curFullPathWithFileName)
		{
			selfIndex = i;
		}
	}
	if (selfIndex != -1)
	{
		files.erase(files.begin() + selfIndex);
	}
	if (files.size() == 0)
	{
		std::cout << "�Ҳ�����Ϸ����!" << std::endl;
		system("pause");
		return 0;
	}
	std::string fullPathWithFileName = files[0];
	int lastPos = fullPathWithFileName.find_last_of('\\');
	std::string gameName = fullPathWithFileName.substr(lastPos + 1, fullPathWithFileName.length() - lastPos - 1);
	std::string fullPath = fullPathWithFileName.substr(0, lastPos);
	// �жϵ�ǰ�Ƿ��Ѿ������˸���Ϸ,����Ѿ�������ֱ���˳�
	std::set<std::string> processList;
	getAllProcess(processList);
	if (processList.find(gameName) != processList.end())
	{
		return 0;
	}
	
	SHELLEXECUTEINFO sei;
	memset(&sei, 0, sizeof(sei));
	sei.cbSize = sizeof(sei);
	sei.lpFile = fullPathWithFileName.c_str();
	sei.lpDirectory = fullPath.c_str();
	sei.lpVerb = "open";
#ifndef __WXWINCE__
	sei.nShow = SW_SHOWNORMAL; // SW_SHOWDEFAULT not defined under CE (#10216)
#else
	sei.nShow = SW_SHOWDEFAULT;
#endif
	sei.fMask = SEE_MASK_FLAG_NO_UI;
	if (!ShellExecuteEx(&sei))
	{
		std::cout << "�Ҳ�����Ϸ����!" << std::endl;
		system("pause");
	}
	return 0;
}

void findFiles(const char * pathName, std::vector<std::string>& files, const std::vector<std::string>& patterns, bool recursive)
{
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	std::string path(pathName);
	if (path.length() == 0)
	{
		return;
	}
	// ����/��β,�����/
	if (path[path.length() - 1] != '/')
	{
		path += "/";
	}

	hFind = FindFirstFileA((path + "*").c_str(), &FindFileData);
	// ����Ҳ����ļ��о�ֱ�ӷ���
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}
	while (::FindNextFileA(hFind, &FindFileData))
	{
		// ����.��..
		if (strcmp(FindFileData.cFileName, ".") == 0
			|| strcmp(FindFileData.cFileName, "..") == 0)
		{
			continue;
		}

		// ��������·��
		std::string fullname = path + std::string(FindFileData.cFileName);
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (recursive)
			{
				findFiles(fullname.c_str(), files, patterns, recursive);
			}
		}
		else
		{
			int patternCount = patterns.size();
			if (patternCount > 0)
			{
				for (int i = 0; i < patternCount; ++i)
				{
					if (fullname.length() >= patterns[i].length() && fullname.substr(fullname.length() - patterns[i].length(), patterns[i].length()) == patterns[i])
					{
						files.push_back(fullname);
					}
				}
			}
			else
			{
				files.push_back(fullname);
			}
		}
	}
	::FindClose(hFind);
}

void leftToRight(std::string& str)
{
	int pathLength = str.length();
	for (int i = 0; i < pathLength; ++i)
	{
		if (str[i] == '/')
		{
			str[i] = '\\';
		}
	}
}

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