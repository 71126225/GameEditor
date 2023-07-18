#include "Utility.h"
#include "CodeNetPacket.h"
#include "CodeUnityBuild.h"
#include "CodeClassDeclareAndHeader.h"

void main()
{
	if (!CodeUtility::initPath())
	{
		system("pause");
		return;
	}

	cout << "0.ȫ������(��������ͨ��Э�����)" << endl;
	cout << "1.ȫ������(��������ͨ��Э�����)" << endl;
	cout << "2.����������Ϣ����" << endl;
	cout << "3.����UnityBuild����" << endl;
	cout << "4.������������ͷ�ļ�����" << endl;
	int input;
	cin >> input;
	if (input == 0)
	{
		CodeUnityBuild::generate();
		CodeClassDeclareAndHeader::generate();
	}
	else if (input == 1)
	{
		CodeNetPacket::generate();
		CodeUnityBuild::generate();
		CodeClassDeclareAndHeader::generate();
	}
	else if (input == 2)
	{
		CodeNetPacket::generate();
	}
	else if (input == 3)
	{
		CodeUnityBuild::generate();
	}
	else if (input == 4)
	{
		CodeClassDeclareAndHeader::generate();
	}
	system("pause");
}