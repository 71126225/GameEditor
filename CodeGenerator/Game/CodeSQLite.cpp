#include "CodeSQLite.h"

void CodeSQLite::generate()
{
	string cppDataPath = cppGamePath + "DataBase/SQLite/Data/";
	string cppTablePath = cppGamePath + "DataBase/SQLite/Table/";
	string csDataGamePath = csGamePath + "DataBase/SQLite/Data/";
	string csDataHotFixPath = csHotfixGamePath + "DataBase/SQLite/Data/";
	string csTableGamePath = csGamePath + "DataBase/SQLite/Table/";
	string csTableHotFixPath = csHotfixGamePath + "DataBase/SQLite/Table/";
	string csTableDeclareGamePath = csGamePath + "Common/";
	string csTableDeclareHotFixPath = csHotfixGamePath + "Common/";

	// ����ģ���ļ�
	string fileContent;
	openTxtFile("SQLite.txt", fileContent);
	if (fileContent.length() == 0)
	{
		ERROR("δ�ҵ�����ʽ�ļ�SQLite.txt");
		return;
	}
	myVector<string> lines;
	split(fileContent.c_str(), "\r\n", lines);
	if (lines.size() == 0)
	{
		return;
	}
	bool ignoreClientServer = false;
	if (lines[0] == "IgnoreClientServer")
	{
		lines.erase(0);
		ignoreClientServer = true;
	}

	bool packetStart = false;
	myVector<SQLiteInfo> sqliteInfoList;
	SQLiteInfo tempInfo;
	bool fileStart = false;
	FOR_VECTOR_CONST(lines)
	{
		if (lines[i] == START_FALG)
		{
			fileStart = true;
			continue;
		}
		if (!fileStart)
		{
			continue;
		}
		string line = lines[i];
		// ���ע��
		if (startWith(line, "//"))
		{
			string comment = line.substr(strlen("//"));
			removeStartAll(comment, ' ');
			tempInfo.mComment += comment;
			continue;
		}
		// ȥ�������Ʊ��,�ֺ�
		removeAll(line, '\t', ';');
		// ��Ա�����б���ʼ
		if (line == "{")
		{
			packetStart = true;
			string lastLine = lines[i - 1];
			int tagStartIndex = 0;
			int startIndex = -1;
			int endIndex = -1;
			// ���ұ�ǩ
			myVector<string> tagList;
			while (true)
			{
				findString(lastLine.c_str(), "[", &startIndex, tagStartIndex);
				findString(lastLine.c_str(), "]", &endIndex, startIndex);
				if (startIndex < 0 || endIndex < 0)
				{
					break;
				}
				tagList.push_back(lastLine.substr(startIndex, endIndex - startIndex + 1));
				tagStartIndex = endIndex;
			}

			// �жϱ�ǩ
			if (tagList.contains("[Client]"))
			{
				tempInfo.mOwner = SQLITE_OWNER::CLIENT_ONLY;
			}
			else if (tagList.contains("[Server]"))
			{
				tempInfo.mOwner = SQLITE_OWNER::SERVER_ONLY;
			}
			else
			{
				tempInfo.mOwner = SQLITE_OWNER::BOTH;
			}
			tempInfo.mHotFix = tagList.contains("[HotFix]");

			// ��ȡԭʼ�ı������
			int firstTagPos = -1;
			if (findString(lastLine.c_str(), "[", &firstTagPos))
			{
				tempInfo.mSQLiteName = lastLine.substr(0, firstTagPos);
			}
			else
			{
				tempInfo.mSQLiteName = lastLine;
			}
			
			tempInfo.mMemberList.clear();
			// ���Ĭ�ϵ�ID�ֶ�
			SQLiteMember idMember;
			idMember.mMemberName = "ID";
			idMember.mOwner = SQLITE_OWNER::BOTH;
			idMember.mTypeName = "int";
			idMember.mComment = "ΨһID";
			tempInfo.mMemberList.push_back(idMember);
			continue;
		}
		// ��Ա�����б����
		if (line == "}")
		{
			sqliteInfoList.push_back(tempInfo);
			tempInfo.mComment = "";
			packetStart = false;
			continue;
		}
		if (packetStart)
		{
			tempInfo.mMemberList.push_back(parseSQLiteMemberLine(line, ignoreClientServer));
		}
	}
	// ɾ��C++�Ĵ����ļ�
	deleteFolder(cppDataPath);
	// ɾ��C#�Ĵ����ļ�,c#��ֻɾ�������ļ�,��ɾ��meta�ļ�
	myVector<string> csDataFileList;
	findFiles(csDataGamePath, csDataFileList, ".cs");
	findFiles(csDataHotFixPath, csDataFileList, ".cs");
	FOR_VECTOR_CONST(csDataFileList)
	{
		deleteFile(csDataFileList[i]);
	}
	myVector<string> csTableFileList;
	findFiles(csTableGamePath, csTableFileList, ".cs");
	findFiles(csTableHotFixPath, csTableFileList, ".cs");
	FOR_VECTOR_CONST(csTableFileList)
	{
		deleteFile(csTableFileList[i]);
	}

	// ���ɴ����ļ�
	FOR_VECTOR_CONST(sqliteInfoList)
	{
		// .h����
		generateCppSQLiteDataFile(sqliteInfoList[i], cppDataPath);
		generateCppSQLiteTableFile(sqliteInfoList[i], cppTablePath);
		// .cs����
		generateCSharpSQLiteDataFile(sqliteInfoList[i], csDataGamePath, csDataHotFixPath);
		generateCSharpSQLiteTableFile(sqliteInfoList[i], csTableGamePath, csTableHotFixPath);
	}

	// ����һ��Ŀ¼����SQLiteHeader.h�ļ�
	string headerPath = getFilePath(cppDataPath) + "/";
	generateCppSQLiteTotalHeaderFile(sqliteInfoList, headerPath);
	generateCppSQLiteRegisteFile(sqliteInfoList, headerPath);
	generateCppSQLiteInstanceDeclare(sqliteInfoList, headerPath);
	generateCppSQLiteSTLPoolRegister(sqliteInfoList, headerPath);

	// ����һ��Ŀ¼����SQLiteRegister.cs�ļ�
	generateCSharpSQLiteRegisteFileFile(sqliteInfoList, getFilePath(csDataHotFixPath) + "/", getFilePath(csDataGamePath) + "/");

	generateCSharpSQLiteDeclare(sqliteInfoList, csTableDeclareHotFixPath, csTableDeclareGamePath);
}

