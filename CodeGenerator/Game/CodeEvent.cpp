#include "CodeEvent.h"

void CodeEvent::generateEventCode(string stringDefinePath, string headerPath)
{
	string cmdFile;
	openTxtFile("Event.txt", cmdFile);
	if (cmdFile.length() == 0)
	{
		ERROR("δ���ļ�Event.txt");
		return;
	}
	cmdFile = UTF8ToANSI(cmdFile.c_str(), true);
	myVector<string> eventLineList;
	split(cmdFile.c_str(), "\r\n", eventLineList);
	myVector<pair<string, string>> eventList;
	FOR_VECTOR(eventLineList)
	{
		myVector<string> splitResult;
		split(eventLineList[i].c_str(), "\t", splitResult);
		if (splitResult.size() != 2)
		{
			ERROR("�¼��ļ���������:" + eventLineList[i]);
		}
		eventList.push_back(make_pair(splitResult[0], splitResult[1]));
	}
	END(eventLineList);
	// ����StringDefineEvent�ļ�
	generateStringDefineEvent(eventList, stringDefinePath);
	// ����EventHeader.h�ļ�
	generateHeaderFile(eventList, headerPath);
	// ����EventType.h�ļ�
	generateEventType(eventList, headerPath);
}

// EventHeader.h�ļ�
void CodeEvent::generateHeaderFile(const myVector<pair<string, string>>& eventList, string headerPath)
{
	string str0;
	line(str0, "#ifndef _EVENT_HEADER_H_");
	line(str0, "#define _EVENT_HEADER_H_");
	line(str0, "");
	uint count = eventList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + eventList[i].first + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif", false);
	validPath(headerPath);
	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(headerPath + "EventHeader.h", str0);
}

// StringDefineEvent.h��StringDefineEvent.cpp
void CodeEvent::generateStringDefineEvent(const myVector<pair<string, string>>& eventList, string stringDefinePath)
{
	// ͷ�ļ�
	string header;
	uint cmdCount = eventList.size();
	FOR_I(cmdCount)
	{
		line(header, "DECLARE_STRING(" + eventList[i].first + ");");
	}
	validPath(stringDefinePath);
	header = ANSIToUTF8(header.c_str(), true);
	writeFile(stringDefinePath + "StringDefineEvent.h", header);

	// Դ�ļ�
	string source;
	line(source, "#include \"StringDefine.h\"");
	line(source, "#include \"EventHeader.h\"");
	line(source, "");
	FOR_I(cmdCount)
	{
		line(source, "DEFINE_STRING(" + eventList[i].first + ");");
	}
	source = ANSIToUTF8(source.c_str(), true);
	writeFile(stringDefinePath + "StringDefineEvent.cpp", source);
}

// EventType.h
void CodeEvent::generateEventType(const myVector<pair<string, string>>& eventList, string headerPath)
{
	string str0;
	line(str0, "#ifndef _EVENT_TYPE_H_");
	line(str0, "#define _EVENT_TYPE_H_");
	line(str0, "");
	line(str0, "// ��Ϸ�¼�����");
	line(str0, "class EVENT_TYPE");
	line(str0, "{");
	line(str0, "public:");
	int maxTableCount = 12;
	FOR_VECTOR_CONST(eventList)
	{
		string type = nameToUpper(eventList[i].first);
		type = type.substr(strlen("_EVENT_"));
		string str = "\tconst static ushort " + type + " = " + intToString(i + 1) + ";";
		// ��һ����һ��Ҫȥ���ַ����еĿ�ͷ���Ʊ��,�ڶ�����һ����Ϊ�ַ�����ͷ�Ѿ���һ���Ʊ����,���Կ����ټ�1��
		int tableCount = ceil((maxTableCount * 4 - (str.length() - 1)) / 4.0f) - 1;
		clampMin(tableCount, 0);
		string tableStr;
		FOR_J((uint)tableCount)
		{
			tableStr += "\t";
		}
		line(str0, str + tableStr + eventList[i].second);
	}
	line(str0, "};");
	line(str0, "");
	line(str0, "#endif", false);
	validPath(headerPath);
	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(headerPath + "EventType.h", str0);
}