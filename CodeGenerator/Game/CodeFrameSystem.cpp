#include "CodeFrameSystem.h"

void CodeFrameSystem::generate()
{
	string cppHeaderPath = cppGamePath + "Common/";

	string frameSystemFile;
	openTxtFile("FrameSystem.txt", frameSystemFile);
	if (frameSystemFile.length() == 0)
	{
		ERROR("δ���ļ�FrameSystem.txt");
		return;
	}
	frameSystemFile = UTF8ToANSI(frameSystemFile.c_str(), true);
	myVector<string> lineList;
	split(frameSystemFile.c_str(), "\r\n", lineList);
	myVector<string> frameSystemList;
	myVector<string> factoryManagerList;
	myVector<string> classPoolList;
	int typeIndex = -1;
	FOR_VECTOR(lineList)
	{
		if (lineList[i] == "{")
		{
			if (lineList[i - 1] == "FrameSystem")
			{
				typeIndex = 0;
			}
			else if (lineList[i - 1] == "FactoryManager")
			{
				typeIndex = 1;
			}
			else if (lineList[i - 1] == "ClassPool")
			{
				typeIndex = 2;
			}
			continue;
		}
		if (lineList[i] == "}")
		{
			typeIndex = -1;
			continue;
		}
		removeAll(lineList[i], '\t');
		if (typeIndex == 0)
		{
			frameSystemList.push_back(lineList[i]);
		}
		else if (typeIndex == 1)
		{
			factoryManagerList.push_back(lineList[i]);
		}
		else if (typeIndex == 2)
		{
			classPoolList.push_back(lineList[i]);
		}
	}
	END(lineList);
	generateHeaderFile(frameSystemList, factoryManagerList, classPoolList, cppHeaderPath);
	generateSystemDefineFile(frameSystemList, factoryManagerList, classPoolList, cppHeaderPath);
}

// FrameSystemHeader.h
void CodeFrameSystem::generateHeaderFile(const myVector<string>& frameSystemList,
										const myVector<string>& factoryManagerList, 
										const myVector<string>& classPoolList, 
										string filePath)
{
	string str0;
	line(str0, "#ifndef _FRAME_SYSTEM_HEADER_H_");
	line(str0, "#define _FRAME_SYSTEM_HEADER_H_");
	line(str0, "");
	line(str0, "// ����ϵͳ���");
	FOR_VECTOR_CONST(frameSystemList)
	{
		line(str0, "#include \"" + frameSystemList[i] + ".h\"");
	}
	line(str0, "// ����������");
	FOR_VECTOR_CONST(factoryManagerList)
	{
		line(str0, "#include \"" + factoryManagerList[i] + ".h\"");
	}
	line(str0, "// �����");
	FOR_VECTOR_CONST(classPoolList)
	{
		line(str0, "#include \"" + classPoolList[i] + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif");
	writeFile(filePath + "FrameSystemHeader.h", ANSIToUTF8(str0.c_str(), true));
}

// FrameSystemRegiste.h, FrameSystemDeclare.h, FrameSystemDefine.cpp, FrameSystemGet.h
void CodeFrameSystem::generateSystemDefineFile(const myVector<string>& frameSystemList,
												const myVector<string>& factoryManagerList,
												const myVector<string>& classPoolList,
												string filePath)
{
	// FrameSystemRegiste.h
	string registeAllSystem;
	line(registeAllSystem, "#ifdef _FRAME_SYSTEM_REGISTE_H_");
	line(registeAllSystem, "#error \"����ͷ�ļ�,ֻ����Game.cpp�е�ָ��λ�ñ�����\"");
	line(registeAllSystem, "#else");
	line(registeAllSystem, "#define _FRAME_SYSTEM_REGISTE_H_");
	line(registeAllSystem, "");
	FOR_VECTOR_CONST(frameSystemList)
	{
		line(registeAllSystem, "REGISTE_SYSTEM(" + frameSystemList[i] + ");");
	}
	FOR_VECTOR_CONST(factoryManagerList)
	{
		line(registeAllSystem, "REGISTE_SYSTEM(" + factoryManagerList[i] + ");");
	}
	FOR_VECTOR_CONST(classPoolList)
	{
		line(registeAllSystem, "REGISTE_SYSTEM(" + classPoolList[i] + ");");
	}
	line(registeAllSystem, "");
	line(registeAllSystem, "#endif");
	writeFile(filePath + "FrameSystemRegiste.h", ANSIToUTF8(registeAllSystem.c_str(), true));

	// FrameSystemDeclare.h
	string declareAllSystem;
	line(declareAllSystem, "#ifdef _FRAME_SYSTEM_DECLARE_H_");
	line(declareAllSystem, "#error \"����ͷ�ļ�,ֻ����GameBase.h�б�����\"");
	line(declareAllSystem, "#else");
	line(declareAllSystem, "#define _FRAME_SYSTEM_DECLARE_H_");
	line(declareAllSystem, "");
	FOR_I(frameSystemListCount)
	{
		line(declareAllSystem, "DECALRE_SYSTEM(" + frameSystemList[i] + ");");
	}
	FOR_I(factoryManagerListCount)
	{
		line(declareAllSystem, "DECALRE_SYSTEM(" + factoryManagerList[i] + ");");
	}
	FOR_I(classPoolListCount)
	{
		line(declareAllSystem, "DECALRE_SYSTEM(" + classPoolList[i] + ");");
	}
	line(declareAllSystem, "");
	line(declareAllSystem, "#endif");
	writeFile(filePath + "FrameSystemDeclare.h", ANSIToUTF8(declareAllSystem.c_str(), true));

	// FrameSystemDefine.cpp
	string defineAllSystem;
	line(defineAllSystem, "#include \"GameHeader.h\"");
	line(defineAllSystem, "");
	FOR_I(frameSystemListCount)
	{
		line(defineAllSystem, "DEFINE_SYSTEM_GAME(" + frameSystemList[i] + ");");
	}
	FOR_I(factoryManagerListCount)
	{
		line(defineAllSystem, "DEFINE_SYSTEM_GAME(" + factoryManagerList[i] + ");");
	}
	FOR_I(classPoolListCount)
	{
		line(defineAllSystem, "DEFINE_SYSTEM_GAME(" + classPoolList[i] + ");");
	}
	writeFile(filePath + "FrameSystemDefine.cpp", ANSIToUTF8(defineAllSystem.c_str(), true));

	// FrameSystemGet.h
	string getAllSystem;
	line(getAllSystem, "#ifdef _FRAME_SYSTEM_GET_H_");
	line(getAllSystem, "#error \"����ͷ�ļ�,ֻ����GameBase.cpp�б�����\"");
	line(getAllSystem, "#else");
	line(getAllSystem, "#define _FRAME_SYSTEM_GET_H_");
	line(getAllSystem, "");
	FOR_I(frameSystemListCount)
	{
		line(getAllSystem, "GET_SYSTEM(" + frameSystemList[i] + ");");
	}
	FOR_I(factoryManagerListCount)
	{
		line(getAllSystem, "GET_SYSTEM(" + factoryManagerList[i] + ");");
	}
	FOR_I(classPoolListCount)
	{
		line(getAllSystem, "GET_SYSTEM(" + classPoolList[i] + ");");
	}
	line(getAllSystem, "");
	line(getAllSystem, "#endif");
	writeFile(filePath + "FrameSystemGet.h", ANSIToUTF8(getAllSystem.c_str(), true));
}