// TDSQLite.h��TDSQLite.cpp�ļ�
void CodeSQLite::generateCppSQLiteDataFile(const SQLiteInfo& sqliteInfo, string dataFilePath)
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
	line(header, "// " + sqliteInfo.mComment);
	line(header, "class " + dataClassName + " : public SQLiteData");
	line(header, "{");
	line(header, "public:");
	uint memberCount = sqliteInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		line(header, "\tstatic const char* " + sqliteInfo.mMemberList[i].mMemberName + ";");
	}
	line(header, "public:");
	FOR_I(memberCount)
	{
		if (sqliteInfo.mMemberList[i].mOwner != SQLITE_OWNER::CLIENT_ONLY)
		{
			string memberLine = "\t" + sqliteInfo.mMemberList[i].mTypeName + " m" + sqliteInfo.mMemberList[i].mMemberName + ";";
			uint tabCount = generateAlignTableCount(memberLine, 40);
			FOR_I(tabCount)
			{
				memberLine += '\t';
			}
			memberLine += "// " + sqliteInfo.mMemberList[i].mComment;
			line(header, memberLine);
		}
	}
	line(header, "public:");
	line(header, "\t" + dataClassName + "()");
	line(header, "\t{");
	FOR_I(memberCount)
	{
		const string& name = sqliteInfo.mMemberList[i].mMemberName;
		if (sqliteInfo.mMemberList[i].mOwner == SQLITE_OWNER::CLIENT_ONLY)
		{
			
			line(header, "\t\tregisteParamEmpty(" + name + ");");
		}
		else
		{
			line(header, "\t\tregisteParam(m" + name + ", " + name + ");");
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
		line(source, "const char* " + dataClassName + "::" + sqliteInfo.mMemberList[i].mMemberName + " = STR(" + sqliteInfo.mMemberList[i].mMemberName + ");");
	}

	writeFile(dataFilePath + dataClassName + ".h", ANSIToUTF8(header.c_str(), true));
	writeFile(dataFilePath + dataClassName + ".cpp", ANSIToUTF8(source.c_str(), true));
}

