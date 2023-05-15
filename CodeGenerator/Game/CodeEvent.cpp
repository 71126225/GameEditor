#include "CodeEvent.h"

void CodeEvent::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppHeaderPath = cppGamePath + "EventSystem/";
	string cppEventFilePath = cppHeaderPath + "Event/";

	string cmdFile = openTxtFile("Event.txt");
	if (cmdFile.length() == 0)
	{
		ERROR("δ���ļ�Event.txt");
		return;
	}
	myVector<string> eventLineList;
	split(cmdFile.c_str(), "\r\n", eventLineList);
	myVector<pair<string, string>> eventList;
	FOR_VECTOR_CONST(eventLineList)
	{
		myVector<string> splitResult;
		split(eventLineList[i].c_str(), "\t", splitResult);
		if (splitResult.size() != 2)
		{
			ERROR("�¼��ļ���������:" + eventLineList[i]);
		}
		eventList.push_back(make_pair(splitResult[0], splitResult[1]));
	}
	// ����StringDefineEvent�ļ�
	generateStringDefineEvent(eventList, cppGameStringDefineFile);
	// ����EventType.h�ļ�
	generateEventType(eventList, cppHeaderPath);
	FOR_VECTOR_CONST(eventList)
	{
		generateEventFile(eventList[i].first, cppEventFilePath);
	}
}

void CodeEvent::generateStringDefineEvent(const myVector<pair<string, string>>& eventList, const string& stringDefineFile)
{
	// ����StringDefine.h���ض�����
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(stringDefineFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// Event"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "}"); }))
	{
		return;
	}

	for (const auto& item : eventList)
	{
		codeList.insert(++lineStart, stringDeclare(item.first));
	}
	writeFile(stringDefineFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

// EventType.h
void CodeEvent::generateEventType(const myVector<pair<string, string>>& eventList, const string& headerPath)
{
	string str0;
	line(str0, "#pragma once");
	line(str0, "");
	line(str0, "// ��Ϸ�¼�����");
	line(str0, "class EVENT_TYPE");
	line(str0, "{");
	line(str0, "public:");
	// ������Ҫ���Ĭ�ϵ���Чֵ,���Ը���һ���б�
	myVector<pair<string, string>> newList;
	newList.push_back(make_pair("None", "// ��Чֵ"));
	newList.addRange(eventList);
	FOR_VECTOR_CONST(newList)
	{
		string type = removeStartString(nameToUpper(newList[i].first), "_EVENT_");
		// ����Noneת����������_��ͷ,���Ի�����Ҫȷ������ĸ��ͷ
		type = removeStartString(type, "_");
		string str = "\tconst static ushort " + type + " = " + intToString(i) + ";";
		uint tableCount = generateAlignTableCount(str, 48);
		FOR_J(tableCount)
		{
			str += "\t";
		}
		line(str0, str + newList[i].second);
	}
	line(str0, "};", false);

	writeFile(headerPath + "EventType.h", ANSIToUTF8(str0.c_str(), true));
}

void CodeEvent::generateEventFile(const string& eventName, const string& eventPath)
{
	string headerFullPath = eventPath + eventName + ".h";
	if (isFileExist(headerFullPath))
	{
		return;
	}
	string header;
	string typeStr = nameToUpper(eventName.substr(strlen("Event")), false);
	line(header, "#pragma once");
	line(header, "");
	line(header, "#include \"GameEvent.h\"");
	line(header, "");
	line(header, "class " + eventName + " : public GameEvent");
	line(header, "{");
	line(header, "public:");
	line(header, "\t" + eventName + "()");
	line(header, "\t{");
	line(header, "\t\tmType = EVENT_TYPE::" + typeStr + ";");
	line(header, "\t}");
	line(header, "public:");
	line(header, "};", false);

	writeFile(headerFullPath, ANSIToUTF8(header.c_str(), true));
}