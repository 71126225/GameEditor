#include <iostream>
#include "txSystemInfo.h"

int main(int argc, char* args[])
{
	if (argc <= 0)
	{
		return 1;
	}
	char* infoFullPath = args[1];
	DeleteFileA(infoFullPath);
	txSystemInfo* info = new txSystemInfo();
	info->init();
	// ��Ӳ����Ϣ�������������д���ļ�
	std::string content = "";
	content += info->getOriMAC();
	content += "\r\n";
	content += info->getHDD();
	content += "\r\n";
	content += info->getMainbord();
	content += "\r\n";
	content += info->getCPU();
	FILE* pFile = NULL;
	fopen_s(&pFile, infoFullPath, "wb");
	fwrite(content.c_str(), sizeof(char), content.length(), pFile);
	fclose(pFile);
	return 0;
}