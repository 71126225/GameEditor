#include <iostream>
#include "txSystemInfo.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (strlen(lpCmdLine) <= 0)
	{
		return 1;
	}
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
	content += "\r\n";
	content += info->getBIOS();
	content += "\r\n";
	content += info->getMainboardType();
	content += "\r\n";
	content += info->getCurMAC();
	std::string fileName = std::string(lpCmdLine) + "Hardware.txt";
	DeleteFileA(fileName.c_str());
	FILE* pFile = NULL;
	fopen_s(&pFile, fileName.c_str(), "wb");
	fwrite(content.c_str(), sizeof(char), content.length(), pFile);
	fclose(pFile);
	return 0;
}