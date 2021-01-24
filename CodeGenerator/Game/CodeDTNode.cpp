#include "CodeDTNode.h"

const string CodeDTNode::cppHeaderPath = cppGamePath + "/Character/Component/DecisionTree/";

void CodeDTNode::generate()
{
	string file;
	openTxtFile("DTNode.txt", file);
	if (file.length() == 0)
	{
		ERROR("δ���ļ�DTNode.txt");
		return;
	}
	file = UTF8ToANSI(file.c_str(), true);
	myVector<string> nodeLineList;
	split(file.c_str(), "\r\n", nodeLineList);
	// ����StringDefineDTNode�ļ�
	generateStringDefine(nodeLineList, cppStringDefinePath);
	// ����DTNodeHeader.h�ļ�
	generateHeaderFile(nodeLineList, cppHeaderPath);
	// ����DTNodeRegister.cpp�ļ�
	generateRegisterFile(nodeLineList, cppHeaderPath);
}

// DTNodeHeader.h�ļ�
void CodeDTNode::generateHeaderFile(const myVector<string>& nodeList, string headerPath)
{
	string str0;
	line(str0, "#ifndef _DT_NODE_HEADER_H_");
	line(str0, "#define _DT_NODE_HEADER_H_");
	line(str0, "");
	uint count = nodeList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + nodeList[i] + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif", false);

	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(headerPath + "DTNodeHeader.h", str0);
}

// StringDefineDTNode.h��StringDefineDTNode.cpp
void CodeDTNode::generateStringDefine(const myVector<string>& nodeList, string stringDefinePath)
{
	// ͷ�ļ�
	string header;
	uint count = nodeList.size();
	FOR_I(count)
	{
		line(header, "DECLARE_STRING(" + nodeList[i] + ");");
	}

	header = ANSIToUTF8(header.c_str(), true);
	writeFile(stringDefinePath + "StringDefineDTNode.h", header);

	// Դ�ļ�
	string source;
	line(source, "#include \"StringDefine.h\"");
	line(source, "#include \"DTNodeHeader.h\"");
	line(source, "");
	FOR_I(count)
	{
		line(source, "DEFINE_STRING(" + nodeList[i] + ");");
	}

	source = ANSIToUTF8(source.c_str(), true);
	writeFile(stringDefinePath + "StringDefineDTNode.cpp", source);
}

// DTNodeRegister.h��DTNodeRegister.cpp
void CodeDTNode::generateRegisterFile(const myVector<string>& nodeList, string headerPath)
{
	string source;
	line(source, "#include \"DTNodeRegister.h\"");
	line(source, "#include \"DTNodeHeader.h\"");
	line(source, "#include \"DTNodeFactoryManager.h\"");
	line(source, "");
	line(source, "#define REGISTE_NODE(type) mDTNodeFactoryManager->addFactory<type>(STR(type));");
	line(source, "");
	line(source, "void DTNodeRegister::registeAll()");
	line(source, "{");
	uint count = nodeList.size();
	FOR_I(count)
	{
		line(source, "\tREGISTE_NODE(" + nodeList[i] + ");");
	}
	line(source, "}", false);

	source = ANSIToUTF8(source.c_str(), true);
	writeFile(headerPath + "DTNodeRegister.cpp", source);
}