// SQLiteTable.h�ļ�
void CodeSQLite::generateCppSQLiteTableFile(const SQLiteInfo& sqliteInfo, string tableFilePath)
{
	if (sqliteInfo.mOwner == SQLITE_OWNER::CLIENT_ONLY)
	{
		return;
	}
	// SQLiteTable.h
	string dataClassName = "TD" + sqliteInfo.mSQLiteName;
	string tableClassName = "SQLite" + sqliteInfo.mSQLiteName;
	string tableFileName = tableFilePath + tableClassName + ".h";
	if (!isFileExist(tableFileName))
	{
		string table;
		string tableHeaderMarco = "_SQLITE" + nameToUpper(sqliteInfo.mSQLiteName) + "_H_";
		line(table, "#ifndef " + tableHeaderMarco);
		line(table, "#define " + tableHeaderMarco);
		line(table, "");
		line(table, "#include \"" + dataClassName + ".h\"");
		line(table, "#include \"SQLiteTable.h\"");
		line(table, "");
		line(table, "class " + tableClassName + " : public SQLiteTable<" + dataClassName + ">");
		line(table, "{");
		line(table, "public:");
		line(table, "};");
		line(table, "");
		line(table, "#endif", false);

		writeFile(tableFileName, ANSIToUTF8(table.c_str(), true));
	}
}

// SQLiteHeader.h�ļ�
void CodeSQLite::generateCppSQLiteTotalHeaderFile(const myVector<SQLiteInfo>& sqliteList, string filePath)
{
	string str0;
	line(str0, "#ifndef _SQLITE_HEADER_H_");
	line(str0, "#define _SQLITE_HEADER_H_");
	line(str0, "");
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

	writeFile(filePath + "SQLiteHeader.h", ANSIToUTF8(str0.c_str(), true));
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
	writeFile(filePath + "SQLiteRegister.h", ANSIToUTF8(str0.c_str(), true));

	string str1;
	line(str1, "#include \"GameHeader.h\"");
	line(str1, "");
	line(str1, "#define REGISTE_SQLITE(classType, tableName) m##classType = NEW(classType, m##classType);mSQLiteManager->addSQLiteTable(m##classType, tableName)");
	line(str1, "");
	line(str1, "void SQLiteRegister::registeAll()");
	line(str1, "{");
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
	writeFile(filePath + "SQLiteRegister.cpp", ANSIToUTF8(str1.c_str(), true));
}

// SQLiteInstanceDeclare.h��SQLiteInstanceDeclare.cpp
void CodeSQLite::generateCppSQLiteInstanceDeclare(const myVector<SQLiteInfo>& sqliteList, string filePath)
{
	string header;
	line(header, "#ifdef _SQLITE_INSTANCE_DECLARE_H_");
	line(header, "#error \"����ͷ�ļ�,ֻ�ܱ�GameBase������\"");
	line(header, "#else");
	line(header, "#define _SQLITE_INSTANCE_DECLARE_H_");
	line(header, "");
	uint count = sqliteList.size();
	FOR_I(count)
	{
		if (sqliteList[i].mOwner == SQLITE_OWNER::CLIENT_ONLY)
		{
			continue;
		}
		line(header, "static SQLite" + sqliteList[i].mSQLiteName + "* mSQLite" + sqliteList[i].mSQLiteName + ";");
	}
	line(header, "");
	line(header, "#endif");
	writeFile(filePath + "SQLiteInstanceDeclare.h", ANSIToUTF8(header.c_str(), true));

	string source;
	line(source, "");
	line(source, "#include \"GameBase.h\"");
	line(source, "");
	FOR_I(count)
	{
		if (sqliteList[i].mOwner == SQLITE_OWNER::CLIENT_ONLY)
		{
			continue;
		}
		line(source, "SQLite" + sqliteList[i].mSQLiteName + "* GameBase::mSQLite" + sqliteList[i].mSQLiteName + ";");
	}
	writeFile(filePath + "SQLiteInstanceDeclare.cpp", ANSIToUTF8(source.c_str(), true));
}

