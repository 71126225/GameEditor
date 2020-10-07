#include "CodeSQLite.h"

void CodeSQLite::generateSQLiteCode(string cppDataPath, string cppTablePath, string csDataPath, string csTablePath)
{
	// ����ģ���ļ�
	string fileContent;
	openTxtFile("SQLite.txt", fileContent);
	if (fileContent.length() == 0)
	{
		ERROR("δ�ҵ�����ʽ�ļ�SQLite.txt");
		return;
	}
	fileContent = UTF8ToANSI(fileContent.c_str(), true);
	myVector<string> lines;
	split(fileContent.c_str(), "\r\n", lines);
	bool packetStart = false;
	myVector<SQLiteInfo> sqliteInfoList;
	SQLiteInfo tempInfo;
	FOR_VECTOR_CONST(lines)
	{
		string line = lines[i];
		// ����ע��
		if (startWith(line, "//"))
		{
			continue;
		}
		// ����������ע��,��ȥ��
		int pos = -1;
		if (findString(line.c_str(), "//", &pos))
		{
			line = line.substr(0, pos);
		}
		// ȥ�������Ʊ��
		strReplaceAll(line, "\t", "");
		// ȥ�����еķֺ�
		strReplaceAll(line, ";", "");
		// ��Ա�����б���ʼ
		if (line == "{")
		{
			packetStart = true;
			string lastLine = lines[i - 1];
			int startIndex = -1;
			int endIndex = -1;
			findString(lastLine.c_str(), "[", &startIndex);
			findString(lastLine.c_str(), "]", &endIndex, startIndex);
			if (startIndex >= 0 && endIndex >= 0)
			{
				tempInfo.mSQLiteName = lastLine.substr(0, startIndex);
				string owner = lastLine.substr(startIndex, endIndex - startIndex + 1);
				if (owner == "[Client]")
				{
					tempInfo.mOwner = SQLITE_OWNER::CLIENT_ONLY;
				}
				else if (owner == "[Server]")
				{
					tempInfo.mOwner = SQLITE_OWNER::SERVER_ONLY;
				}
				else
				{
					tempInfo.mOwner = SQLITE_OWNER::BOTH;
				}
			}
			else
			{
				tempInfo.mSQLiteName = lastLine;
				tempInfo.mOwner = SQLITE_OWNER::BOTH;
			}
			tempInfo.mMemberList.clear();
			// ���Ĭ�ϵ�ID�ֶ�
			SQLiteMember idMember;
			idMember.mMemberName = "ID";
			idMember.mOwner = SQLITE_OWNER::BOTH;
			idMember.mTypeName = "int";
			tempInfo.mMemberList.push_back(idMember);
			continue;
		}
		// ��Ա�����б����
		if (line == "}")
		{
			sqliteInfoList.push_back(tempInfo);
			packetStart = false;
			continue;
		}
		if (packetStart)
		{
			tempInfo.mMemberList.push_back(parseSQLiteMemberLine(line));
		}
	}
	// ɾ��C++�Ĵ����ļ�
	deleteFolder(cppDataPath);
	deleteFolder(cppTablePath);
	// ɾ��C#�Ĵ����ļ�,c#��ֻɾ�������ļ�,��ɾ��meta�ļ�
	myVector<string> csDataFileList;
	findFiles(csDataPath, csDataFileList, ".cs");
	FOR_VECTOR_CONST(csDataFileList)
	{
		deleteFile(csDataFileList[i]);
	}
	myVector<string> csTableFileList;
	findFiles(csTablePath, csTableFileList, ".cs");
	FOR_VECTOR_CONST(csTableFileList)
	{
		deleteFile(csTableFileList[i]);
	}

	// ���ɴ����ļ�
	FOR_VECTOR_CONST(sqliteInfoList)
	{
		// .h����
		generateCppSQLiteDataFile(sqliteInfoList[i], cppDataPath, cppTablePath);
		// .cs����
		generateCSharpSQLiteDataFile(sqliteInfoList[i], csDataPath, csTablePath);
	}

	// ����һ��Ŀ¼����SQLiteHeader.h�ļ�
	string headerPath = cppDataPath;
	if (endWith(headerPath, "/") || endWith(headerPath, "\\"))
	{
		headerPath = headerPath.substr(0, headerPath.length() - 1);
	}
	headerPath = getFilePath(headerPath);
	generateCppSQLiteTotalHeaderFile(sqliteInfoList, headerPath);
	generateCppSQLiteRegisteFile(sqliteInfoList, headerPath);
	generateCppSQLiteClassDeclare(sqliteInfoList, headerPath);
	generateCppSQLiteInstanceDeclare(sqliteInfoList, headerPath);

	// ����һ��Ŀ¼����SQLiteRegister.cs�ļ�
	string registerPath = csDataPath;
	if (registerPath[registerPath.length() - 1] == '/' || registerPath[registerPath.length() - 1] == '\\')
	{
		registerPath = registerPath.substr(0, registerPath.length() - 1);
	}
	registerPath = getFilePath(registerPath);
	generateCSharpSQLiteRegisteFileFile(sqliteInfoList, registerPath);
}

