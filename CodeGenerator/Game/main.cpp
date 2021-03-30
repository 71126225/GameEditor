#include "Utility.h"
#include "CodeCmd.h"
#include "CodeSkill.h"
#include "CodeState.h"
#include "CodeMySQL.h"
#include "CodeSQLite.h"
#include "CodeNetPacket.h"
#include "CodeAchivement.h"
#include "CodeEvent.h"
#include "CodeDTNode.h"
#include "CodeUnityBuild.h"
#include "CodeFrameSystem.h"
#include "CodeClassDeclare.h"
#include "CodeObjectItem.h"
#include "CodeStateGroup.h"
#include "CodeComponent.h"
#include "CodeSceneTrigger.h"

void main()
{
	if (!CodeUtility::initPath())
	{
		cout << "δ�ҵ���������SERVER_PROJECT_PATH��GAME_PROJECT_PATH" << endl;
		system("pause");
		return;
	}

	cout << "0.ȫ������" << endl;
	cout << "1.����ͨ��Э�����" << endl;
	cout << "2.SQLite���ݴ���" << endl;
	cout << "3.MySQL���ݴ���" << endl;
	cout << "4.�������" << endl;
	cout << "5.���ܴ���" << endl;
	cout << "6.״̬����" << endl;
	cout << "7.�ɾʹ���" << endl;
	cout << "8.�¼�����" << endl;
	cout << "9.��Ϊ���ڵ����" << endl;
	cout << "10.UnityBuild�ļ�" << endl;
	cout << "11.FrameSystem�ļ�" << endl;
	cout << "12.�������ļ�" << endl;
	cout << "13.��������������" << endl;
	int input;
	cin >> input;
	if (input == 0)
	{
		CodeNetPacket::generate();
		CodeSQLite::generate();
		CodeMySQL::generate();
		CodeCmd::generate();
		CodeSkill::generate();
		CodeState::generate();
		CodeAchivement::generate();
		CodeEvent::generate();
		CodeDTNode::generate();
		CodeUnityBuild::generate();
		CodeFrameSystem::generate();
		CodeClassDeclare::generate();
		CodeObjectItem::generate();
		CodeStateGroup::generate();
		CodeComponent::generate();
		CodeSceneTrigger::generate();
	}
	else if (input == 1)
	{
		CodeNetPacket::generate();
	}
	else if (input == 2)
	{
		CodeSQLite::generate();
	}
	else if (input == 3)
	{
		CodeMySQL::generate();
	}
	else if (input == 4)
	{
		CodeCmd::generate();
	}
	else if (input == 5)
	{
		CodeSkill::generate();
	}
	else if (input == 6)
	{
		CodeState::generate();
	}
	else if (input == 7)
	{
		CodeAchivement::generate();
	}
	else if (input == 8)
	{
		CodeEvent::generate();
	}
	else if (input == 9)
	{
		CodeDTNode::generate();
	}
	else if (input == 10)
	{
		CodeUnityBuild::generate();
	}
	else if (input == 11)
	{
		CodeFrameSystem::generate();
	}
	else if (input == 12)
	{
		CodeClassDeclare::generate();
	}
	else if (input == 13)
	{
		CodeSceneTrigger::generate();
	}
	system("pause");
}