#include "Utility.h"
#include "txMemoryTrace.h"
#include "ToolCoreLog.h"
#include "ToolCoreUtility.h"

void FileUtility::findFiles(const std::string& pathName, txVector<std::string>& files, const std::string& patterns, const bool& recursive)
{
	txVector<std::string> patternList;
	patternList.push_back(patterns);
	findFiles(pathName, files, patternList, recursive);
}

void FileUtility::findFiles(std::string path, txVector<std::string>& files, txVector<std::string>& patterns, const bool& recursive)
{
	if (path.length() == 0)
	{
		return;
	}
	// ����/��β,�����/
	if (path[path.length() - 1] != '/')
	{
		path += "/";
	}
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA((path + "*").c_str(), &FindFileData);
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
					if (StringUtility::endWith(fullname, patterns[i]))
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

void FileUtility::findFolders(std::string path, txVector<std::string>& folders, const bool& recursive)
{
	if (path.length() == 0)
	{
		return;
	}
	// ����/��β,�����/
	if (path[path.length() - 1] != '/')
	{
		path += "/";
	}
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA((path + "*").c_str(), &FindFileData);
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
		std::string fullname = path + std::string(FindFileData.cFileName);
		// ���ļ������ȷ����б�,Ȼ���ж��Ƿ�ݹ����
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			folders.push_back(fullname);
			if (recursive)
			{
				findFolders(fullname.c_str(), folders, recursive);
			}
		}
	} while (::FindNextFileA(hFind, &FindFileData));
	::FindClose(hFind);
}

void FileUtility::deleteFolder(const std::string& path)
{
	WIN32_FIND_DATAA FindData;
	// ����·��
	std::string pathName = path + "/*.*";
	HANDLE hFind = FindFirstFileA(pathName.c_str(), &FindData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}
	while (::FindNextFileA(hFind, &FindData))
	{
		// ����.��..
		if (strcmp(FindData.cFileName, ".") == 0
			|| strcmp(FindData.cFileName, "..") == 0)
		{
			continue;
		}

		// ��������·��
		std::string fullname = path + "/" + std::string(FindData.cFileName);
		// ������ļ���,��ݹ�ɾ���ļ���
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			deleteFolder(fullname);
		}
		// ������ļ�,��ֱ��ɾ���ļ�
		else
		{
			DeleteFileA(fullname.c_str());
		}
	}
	::FindClose(hFind);
	// ɾ���ļ�������
	RemoveDirectoryA(path.c_str());
}

bool FileUtility::isFileExist(const std::string& fullPath)
{
	if (fullPath == "." || fullPath == "..")
	{
		return true;
	}
	int ret = _access(fullPath.c_str(), 0);
	return ret == 0;
}

bool FileUtility::copyFile(const std::string& sourceFile, const std::string& destFile, bool overWrite)
{
	// ���Ŀ���ļ����ڵ�Ŀ¼������,���ȴ���Ŀ¼
	std::string parentDir = StringUtility::getParentDir(destFile);
	createFolder(parentDir);
	return CopyFileA(sourceFile.c_str(), destFile.c_str(), !overWrite) == TRUE;
}

bool FileUtility::createFolder(const std::string& path)
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

bool FileUtility::writeFile(std::string filePath, int length, const char* buffer, bool append)
{
	StringUtility::rightToLeft(filePath);
	std::string tempPath = filePath;
	int pos = filePath.find_last_of('/');
	if (pos != -1)
	{
		tempPath = filePath.substr(0, pos);
	}
	if (!createFolder(tempPath))
	{
		TOOL_CORE_ERROR("error : can not create folder, name : %s", filePath.c_str());
		return false;
	}
	char* openMode = "";
	if (append)
	{
		// ������ļ�,�����ļ�ָʾ���Ƶ��ļ�ĩβ
		openMode = "ab+";
	}
	else
	{
		// ����ļ�,�����ļ�ָʾ���Ƶ��ļ���ͷ
		openMode = "wb";
	}
	FILE* pFile = NULL;
	fopen_s(&pFile, filePath.c_str(), openMode);
	if (pFile == NULL)
	{
		TOOL_CORE_ERROR("error : can not write file, name : %s", filePath.c_str());
		return false;
	}
	if (buffer != NULL && length > 0)
	{
		fwrite(buffer, sizeof(char), length, pFile);
	}
	fclose(pFile);
	return true;
}

char* FileUtility::openFile(const std::string& filePath, int* bufferSize, bool addZero)
{
	FILE* pFile = NULL;
	fopen_s(&pFile, filePath.c_str(), "rb");
	if (pFile == NULL)
	{
		return NULL;
	}
	fseek(pFile, 0, SEEK_END);
	int fileSize = ftell(pFile);
	rewind(pFile);
	int bufferLen = addZero ? fileSize + 1 : fileSize;
	if (bufferSize != NULL)
	{
		*bufferSize = bufferLen;
	}
	char* buffer = TRACE_NEW_ARRAY(char, bufferLen, buffer);
	if (buffer != NULL)
	{
		fread(buffer, sizeof(char), fileSize, pFile);
	}
	fclose(pFile);
	return buffer;
}

std::string FileUtility::openTxtFile(const std::string& filePath)
{
	char* buffer = openFile(filePath, NULL, true);
	if (buffer == NULL)
	{
		return EMPTY_STRING;
	}
	std::string str(buffer);
	TRACE_DELETE_ARRAY(buffer);
	return str;
}

char* FileUtility::openBinaryFile(const std::string& filePath, int* bufferSize)
{
	return openFile(filePath, bufferSize, false);
}