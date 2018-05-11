#pragma once

#include <string>
#include <vector>

#include "txConfig.h"

#define TX_MAX(x, y) ((x) > (y) ? (x) : (y))
#define TX_MIN(x, y) ((x) < (y) ? (x) : (y))

class Utility
{
public:
	Utility();
	~Utility();
public:
#if RUN_PLATFORM == PLATFORM_ANDROID
	static bool IsDirectory(char *pszName);
	static void findFiles(const char * pathName, std::vector<std::string>& files, std::vector<std::string> patterns);
#elif RUN_PLATFORM == PLATFORM_WINDOWS
	static void findFiles(const char * pathName, std::vector<std::string>& files, std::vector<std::string> patterns);
	static void deleteFolder(std::string path);
#endif

	// ��sourceFile������destFile,sourceFile��destFile���Ǵ���ֱ�ӷ��ʵ�·�����ļ���,overWriteָ����Ŀ���ļ��Ѿ�����ʱ�Ƿ�Ҫ�����ļ�
	static bool copyFile(std::string sourceFile, std::string destFile, bool overWrite = true);
	// ����һ���ļ���,path��һ������/��β�Ŀ�ֱ�ӷ��ʵ���Ի��߾��Ե��ļ�����
	static bool createFolder(std::string path);
	static bool writeFile(std::string filePath, int length, const char* buffer);

	static void rightToLeft(std::string& str)
	{
		int pathLength = str.length();
		for (int i = 0; i < pathLength; ++i)
		{
			if ('\\' == str[i])
			{
				str[i] = '/';
			}
		}
	}

	static bool Utility::isFileExist(std::string fullPath);

	static std::string intToString(int i, int limitLen = 0); // limitLen���ַ�������С����,���������λ��������С����,�����ǰ���0
	static int stringToInt(std::string str)
	{
		return atoi(str.c_str());
	}
	static float pow_f(float f, int p);
	static std::string floatToString(float f, int precision);	//precision -> ����,������С����λ��
	static float stringToFloat(std::string str)
	{
		return (float)atof(str.c_str());
	}

	static void split(std::string str, const char * deli, std::vector<std::string> *vec);

	// ����media��·��,����/
	static std::string getMediaPath()
	{
#if RUN_PLATFORM == PLATFORM_WINDOWS
		return "./ToolMedia";
#elif RUN_PLATFORM == PLATFORM_ANDROID
		// �����AssetManager�м�����Դ,��mediaĿ¼Ϊ���ַ���
#ifdef LOAD_FROM_ASSETMANAGER
		return "";
#else
		return "/sdcard/txtj";
#endif
#endif
	}

	// ͨ��һ��media�µ����·��,���߾���·��,ת��Ϊһ����ֱ�Ӵ򿪵�·��
	static std::string getAvailableResourcePath(std::string name)
	{
		std::string mediaPath = getMediaPath();
		// ����ļ����Ѿ��ǲ���media·�����Ҳ��Ǿ���·��
		if (mediaPath != "" && (name.length() <= mediaPath.length() || name.substr(0, mediaPath.length()) != mediaPath) && (name.length() > 1 && name[1] != ':'))
		{
			name = mediaPath + "/" + name;
		}
		return name;
	}
	// �ж�oriString�Ƿ���pattern��β
	static bool endWith(std::string oriString, std::string pattern)
	{
		if (oriString.length() < pattern.length())
		{
			return false;
		}
		std::string endString = oriString.substr(oriString.length() - pattern.length(), pattern.length());
		return endString == pattern;
	}
	// �ж�oriString�Ƿ���pattern��ͷ
	static bool startWith(std::string oriString, std::string pattern)
	{
		if (oriString.length() < pattern.length())
		{
			return false;
		}
		std::string startString = oriString.substr(0, pattern.length());
		return startString == pattern;
	}

	static std::string getParentDir(std::string dir)
	{
		rightToLeft(dir);
		int pos = dir.find_last_of('/');
		std::string tempDir = dir;
		if (-1 != pos)
		{
			tempDir = dir.substr(0, pos);
		}

		return tempDir;
	}

	static std::string getFileName(std::string str)
	{
		rightToLeft(str);
		int dotPos = str.find_last_of('/');
		if (dotPos != -1)
		{
			return str.substr(dotPos + 1, str.length() - 1);
		}
		return str;
	}

	static std::string getFileNameNoSuffix(std::string str)
	{
		rightToLeft(str);
		int namePos = str.find_last_of('/');
		int dotPos = str.find_last_of('.');
		if (dotPos != -1)
		{
			return str.substr(namePos + 1, dotPos - namePos - 1);
		}

		return str;
	}
};