// SQLiteSTLPoolRegister.h
void CodeSQLite::generateCppSQLiteSTLPoolRegister(const myVector<SQLiteInfo>& sqliteList, string filePath)
{
	string header;
	line(header, "#ifdef _SQLITE_STL_POOL_REGISTER_H_");
	line(header, "#error \"����ͷ�ļ�,ֻ�ܱ�GameSTLPoolRegister.cpp������\"");
	line(header, "#else");
	line(header, "#define _SQLITE_STL_POOL_REGISTER_H_");
	line(header, "");
	uint count = sqliteList.size();
	FOR_I(count)
	{
		if (sqliteList[i].mOwner == SQLITE_OWNER::CLIENT_ONLY)
		{
			continue;
		}
		line(header, "REGISTE_VECTOR_POOL(TD" + sqliteList[i].mSQLiteName + "*);");
	}
	line(header, "");
	line(header, "#endif");
	writeFile(filePath + "SQLiteSTLPoolRegister.h", ANSIToUTF8(header.c_str(), true));
}

// TDSQLite.cs�ļ�
void CodeSQLite::generateCSharpSQLiteDataFile(const SQLiteInfo& sqliteInfo, string dataFileGamePath, string dataFileHotFixPath)
{
	if (sqliteInfo.mOwner == SQLITE_OWNER::SERVER_ONLY)
	{
		return;
	}
	string file;
	string dataClassName = "TD" + sqliteInfo.mSQLiteName;
	line(file, "using Mono.Data.Sqlite;");
	line(file, "using System;");
	line(file, "using System.Collections.Generic;");
	line(file, "using UnityEngine;");
	line(file, "");
	line(file, "// " + sqliteInfo.mComment);
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
		// ��Ϊģ���ļ��ǰ���C++��д��,������Щ������C#����û�е�,����Ҫת��ΪC#�ж�Ӧ������
		// myVector�滻ΪList,Vector2UShort�滻ΪVector2Int,char�滻Ϊsbyte
		if (startWith(typeName, "myVector"))
		{
			strReplaceAll(typeName, "myVector", "List");
		}
		else if (typeName == "Vector2UShort")
		{
			typeName = "Vector2Int";
		}
		else if (typeName == "char")
		{
			typeName = "sbyte";
		}
		string memberLine;
		if (findString(typeName.c_str(), "List", NULL))
		{
			memberLine = "\tpublic " + typeName + " m" + sqliteInfo.mMemberList[i].mMemberName + " = new " + typeName + "();";
		}
		else
		{
			memberLine = "\tpublic " + typeName + " m" + sqliteInfo.mMemberList[i].mMemberName + ";";
		}
		uint tabCount = generateAlignTableCount(memberLine, 40);
		FOR_I(tabCount)
		{
			memberLine += '\t';
		}
		memberLine += "// " + sqliteInfo.mMemberList[i].mComment;
		line(file, memberLine);
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
		line(file, "\t\tparseParam(reader, ref m" + sqliteInfo.mMemberList[i].mMemberName + ", " + intToString(i) + ");");
	}
	line(file, "\t}");
	line(file, "\tpublic override bool checkData()");
	line(file, "\t{");
	// ��Ҫ���ո���ֶ�
	myVector<string> checkValueTypeList{"Vector2", "Vector2Int", "Vector2UShort", 
										"myVector<byte>", "myVector<short>", "myVector<ushort>", 
										"myVector<int>", "myVector<uint>", "myVector<float>" };
	myVector<int> checkMemberList;
	FOR_I(memberCount)
	{
		if (checkValueTypeList.contains(sqliteInfo.mMemberList[i].mTypeName))
		{
			checkMemberList.push_back(i);
		}
	}
	if (checkMemberList.size() == 0)
	{
		line(file, "\t\treturn true;");
	}
	else if (checkMemberList.size() == 1)
	{
		line(file, "\t\treturn !mValues[" + intToString(checkMemberList[0]) + "].Contains(\" \");");
	}
	else
	{
		FOR_VECTOR(checkMemberList)
		{
			if (i == 0)
			{
				line(file, "\t\treturn !mValues[" + intToString(checkMemberList[i]) + "].Contains(\" \") && ");
			}
			else if (i == checkMemberListCount - 1)
			{
				line(file, "\t\t\t\t!mValues[" + intToString(checkMemberList[i]) + "].Contains(\" \");");
			}
			else
			{
				line(file, "\t\t\t\t!mValues[" + intToString(checkMemberList[i]) + "].Contains(\" \") && ");
			}
		}
		END(checkMemberList);
	}
	line(file, "\t}");
	line(file, "}", false);
	string dataFilePath = sqliteInfo.mHotFix ? dataFileHotFixPath : dataFileGamePath;
	writeFile(dataFilePath + dataClassName + ".cs", ANSIToUTF8(file.c_str(), true));
}

