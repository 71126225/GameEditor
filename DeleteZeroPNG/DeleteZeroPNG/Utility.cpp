#include "Utility.h"

std::string Utility::validPath(const std::string& path)
{
	std::string temp = path;
	if (temp.length() > 0)
	{
		// ����/��β,�����/
		if (temp[temp.length() - 1] != '/')
		{
			temp += "/";
		}
	}
	return temp;
}

void Utility::findFiles(const std::string& pathName, std::vector<std::string>& files, const std::string& patterns, bool recursive)
{
	std::vector<std::string> patternList;
	patternList.push_back(patterns);
	findFiles(pathName, files, patternList, recursive);
}

void Utility::findFiles(const std::string& path, std::vector<std::string>& files, const std::vector<std::string>& patterns, bool recursive)
{
	std::string tempPath = validPath(path);
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA((tempPath + "*").c_str(), &FindFileData);
	// ����Ҳ����ļ��о�ֱ�ӷ���
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}
	do
	{
		// ����.��..
		if (strcmp(FindFileData.cFileName, ".") == 0
			|| strcmp(FindFileData.cFileName, "..") == 0)
		{
			continue;
		}

		// ��������·��
		std::string fullname = tempPath + std::string(FindFileData.cFileName);
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
					if (endWith(fullname, patterns[i], false))
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
	} while (::FindNextFileA(hFind, &FindFileData));
	::FindClose(hFind);
}

bool Utility::endWith(const std::string& oriString, const std::string& pattern, bool sensitive)
{
	if (oriString.length() < pattern.length())
	{
		return false;
	}
	std::string endString = oriString.substr(oriString.length() - pattern.length(), pattern.length());
	if (sensitive)
	{
		return endString == pattern;
	}
	else
	{
		strToLower(endString);
		std::string temp = pattern;
		strToLower(temp);
		return endString == temp;
	}
}

void Utility::strToLower(std::string& str)
{
	std::string::size_type size = str.length();
	for (std::string::size_type i = 0; i != size; ++i)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
		{
			str[i] += 'a' - 'A';
		}
	}
}

void Utility::moveFile(const std::string& source, const std::string& dest)
{
	MoveFileA(source.c_str(), dest.c_str());
}

std::string Utility::intToString(int i, int limitLen)
{
	char str[256];
	sprintf_s(str, 256, "%d", i);
	std::string retString(str);
	int addLen = limitLen - strlen(str);
	if (addLen > 0)
	{
		char addStr[256];
		memset(addStr, 0, 256);
		for (int j = 0; j < addLen; ++j)
		{
			addStr[j] = '0';
		}
		retString = std::string(addStr) + retString;
	}
	return retString;
}

bool Utility::createFolder(const std::string& path)
{
	// ���Ŀ¼�Ѿ�����,�򷵻�true
	if (isFileExist(path))
	{
		return true;
	}
	// ����ļ����������鸸Ŀ¼�Ƿ����
	if (!isFileExist(path))
	{
		// �������һ��Ŀ¼,������һ��Ŀ¼������,���ȴ�����һ��Ŀ¼
		std::string parentDir = Utility::getFilePath(path);
		if (parentDir != path)
		{
			createFolder(parentDir);
		}
		if (0 != _mkdir(path.c_str()))
		{
			return false;
		}
	}
	return true;
}

bool Utility::isFileExist(const std::string& fullPath)
{
	int ret = _access(fullPath.c_str(), 0);
	return ret == 0;
}

std::string Utility::getFilePath(std::string dir)
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

void Utility::rightToLeft(std::string& str)
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

bool Utility::copyFile(const std::string& sourceFile, const std::string& destFile, bool overWrite)
{
	// ���Ŀ���ļ����ڵ�Ŀ¼������,���ȴ���Ŀ¼
	std::string parentDir = Utility::getFilePath(destFile);
	createFolder(parentDir);
	return CopyFileA(sourceFile.c_str(), destFile.c_str(), !overWrite) == TRUE;
}

std::string Utility::getFileName(std::string str)
{
	rightToLeft(str);
	int dotPos = str.find_last_of('/');
	if (dotPos != -1)
	{
		return str.substr(dotPos + 1, str.length() - 1);
	}
	return str;
}

void Utility::deleteFile(const std::string& fileName)
{
	DeleteFileA(fileName.c_str());
}