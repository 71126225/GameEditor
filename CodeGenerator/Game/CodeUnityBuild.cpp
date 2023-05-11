#include "CodeUnityBuild.h"

void CodeUnityBuild::generate()
{
	if (ServerProjectPath.length() == 0)
	{
		return;
	}

	// ����UnityBuild.cpp�ļ�
	generateCppUnityBuild(cppGameProjectPath, "UnityBuild.cpp");
	generateCppUnityBuild(cppFrameProjectPath, "UnityBuildFrame.cpp");
}

// UnityBuild.cpp
void CodeUnityBuild::generateCppUnityBuild(const string& filePath, const string& unityBuildFileName)
{
	myVector<string> fileList;
	myVector<string> patterns{ ".cpp" };
	findFiles(filePath, fileList, patterns.data(), patterns.size());
	for(string& fileName : fileList)
	{
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
	fileList.remove(unityBuildFileName);

	string str0;
	uint count = fileList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + fileList[i] + "\"", i != count - 1);
	}
	cout << "����unity�ļ�:" << filePath << ":" << unityBuildFileName << ",����:" << count << endl;
	writeFile(filePath + unityBuildFileName, ANSIToUTF8(str0.c_str(), true));
}