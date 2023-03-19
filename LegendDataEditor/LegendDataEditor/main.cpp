#include "ImageUtility.h"
#include "SQLiteData.h"

void main()
{
	// �������ߺ����ĳ�ʼ����Ҫ���ȵ���
	MathUtility::initMathUtility();
	StringUtility::initStringUtility();
	SQLiteData::initFunctionList();

	while (true)
	{
		cout << "ѡ����" << endl;
		cout << "2:�Զ������ɫ,����,���ͼƬ��" << endl;
		cout << "4:��������ļ��ṹ0" << endl;
		cout << "5:������������ļ����Զ����㷽��" << endl;
		cout << "6:������Ч�ļ��ṹ" << endl;
		cout << "7:�Զ����㷽��" << endl;
		cout << "8:���λ���ļ�" << endl;
		cout << "9:ɾ����ЧͼƬ" << endl;
		cout << "10:������������ļ�,������ֺ�׺" << endl;
		cout << "11:������Ϊ����֡��ʽ" << endl;
		cout << "14:���ȫ��ͼ��" << endl;
		cout << "15:��������wix��wil�ļ�" << endl;
		cout << "16:����NPC�ļ��ṹ" << endl;
		cout << "17:��media��ȫ������֡����д��SQLite" << endl;
		cout << "19:��ͼ:�������е�ͼ�赲�ļ�" << endl;
		cout << "20:����ͼƬ��Ӱ" << endl;
		cout << "22:���µ�ͼ�е���Ч��Ϣ��SQLite" << endl;
		cout << "25:�������ɰ���ƫ������ͼƬ" << endl;
		cout << "26:�ü�����֡ͼƬ����С�ߴ�" << endl;
		cout << "27:��ͼ�������ͳһ��36*36" << endl;
		cout << "29:��ͼ:ת�����е�ͼ�ļ�Ϊ�򻯰汾" << endl;
		cout << "32:��ͼƬ�������Ϊԭ����һ��" << endl;
		cout << "33:��ͼƬ�����пհ�ȥ��,������ƫ����" << endl;
		cout << "34:������е�ͼ�Ƿ�����Ч��ש�±�" << endl;
		cout << "35:����ɫ����д��Animation���" << endl;
		cout << "36:��ͼ:�����ͼ�����ı�" << endl;
		cout << "37:ͼƬ���ֵ����ͼ��" << endl;
		cout << "38:������Ŀ¼��ͼƬ���ֵ����ͼ��" << endl;
		cout << "39:��ͼ:���ɵ�ͼ��שͼƬ��Ϣ�ļ�" << endl;
		cout << "0:�˳�" << endl;
		int input;
		cin >> input;
		if (input == 0)
		{
			break;
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
			cout << "��ʼ������������ļ����Զ����㷽��..." << endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupMonsterImage1("../media/" + fileName);
			cout << "�ļ��������,�������ֶ�������,��ȷ��ÿ���ļ����е�ͼƬ����󲻻ᳬ��ͼ����С,�ٷ���ָ����Ŀ¼��,Ȼ���ٽ�֡����д��SQLite" << endl;
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
			ImageUtility::renameImageToNumber("../media/" + fileName);
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
		else if (input == 14)
		{
			cout << "����media�����·��:";
			string fileName;
			cin >> fileName;
			cout << "��ʼ���ȫ��ͼ��..." << endl;
			long startTime = timeGetTime();
			ImageUtility::texturePackerAll("../media/" + fileName);
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 15)
		{
			cout << "��ʼ��������wil..." << endl;
			long startTime = timeGetTime();
			ImageUtility::allWixWilToPNG("../media");
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 16)
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
		else if (input == 17)
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
		else if (input == 19)
		{
			cout << "���������赲�ļ�,ֻɨ��򻯰汾�ĵ�ͼ�ļ�..." << endl;
			long startTime = timeGetTime();
			ImageUtility::generateAllUnreachFile("../media");
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 20)
		{
			cout << "���ڴ���ͼƬ��Ӱ..." << endl;
			long startTime = timeGetTime();
			ImageUtility::processAllShadow("../media/");
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 22)
		{
			cout << "���ڸ��µ�ͼ�е���Ч��Ϣ��SQLite..." << endl;
			long startTime = timeGetTime();
			ImageUtility::updateMapEffect();
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 25)
		{
			cout << "�����ļ�����:";
			string fileName;
			cin >> fileName;
			cout << "��ʼ����..." << endl;
			long startTime = timeGetTime();
			ImageUtility::generateAllOffsetedImage("../media/" + fileName);
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 26)
		{
			cout << "�����ļ�����:";
			string fileName;
			cin >> fileName;
			cout << "�������͸����:";
			int minAlpha;
			cin >> minAlpha;
			cout << "��ʼ�ü�..." << endl;
			long startTime = timeGetTime();
			ImageUtility::trimAllImage("../media/" + fileName, minAlpha);
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 27)
		{
			cout << "�����ļ�����:";
			string fileName;
			cin >> fileName;
			cout << "��ʼ����ͼ���С..." << endl;
			long startTime = timeGetTime();
			ImageUtility::generateAllIconTo36("../media/" + fileName);
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 29)
		{
			cout << "��ʼת����ͼ..." << endl;
			long startTime = timeGetTime();
			ImageUtility::convertMapFile("../media");
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 32)
		{
			cout << "�����ļ�����:";
			string fileName;
			cin >> fileName;
			ImageUtility::scaleTexture("../media/" + fileName, 0.5f);
		}
		else if (input == 33)
		{
			cout << "�����ļ�����:";
			string fileName;
			cin >> fileName;
			ImageUtility::fullImageToMinimal("../media/" + fileName);
		}
		else if (input == 34)
		{
			cout << "�����ļ�����:";
			string fileName;
			cin >> fileName;
			ImageUtility::checkMapTile("../media/" + fileName);
		}
		else if (input == 35)
		{
			cout << "�����ɫ��������:";
			string clothName;
			cin >> clothName;
			cout << "����ImagePositionAnimation�е���ʼID:";
			int startID;
			cin >> startID;
			ImageUtility::autoFillAnimationTable(clothName, startID);
		}
		else if (input == 36)
		{
			cout << "�����ͼ�ļ���,��·��:";
			string fileName;
			cin >> fileName;
			ImageUtility::dumpMapFile("../media/" + fileName);
		}
		else if (input == 37)
		{
			cout << "�����ļ�����:";
			string fileName;
			cin >> fileName;
			ImageUtility::makeAtlasGroup("../media/" + fileName);
		}
		else if (input == 38)
		{
			const long startTime = timeGetTime();
			ImageUtility::makeAtlasGroupAll();
			cout << "��ʱ : " << (timeGetTime() - startTime) / 1000.0f << "��" << endl;
		}
		else if (input == 39)
		{
			cout << "�����ͼ�ļ���:";
			string fileName;
			cin >> fileName;
			ImageUtility::generateAtlasInfoFile("../media/" + fileName);
		}
		system("pause");
		cout << endl;
	}
	return;
}