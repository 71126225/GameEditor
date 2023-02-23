#include "CodeDTNode.h"

void CodeDTNode::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	const string cppHeaderPath = cppGamePath + "/Character/Component/DecisionTree/";

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
	// ����DTNodeRegister.cpp�ļ�
	generateRegisterFile(nodeLineList, cppHeaderPath);
}

// StringDefineDTNode.h��StringDefineDTNode.cpp
void CodeDTNode::generateStringDefine(const myVector<string>& nodeList, const string& stringDefinePath)
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
	line(header, "#endif", false);
	writeFile(stringDefinePath + "StringDefineDTNode.h", ANSIToUTF8(header.c_str(), true));
}

// DTNodeRegister.h��DTNodeRegister.cpp
void CodeDTNode::generateRegisterFile(const myVector<string>& nodeList, const string& headerPath)
{
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	line(source, "void DTNodeRegister::registeAll()");
	line(source, "{");
	const uint count = nodeList.size();
	FOR_I(count)
	{
		line(source, "\tmDTNodeFactoryManager->addFactory<" + nodeList[i] + ">(NAME(" + nodeList[i] + "));");
	}
	line(source, "}", false);

	writeFile(headerPath + "DTNodeRegister.cpp", ANSIToUTF8(source.c_str(), true));
}