void CodeSQLite::generateCSharpSQLiteTableFile(const SQLiteInfo& sqliteInfo, string tableFileGamePath, string tableFileHotFixPath)
{
	if (sqliteInfo.mOwner == SQLITE_OWNER::SERVER_ONLY)
	{
		return;
	}
	// SQLiteTable.cs�ļ�
	string dataClassName = "TD" + sqliteInfo.mSQLiteName;
	string tableClassName = "SQLite" + sqliteInfo.mSQLiteName;
	string table;
	line(table, "using System;");
	line(table, "using System.Collections.Generic;");
	line(table, "");
	line(table, "public partial class " + tableClassName + " : SQLiteTable");
	line(table, "{");
	line(table, "\t// ���ڻ����޷�֪������ľ�������,���Խ�List���͵��б��嵽������.��Ϊ�󲿷�ʱ���ⲿʹ�õĶ���List���͵��б�");
	line(table, "\t// �˴�����һ��List��Ϊ�˷����ⲿ��ֱ�ӻ�ȡ,����ÿ��queryAllʱ���ᴴ���б�");
	line(table, "\tprotected List<" + dataClassName + "> mDataList;");
	line(table, "\tprotected bool mDataAvailable;");
	line(table, "\tpublic " + tableClassName + "()");
	line(table, "\t{");
	line(table, "\t\tmDataList = new List<" + dataClassName + ">();");
	line(table, "\t\tmDataAvailable = false;");
	line(table, "\t}");
	line(table, "\tpublic " + dataClassName + " query(int id)");
	line(table, "\t{");
	line(table, "\t\treturn query(typeof(" + dataClassName + "), id) as " + dataClassName + ";");
	line(table, "\t}");
	line(table, "\tpublic List<" + dataClassName + "> queryAll()");
	line(table, "\t{");
	line(table, "\t\tif(!mDataAvailable)");
	line(table, "\t\t{");
	line(table, "\t\t\tqueryAll(typeof(" + dataClassName + "), mDataList);");
	line(table, "\t\t\tmDataAvailable = true;");
	line(table, "\t\t}");
	line(table, "\t\treturn mDataList;");
	line(table, "\t}");
	line(table, "\tprotected override void clearAll()");
	line(table, "\t{");
	line(table, "\t\tbase.clearAll();");
	line(table, "\t\tmDataList.Clear();");
	line(table, "\t\tmDataAvailable = false;");
	line(table, "\t}");
	line(table, "}", false);
	string tableFilePath = sqliteInfo.mHotFix ? tableFileHotFixPath : tableFileGamePath;
	writeFile(tableFilePath + tableClassName + ".cs", ANSIToUTF8(table.c_str(), true));
}

