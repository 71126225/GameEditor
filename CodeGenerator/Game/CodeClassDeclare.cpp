#include "CodeClassDeclare.h"

void CodeClassDeclare::generate()
{
	if (cppFramePath.length() == 0)
	{
		return;
	}

	generateCppFrameClassAndHeader(cppFrameProjectPath, cppFramePath + "Common/");
	generateCppGameClassAndHeader(cppGameProjectPath, cppGamePath + "Common/");
}

void CodeClassDeclare::generateCppFrameClassAndHeader(const string& path, const string& targetFilePath)
{
	myVector<string> frameClassList;
	myVector<string> frameHeaderList;
	myVector<string> fileList;
	findFiles(path, fileList, ".h");
	FOR_VECTOR(fileList)
	{
		const string& fileName = fileList[i];
		// ���Ե���������ļ�
		if (findSubstr(fileName, "Dependency/") != NULL)
		{
			continue;
		}
		string fileNameNoSuffix = getFileNameNoSuffix(fileName, true);
		if (fileNameNoSuffix != "FrameHeader")
		{
			frameHeaderList.push_back(getFileName(fileName));
		}
		myVector<string> fileLines;
		split(openTxtFile(fileName).c_str(), "\r\n", fileLines);
		uint lineCount = fileLines.size();
		FOR_J(lineCount)
		{
			string className = findClassName(fileLines[j], j > 0 ? fileLines[j - 1] : EMPTY_STRING);
			if (!className.empty())
			{
				frameClassList.push_back(className);
			}
		}
	}
	END(fileList);

	// FrameClassDeclare.cpp
	string str1;
	line(str1, "#pragma once");
	line(str1, "");
	uint count0 = frameClassList.size();
	FOR_I(count0)
	{
		line(str1, "class " + frameClassList[i] + ";", i != count0 - 1);
	}
	writeFile(targetFilePath + "FrameClassDeclare.h", ANSIToUTF8(str1.c_str(), true));

	// FrameHeader.h
	string str0;
	line(str0, "#pragma once");
	line(str0, "");
	uint count1 = frameHeaderList.size();
	FOR_I(count1)
	{
		line(str0, "#include \"" + frameHeaderList[i] + "\"", i != count1 - 1);
	}
	writeFile(targetFilePath + "FrameHeader.h", ANSIToUTF8(str0.c_str(), true));
}

void CodeClassDeclare::generateCppGameClassAndHeader(const string& path, const string& targetFilePath)
{
	myVector<string> gameClassList;
	myVector<string> gameHeaderList;
	myVector<string> fileList;
	findFiles(path, fileList, ".h");
	FOR_VECTOR(fileList)
	{
		const string& fileName = fileList[i];
		string fileNameNoSuffix = getFileNameNoSuffix(fileName, true);

		// StringDefine�ĸ���ͷ�ļ��ǽ���StringDefine.h�а�����,���Բ����������ط��ٴΰ���,����linux�±���ᱨ��������,�൱�����е�Դ�ļ��������˴�����δʹ�ñ���
		if (fileNameNoSuffix != "GameHeader" &&
			(!startWith(fileNameNoSuffix, "StringDefine") || fileNameNoSuffix == "StringDefine") &&
			fileNameNoSuffix != "MySQLInstanceDeclare" &&
			fileNameNoSuffix != "MySQLInstanceClear" &&
			fileNameNoSuffix != "SQLiteInstanceDeclare" &&
			fileNameNoSuffix != "SQLiteInstanceClear" &&
			fileNameNoSuffix != "FrameSystemDeclare" &&
			fileNameNoSuffix != "SQLiteSTLPoolRegister" &&
			fileNameNoSuffix != "FrameSystemGet" &&
			fileNameNoSuffix != "FrameSystemClear" &&
			fileNameNoSuffix != "FrameSystemRegiste")
		{
			gameHeaderList.push_back(getFileName(fileName));
		}
		myVector<string> fileLines;
		split(openTxtFile(fileName).c_str(), "\r\n", fileLines);
		uint lineCount = fileLines.size();
		FOR_J(lineCount)
		{
			string className = findClassName(fileLines[j], j > 0 ? fileLines[j - 1] : EMPTY_STRING);
			if (!className.empty())
			{
				gameClassList.push_back(className);
			}
		}
	}
	END(fileList);

	// GameClassDeclare.cpp
	string str1;
	line(str1, "#pragma once");
	line(str1, "");
	uint count0 = gameClassList.size();
	FOR_I(count0)
	{
		line(str1, "class " + gameClassList[i] + ";", i != count0 - 1);
	}
	writeFile(targetFilePath + "GameClassDeclare.h", ANSIToUTF8(str1.c_str(), true));

	// GameHeader.h
	string str0;
	line(str0, "#pragma once");
	line(str0, "");
	line(str0, "#include \"FrameHeader.h\"");
	uint count1 = gameHeaderList.size();
	FOR_I(count1)
	{
		line(str0, "#include \"" + gameHeaderList[i] + "\"", i != count1 - 1);
	}
	writeFile(targetFilePath + "GameHeader.h", ANSIToUTF8(str0.c_str(), true));
}

string CodeClassDeclare::findClassName(const string& line, const string& lastLine)
{
	if (startWith(line, "class ") &&
		!findSubstr(line, ";") &&
		!findSubstr(lastLine, "template"))
	{
		// ��ȡ��:ǰ����ַ���
		int colonPos = (int)line.find(':');
		// ��:,���:�Ĳ�������
		if (colonPos != -1)
		{
			myVector<string> elements;
			split(line.substr(0, colonPos).c_str(), " ", elements);
			if (elements.size() == 0)
			{
				return "";
			}
			return elements[elements.size() - 1];
		}
		// û��:,�����һ��Ԫ��������
		else
		{
			myVector<string> elements;
			split(line.c_str(), " ", elements);
			if (elements.size() == 0)
			{
				return "";
			}
			return elements[elements.size() - 1];
		}
	}
	return "";
}