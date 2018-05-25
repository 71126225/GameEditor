#ifndef _EDITOR_UTILITY_H_
#define _EDITOR_UTILITY_H_

#include "txEngineDefine.h"

#include "EditorHeader.h"

class EditorUtility
{
public:
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
		return EMPTY_STRING;
	}
};

#endif