// TDSQLite.h��TDSQLite.cpp,SQLiteTable.h�ļ�
void CodeSQLite::generateCppSQLiteDataFile(const SQLiteInfo& sqliteInfo, string dataFilePath, string tableFilePath)
{
	if (sqliteInfo.mOwner == SQLITE_OWNER::CLIENT_ONLY)
	{
		return;
	}
	// TDSQLite.h
	string header;
	string dataClassName = "TD" + sqliteInfo.mSQLiteName;
	string headerMacro = "_TD" + nameToUpper(sqliteInfo.mSQLiteName) + "_H_";
	line(header, "#ifndef " + headerMacro);
	line(header, "#define " + headerMacro);
	line(header, "");
	line(header, "#include \"SQLiteData.h\"");
	line(header, "");
	line(header, "class " + dataClassName + " : public SQLiteData");
	line(header, "{");
	line(header, "public:");
	uint memberCount = sqliteInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		if (sqliteInfo.mMemberList[i].mOwner == SQLITE_OWNER::CLIENT_ONLY)
		{
			line(header, "\tCOL_EMPTY(" + sqliteInfo.mMemberList[i].mTypeName + ", " + sqliteInfo.mMemberList[i].mMemberName + ");");
		}
		else
		{
			line(header, "\tCOL(" + sqliteInfo.mMemberList[i].mTypeName + ", " + sqliteInfo.mMemberList[i].mMemberName + ");");
		}
	}
	line(header, "public:");
	line(header, "\t" + dataClassName + "()");
	line(header, "\t{");
	FOR_I(memberCount)
	{
		if (sqliteInfo.mMemberList[i].mOwner == SQLITE_OWNER::CLIENT_ONLY)
		{
			line(header, "\t\tREGISTE_PARAM_EMPTY(" + sqliteInfo.mMemberList[i].mMemberName + ");");
		}
		else
		{
			line(header, "\t\tREGISTE_PARAM(" + sqliteInfo.mMemberList[i].mMemberName + ");");
		}
	}
	line(header, "\t}");
	line(header, "};");
	line(header, "");
	line(header, "#endif", false);

	// TDSQLite.cpp
	string source;
	line(source, "#include \"" + dataClassName + ".h\"");
	line(source, "");
	FOR_I(memberCount)
	{
		line(source, "COL_DEFINE(" + dataClassName + ", " + sqliteInfo.mMemberList[i].mMemberName + ");");
	}
	validPath(dataFilePath);
	header = ANSIToUTF8(header.c_str(), true);
	source = ANSIToUTF8(source.c_str(), true);
	writeFile(dataFilePath + dataClassName + ".h", header);
	writeFile(dataFilePath + dataClassName + ".cpp", source);

	// SQLiteTable.h
	string table;
	string tableHeaderMarco = "_SQLITE" + nameToUpper(sqliteInfo.mSQLiteName) + "_H_";
	line(table, "#ifndef " + tableHeaderMarco);
	line(table, "#define " + tableHeaderMarco);
	line(table, "");
	line(table, "#include \"" + dataClassName + ".h\"");
	line(table, "");
	string tableClassName = "SQLite" + sqliteInfo.mSQLiteName;
	line(table, "class " + tableClassName + " : public SQLiteTable<" + dataClassName + ">");
	line(table, "{");
	line(table, "public:");
	line(table, "\t" + tableClassName + "(const char* tableName, ISQLite* sqlite)");
	line(table, "\t\t:SQLiteTable(tableName, sqlite) {}");
	line(table, "};");
	line(table, "");
	line(table, "#endif", false);
	validPath(tableFilePath);
	table = ANSIToUTF8(table.c_str(), true);
	writeFile(tableFilePath + tableClassName + ".h", table);
}

