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
17:�Զ����㷽�� \n\
18:���λ���ļ� \n\
19:ɾ����ЧͼƬ \n\
20:������������ļ� \n\
21:������Ϊ����֡��ʽ \n\
22:��ȡ��ͼ��Դ\n\
23:����ͼ��\n\
24:���ͼ��\n\
25:���ȫ��ͼ��\n\
26:ת����ͼ�ļ�\n\
27:ת��ȫ����ͼ�ļ�\n\
28:��������wix��wil�ļ�\n\
29:����NPC�ļ��ṹ\n\
31:��media��ȫ������֡����д��SQLite \n\
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
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ����..." << std::endl;
			long startTime = timeGetTime();
			string filePath = "../media/" + fileName;
			string outputPath = "../media/" + fileName + "/";
			string WIX_SUFFIX = ".wix";
			string WIL_SUFFIX = ".wil";
			ImageUtility::wixWilToPNG(filePath + WIX_SUFFIX, filePath + WIL_SUFFIX, outputPath);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 2)
		{
			std::cout << "�����ļ�����:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ�����ɫ�ļ��ṹ..." << std::endl;
			long startTime = timeGetTime();
			string filePath = "../media/" + fileName;
			ImageUtility::autoGroupHumanImage(filePath);
			std::cout << "�ļ��������,�����ȸ�ÿ������ֶ��������ٽ�֡����д��SQLite" << std::endl;
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 3)
		{
			std::cout << "�����ļ�����:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ���������ļ��ṹ..." << std::endl;
			long startTime = timeGetTime();
			string filePath = "../media/" + fileName;
			ImageUtility::autoGroupWeaponImage(filePath);
			std::cout << "�ļ��������,�����ȸ�ÿ�������ֶ��������ٽ�֡����д��SQLite" << std::endl;
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 4)
		{
			std::cout << "�����ļ���:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ��������ļ��ṹ0..." << std::endl;
			long startTime = timeGetTime();
			string filePath = "../media/" + fileName;
			ImageUtility::autoGroupMonsterImage0(filePath);
			std::cout << "���ֶ�Ϊÿһ��������ж�������,Ȼ��ѡ����������ļ��ṹ1" << std::endl;
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 5)
		{
			std::cout << "�����ļ���:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ��������ļ��ṹ1..." << std::endl;
			long startTime = timeGetTime();
			string filePath = "../media/" + fileName;
			ImageUtility::autoGroupMonsterImage1(filePath);
			std::cout << "�ļ��������,�����ȸ�ÿ�������ֶ��������ٽ�֡����д��SQLite" << std::endl;
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 6)
		{
			std::cout << "�����ļ�����:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ������Ч�ļ��ṹ..." << std::endl;
			long startTime = timeGetTime();
			string filePath = "../media/" + fileName;
			ImageUtility::autoGroupEffectImage(filePath);
			std::cout << "�ļ��������,�����ȸ�ÿ������ֶ��������ٽ�֡����д��SQLite" << std::endl;
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 17)
		{
			std::cout << "�����ļ�����:";
			string fileName;
			std::cin >> fileName;
			std::cout << "���뷽������:";
			int directionCount = 1;
			std::cin >> directionCount;
			std::cout << "��ʼ�Զ����㷽��..." << std::endl;
			long startTime = timeGetTime();
			string filePath = "../media/" + fileName;
			ImageUtility::renameByDirection(filePath, directionCount, false);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 18)
		{
			std::cout << "�����ļ�����:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ���λ���ļ�..." << std::endl;
			long startTime = timeGetTime();
			string filePath = "../media/" + fileName;
			ImageUtility::splitPositionFile(filePath);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 19)
		{
			std::cout << "�����ļ�����:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼɾ����Ч�ļ�..." << std::endl;
			long startTime = timeGetTime();
			string filePath = "../media/" + fileName;
			ImageUtility::deleteInvalidImage(filePath);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 20)
		{
			std::cout << "�����ļ�����:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ������������ļ�..." << std::endl;
			long startTime = timeGetTime();
			string filePath = "../media/" + fileName;
			ImageUtility::renameImage(filePath);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 21)
		{
			std::cout << "�����ļ�����:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ������Ϊ����֡��ʽ..." << std::endl;
			long startTime = timeGetTime();
			string filePath = "../media/" + fileName;
			ImageUtility::renameImageToAnim(filePath);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 22)
		{
			std::cout << "�����ͼ���:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ��ȡ��ͼ��Դ..." << std::endl;
			long startTime = timeGetTime();
			string filePath = "../media/Map/" + fileName;
			ImageUtility::collectMapTexture(filePath);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 23)
		{
			std::cout << "�����ļ�����:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ����ͼ��..." << std::endl;
			long startTime = timeGetTime();
			string filePath = "../media/" + fileName;
			ImageUtility::groupAtlas(filePath, 100);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 24)
		{
			std::cout << "����media�����·��:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ���ͼ��..." << std::endl;
			long startTime = timeGetTime();
			string filePath = "../media/" + fileName;
			ImageUtility::texturePacker(filePath);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 25)
		{
			std::cout << "����media�����·��:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ���ȫ��ͼ��..." << std::endl;
			long startTime = timeGetTime();
			string filePath = "../media/" + fileName;
			ImageUtility::texturePackerAll(filePath);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 26)
		{
			std::cout << "�����ͼ��:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼת����ͼ�ļ�..." << std::endl;
			long startTime = timeGetTime();
			string filePath = "../media/Map/" + fileName;
			ImageUtility::convertMapFile(filePath);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 27)
		{
			std::cout << "�����ļ�����:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼת����ͼ�ļ�..." << std::endl;
			long startTime = timeGetTime();
			string filePath = "../media/" + fileName;
			ImageUtility::convertAllMapFile(filePath);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 28)
		{
			std::cout << "��ʼ��������wil..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::allWixWilToPNG("../media");
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 29)
		{
			std::cout << "�����ļ�����:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ����NPC�ļ��ṹ..." << std::endl;
			long startTime = timeGetTime();
			string filePath = "../media/" + fileName;
			ImageUtility::autoGroupNPCImage(filePath);
			std::cout << "�ļ��������" << std::endl;
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 31)
		{
			std::cout << "��ʼ����������֡����д��SQLite..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::writeAnimFrameSQLite();
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		system("pause");
		std::cout << std::endl;
	}
	return;
}