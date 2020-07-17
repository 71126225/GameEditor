#include "Utility.h"
#include "CodeGenerator.h"

void main()
{
	cout << "1.��������ͨ��Э�����" << endl;
	cout << "2.����SQLite���ݽṹ����" << endl;
	int input;
	cin >> input;
	if (input == 1)
	{
		// ������Ϣ����
		string cppHeaderPath = "E:/Github/MicroLegend_Server/MicroLegend_Server/Game/Socket/PacketHeader";
		string cppPacketDefinePath = "E:/Github/MicroLegend_Server/MicroLegend_Server/Game/Socket";
		string csHeaderPath = "E:/Github/MicroLegend/Assets/Scripts/Game/Socket/PacketHeader";
		string csPacketDefinePath = "E:/Github/MicroLegend/Assets/Scripts/Game/Socket";
		CodeGenerator::generatePacketCode(cppHeaderPath, csHeaderPath, cppPacketDefinePath, csPacketDefinePath);
	}
	else if (input == 2)
	{
		string cppDataPath = "E:/Github/MicroLegend_Server/MicroLegend_Server/Game/DataBase/SQLite/Data";
		string csDataPath = "E:/Github/MicroLegend/Assets/Scripts/Game/DataBase/SQLite/SQLiteData";
		// SQLite���ݽṹ����
		CodeGenerator::generateSQLiteCode(cppDataPath, csDataPath);
	}
	system("pause");
}