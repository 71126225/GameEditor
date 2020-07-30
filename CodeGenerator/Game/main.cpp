#include "Utility.h"
#include "CodeGenerator.h"

void main()
{
	string cppGamePath = "E:/Github/MicroLegend_Server/MicroLegend_Server/Game/";
	string csGamePath = "E:/Github/MicroLegend/Assets/Scripts/Game/";
	string cppStringDefinePath = cppGamePath + "StringDefine";
	cout << "1.��������ͨ��Э�����" << endl;
	cout << "2.����SQLite���ݽṹ����" << endl;
	cout << "3.����MySQL���ݽṹ����" << endl;
	cout << "4.��������ṹ����" << endl;
	cout << "5.���ɼ��ܽṹ����" << endl;
	cout << "6.����״̬�ṹ����" << endl;
	int input;
	cin >> input;
	if (input == 1)
	{
		string cppHeaderPath = cppGamePath + "Socket/PacketHeader";
		string cppPacketDefinePath = cppGamePath + "Socket";
		string csHeaderPath = csGamePath + "Socket/PacketHeader";
		string csPacketDefinePath = csGamePath + "Socket";
		CodeGenerator::generatePacketCode(cppHeaderPath, csHeaderPath, cppPacketDefinePath, csPacketDefinePath, cppStringDefinePath);
	}
	else if (input == 2)
	{
		string cppDataPath = cppGamePath + "DataBase/SQLite/Data";
		string cppTablePath = cppGamePath + "DataBase/SQLite/Table";
		string csDataPath = csGamePath + "DataBase/SQLite/Data";
		string csTablePath = csGamePath + "DataBase/SQLite/Table";
		CodeGenerator::generateSQLiteCode(cppDataPath, cppTablePath, csDataPath, csTablePath);
	}
	else if (input == 3)
	{
		string cppDataPath = cppGamePath + "DataBase/MySQL/Data";
		CodeGenerator::generateMySQLCode(cppDataPath, cppStringDefinePath);
	}
	else if (input == 4)
	{
		string cppHeaderPath = cppGamePath + "CommandSystem";
		CodeGenerator::generateCmdCode(cppStringDefinePath, cppHeaderPath);
	}
	else if (input == 5)
	{
		string cppHeaderPath = cppGamePath + "Character/Skill";
		CodeGenerator::generateSkillCode(cppStringDefinePath, cppHeaderPath);
	}
	else if (input == 6)
	{
		string cppHeaderPath = cppGamePath + "StateMachine";
		CodeGenerator::generateStateCode(cppStringDefinePath, cppHeaderPath);
	}
	system("pause");
}