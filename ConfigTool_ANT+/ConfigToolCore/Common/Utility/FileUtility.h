#ifndef _FILE_UTILITY_H_
#define _FILE_UTILITY_H_

#include "CommonDefine.h"

class FileUtility
{
public:
	static void findFiles(const std::string& path, txVector<std::string>& files, const std::string& patterns, const bool& recursive = true);
	static void findFiles(std::string path, txVector<std::string>& files, txVector<std::string>& patterns = txVector<std::string>(), const bool& recursive = true);
	static void findFolders(std::string path, txVector<std::string>& folders, const bool& recursive = false);
	static void deleteFolder(const std::string& path);
	static bool isFileExist(const std::string& fullPath);
	// ��sourceFile������destFile,sourceFile��destFile���Ǵ���ֱ�ӷ��ʵ�·�����ļ���,overWriteָ����Ŀ���ļ��Ѿ�����ʱ�Ƿ�Ҫ�����ļ�
	static bool copyFile(const std::string& sourceFile, const std::string& destFile, bool overWrite = true);
	// ����һ���ļ���,path��һ������/��β�Ŀ�ֱ�ӷ��ʵ���Ի��߾��Ե��ļ�����
	static bool createFolder(const std::string& path);
	static bool writeFile(std::string filePath, int length, const char* buffer, bool append = false);
	static char* openFile(const std::string& filePath, int* bufferSize, bool addZero);
	static std::string openTxtFile(const std::string& filePath);
	static char* openBinaryFile(const std::string& filePath, int* bufferSize);
};

#endif