// SQLiteHeader.h�ļ�
void CodeSQLite::generateCppSQLiteTotalHeaderFile(const myVector<SQLiteInfo>& sqliteList, string filePath)
{
	string str0;
	line(str0, "#ifndef _SQLITE_HEADER_H_");
	line(str0, "#define _SQLITE_HEADER_H_");
	line(str0, "");
	line(str0, "#include \"SQLite.h\"");
	uint packetCount = sqliteList.size();
	FOR_I(packetCount)
	{
		if (sqliteList[i].mOwner == SQLITE_OWNER::CLIENT_ONLY)
		{
			continue;
		}
		line(str0, "#include \"SQLite" + sqliteList[i].mSQLiteName + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif", false);
	validPath(filePath);
	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "SQLiteHeader.h", str0);
}

// SQLiteRegister.h��SQLiteRegister.cpp�ļ�
void CodeSQLite::generateCppSQLiteRegisteFile(const myVector<SQLiteInfo>& sqliteList, string filePath)
{
	// SQLiteRegister.h
	string str0;
	line(str0, "#ifndef _SQLITE_REGISTER_H_");
	line(str0, "#define _SQLITE_REGISTER_H_");
	line(str0, "");
	line(str0, "#include \"GameBase.h\"");
	line(str0, "");
	line(str0, "class SQLiteRegister : public GameBase");
	line(str0, "{");
	line(str0, "public:");
	line(str0, "\tstatic void registeAll();");
	line(str0, "};");
	line(str0, "");
	line(str0, "#endif", false);
	validPath(filePath);
	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "SQLiteRegister.h", str0);

	string str1;
	line(str1, "#include \"SQLiteRegister.h\"");
	line(str1, "#include \"SQLiteManager.h\"");
	line(str1, "#include \"GameDefine.h\"");
	line(str1, "#include \"SQLiteHeader.h\"");
	line(str1, "");
	line(str1, "#define REGISTE_SQLITE(classType, tableName) m##classType = NEW(classType, m##classType, tableName, sqlite);sqlite->addTable(m##classType);");
	line(str1, "");
	line(str1, "void SQLiteRegister::registeAll()");
	line(str1, "{");
	line(str1, "\tSQLite* sqlite = mSQLiteManager->createSQLite(GameDefine::SQLITE_DATA_BASE);");
	uint count = sqliteList.size();
	FOR_I(count)
	{
		if (sqliteList[i].mOwner == SQLITE_OWNER::CLIENT_ONLY)
		{
			continue;
		}
		line(str1, "\tREGISTE_SQLITE(SQLite" + sqliteList[i].mSQLiteName + ", \"" + sqliteList[i].mSQLiteName + "\");");
	}
	line(str1, "}", false);
	str1 = ANSIToUTF8(str1.c_str(), true);
	writeFile(filePath + "SQLiteRegister.cpp", str1);
}

