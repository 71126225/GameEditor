#ifndef _FILE_UTILITY_H_
#define _FILE_UTILITY_H_

#include "Header.h"

class FileUtility
{
public:
	static std::string validPath(const std::string& path);
	static void findFiles(std::string pathName, std::vector<std::string>& files, const std::vector<std::string>& patterns = std::vector<std::string>(), const bool& recursive = true);
	static void findFolers(std::string path, std::vector<std::string>& folders, const bool& recursive = false);
	static void deleteFolder(std::string path);
	static bool deleteFile(const std::string& fileName);
	static bool deleteEmptyFolder(std::string path);	// ����ֵΪ�Ƿ�ɾ���ɹ�
	static bool renameFile(const std::string& fileName, const std::string& newName);
	static bool isFileExist(std::string fullPath);
	// ��sourceFile������destFile,sourceFile��destFile���Ǵ���ֱ�ӷ��ʵ�·�����ļ���,overWriteָ����Ŀ���ļ��Ѿ�����ʱ�Ƿ�Ҫ�����ļ�
	static bool copyFile(std::string sourceFile, std::string destFile, bool overWrite = true);
	// ����һ���ļ���,path��һ������/��β�Ŀ�ֱ�ӷ��ʵ���Ի��߾��Ե��ļ�����
	static bool createFolder(std::string path);
	static bool openFile(std::string filePath, int& length, char*& buffer, const bool& addZero);
	static bool writeFile(std::string filePath, int length, char* buffer, const bool& appendData = false);
	static int getFileSize(const std::string& filePath);
};

#endif