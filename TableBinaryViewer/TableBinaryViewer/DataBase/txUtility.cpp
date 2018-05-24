#include "txUtility.h"

// ����media��·��,����/
std::string txUtility::getMediaPath()
{
	return "../media";
}

void txUtility::rightToLeft(std::string& str)
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

// ͨ��һ��media�µ����·��,���߾���·��,ת��Ϊһ����ֱ�Ӵ򿪵�·��
std::string txUtility::getAvailableResourcePath(std::string name)
{
	std::string mediaPath = getMediaPath();
	// ����ļ����Ѿ��ǲ���media·�����Ҳ��Ǿ���·��
	if (mediaPath != "" && (name.length() <= mediaPath.length() || name.substr(0, mediaPath.length()) != mediaPath) && (name.length() > 1 && name[1] != ':'))
	{
		name = mediaPath + "/" + name;
	}
	return name;
}

void txUtility::split(std::string str, const char* deli, std::vector<std::string>* vec)
{
	while (true)
	{
		int devidePos = str.find_first_of(deli);
		if (devidePos == -1)
		{
			vec->push_back(str);
			break;
		}
		else
		{
			std::string curString = str.substr(0, devidePos);
			vec->push_back(curString);
			str = str.substr(devidePos + strlen(deli), str.length() - devidePos - strlen(deli));
		}
	}
}

void txUtility::findFiles(const char * pathName, std::vector<std::string>& files, const std::vector<std::string>& patterns)
{
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	CHAR PathBuffer[_MAX_PATH];

	strcpy_s(PathBuffer, _MAX_PATH, pathName);
	if (pathName[strlen(pathName) - 1] != '/')
	{
		strcat_s(PathBuffer, _MAX_PATH, "/*");
	}
	else
	{
		strcat_s(PathBuffer, _MAX_PATH, "*");
	}

	hFind = FindFirstFileA(PathBuffer, &FindFileData);
	// ����Ҳ����ļ��о�ֱ�ӷ���
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}
	while (::FindNextFileA(hFind, &FindFileData))
	{
		// ����.��..

		if (std::string(FindFileData.cFileName) == "." || std::string(FindFileData.cFileName) == "..")
		{
			continue;
		}

		// ��������·��
		std::string fullname = std::string(pathName) + "/" + std::string(FindFileData.cFileName);
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			findFiles(fullname.c_str(), files, patterns);
		}
		else
		{
			int patternCount = patterns.size();
			for (int i = 0; i < patternCount; ++i)
			{
				if (fullname.length() >= patterns[i].length() && fullname.substr(fullname.length() - patterns[i].length(), patterns[i].length()) == patterns[i])
				{
					files.push_back(fullname);
				}
			}
		}
	}
	::FindClose(hFind);
}

bool txUtility::isFileExist(std::string fullPath)
{
	int ret = _access(fullPath.c_str(), 0);
	return ret == 0;
}

