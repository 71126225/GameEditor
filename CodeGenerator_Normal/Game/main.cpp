#include "Utility.h"
#include "CodeNetPacket.h"
#include "CodeUnityBuild.h"
#include "CodeClassDeclareAndHeader.h"
#include "CodeFrameSystem.h"

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
		CodeFrameSystem::generate();
		CodeClassDeclareAndHeader::generate();
		CodeUnityBuild::generate();
	}
	else if (input == 1)
	{
		CodeNetPacket::generate();
		CodeFrameSystem::generate();
		CodeClassDeclareAndHeader::generate();
		CodeUnityBuild::generate();
	}
	system("pause");
}