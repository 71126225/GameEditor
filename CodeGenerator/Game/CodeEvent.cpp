#include "CodeEvent.h"

void CodeEvent::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppHeaderPath = cppGamePath + "EventSystem/";
	string cppEventFilePath = cppHeaderPath + "Event/";

	string cmdFile;
	openTxtFile("Event.txt", cmdFile);
	if (cmdFile.length() == 0)
	{
		ERROR("δ���ļ�Event.txt");
		return;
	}
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
	generateStringDefineEvent(eventList, cppStringDefinePath);
	// ����EventHeader.h�ļ�
	generateHeaderFile(eventList, cppHeaderPath);
	// ����EventType.h�ļ�
	generateEventType(eventList, cppHeaderPath);
	FOR_VECTOR(eventList)
	{
		generateEventFile(eventList[i].first, cppEventFilePath);
	}
	END(eventList);
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

	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(headerPath + "EventHeader.h", str0);
}

// StringDefineEvent.h��StringDefineEvent.cpp
void CodeEvent::generateStringDefineEvent(const myVector<pair<string, string>>& eventList, string stringDefinePath)
{
	// ͷ�ļ�
	string header;
	line(header, "#ifdef _STRING_DEFINE_EVENT_H_");
	line(header, "#error \"����ͷ�ļ�,ֻ�ܱ�StringDefine.h������\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_EVENT_H_");
	line(header, "");
	uint cmdCount = eventList.size();
	FOR_I(cmdCount)
	{
		line(header, stringDeclare(eventList[i].first));
	}
	line(header, "");
	line(header, "#endif");
	writeFile(stringDefinePath + "StringDefineEvent.h", ANSIToUTF8(header.c_str(), true));

	// Դ�ļ�
	string source;
	line(source, "#include \"GameHeader.h\"");
	writeFile(stringDefinePath + "StringDefineEvent.cpp", ANSIToUTF8(source.c_str(), true));
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
	// ������Ҫ���Ĭ�ϵ���Чֵ,���Ը���һ���б�
	myVector<pair<string, string>> newList;
	newList.push_back(make_pair("None", "// ��Чֵ"));
	newList.merge(eventList);
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
	line(str0, "};");
	line(str0, "");
	line(str0, "#endif", false);

	writeFile(headerPath + "EventType.h", ANSIToUTF8(str0.c_str(), true));
}

void CodeEvent::generateEventFile(const string& eventName, string eventPath)
{
	string headerFullPath = eventPath + eventName + ".h";
	if (isFileExist(headerFullPath))
	{
		return;
	}
	string header;
	string marcoName = nameToUpper(eventName) + "_H_";
	string typeStr = nameToUpper(eventName.substr(strlen("Event")), false);
	line(header, "#ifndef " + marcoName);
	line(header, "#define " + marcoName);
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
	line(header, "};");
	line(header, "");
	line(header, "#endif", false);

	writeFile(headerFullPath, ANSIToUTF8(header.c_str(), true));
}