bool txUtility::createFolder(std::string path)
{
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

bool txUtility::writeFile(std::string filePath, int length, const char* buffer)
{
	rightToLeft(filePath);

	int pos = filePath.find_last_of('/');
	if (-1 != pos)
	{
		std::string dirPath = filePath.substr(0, pos);
		if (!createFolder(dirPath))
		{
			return false;
		}
	}
	else
	{
		if (!createFolder(filePath))
		{
			return false;
		}
	}

	FILE* pFile = NULL;
	fopen_s(&pFile, filePath.c_str(), "wb");
	if (pFile == NULL)
	{
		return false;
	}
	if (length > 0 && NULL != buffer)
	{
		fwrite(buffer, sizeof(char), length, pFile);
	}
	fclose(pFile);

	return true;
}

bool txUtility::loadFile(const std::string& fullFileName, char*& buffer, int& bufferSize, bool addZero)
{
	FILE* pFile = NULL;
	fopen_s(&pFile, fullFileName.c_str(), "rb");
	if (pFile == NULL)
	{
		return false;
	}
	fseek(pFile, 0, SEEK_END);
	int fileSize = ftell(pFile);
	rewind(pFile);
	bufferSize = addZero ? fileSize + 1 : fileSize;
	buffer = TRACE_NEW_ARRAY(char, bufferSize, buffer);
	if (bufferSize != NULL && fileSize > 0)
	{
		fread(buffer, sizeof(char), fileSize, pFile);
	}
	fclose(pFile);
	return true;
}

std::string txUtility::intToString(int i, int limitLen)
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


int txUtility::stringToInt(const std::string& str)
{
	if (str == "")
	{
		return 0;
	}
	return atoi(str.c_str());
}

float txUtility::pow_f(float f, int p)
{
	float ret = 1.0f;
	while (p)
	{
		ret *= f;
		--p;
	}
	return ret;
}

std::string txUtility::floatToString(float f, int precision)
{
	f = f * pow_f(10.0f, precision) + 0.5f;
	int MAX_INT = 0x7FFFFFFF;
	if (f > (float)MAX_INT)
	{
		char str[256];
		sprintf_s(str, 256, "%f", f);
		return std::string(str);
	}

	int intValue = (int)f;

	char str[256];
	sprintf_s(str, 256, "%d", intValue);
	if (precision == 0)
	{
		return std::string(str);
	}
	else
	{
		int length = strlen(str);
		if (length <= precision)
		{
			char newStr[256];
			memset(newStr, 0, 256 * sizeof(char));
			for (int i = 0; i < precision - length + 1; ++i)
			{
				newStr[i] = '0';
			}
			memcpy(newStr + precision - length + 1, str, length);

			char intStr[256];
			memset(intStr, 0, 256 * sizeof(char));
			char decimalStr[256];
			memset(decimalStr, 0, 256 * sizeof(char));
			memcpy(intStr, newStr, strlen(newStr) - precision);
			memcpy(decimalStr, newStr + strlen(newStr) - precision, precision);
			return std::string(intStr) + "." + std::string(decimalStr);
		}
		else
		{
			char intStr[256];
			memset(intStr, 0, 256 * sizeof(char));
			char decimalStr[256];
			memset(decimalStr, 0, 256 * sizeof(char));
			memcpy(intStr, str, strlen(str) - precision);
			memcpy(decimalStr, str + strlen(str) - precision, precision);
			return std::string(intStr) + "." + std::string(decimalStr);
		}
	}
}

float txUtility::stringToFloat(std::string str)
{
	return (float)atof(str.c_str());
}

std::string txUtility::boolToString(bool value)
{
	return value ? "True" : "False";
}

bool txUtility::stringToBool(const std::string& str)
{
	return str == "true" || str == "True";
}

std::string txUtility::getFileNameNoSuffix(std::string str)
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

std::string txUtility::getFileName(std::string str)
{
	rightToLeft(str);
	int dotPos = str.find_last_of('/');
	if (dotPos != -1)
	{
		return str.substr(dotPos + 1, str.length() - 1);
	}
	return str;
}

bool txUtility::endWith(const std::string& oriString, const std::string& pattern)
{
	if (oriString.length() < pattern.length())
	{
		return false;
	}
	std::string endString = oriString.substr(oriString.length() - pattern.length(), pattern.length());
	return endString == pattern;
}

bool txUtility::startWith(const std::string& oriString, const std::string& pattern)
{
	if (oriString.length() < pattern.length())
	{
		return false;
	}
	std::string startString = oriString.substr(0, pattern.length());
	return startString == pattern;
}

std::string txUtility::strReplace(const std::string& str, int begin, int end, const std::string& reStr)
{
	std::string sub1 = str.substr(0, begin);
	std::string sub2 = str.substr(end, str.length() - end);
	return sub1 + reStr + sub2;
}

float txUtility::calculateFloat(std::string str)
{
	// �ж��ַ����Ƿ��зǷ��ַ�,Ҳ���ǳ�����,С����,�����������ַ�
	for (int i = 0; i < (int)str.length();)
	{
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.' && str[i] != '+'
			&& str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '(' && str[i] != ')')
		{
			str.erase(i, 1);
		}
		else
		{
			++i;
		}
	}
	// �ж��������������Ƿ����
	int leftBracketCount = 0;
	int rightBracketCount = 0;
	int strLen = (int)str.length();
	for (int i = 0; i < strLen; ++i)
	{
		if (str[i] == '(')
		{
			++leftBracketCount;
		}
		else if (str[i] == ')')
		{
			++rightBracketCount;
		}
	}
	if (leftBracketCount != rightBracketCount)
	{
		// �������,����������������Ӧ
		return 0;
	}

	// ѭ���жϴ�����ַ�����û������
	while (true)
	{
		// ���ж���û�����ţ���������ž������������,���û�о��˳�whileѭ��
		if (str.find_first_of("(") != -1 || str.find_first_of(")") != -1)
		{
			int curpos = str.find_last_of("(");
			std::string strInBracket = str.substr(curpos + 1, str.length() - curpos - 1);
			strInBracket = strInBracket.substr(0, strInBracket.find_first_of(")"));
			float ret = calculateFloat(strInBracket);
			// ��������еļ������Ǹ���,����Ϊ����
			bool isMinus = false;
			if (ret < 0)
			{
				ret = -ret;
				isMinus = true;
			}
			// �������еļ������滻ԭ���ı��ʽ,��������Ҳһ���滻
			std::string floatStr = floatToString(ret, 4);
			str = strReplace(str, curpos, curpos + strInBracket.length() + 2, floatStr);
			if (isMinus)
			{
				// ��������м�������Ǹ���,�򽫸�����ȡ����,����ߵĵ�һ���Ӽ��Ÿ�Ϊ�෴�ķ���
				bool changeMark = false;
				for (int i = curpos - 1; i >= 0; --i)
				{
					// �ҵ���һ��+��,��ֱ�Ӹ�Ϊ����,Ȼ���˳�����
					if (str[i] == '+')
					{
						str[i] = '-';
						changeMark = true;
						break;
					}
					// �ҵ���һ������,������ŵ����������,��ֱ�Ӹ�Ϊ+��
					// ������ŵ���߲�������,��ü����Ǹ���,������ȥ��,
					else if (str[i] == '-')
					{
						if (str[i - 1] >= '0' && str[i - 1] <= '9')
						{
							str[i] = '+';
						}
						else
						{
							str = strReplace(str, i, i + 1, "");
						}
						changeMark = true;
						break;
					}
				}
				// ����������˻�û���ҵ������滻�ķ���,���ڱ��ʽ��ǰ���һ������
				if (!changeMark)
				{
					str = "-" + str;
				}
			}
		}
		else
		{
			break;
		}
	}

	std::vector<float> numbers;
	std::vector<char> factors;
	// ��ʾ��һ����������±�+1
	int beginpos = 0;
	strLen = (int)str.length();
	for (int i = 0; i < strLen; ++i)
	{
		// �����������һ���ַ�,��ֱ�Ӱ����һ�����ַ����б�,Ȼ���˳�ѭ��
		if (i == strLen - 1)
		{
			std::string num = str.substr(beginpos, strLen - beginpos);
			float fNum = stringToFloat(num);
			numbers.push_back(fNum);
			break;
		}
		// �ҵ���һ�������
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.')
		{
			if (i != 0)
			{
				std::string num = str.substr(beginpos, i - beginpos);
				float fNum = stringToFloat(num);
				numbers.push_back(fNum);
			}
			// ����ڱ��ʽ�Ŀ�ʼ�ͷ����������,���ʾ��һ�����Ǹ���,�Ǿʹ���Ϊ0��ȥ������ľ���ֵ
			else
			{
				numbers.push_back(0);
			}
			factors.push_back(str[i]);
			beginpos = i + 1;
		}
	}
	if (factors.size() + 1 != numbers.size())
	{
		// �������,�������������������
		return 0;
	}
	// ���ڿ�ʼ������ʽ,�����������ȼ�,�ȼ���˳���ȡ��
	while (true)
	{
		// ��ʾ�Ƿ��г˳����ʽ
		bool hasMS = false;
		for (int i = 0; i < (int)factors.size(); ++i)
		{
			// �ȱ������ĸ����ȼ����ĸ�
			if (factors[i] == '*' || factors[i] == '/')
			{
				// ��һ�����������±�����������±�����ͬ��
				float num1 = numbers[i];
				float num2 = numbers[i + 1];
				float num3 = 0.0f;
				if (factors[i] == '*')
				{
					num3 = num1 * num2;
				}
				else if (factors[i] == '/')
				{
					num3 = num1 / num2;
				}
				// ɾ����i + 1����,Ȼ�󽫵�i�����滻Ϊ������
				numbers.erase(numbers.begin() + i + 1);
				if (numbers.size() == 0)
				{
					// �������
					return 0;
				}
				numbers[i] = num3;
				// ɾ����i�������
				factors.erase(factors.begin() + i);
				hasMS = true;
				break;
			}
		}
		if (!hasMS)
		{
			break;
		}
	}
	// �ټ���Ӽ���
	while (true)
	{
		if (factors.size() == 0)
		{
			break;
		}
		if (factors[0] == '+' || factors[0] == '-')
		{
			// ��һ�����������±�����������±�����ͬ��
			float num1 = numbers[0];
			float num2 = numbers[1];
			float num3 = 0.0f;
			if (factors[0] == '+')
			{
				num3 = num1 + num2;
			}
			else if (factors[0] == '-')
			{
				num3 = num1 - num2;
			}
			// ɾ����1����,Ȼ�󽫵�0�����滻Ϊ������
			numbers.erase(numbers.begin() + 1);
			if (numbers.size() == 0)
			{
				// �������
				return 0;
			}
			numbers[0] = num3;
			// ɾ����0�������
			factors.erase(factors.begin());
		}
	}
	if (numbers.size() != 1)
	{
		// �������
		return 0;
	}
	else
	{
		return numbers[0];
	}
}

