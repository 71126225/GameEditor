#include <iostream>
#include "Utility.h"
#include "SQLite.h"

void main()
{
	std::cout << "ѡ����(1:����wix��wil�ļ�. 2:�����ɫ����):";
	int input;
	std::cin >> input;
	if (input == 1)
	{
		std::cout << "�����ļ���:";
		std::string fileName;
		std::cin >> fileName;
		std::cout << "��ʼ����...";
		std::string filePath = "../media/" + fileName;
		std::string outputPath = "../media/" + fileName + "/";
		std::string WIX_SUFFIX = ".wix";
		std::string WIL_SUFFIX = ".wil";
		ImageUtility::wixWilToPNG(filePath + WIX_SUFFIX, filePath + WIL_SUFFIX, outputPath);
	}
	else if(input == 2)
	{
		std::cout << "�����ļ�����:";
		std::string fileName;
		std::cin >> fileName;
		std::cout << "��ʼ����..." << std::endl;
		long startTime = timeGetTime();
		std::string filePath = "../media/" + fileName;
		SQLite* mSQLite = TRACE_NEW(SQLite, mSQLite, "../media/MicroLegend.db");
		ImageUtility::groupImage(filePath, IT_HUMAN, mSQLite);
		TRACE_DELETE(mSQLite);
		std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
	}
	//system("pause");
	return;
}