// SQLiteClassDeclare.h
void CodeSQLite::generateCppSQLiteClassDeclare(const myVector<SQLiteInfo>& sqliteList, string filePath)
{
	validPath(filePath);
	string str1;
	uint count = sqliteList.size();
	FOR_I(count)
	{
		if (sqliteList[i].mOwner == SQLITE_OWNER::CLIENT_ONLY)
		{
			continue;
		}
		line(str1, "class SQLite" + sqliteList[i].mSQLiteName + ";");
	}
	str1 = ANSIToUTF8(str1.c_str(), true);
	writeFile(filePath + "SQLiteClassDeclare.h", str1);
}

// SQLiteInstanceDeclare.h��SQLiteInstanceDeclare.cpp
void CodeSQLite::generateCppSQLiteInstanceDeclare(const myVector<SQLiteInfo>& sqliteList, string filePath)
{
	validPath(filePath);
	string str0;
	line(str0, "// auto generated file, so it looks might be strange");
	line(str0, "");
	uint count = sqliteList.size();
	FOR_I(count)
	{
		if (sqliteList[i].mOwner == SQLITE_OWNER::CLIENT_ONLY)
		{
			continue;
		}
		line(str0, "static SQLite" + sqliteList[i].mSQLiteName + "* mSQLite" + sqliteList[i].mSQLiteName + ";");
	}
	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "SQLiteInstanceDeclare.h", str0);

	string str1;
	line(str1, "// auto generated file, so it looks might be strange");
	line(str1, "");
	line(str1, "#include \"GameBase.h\"");
	line(str1, "");
	FOR_I(count)
	{
		if (sqliteList[i].mOwner == SQLITE_OWNER::CLIENT_ONLY)
		{
			continue;
		}
		line(str1, "SQLite" + sqliteList[i].mSQLiteName + "* GameBase::mSQLite" + sqliteList[i].mSQLiteName + ";");
	}
	str1 = ANSIToUTF8(str1.c_str(), true);
	writeFile(filePath + "SQLiteInstanceDeclare.cpp", str1);
}

