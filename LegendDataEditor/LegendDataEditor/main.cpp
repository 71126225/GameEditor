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
7:�Զ����㷽�� \n\
8:���λ���ļ� \n\
9:ɾ����ЧͼƬ \n\
10:������������ļ� \n\
11:������Ϊ����֡��ʽ \n\
12:��ȡ��ͼ��Դ\n\
13:����ͼ��\n\
14:���ͼ��\n\
15:���ȫ��ͼ��\n\
16:ת����ͼ�ļ�\n\
17:ת��ȫ����ͼ�ļ�\n\
18:��������wix��wil�ļ�\n\
19:����NPC�ļ��ṹ\n\
20:��media��ȫ������֡����д��SQLite \n\
21:��ͼƬλ��ƫ��д��SQLite \n\
22:��������ײ��Χд��SQLite \n\
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
			ImageUtility::wixWilToPNG(filePath + ".wix", filePath + ".wil", outputPath);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 2)
		{
			std::cout << "�����ļ�����:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ�����ɫ�ļ��ṹ..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupHumanImage("../media/" + fileName);
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
			ImageUtility::autoGroupWeaponImage("../media/" + fileName);
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
			ImageUtility::autoGroupMonsterImage0("../media/" + fileName);
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
			ImageUtility::autoGroupMonsterImage1("../media/" + fileName);
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
			ImageUtility::autoGroupEffectImage("../media/" + fileName);
			std::cout << "�ļ��������,�����ȸ�ÿ������ֶ��������ٽ�֡����д��SQLite" << std::endl;
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 7)
		{
			std::cout << "�����ļ�����:";
			string fileName;
			std::cin >> fileName;
			std::cout << "���뷽������:";
			int directionCount = 1;
			std::cin >> directionCount;
			std::cout << "��ʼ�Զ����㷽��..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::renameByDirection("../media/" + fileName, directionCount, false);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 8)
		{
			std::cout << "�����ļ�����:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ���λ���ļ�..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::splitPositionFile("../media/" + fileName);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 9)
		{
			std::cout << "�����ļ�����:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼɾ����Ч�ļ�..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::deleteInvalidImage("../media/" + fileName);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 10)
		{
			std::cout << "�����ļ�����:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ������������ļ�..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::renameImage("../media/" + fileName);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 11)
		{
			std::cout << "�����ļ�����:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ������Ϊ����֡��ʽ..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::renameImageToAnim("../media/" + fileName);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 12)
		{
			std::cout << "�����ͼ���:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ��ȡ��ͼ��Դ..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::collectMapTexture("../media/Map/" + fileName);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 13)
		{
			std::cout << "�����ļ�����:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ����ͼ��..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::groupAtlas("../media/" + fileName, 100);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 14)
		{
			std::cout << "����media�����·��:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ���ͼ��..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::texturePacker("../media/" + fileName);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 15)
		{
			std::cout << "����media�����·��:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ���ȫ��ͼ��..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::texturePackerAll("../media/" + fileName);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 16)
		{
			std::cout << "�����ͼ��:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼת����ͼ�ļ�..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::convertMapFile("../media/Map/" + fileName);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 17)
		{
			std::cout << "�����ļ�����:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼת����ͼ�ļ�..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::convertAllMapFile("../media/" + fileName);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 18)
		{
			std::cout << "��ʼ��������wil..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::allWixWilToPNG("../media");
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 19)
		{
			std::cout << "�����ļ�����:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ����NPC�ļ��ṹ..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupNPCImage("../media/" + fileName);
			std::cout << "�ļ��������" << std::endl;
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 20)
		{
			std::cout << "��ʼ����������֡����д��SQLite..." << std::endl;
			char updateOnly = 'y';
			std::cout << "�Ƿ�ֻ��������? y(��)/n(��)";
			std::cin >> updateOnly;
			long startTime = timeGetTime();
			if (updateOnly == 'y' || updateOnly == 'n')
			{
				ImageUtility::writeAnimFrameSQLite(updateOnly == 'y');
			}
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 21)
		{
			std::cout << "����media�����·��:";
			string fileName;
			std::cin >> fileName;
			std::cout << "��ʼ��ͼƬλ��ƫ��д��SQLite..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::writeImagePosSQLite("../media/" + fileName);
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		else if (input == 22)
		{
			std::cout << "��ʼ��������ײ��Χ����д��SQLite..." << std::endl;
			char updateOnly = 'y';
			std::cout << "�Ƿ�ֻ��������? y(��)/n(��)";
			std::cin >> updateOnly;
			long startTime = timeGetTime();
			if (updateOnly == 'y' || updateOnly == 'n')
			{
				ImageUtility::writeMonsterColliderSQLite(updateOnly == 'y');
			}
			std::cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << std::endl;
		}
		system("pause");
		std::cout << std::endl;
	}
	return;
}