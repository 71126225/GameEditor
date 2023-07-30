#include "CodeUnityBuild.h"

void CodeUnityBuild::generate()
{
	if (ServerProjectPath.empty())
	{
		return;
	}
	generateCppUnityBuild(ServerProjectPath, ServerProjectPath + "Game/UnityBuild.cpp");
}

// UnityBuild.cpp
void CodeUnityBuild::generateCppUnityBuild(const string& filePath, const string& unityBuildFileName)
{
	myVector<string> fileList;
	myVector<string> patterns{ ".cpp" };
	findFiles(filePath, fileList, patterns.data(), patterns.size());
	for (string& fileName : fileList)
	{
		// ������Ŀ�ļ��ĸ��Ӱ���Ŀ¼����������ʲôĿ¼
		string excludePath;
		for (const string& exclude : ServerExcludeIncludePath)
		{
			if (startWith(fileName, exclude))
			{
				excludePath = exclude;
				break;
			}
		}
		if (excludePath.empty())
		{
			fileName = getFileName(fileName);
		}
		else
		{
			// �ļ�����Ŀ¼���ڰ���Ŀ¼��ʱ,��ʱֻ�������ļ������һ��Ŀ¼
			fileName = getFileName(getFilePath(fileName)) + "/" + getFileName(fileName);
		}
	}
	fileList.remove(getFileName(unityBuildFileName));

	string str0;
	const uint count = fileList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + fileList[i] + "\"", i != count - 1);
	}
	writeFileIfChanged(unityBuildFileName, ANSIToUTF8(str0.c_str(), true));
}