// SQLiteRegister.cs�ļ�
void CodeSQLite::generateCSharpSQLiteRegisteFileFile(const myVector<SQLiteInfo>& sqliteInfo, string fileHotFixPath, string fileGamePath)
{
	// �������еı��ע��
	string mainFile;
	line(mainFile, "using System;");
	line(mainFile, "using System.Collections.Generic;");
	line(mainFile, "");
	line(mainFile, "public class SQLiteRegisterMain : GameBase");
	line(mainFile, "{");
	line(mainFile, "\tpublic static void registeAll()");
	line(mainFile, "\t{");
	uint count = sqliteInfo.size();
	FOR_I(count)
	{
		if (sqliteInfo[i].mOwner != SQLITE_OWNER::SERVER_ONLY && !sqliteInfo[i].mHotFix)
		{
			string lineStr = "\t\tregisteTable(out mSQLite%s, typeof(TD%s), \"%s\");";
			replaceAll(lineStr, "%s", sqliteInfo[i].mSQLiteName);
			line(mainFile, lineStr);
		}
	}
	line(mainFile, "\t}");
	line(mainFile, "\t//-------------------------------------------------------------------------------------------------------------");
	line(mainFile, "\tprotected static void registeTable<T>(out T sqliteTable, Type dataType, string tableName) where T : SQLiteTable");
	line(mainFile, "\t{");
	line(mainFile, "\t\tsqliteTable = mSQLiteManager.registeTable(typeof(T), dataType, tableName) as T;");
	line(mainFile, "\t}");
	line(mainFile, "}", false);

	writeFile(fileGamePath + "SQLiteRegisterMain.cs", ANSIToUTF8(mainFile.c_str(), true));

	// �ȸ������еı��ע��
	string hotFixfile;
	line(hotFixfile, "using System;");
	line(hotFixfile, "using System.Collections.Generic;");
	line(hotFixfile, "");
	line(hotFixfile, "public class SQLiteRegisterILR : GB");
	line(hotFixfile, "{");
	line(hotFixfile, "\tpublic static void registeAll()");
	line(hotFixfile, "\t{");
	FOR_I(count)
	{
		if (sqliteInfo[i].mOwner != SQLITE_OWNER::SERVER_ONLY && sqliteInfo[i].mHotFix)
		{
			string lineStr = "\t\tregisteTable(out mSQLite%s, typeof(TD%s), \"%s\");";
			replaceAll(lineStr, "%s", sqliteInfo[i].mSQLiteName);
			line(hotFixfile, lineStr);
		}
	}
	line(hotFixfile, "\t}");
	line(hotFixfile, "\t//-------------------------------------------------------------------------------------------------------------");
	line(hotFixfile, "\tprotected static void registeTable<T>(out T sqliteTable, Type dataType, string tableName) where T : SQLiteTable");
	line(hotFixfile, "\t{");
	line(hotFixfile, "\t\tsqliteTable = mSQLiteManager.registeTable(typeof(T), dataType, tableName) as T;");
	line(hotFixfile, "\t}");
	line(hotFixfile, "}", false);

	writeFile(fileHotFixPath + "SQLiteRegisterILR.cs", ANSIToUTF8(hotFixfile.c_str(), true));
}

// GameBaseSQLite.cs�ļ�
void CodeSQLite::generateCSharpSQLiteDeclare(const myVector<SQLiteInfo>& sqliteInfo, string fileHotFixPath, string fileGamePath)
{
	// �������еı��ע��
	string mainFile;
	line(mainFile, "using System;");
	line(mainFile, "");
	line(mainFile, "// GameBase�Ĳ�����,���ڶ���SQLite���Ķ���");
	line(mainFile, "public partial class GameBase : FrameBase");
	line(mainFile, "{");
	uint sqliteCount = sqliteInfo.size();
	FOR_I(sqliteCount)
	{
		if (sqliteInfo[i].mOwner != SQLITE_OWNER::SERVER_ONLY && !sqliteInfo[i].mHotFix)
		{
			line(mainFile, "\tpublic static SQLite" + sqliteInfo[i].mSQLiteName + " mSQLite" + sqliteInfo[i].mSQLiteName + ";");
		}
	}
	line(mainFile, "}");

	writeFile(fileGamePath + "GameBaseSQLite.cs", ANSIToUTF8(mainFile.c_str(), true));

	// �ȸ������еı��ע��
	string hotFixfile;
	line(hotFixfile, "using System;");
	line(hotFixfile, "");
	line(hotFixfile, "// GameBase�Ĳ�����,���ڶ���SQLite���Ķ���");
	line(hotFixfile, "public partial class GB : FrameUtilityILR");
	line(hotFixfile, "{");
	FOR_I(sqliteCount)
	{
		if (sqliteInfo[i].mOwner != SQLITE_OWNER::SERVER_ONLY && sqliteInfo[i].mHotFix)
		{
			line(hotFixfile, "\tpublic static SQLite" + sqliteInfo[i].mSQLiteName + " mSQLite" + sqliteInfo[i].mSQLiteName + ";");
		}
	}
	line(hotFixfile, "}");

	writeFile(fileHotFixPath + "GameBaseSQLiteILR.cs", ANSIToUTF8(hotFixfile.c_str(), true));
}