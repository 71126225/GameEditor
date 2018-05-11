#ifndef _TX_UTILITY_H_
#define _TX_UTILITY_H_

#include "DataDefine.h"

class txUtility
{
public:
	// ����media��·��,����/
	static std::string getMediaPath();
	static void rightToLeft(std::string& str);
	static std::string getAvailableResourcePath(std::string name);
	static void split(std::string str, const char* deli, std::vector<std::string>* vec);
	static void findFiles(const char * pathName, std::vector<std::string>& files, const std::vector<std::string>& patterns);
	static bool isFileExist(std::string fullPath);
	static bool createFolder(std::string path);
	static bool writeFile(std::string filePath, int length, const char* buffer);
	static bool loadFile(const std::string& fullFileName, char*& buffer, int& bufferSize, bool addZero);
	static std::string intToString(int i, int limitLen = 0); // limitLen���ַ�������С����,���������λ��������С����,�����ǰ���0
	static int stringToInt(const std::string& str);
	static float pow_f(float f, int p);
	static std::string floatToString(float f, int precision);	//precision -> ����,������С����λ��
	static float stringToFloat(std::string str);
	static std::string boolToString(bool value);
	static bool stringToBool(const std::string& str);
	static std::string getFileNameNoSuffix(std::string str);
	static std::string getFileName(std::string str);
	static bool endWith(const std::string& oriString, const std::string& pattern);// �ж�oriString�Ƿ���pattern��β
	static bool startWith(const std::string& oriString, const std::string& pattern);// �ж�oriString�Ƿ���pattern��ͷ
	static std::string strReplace(const std::string& str, int begin, int end, const std::string& reStr);
	static float calculateFloat(std::string str);	// �Ը������ļ��㷨�����һ�����ʽ,ֻ֧�ּӼ��˳�������
	static int calculateInt(std::string str);		// �������ļ��㷨�����һ�����ʽ,֧��ȡ��,�Ӽ��˳�������
	static int strFind(const std::string& res, const std::string& sub, int startPos, bool fromStart);
	static bool findSubstr(std::string res, std::string dst, bool sensitive, int* pos = NULL, int startPose = 0, bool firstOrLast = true);
	static void strToLower(std::string& str);
	static void strToUpper(std::string& str);
};

#endif