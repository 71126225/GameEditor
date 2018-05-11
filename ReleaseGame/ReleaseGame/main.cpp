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
			if (ignoreFiles[i] == Utility::getFileName(files[j]))
			{
				files.erase(files.begin() + j);
				break;
			}
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	 TRACE_NEW(GameFramework, mGameFramework);
	mGameFramework->init();

	// ��ȡ�����ļ��������Ҫ���ص����������б�
	// ����ÿһ���ļ�����������Ӧ���ļ�

	// ��Ҫ���Ե��ļ�
	std::string ignoreFile = GameConfig::getStringParam(GDS_EXCEPT_FILE);
	std::vector<std::string> ignoreFiles;
	Utility::split(ignoreFile, ",", &ignoreFiles);

	// ����exe
	std::vector<std::string> fileList;
	// ԴĿ¼
	std::string strPathName = GameConfig::getStringParam(GDS_COPY_RESOUCE_DIR);
	std::vector<std::string> patterns;
	patterns.push_back(".dll");
	patterns.push_back(".exe");
	patterns.push_back(".pdb");
	Utility::findFiles(strPathName.c_str(), fileList, patterns);
	// ȥ�����к��Ե��ļ�
	removeIgnoreFile(ignoreFiles, fileList);
	// ��Ϸ��
	std::string gameName = GameConfig::getStringParam(GDS_COPY_DEST_GAME_NAME);
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char timeName[1024];
	sprintf_s(timeName, 1024, "_%d-%d-%d", sys.wYear, sys.wMonth, sys.wDay);

	// Ŀ��Ŀ¼
	std::string destPath = gameName + timeName + "/" + GameConfig::getStringParam(GDS_COPY_DEST_DIR);
	Utility::createFolder(destPath);

	// binĿ¼�������ļ�
	int listSize = fileList.size();
	for (int i = 0; i < listSize; ++i)
	{
		std::string destName;
		int splitPos = fileList[i].find_first_of(strPathName);
		int lastPos = fileList[i].find_last_of("/");
		std::string folderName = destPath + fileList[i].substr(splitPos + strPathName.length(), lastPos - splitPos - strPathName.length());
		Utility::createFolder(folderName);
		
		if (!Utility::endWith(fileList[i], "exe"))
		{
			destName = folderName + "/" + Utility::getFileName(fileList[i]);
		}
		else
		{
			destName = folderName + "/" + GameConfig::getStringParam(GDS_COPY_DEST_NAME) + ".exe";
		}
		Utility::copyFile(fileList[i], destName, true);
	}

	// ����media
	std::vector<std::string> mediaFileList;
	// ԴĿ¼
	std::string mediaPathName = GameConfig::getStringParam(GDS_COPY_RESOUCE_MEDIA_DIR); 
	std::vector<std::string> mediaPatterns;
	Utility::findFiles(mediaPathName.c_str(), mediaFileList, mediaPatterns);
	// ȥ�����к��Ե��ļ�
	removeIgnoreFile(ignoreFiles, mediaFileList);
	// Ŀ��Ŀ¼
	std::string mediaDestPath = gameName + timeName + "/" +GameConfig::getStringParam(GDS_COPY_DEST_MEDIA_DIR);
	Utility::createFolder(mediaDestPath);
	int mediaListSize = mediaFileList.size();
	for (int i = 0; i < mediaListSize; ++i)
	{
		std::string destName;
		int splitPos = mediaFileList[i].find_first_of(mediaPathName);
		int lastPos = mediaFileList[i].find_last_of("/");
		std::string folderName = mediaDestPath + mediaFileList[i].substr(splitPos + mediaPathName.length(), lastPos - splitPos - mediaPathName.length());
		Utility::createFolder(folderName);
		destName = folderName + "/" + Utility::getFileName(mediaFileList[i]);
		Utility::copyFile(mediaFileList[i], destName, true);
	}
	std::cout << "�汾���������!" << std::endl;
	system("pause");
	return 0;
}