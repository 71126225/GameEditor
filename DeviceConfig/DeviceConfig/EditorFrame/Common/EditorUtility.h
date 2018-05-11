#ifndef _EDITOR_UTILITY_H_
#define _EDITOR_UTILITY_H_

#include <set>
#include <vector>
#include <string>

#include "EditorHeader.h"
#include "EditorBase.h"
#include "EditorFrame.h"

class EditorUtility : public EditorBase
{
public:
	static void logError(std::string info)
	{
		wxMessageBox(info, "����", wxOK | wxCENTRE, (wxWindow*)(mEditorFrame));
	}
	static void logInfo(std::string info)
	{
		wxMessageBox(info, "��ʾ", wxOK | wxCENTRE, (wxWindow*)(mEditorFrame));
	}
	// ȥ���ļ����ĺ�׺,���ص����ļ����ĺ�׺,����.��
	static wxString removeSuffix(wxString& fileName)
	{
		int dotPos = fileName.find_last_of('.');
		if (dotPos != -1)
		{
			wxString suffix = fileName.substr(dotPos + 1, fileName.length() - dotPos - 1);
			fileName = fileName.substr(0, dotPos);
			return suffix;
		}
		return "";
	}

	static std::string intToString(int i)
	{
		char str[256];
		sprintf_s(str, 256, "%d", i);
		return std::string(str);
	}

	// �ж�oriString�Ƿ���pattern��β
	static bool endWith(std::string oriString, std::string pattern)
	{
		if (oriString.length() < pattern.length())
		{
			return false;
		}
		std::string endString = oriString.substr(oriString.length() - pattern.length(), pattern.length());
		return endString == pattern;
	}
	// �ж�oriString�Ƿ���pattern��ͷ
	static bool startWith(std::string oriString, std::string pattern)
	{
		if (oriString.length() < pattern.length())
		{
			return false;
		}
		std::string startString = oriString.substr(0, pattern.length());
		return startString == pattern;
	}

	static float pow_f(float f, int p);

	static std::string floatToString(float f, int precision = 3);

	static bool isIgnoreClass(std::set<std::string>& keywordList, std::string name);
	static bool isIgnoreClass(std::vector<std::string>& keywordList, std::string name);
};

#endif