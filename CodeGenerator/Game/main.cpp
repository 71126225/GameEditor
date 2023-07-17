#include "Utility.h"
#include "CodeState.h"
#include "CodeMySQL.h"
#include "CodeSQLite.h"
#include "CodeNetPacket.h"
#include "CodeUnityBuild.h"
#include "CodeFrameSystem.h"
#include "CodeClassDeclare.h"
#include "CodeComponent.h"

void main()
{
	if (!CodeUtility::initPath())
	{
		system("pause");
		return;
	}

	cout << "0.ȫ������(��������ͨ��Э�����)" << endl;
	cout << "1.ȫ������(��������ͨ��Э�����)" << endl;
	int input;
	cin >> input;
	if (input == 0)
	{
		CodeSQLite::generate();
		CodeMySQL::generate();
		CodeState::generate();
		CodeUnityBuild::generate();
		CodeFrameSystem::generate();
		CodeClassDeclare::generate();
		CodeComponent::generate();
	}
	else if (input == 1)
	{
		CodeNetPacket::generate();
		CodeSQLite::generate();
		CodeMySQL::generate();
		CodeState::generate();
		CodeUnityBuild::generate();
		CodeFrameSystem::generate();
		CodeClassDeclare::generate();
		CodeComponent::generate();
	}
	system("pause");
}