int txUtility::calculateInt(std::string str)
{
	// �ж��ַ����Ƿ��зǷ��ַ�,Ҳ���ǳ�����,С����,�����������ַ�
	for (int i = 0; i < (int)str.length();)
	{
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.' && str[i] != '+'
			&& str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '%'
			&& str[i] != '(' && str[i] != ')')
		{
			str.erase(i, 1);
		}
		else
		{
			++i;
		}
	}
	// �ж��������������Ƿ����
	int leftBracketCount = 0;
	int rightBracketCount = 0;
	int strLen = (int)str.length();
	for (int i = 0; i < strLen; ++i)
	{
		if (str[i] == '(')
		{
			++leftBracketCount;
		}
		else if (str[i] == ')')
		{
			++rightBracketCount;
		}
	}
	if (leftBracketCount != rightBracketCount)
	{
		// �������,����������������Ӧ
		return 0;
	}

	// ѭ���жϴ�����ַ�����û������
	while (true)
	{
		// ���ж���û�����ţ���������ž������������,���û�о��˳�whileѭ��
		if (str.find_first_of("(") != -1 || str.find_first_of(")") != -1)
		{
			int curpos = str.find_last_of("(");
			std::string strInBracket = str.substr(curpos + 1, str.length() - curpos - 1);
			strInBracket = strInBracket.substr(0, strInBracket.find_first_of(")"));
			int ret = calculateInt(strInBracket);
			// ��������еļ������Ǹ���,����Ϊ����
			bool isMinus = false;
			if (ret < 0)
			{
				ret = -ret;
				isMinus = true;
			}
			// �������еļ������滻ԭ���ı��ʽ,��������Ҳһ���滻
			std::string intStr = intToString(ret, 4);
			str = strReplace(str, curpos, curpos + strInBracket.length() + 2, intStr);
			if (isMinus)
			{
				// ��������м�������Ǹ���,�򽫸�����ȡ����,����ߵĵ�һ���Ӽ��Ÿ�Ϊ�෴�ķ���
				bool changeMark = false;
				for (int i = curpos - 1; i >= 0; --i)
				{
					// �ҵ���һ��+��,��ֱ�Ӹ�Ϊ����,Ȼ���˳�����
					if (str[i] == '+')
					{
						str[i] = '-';
						changeMark = true;
						break;
					}
					// �ҵ���һ������,������ŵ����������,��ֱ�Ӹ�Ϊ+��
					// ������ŵ���߲�������,��ü����Ǹ���,������ȥ��,
					else if (str[i] == '-')
					{
						if (str[i - 1] >= '0' && str[i - 1] <= '9')
						{
							str[i] = '+';
						}
						else
						{
							str = strReplace(str, i, i + 1, "");
						}
						changeMark = true;
						break;
					}
				}
				// ����������˻�û���ҵ������滻�ķ���,���ڱ��ʽ��ǰ���һ������
				if (!changeMark)
				{
					str = "-" + str;
				}
			}
		}
		else
		{
			break;
		}
	}

	std::vector<int> numbers;
	std::vector<char> factors;
	// ��ʾ��һ����������±�+1
	int beginpos = 0;
	strLen = (int)str.length();
	for (int i = 0; i < strLen; ++i)
	{
		// �����������һ���ַ�,��ֱ�Ӱ����һ�����ַ����б�,Ȼ���˳�ѭ��
		if (i == strLen - 1)
		{
			std::string num = str.substr(beginpos, strLen - beginpos);
			int iNum = stringToInt(num);
			numbers.push_back(iNum);
			break;
		}
		// �ҵ���һ�������
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.')
		{
			if (i != 0)
			{
				std::string num = str.substr(beginpos, i - beginpos);
				int iNum = stringToInt(num);
				numbers.push_back(iNum);
			}
			// ����ڱ��ʽ�Ŀ�ʼ�ͷ����������,���ʾ��һ�����Ǹ���,�Ǿʹ���Ϊ0��ȥ������ľ���ֵ
			else
			{
				numbers.push_back(0);
			}
			factors.push_back(str[i]);
			beginpos = i + 1;
		}
	}
	if (factors.size() + 1 != numbers.size())
	{
		// �������,�������������������
		return 0;
	}
	// ���ڿ�ʼ������ʽ,�����������ȼ�,�ȼ���˳���ȡ��
	while (true)
	{
		// ��ʾ�Ƿ��г˳����ʽ
		bool hasMS = false;
		for (int i = 0; i < (int)factors.size(); ++i)
		{
			// �ȱ������ĸ����ȼ����ĸ�
			if (factors[i] == '*' || factors[i] == '/' || factors[i] == '%')
			{
				// ��һ�����������±�����������±�����ͬ��
				int num1 = numbers[i];
				int num2 = numbers[i + 1];
				int num3 = 0;
				if (factors[i] == '*')
				{
					num3 = num1 * num2;
				}
				else if (factors[i] == '/')
				{
					num3 = num1 / num2;
				}
				else if (factors[i] == '%')
				{
					num3 = num1 % num2;
				}
				// ɾ����i + 1����,Ȼ�󽫵�i�����滻Ϊ������
				numbers.erase(numbers.begin() + i + 1);
				if (numbers.size() == 0)
				{
					// �������
					return 0;
				}
				numbers[i] = num3;
				// ɾ����i�������
				factors.erase(factors.begin() + i);
				hasMS = true;
				break;
			}
		}
		if (!hasMS)
		{
			break;
		}
	}
	// �ټ���Ӽ���
	while (true)
	{
		if (factors.size() == 0)
		{
			break;
		}
		if (factors[0] == '+' || factors[0] == '-')
		{
			// ��һ�����������±�����������±�����ͬ��
			int num1 = numbers[0];
			int num2 = numbers[1];
			int num3 = 0;
			if (factors[0] == '+')
			{
				num3 = num1 + num2;
			}
			else if (factors[0] == '-')
			{
				num3 = num1 - num2;
			}
			// ɾ����1����,Ȼ�󽫵�0�����滻Ϊ������
			numbers.erase(numbers.begin() + 1);
			if (numbers.size() == 0)
			{
				// �������
				return 0;
			}
			numbers[0] = num3;
			// ɾ����0�������
			factors.erase(factors.begin());
		}
	}
	if (numbers.size() != 1)
	{
		// �������
		return 0;
	}
	else
	{
		return numbers[0];
	}
}

