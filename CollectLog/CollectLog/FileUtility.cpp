#include "FileUtility.h"
#include "StringUtility.h"

std::string FileUtility::validPath(const std::string& path)
{
	std::string newPath = path;
	if (newPath.length() > 0)
	{
		// ����/��β,�����/
		if (newPath[newPath.length() - 1] != '/')
		{
			newPath += "/";
		}
	}	
	return newPath;
}

void FileUtility::findFiles(std::string path, std::vector<std::string>& files, const std::vector<std::string>& patterns, const bool& recursive)
{
	path = validPath(path);
	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA((path + "*").c_str(), &findFileData);
	// ����Ҳ����ļ��о�ֱ�ӷ���
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}
	bool hasFile = false;
	while (::FindNextFileA(hFind, &findFileData))
	{
		// ����.��..
		if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0)
		{
			continue;
		}
		// ��������·��
		std::string fullname = path + std::string(findFileData.cFileName);
		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
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

void FileUtility::findFolers(std::string path, std::vector<std::string>& folders, const bool& recursive)
{
	path = validPath(path);
	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA((path + "*").c_str(), &findFileData);
	// ����Ҳ����ļ��о�ֱ�ӷ���
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}
	bool hasFile = false;
	while (::FindNextFileA(hFind, &findFileData))
	{
		// ����.��..
		if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0)
		{
			continue;
		}
		// ��������·��
		std::string fullname = path + std::string(findFileData.cFileName);
		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (recursive)
			{
				findFolers(fullname.c_str(), folders, recursive);
			}
		}
	}
	::FindClose(hFind);
}

void FileUtility::deleteFolder(std::string path)
{
	path = validPath(path);
	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA((path + "*").c_str(), &findFileData);
	// ����Ҳ����ļ��о�ֱ�ӷ���
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}
	bool hasFile = false;
	while (::FindNextFileA(hFind, &findFileData))
	{
		// ����.��..
		if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0)
		{
			continue;
		}
		// ��������·��
		std::string fullname = path + std::string(findFileData.cFileName);
		// ������ļ���,��ݹ�ɾ���ļ���
		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			deleteFolder(fullname);
			// Ȼ��ɾ���ļ���
			RemoveDirectoryA(fullname.c_str());
		}
		// ������ļ�,��ֱ��ɾ���ļ�
		else
		{
			deleteFile(fullname);
		}
	}
	::FindClose(hFind);
	// ɾ���ļ�������
	BOOL ret = RemoveDirectoryA(path.c_str());
}

bool FileUtility::deleteFile(const std::string& fileName)
{
	return ::DeleteFileA(fileName.c_str()) == TRUE;
}

bool FileUtility::deleteEmptyFolder(std::string path)
{
	path = validPath(path);
	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA((path + "*").c_str(), &findFileData);
	// ����Ҳ����ļ��о�ֱ�ӷ���
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return true;
	}
	bool isEmpty = true;
	while (::FindNextFileA(hFind, &findFileData))
	{
		// ����.��..
		if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0)
		{
			continue;
		}
		// ��������·��
		std::string fullname = path + std::string(findFileData.cFileName);
		// ������ļ���,��ݹ�ɾ�����ļ���
		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// ����ļ���δɾ���ɹ�,���ʾ���ǿ��ļ���
			if (!deleteEmptyFolder(fullname))
			{
				isEmpty = false;
			}
		}
		// ���ļ�,����Ϊ���ļ�,Ȼ���˳�ѭ��
		else
		{
			isEmpty = false;
		}
	}
	::FindClose(hFind);
	if (isEmpty)
	{
		RemoveDirectoryA(path.c_str());
	}
	return isEmpty;
}

bool FileUtility::isFileExist(std::string fullPath)
{
	int ret = _access(fullPath.c_str(), 0);
	return ret == 0;
}

bool FileUtility::renameFile(const std::string& fileName, const std::string& newName)
{
	return rename(fileName.c_str(), newName.c_str()) == 0;
}

bool FileUtility::copyFile(std::string sourceFile, std::string destFile, bool overWrite)
{
	// ���Ŀ���ļ����ڵ�Ŀ¼������,���ȴ���Ŀ¼
	std::string parentDir = StringUtility::getParentDir(destFile);
	createFolder(parentDir);
#if RUN_PLATFORM == PLATFORM_WINDOWS
	return CopyFileA(sourceFile.c_str(), destFile.c_str(), !overWrite) == TRUE;
#elif RUN_PLATFORM == PLATFORM_ANDROID
	return false;
#endif
}

bool FileUtility::createFolder(std::string path)
{
	// �������һ��Ŀ¼,������һ��Ŀ¼������,���ȴ�����һ��Ŀ¼
	std::string parentDir = StringUtility::getParentDir(path);
	if (parentDir != path)
	{
		createFolder(parentDir);
	}
	// ����ļ��������򴴽��ļ�
	if (!isFileExist(path))
	{
		if (0 != _mkdir(path.c_str()))
		{
			return false;
		}
	}
	return true;
}

bool FileUtility::openFile(std::string filePath, int& length, char*& buffer, const bool& addZero)
{
	FILE* pFile = NULL;
	fopen_s(&pFile, filePath.c_str(), "rb");
	if (pFile == NULL)
	{
		return false;
	}
	fseek(pFile, 0, SEEK_END);
	length = ftell(pFile);
	rewind(pFile);
	int bufferSize = length;
	if (addZero)
	{
		bufferSize += 1;
	}
	buffer = new char[bufferSize];
	memset(buffer, 0, bufferSize);
	fread(buffer, sizeof(char), length, pFile);
	fclose(pFile);
	return true;
}

bool FileUtility::writeFile(std::string filePath, int length, char* buffer, const bool& appendData)
{
	if (length <= 0 || NULL == buffer)
	{
		return false;
	}

	// ����·��
	StringUtility::rightToLeft(filePath);
	int pos = filePath.find_last_of('/');
	if (-1 != pos)
	{
		std::string dirPath = filePath.substr(0, pos);
		if (!createFolder(dirPath))
		{
			return false;
		}
	}
	// д���ļ�
	FILE* pFile = NULL;
	if (appendData)
	{
		fopen_s(&pFile, filePath.c_str(), "ab+");
	}
	else
	{
		fopen_s(&pFile, filePath.c_str(), "wb");
	}
	if (pFile == NULL)
	{
		return false;
	}
	fwrite(buffer, sizeof(char), length, pFile);
	fclose(pFile);
	return true;
}

int FileUtility::getFileSize(const std::string& filePath)
{
	struct _stat info;
	_stat(filePath.c_str(), &info);
	long size = info.st_size;
	return size;
}