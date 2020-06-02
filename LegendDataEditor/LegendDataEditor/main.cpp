#include <iostream>
#include "Utility.h"
#include "SQLite.h"

using namespace std;

void main()
{
	while (true)
	{
		cout << "ѡ����\n\
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
13:����ͼ��\n\
14:���ͼ��\n\
15:���ȫ��ͼ��\n\
18:��������wix��wil�ļ�\n\
19:����NPC�ļ��ṹ\n\
20:��media��ȫ������֡����д��SQLite \n\
21:�����ͼͼ�� \n\
22:���ɵ�ͼ�赲�ļ� \n\
23:�������е�ͼ�赲�ļ� \n\
24:���³������͵��б� \n\
25:���³���NPC�б� \n\
26:���³���ˢ�������б� \n\
27:���³�����ȫ���б� \n\
28:����ͼƬ��Ӱ\n\
0:�˳�" << endl;
		int input;
		cin >> input;
		if (input == 0)
		{
			break;
		}
		else if (input == 1)
		{
			cout << "�����ļ���:";
			string fileName;
			cin >> fileName;
			cout << "��ʼ����..." << endl;
			long startTime = timeGetTime();
			string filePath = "../media/" + fileName;
			string outputPath = "../media/" + fileName + "/";
			ImageUtility::wixWilToPNG(filePath + ".wix", filePath + ".wil", outputPath);
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 2)
		{
			cout << "�����ļ�����:";
			string fileName;
			cin >> fileName;
			cout << "��ʼ�����ɫ�ļ��ṹ..." << endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupHumanImage("../media/" + fileName);
			cout << "�ļ��������,�����ȸ�ÿ������ֶ��������ٽ�֡����д��SQLite" << endl;
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 3)
		{
			cout << "�����ļ�����:";
			string fileName;
			cin >> fileName;
			cout << "��ʼ���������ļ��ṹ..." << endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupWeaponImage("../media/" + fileName);
			cout << "�ļ��������,�����ȸ�ÿ�������ֶ��������ٽ�֡����д��SQLite" << endl;
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 4)
		{
			cout << "�����ļ���:";
			string fileName;
			cin >> fileName;
			cout << "��ʼ��������ļ��ṹ0..." << endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupMonsterImage0("../media/" + fileName);
			cout << "���ֶ�Ϊÿһ��������ж�������,Ȼ��ѡ����������ļ��ṹ1" << endl;
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 5)
		{
			cout << "�����ļ���:";
			string fileName;
			cin >> fileName;
			cout << "��ʼ��������ļ��ṹ1..." << endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupMonsterImage1("../media/" + fileName);
			cout << "�ļ��������,�����ȸ�ÿ�������ֶ��������ٽ�֡����д��SQLite" << endl;
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 6)
		{
			cout << "�����ļ�����:";
			string fileName;
			cin >> fileName;
			cout << "��ʼ������Ч�ļ��ṹ..." << endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupEffectImage("../media/" + fileName);
			cout << "�ļ��������,�����ȸ�ÿ������ֶ��������ٽ�֡����д��SQLite" << endl;
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 7)
		{
			cout << "�����ļ�����:";
			string fileName;
			cin >> fileName;
			cout << "���뷽������:";
			int directionCount = 1;
			cin >> directionCount;
			cout << "��ʼ�Զ����㷽��..." << endl;
			long startTime = timeGetTime();
			ImageUtility::renameByDirection("../media/" + fileName, directionCount, false);
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 8)
		{
			cout << "�����ļ�����:";
			string fileName;
			cin >> fileName;
			cout << "��ʼ���λ���ļ�..." << endl;
			long startTime = timeGetTime();
			ImageUtility::splitPositionFile("../media/" + fileName);
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 9)
		{
			cout << "�����ļ�����:";
			string fileName;
			cin >> fileName;
			cout << "��ʼɾ����Ч�ļ�..." << endl;
			long startTime = timeGetTime();
			ImageUtility::deleteInvalidImage("../media/" + fileName);
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 10)
		{
			cout << "�����ļ�����:";
			string fileName;
			cin >> fileName;
			cout << "��ʼ������������ļ�..." << endl;
			long startTime = timeGetTime();
			ImageUtility::renameImage("../media/" + fileName);
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 11)
		{
			cout << "�����ļ�����:";
			string fileName;
			cin >> fileName;
			cout << "��ʼ������Ϊ����֡��ʽ..." << endl;
			long startTime = timeGetTime();
			ImageUtility::renameImageToAnim("../media/" + fileName);
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 13)
		{
			cout << "�����ļ�����:";
			string fileName;
			cin >> fileName;
			cout << "��ʼ����ͼ��..." << endl;
			long startTime = timeGetTime();
			ImageUtility::groupAtlas("../media/" + fileName, 100);
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 14)
		{
			cout << "����media�����·��:";
			string fileName;
			cin >> fileName;
			cout << "��ʼ���ͼ��..." << endl;
			long startTime = timeGetTime();
			ImageUtility::texturePacker("../media/" + fileName);
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 15)
		{
			cout << "����media�����·��:";
			string fileName;
			cin >> fileName;
			cout << "��ʼ���ȫ��ͼ��..." << endl;
			long startTime = timeGetTime();
			ImageUtility::texturePackerAll("../media/" + fileName);
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 18)
		{
			cout << "��ʼ��������wil..." << endl;
			long startTime = timeGetTime();
			ImageUtility::allWixWilToPNG("../media");
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 19)
		{
			cout << "�����ļ�����:";
			string fileName;
			cin >> fileName;
			cout << "��ʼ����NPC�ļ��ṹ..." << endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupNPCImage("../media/" + fileName);
			cout << "�ļ��������" << endl;
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 20)
		{
			cout << "��ʼ����������֡����д��SQLite..." << endl;
			char updateOnly = 'y';
			cout << "�Ƿ�ֻ��������? y(��)/n(��)";
			cin >> updateOnly;
			long startTime = timeGetTime();
			if (updateOnly == 'y' || updateOnly == 'n')
			{
				ImageUtility::writeAnimFrameSQLite(updateOnly == 'y');
			}
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 21)
		{
			cout << "����media�����·��:";
			string fileName;
			cin >> fileName;
			cout << "��ʼ�����ͼͼ��..." << endl;
			long startTime = timeGetTime();
			ImageUtility::packMapTextureAll("../media/" + fileName);
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 22)
		{
			cout << "����media�����·��:";
			string fileName;
			cin >> fileName;
			cout << "���������赲�ļ�..." << endl;
			long startTime = timeGetTime();
			ImageUtility::generateUnreachFile("../media/" + fileName);
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 23)
		{
			cout << "���������赲�ļ�..." << endl;
			long startTime = timeGetTime();
			ImageUtility::generateAllUnreachFile("../media");
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 24)
		{
			cout << "���ڸ��³������͵��б�..." << endl;
			long startTime = timeGetTime();
			ImageUtility::updateSceneMapTransferSQLite();
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 25)
		{
			cout << "���ڸ��³���NPC�б�..." << endl;
			long startTime = timeGetTime();
			ImageUtility::updateSceneMapNPCSQLite();
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 26)
		{
			cout << "���ڸ��³���ˢ�������б�..." << endl;
			long startTime = timeGetTime();
			ImageUtility::updateSceneMapMonsterRegionSQLite();
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 27)
		{
		cout << "���ڸ��³�����ȫ���б�..." << endl;
		long startTime = timeGetTime();
		ImageUtility::updateSceneMapPeaceAreaSQLite();
		cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 28)
		{
			cout << "���ڴ���ͼƬ��Ӱ..." << endl;
			long startTime = timeGetTime();
			ImageUtility::processAllShadow("../media/");
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		system("pause");
		cout << endl;
	}
	return;
}