bool txUtility::findSubstr(std::string res, std::string dst, bool sensitive, int* pos, int startPose, bool firstOrLast)
{
	// ��������ִ�Сд
	if (!sensitive)
	{
		// ȫת��ΪСд
		strToLower(res);
		strToLower(dst);
	}
	int posFind = -1;
	int subLen = (int)dst.length();
	int searchLength = res.length() - subLen;
	if (searchLength < 0)
	{
		return false;
	}
	int start = firstOrLast ? startPose : searchLength;
	int delta = firstOrLast ? 1 : -1;
	int end = firstOrLast ? searchLength : startPose;
	end += delta;
	for (int i = start; i != end; i += delta)
	{
		int j = 0;
		for (j = 0; j != subLen; ++j)
		{
			if (res[i + j] != dst[j])
			{
				break;
			}
		}
		if (j == subLen)
		{
			posFind = i;
		}
	}
	if (pos != NULL)
	{
		*pos = posFind;
	}
	return posFind != -1;
}

void txUtility::strToLower(std::string& str)
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

void txUtility::strToUpper(std::string& str)
{
	std::string::size_type size = str.length();
	for (std::string::size_type i = 0; i != size; ++i)
	{
		if (str[i] >= 'a' && str[i] <= 'z')
		{
			str[i] -= 'a' - 'A';
		}
	}
}