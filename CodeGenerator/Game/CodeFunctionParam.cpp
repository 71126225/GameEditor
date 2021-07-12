#include "CodeFunctionParam.h"

void CodeFunctionParam::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppHeaderPath = cppGamePath + "FunctionParam/";

	string file;
	openTxtFile("FunctionParam.txt", file);
	if (file.length() == 0)
	{
		ERROR("δ���ļ�FunctionParam.txt");
		return;
	}
	myVector<string> list;
	split(file.c_str(), "\r\n", list);
	// ����StringDefineFunctionParam�ļ�
	generateStringDefine(list, cppStringDefinePath);
	// ����FunctionParamHeader.h�ļ�
	generateCppTotalHeaderFile(list, cppHeaderPath);
	// ����FunctionParamRegister�ļ�
	generateCppRegister(list, cppHeaderPath);
}

// FunctionParamHeader.h
void CodeFunctionParam::generateCppTotalHeaderFile(const myVector<string>& list, string filePath)
{
	string str0;
	line(str0, "#ifndef _FUNCTION_PARAM_HEADER_H_");
	line(str0, "#define _FUNCTION_PARAM_HEADER_H_");
	line(str0, "");
	line(str0, "#include \"FunctionParam.h\"");
	uint count = list.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + list[i] + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif", false);

	writeFile(filePath + "FunctionParamHeader.h", ANSIToUTF8(str0.c_str(), true));
}

// FunctionParamRegister�ļ�
void CodeFunctionParam::generateCppRegister(const myVector<string>& list, string filePath)
{
	string str0;
	line(str0, "#include \"GameHeader.h\"");
	line(str0, "");
	line(str0, "#define ADD_FUNCTION_PARAM(param) mFunctionParamFactoryManager->addFactory<param>(NAME(param))");
	line(str0, "");
	line(str0, "void FunctionParamRegister::registeAll()");
	line(str0, "{");
	uint count = list.size();
	FOR_I(count)
	{
		line(str0, "\tADD_FUNCTION_PARAM(" + list[i] + ");");
	}
	line(str0, "}");

	writeFile(filePath + "FunctionParamRegister.cpp", ANSIToUTF8(str0.c_str(), true));
}

// StringDefineFunctionParam.h��StringDefineFunctionParam.cpp
void CodeFunctionParam::generateStringDefine(const myVector<string>& list, string filePath)
{
	// ͷ�ļ�
	string header;
	line(header, "#ifdef _STRING_DEFINE_FUNCTION_PARAM_H_");
	line(header, "#error \"����ͷ�ļ�,ֻ�ܱ�StringDefine.h������\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_FUNCTION_PARAM_H_");
	line(header, "");
	uint count = list.size();
	FOR_I(count)
	{
		line(header, stringDeclare(list[i]));
	}
	line(header, "");
	line(header, "#endif");
	writeFile(filePath + "StringDefineFunctionParam.h", ANSIToUTF8(header.c_str(), true));

	// Դ�ļ�
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(count)
	{
		line(source, stringDefine(list[i]));
	}
	writeFile(filePath + "StringDefineFunctionParam.cpp", ANSIToUTF8(source.c_str(), true));
}