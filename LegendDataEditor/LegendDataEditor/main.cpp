#include <iostream>
#include "Utility.h"
#include "SQLite.h"

void main()
{
	while (true)
	{
		std::cout << "ѡ����\n\
1:����wix��wil�ļ�. \n\
2:�����ɫ�ļ��ṹ \n\
3:���������ļ��ṹ \n\
4:��������ļ��ṹ0 \n\
5:��������ļ��ṹ1 \n\
6:������Ч�ļ��ṹ \n\
7:����ɫ�ļ�֡����д��SQLite \n\
8:�������ļ�֡����д��SQLite \n\
9:�������ļ�֡����д��SQLite \n\
17:�Զ����㷽�򲢷��� \n\
18:���λ���ļ� \n\
19:ɾ����ЧͼƬ \n\
20:������������ļ� \n\
21:������Ϊ����֡��ʽ \n\
0:�˳�" << std::endl;
		int input;
		std::cin >> input;
		if (input == 0)
		{
			break;
		}
		else if (input == 1)
		{
			std::cout << "�����ļ���:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ����..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			std::string outputPath = "../media/" + fileName + "/";
			std::string WIX_SUFFIX = ".wix";
			std::string WIL_SUFFIX = ".wil";
			ImageUtility::wixWilToPNG(filePath + WIX_SUFFIX, filePath + WIL_SUFFIX, outputPath);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 2)
		{
			std::cout << "�����ļ�����:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ�����ɫ�ļ��ṹ..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::autoGroupHumanImage(filePath);
			std::cout << "�ļ��������,�����ȸ�ÿ������ֶ��������ٽ�֡����д��SQLite" << std::endl;
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 3)
		{
			std::cout << "�����ļ�����:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ���������ļ��ṹ..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::autoGroupWeaponImage(filePath);
			std::cout << "�ļ��������,�����ȸ�ÿ�������ֶ��������ٽ�֡����д��SQLite" << std::endl;
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 4)
		{
			std::cout << "�����ļ���:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ��������ļ��ṹ0..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::autoGroupMonsterImage0(filePath);
			std::cout << "���ֶ�Ϊÿһ��������ж�������,Ȼ��ѡ����������ļ��ṹ1" << std::endl;
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 5)
		{
			std::cout << "�����ļ���:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ��������ļ��ṹ1..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::autoGroupMonsterImage1(filePath);
			std::cout << "�ļ��������,�����ȸ�ÿ�������ֶ��������ٽ�֡����д��SQLite" << std::endl;
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 6)
		{
			std::cout << "�����ļ�����:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ������Ч�ļ��ṹ..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::autoGroupEffectImage(filePath);
			std::cout << "�ļ��������,�����ȸ�ÿ������ֶ��������ٽ�֡����д��SQLite" << std::endl;
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 7)
		{
			std::cout << "�����ļ���:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ����ɫ�ļ�֡����д��SQLite..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			SQLite* mSQLite = TRACE_NEW(SQLite, mSQLite, "../media/MicroLegend.db");
			ImageUtility::saveFrameInfo(filePath, IT_HUMAN, mSQLite);
			TRACE_DELETE(mSQLite);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 8)
		{
			std::cout << "�����ļ���:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ�������ļ�֡����д��SQLite..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			SQLite* mSQLite = TRACE_NEW(SQLite, mSQLite, "../media/MicroLegend.db");
			ImageUtility::saveFrameInfo(filePath, IT_WEAPON, mSQLite);
			TRACE_DELETE(mSQLite);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 9)
		{
			std::cout << "�����ļ���:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ�������ļ�֡����д��SQLite..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			SQLite* mSQLite = TRACE_NEW(SQLite, mSQLite, "../media/MicroLegend.db");
			ImageUtility::saveFrameInfo(filePath, IT_MONSTER, mSQLite);
			TRACE_DELETE(mSQLite);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 17)
		{
			std::cout << "�����ļ�����:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ�Զ����㷽�򲢷���..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::renameByDirection(filePath);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 18)
		{
			std::cout << "�����ļ�����:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ���λ���ļ�..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::splitPositionFile(filePath);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 19)
		{
			std::cout << "�����ļ�����:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "��ʼɾ����Ч�ļ�..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::deleteInvalidImage(filePath);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 20)
		{
			std::cout << "�����ļ�����:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ������������ļ�..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::renameImage(filePath);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 21)
		{
			std::cout << "�����ļ�����:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ������Ϊ����֡��ʽ..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::renameImageToAnim(filePath);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		system("pause");
		std::cout << std::endl;
	}
	return;
}