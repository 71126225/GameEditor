#include "CodeDTNode.h"

void CodeDTNode::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	string cppHeaderPath = cppGamePath + "/Character/Component/DecisionTree/";

	string file;
	openTxtFile("DTNode.txt", file);
	if (file.length() == 0)
	{
		ERROR("δ���ļ�DTNode.txt");
		return;
	}
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

	writeFile(headerPath + "DTNodeHeader.h", ANSIToUTF8(str0.c_str(), true));
}

// StringDefineDTNode.h��StringDefineDTNode.cpp
void CodeDTNode::generateStringDefine(const myVector<string>& nodeList, string stringDefinePath)
{
	// ͷ�ļ�
	string header;
	line(header, "#ifdef _STRING_DEFINE_DTNODE_H_");
	line(header, "#error \"����ͷ�ļ�,ֻ�ܱ�StringDefine.h������\"");
	line(header, "#else");
	line(header, "#define _STRING_DEFINE_DTNODE_H_");
	line(header, "");
	uint count = nodeList.size();
	FOR_I(count)
	{
		line(header, stringDeclare(nodeList[i]));
	}
	line(header, "");
	line(header, "#endif");
	writeFile(stringDefinePath + "StringDefineDTNode.h", ANSIToUTF8(header.c_str(), true));

	// Դ�ļ�
	string source;
	line(source, "#include \"GameHeader.h\"");
	writeFile(stringDefinePath + "StringDefineDTNode.cpp", ANSIToUTF8(source.c_str(), true));
}

// DTNodeRegister.h��DTNodeRegister.cpp
void CodeDTNode::generateRegisterFile(const myVector<string>& nodeList, string headerPath)
{
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	line(source, "#define REGISTE_NODE(type) mDTNodeFactoryManager->addFactory<type>(NAME(type));");
	line(source, "");
	line(source, "void DTNodeRegister::registeAll()");
	line(source, "{");
	uint count = nodeList.size();
	FOR_I(count)
	{
		line(source, "\tREGISTE_NODE(" + nodeList[i] + ");");
	}
	line(source, "}", false);

	writeFile(headerPath + "DTNodeRegister.cpp", ANSIToUTF8(source.c_str(), true));
}