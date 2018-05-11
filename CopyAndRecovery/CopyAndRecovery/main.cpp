// ReleaseGame.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <stdio.h>
#include <tchar.h>
#include <vector>

#include <windows.h>      // Header File For Windows
#include <windowsx.h>
#include <mmsystem.h>

#include "Utility.h"
#include "CommonDefine.h"
#include "GameConfig.h"
#include "GameFramework.h"
#include "txMemeryTrace.h"

GameFramework* mGameFramework = NULL;

void removeIgnoreFile(const std::vector<std::string>& ignoreFiles, std::vector<std::string>& files)
{
	int ignoreFileCount = ignoreFiles.size();
	for (int i = 0; i < ignoreFileCount; ++i)
	{
		int allFileCount = files.size();
		for (int j = 0; j < allFileCount; ++j)
		{
			if (ignoreFiles[i] == files[j])
			{
				files.erase(files.begin() + j);
				break;
			}
		}
	}
}

// �ƶ�fileList�е������ļ�,���б��е������ļ���sourcePrePath�ƶ���destPrePath��
void moveResources(const std::vector<std::string>& fileList, const std::string& sourcePrePath, const std::string& destPrePath)
{
	int fileCount = fileList.size();
	for (int i = 0; i < fileCount; ++i)
	{
		std::string fileName = fileList[i];
		std::string sourceFile = fileName;
		if (Utility::startWith(fileName, sourcePrePath))
		{
			sourceFile = fileName.substr(sourcePrePath.length(), fileName.length() - sourcePrePath.length());
		}
		std::string destFile = destPrePath + sourceFile;
		Utility::createFolder(Utility::getParentDir(destFile));
		if (!Utility::copyFile(fileName, destFile))
		{
			std::cout << "����" << fileName << "ʧ��" << std::endl;
			system("pause");
		}
		else
		{
			if (!Utility::deleteFile(fileName))
			{
				std::cout << "ɾ��" << fileName << "ʧ��" << std::endl;
				system("pause");
			}
		}
	}
}

// ɾ�����ļ���
void deleteFolder(std::string path, std::vector<std::string> folderList)
{
	Utility::deleteEmptyFolder(path, folderList);
	int count = (int)folderList.size();
	for (int i = 0; i < count; ++i)
	{
		Utility::deleteFolder(folderList[i], true);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	TRACE_NEW(GameFramework, mGameFramework);
	mGameFramework->init();
	std::cout << "1.������Դ" << std::endl;
	std::cout << "2.�ָ���Դ" << std::endl;
	int a = 0;
	std::cin >> a;
	std::vector<std::string> folderList;
	std::string backupPath = GameConfig::getStringParam(GDS_COPY_FILE_DEST_DIR);
	if (a == 1)
	{
		std::string sourcePath = GameConfig::getStringParam(GDS_COPY_FILE_DIR);
		std::vector<std::string> backupFileList;
		// ������Ҫ���ݵ�ȫ���ļ�
		std::vector<std::string> pathList;
		Utility::split(sourcePath, ",", &pathList);
		int count = pathList.size();
		for (int i = 0; i < count; ++i)
		{
			Utility::findFiles(pathList[i].c_str(), backupFileList);
		}
		// �ų������ݵ��ļ�
		std::string exceptPath = GameConfig::getStringParam(GDS_EXCEPT_FILE);
		std::vector<std::string> exceptPathList;
		Utility::split(exceptPath, ",", &exceptPathList);
		int exceptPathCount = (int)exceptPathList.size();
		for (int i = 0; i < (int)backupFileList.size();)
		{
			bool isExceptFile = false;
			for (int j = 0; j < exceptPathCount; ++j)
			{
				if (Utility::startWith(backupFileList[i], exceptPathList[j]))
				{
					isExceptFile = true;
					break;
				}
			}
			if (isExceptFile)
			{
				backupFileList.erase(backupFileList.begin() + i);
			}
			else
			{
				++i;
			}
		}
		// �����ļ��������ļ���
		Utility::deleteFolder(backupPath, false);
		moveResources(backupFileList, "../", backupPath + "/");
		for (int i = 0; i < count; ++i)
		{
			deleteFolder(pathList[i], folderList);
		}
	}
	else
	{
		std::vector<std::string> backupFileList;
		Utility::findFiles(backupPath.c_str(), backupFileList);
		moveResources(backupFileList, backupPath + "/", "../");
		deleteFolder(backupPath, folderList);
	}
	std::cout << "���!" << std::endl;
	system("pause");
	return 0;
}