// TDSQLite.cs��SQLiteTable.cs�ļ�
void CodeSQLite::generateCSharpSQLiteDataFile(const SQLiteInfo& sqliteInfo, string dataFilePath, string tableFilePath)
{
	if (sqliteInfo.mOwner == SQLITE_OWNER::SERVER_ONLY)
	{
		return;
	}
	// TDSQLite.cs�ļ�
	string file;
	string dataClassName = "TD" + sqliteInfo.mSQLiteName;
	line(file, "using Mono.Data.Sqlite;");
	line(file, "using System;");
	line(file, "using System.Collections.Generic;");
	line(file, "using UnityEngine;");
	line(file, "");
	line(file, "public class " + dataClassName + " : SQLiteData");
	line(file, "{");
	uint memberCount = sqliteInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		if (sqliteInfo.mMemberList[i].mMemberName == "ID")
		{
			continue;
		}
		line(file, "\tpublic static string " + sqliteInfo.mMemberList[i].mMemberName + " = \"" + sqliteInfo.mMemberList[i].mMemberName + "\";");
	}
	FOR_I(memberCount)
	{
		if (sqliteInfo.mMemberList[i].mMemberName == "ID")
		{
			continue;
		}
		string typeName = sqliteInfo.mMemberList[i].mTypeName;
		// ��myVector�滻ΪList,Vector2UShort�滻ΪVector2Int
		if (startWith(typeName, "myVector"))
		{
			strReplaceAll(typeName, "myVector", "List");
		}
		else if (typeName == "Vector2UShort")
		{
			typeName = "Vector2Int";
		}
		if (findString(typeName.c_str(), "List", NULL))
		{
			line(file, "\tpublic " + typeName + " m" + sqliteInfo.mMemberList[i].mMemberName + " = new " + typeName + "();");
		}
		else
		{
			line(file, "\tpublic " + typeName + " m" + sqliteInfo.mMemberList[i].mMemberName + ";");
		}
	}
	line(file, "\tpublic override void parse(SqliteDataReader reader)");
	line(file, "\t{");
	line(file, "\t\tbase.parse(reader);");
	FOR_I(memberCount)
	{
		if (sqliteInfo.mMemberList[i].mMemberName == "ID")
		{
			continue;
		}
		line(file, "\t\tparseParam(reader, ref m" + sqliteInfo.mMemberList[i].mMemberName + ", " + sqliteInfo.mMemberList[i].mMemberName + ");");
	}
	line(file, "\t}");
	line(file, "\tpublic static void link(SQLiteTable table)");
	line(file, "\t{");
	FOR_I(memberCount)
	{
		if (sqliteInfo.mMemberList[i].mLinkTable.length() > 0)
		{
			line(file, "\t\ttable.link(" + sqliteInfo.mMemberList[i].mMemberName + ", mSQLite" + sqliteInfo.mMemberList[i].mLinkTable + ");");
		}
	}
	line(file, "\t}");
	line(file, "}", false);
	validPath(dataFilePath);
	file = ANSIToUTF8(file.c_str(), true);
	writeFile(dataFilePath + dataClassName + ".cs", file);

	// SQLiteTable.cs�ļ�
	string table;
	line(table, "using System;");
	line(table, "using System.Collections.Generic;");
	line(table, "");
	string tableClassName = "SQLite" + sqliteInfo.mSQLiteName;
	line(table, "public partial class " + tableClassName + " : SQLiteTable");
	line(table, "{");
	line(table, "\tpublic override void linkTable()");
	line(table, "\t{");
	line(table, "\t\t// ֮���Դ˴����ǵ���TableData�ĺ���,��Ϊ��ʹ���ӱ��Ĵ���Ҳ�����ṹ����һ���Զ�����");
	line(table, "\t\t" + dataClassName + ".link(this);");
	line(table, "\t}");
	line(table, "}", false);
	validPath(tableFilePath);
	table = ANSIToUTF8(table.c_str(), true);
	writeFile(tableFilePath + tableClassName + ".cs", table);
}

// SQLiteRegister.cs�ļ�
void CodeSQLite::generateCSharpSQLiteRegisteFileFile(const myVector<SQLiteInfo>& sqliteInfo, string filePath)
{
	string file;
	line(file, "using System;");
	line(file, "using System.Collections;");
	line(file, "using System.Collections.Generic;");
	line(file, "");
	line(file, "public class SQLiteRegister : GameBase");
	line(file, "{");
	line(file, "\tpublic static void registeAll()");
	line(file, "\t{");
	uint count = sqliteInfo.size();
	FOR_I(count)
	{
		if (sqliteInfo[i].mOwner != SQLITE_OWNER::SERVER_ONLY)
		{
			string lineStr = "\t\tregisteTable(out mSQLite%s, Typeof<SQLite%s>(), Typeof<TD%s>(), \"%s\");";
			replaceAll(lineStr, "%s", sqliteInfo[i].mSQLiteName);
			line(file, lineStr);
		}
	}
	line(file, "\t\tmSQLite.linkAllTable();");
	line(file, "\t}");
	line(file, "\t//-------------------------------------------------------------------------------------------------------------");
	line(file, "\tprotected static void registeTable<T>(out T sqliteTable, Type tableType, Type dataType, string tableName) where T : SQLiteTable");
	line(file, "\t{");
	line(file, "\t\tsqliteTable = mSQLite.registeTable(tableType, dataType, tableName) as T;");
	line(file, "\t}");
	line(file, "}", false);
	validPath(filePath);
	file = ANSIToUTF8(file.c_str(), true);
	writeFile(filePath + "SQLiteRegister.cs", file);
}