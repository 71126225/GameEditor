#include "CodeUnityBuild.h"

void CodeUnityBuild::generate()
{
	if (cppGamePath.length() == 0)
	{
		return;
	}

	myVector<string> fileList;
	myVector<string> patterns{".cpp"};
	findFiles(cppProjectPath, fileList, patterns.data(), patterns.size());
	FOR_VECTOR(fileList)
	{
		string& fileName = fileList[i];
		// �����Dependency�е��ļ�,����Ҫ����Dependency·��,����.c����Ҫ�ų����ļ�,�������⴦��
		int index = 0;
		if (findSubstr(fileName, "Dependency/", &index))
		{
			fileName = fileName.substr(index + strlen("Dependency/"));
		}
		else
		{
			fileName = getFileName(fileName);
		}
	}
	END(fileList);
	fileList.remove("main.cpp");
	fileList.remove("UnityBuild.cpp");
	// ����UnityBuild.cpp�ļ�
	generateCppUnityBuild(fileList, cppProjectPath);
}

// UnityBuild.cpp
void CodeUnityBuild::generateCppUnityBuild(const myVector<string>& fileList, string filePath)
{
	string str0;
	uint count = fileList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + fileList[i] + "\"", i != count - 1);
	}

	writeFile(filePath + "UnityBuild.cpp", ANSIToUTF8(str0.c_str(), true));
}