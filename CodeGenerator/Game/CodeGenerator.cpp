#include "CodeGenerator.h"

void CodeGenerator::generatePacketCode(string cppDeclareFilePath, string csFilePath, string cppPacketDefineFilePath, string csPacketDefineFilePath, string cppStringDefinePath)
{
	// ����ģ���ļ�
	string fileContent;
	openTxtFile("PacketProtocal.txt", fileContent);
	if (fileContent.length() == 0)
	{
		ERROR("δ�ҵ�Э���ļ�PacketProtocal.txt");
		return;
	}
	myVector<string> lines;
	split(fileContent.c_str(), "\r\n", lines);
	bool packetStart = false;
	myVector<PacketInfo> packetInfoList;
	myVector<PacketMember> tempMemberList;
	string tempPacketName;
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
		// û�г�Ա��������Ϣ��
		if (line == "{}")
		{
			PacketInfo info;
			info.mPacketName = lines[i - 1];
			packetInfoList.push_back(info);
			continue;
		}
		// ��Ա�����б���ʼ
		if (line == "{")
		{
			packetStart = true;
			tempPacketName = lines[i - 1];
			tempMemberList.clear();
			continue;
		}
		// ��Ա�����б����
		if (line == "}")
		{
			PacketInfo info;
			info.mPacketName = tempPacketName;
			info.mMemberList = tempMemberList;
			packetInfoList.push_back(info);
			packetStart = false;
			tempMemberList.clear();
			tempPacketName = "";
			continue;
		}
		if (packetStart)
		{
			tempMemberList.push_back(parseMemberLine(line));
		}
	}
	deleteFolder(cppDeclareFilePath);
	// c#��ֻɾ�������ļ�,��ɾ��meta�ļ�
	myVector<string> csFileList;
	findFiles(csFilePath, csFileList, ".cs");
	FOR_VECTOR_CONST(csFileList)
	{
		deleteFile(csFileList[i]);
	}
	myVector<string> packetList;
	FOR_VECTOR_CONST(packetInfoList)
	{
		// ���ɴ����ļ�
		// .h����
		generateCppPacketDeclareFile(packetInfoList[i], cppDeclareFilePath);
		// .cs����
		generateCSharpFile(packetInfoList[i], csFilePath);
		packetList.push_back(packetInfoList[i].mPacketName);
	}
	// c++
	generateCppPacketDefineFile(packetInfoList, cppPacketDefineFilePath);
	generateCppPacketRegisteFile(packetInfoList, cppPacketDefineFilePath);
	generateCppPacketTotalHeaderFile(packetInfoList, cppPacketDefineFilePath);
	generateStringDefinePacket(packetList, cppStringDefinePath);
	// c#
	generateCSharpPacketDefineFile(packetInfoList, csPacketDefineFilePath);
	generateCSharpPacketRegisteFile(packetInfoList, csPacketDefineFilePath);
}

void CodeGenerator::generateSQLiteCode(string cppDataPath, string cppTablePath, string csDataPath, string csTablePath)
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

	// ����һ��Ŀ¼����SQLiteRegister.cs�ļ�
	string registerPath = csDataPath;
	if (registerPath[registerPath.length() - 1] == '/' || registerPath[registerPath.length() - 1] == '\\')
	{
		registerPath = registerPath.substr(0, registerPath.length() - 1);
	}
	registerPath = getFilePath(registerPath);
	generateCSharpSQLiteRegisteFileFile(sqliteInfoList, registerPath);
}

void CodeGenerator::generateMySQLCode(string cppDataPath, string cppStringDefinePath)
{
	// ����ģ���ļ�
	string fileContent;
	openTxtFile("MySQL.txt", fileContent);
	if (fileContent.length() == 0)
	{
		ERROR("δ�ҵ�����ʽ�ļ�MySQL.txt");
		return;
	}
	fileContent = UTF8ToANSI(fileContent.c_str(), true);
	myVector<string> lines;
	split(fileContent.c_str(), "\r\n", lines);
	bool packetStart = false;
	myVector<MySQLInfo> mysqlInfoList;
	MySQLInfo tempInfo;
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
			tempInfo.mMySQLName = lines[i - 1];
			tempInfo.mMemberList.clear();
			continue;
		}
		// ��Ա�����б����
		if (line == "}")
		{
			mysqlInfoList.push_back(tempInfo);
			packetStart = false;
			continue;
		}
		if (packetStart)
		{
			tempInfo.mMemberList.push_back(parseMySQLMemberLine(line));
		}
	}
	deleteFolder(cppDataPath);
	myVector<string> mysqlList;
	FOR_VECTOR_CONST(mysqlInfoList)
	{
		// ���ɴ����ļ�
		generateCppMySQLDataFile(mysqlInfoList[i], cppDataPath);
		mysqlList.push_back(mysqlInfoList[i].mMySQLName);
	}
	// ��һ��Ŀ¼����MySQLHeader.h
	string totalHeaderPath = cppDataPath;
	if (endWith(totalHeaderPath, "/") || endWith(totalHeaderPath, "\\"))
	{
		totalHeaderPath = totalHeaderPath.substr(0, totalHeaderPath.length() - 1);
	}
	totalHeaderPath = getFilePath(totalHeaderPath);
	generateCppMySQLTotalHeaderFile(mysqlInfoList, totalHeaderPath);
	generateCppMySQLRegisteFile(mysqlInfoList, totalHeaderPath);
	generateStringDefineMySQL(mysqlList, cppStringDefinePath);
}

void CodeGenerator::generateCmdCode(string filePath, string headerPath)
{
	string cmdFile;
	openTxtFile("Cmd.txt", cmdFile);
	if (cmdFile.length() == 0)
	{
		ERROR("δ���ļ�Cmd.txt");
		return;
	}
	myVector<string> cmdList;
	split(cmdFile.c_str(), "\r\n", cmdList);
	// ����StringDefineCmd�ļ�
	generateStringDefineCmd(cmdList, filePath);
	// ����CommandHeader.h�ļ�
	generateCppCmdTotalHeaderFile(cmdList, headerPath);
}

void CodeGenerator::generateStateCode(string filePath, string headerPath)
{
	string stateFile;
	openTxtFile("State.txt", stateFile);
	if (stateFile.length() == 0)
	{
		ERROR("δ���ļ�State.txt");
		return;
	}
	myVector<string> stateList;
	split(stateFile.c_str(), "\r\n", stateList);
	// ����StringDefineState�ļ�
	generateStringDefineState(stateList, filePath);
	// ����StateHeader.h�ļ�
	generateCppStateTotalHeaderFile(stateList, headerPath);
}

void CodeGenerator::generateSkillCode(string filePath, string headerPath)
{
	string skillFile;
	openTxtFile("Skill.txt", skillFile);
	if (skillFile.length() == 0)
	{
		ERROR("δ���ļ�Skill.txt");
		return;
	}
	myVector<string> skillList;
	split(skillFile.c_str(), "\r\n", skillList);
	// ����StringDefineSkill�ļ�
	generateStringDefineSkill(skillList, filePath);
	// ����CharacterSkillHeader.h�ļ�
	generateCppSkillTotalHeaderFile(skillList, headerPath);
}

// ����MySQLData.h��MySQLData.cpp�ļ�
void CodeGenerator::generateCppMySQLDataFile(const MySQLInfo& mysqlInfo, string filePath)
{
	// ͷ�ļ�
	string header;
	string className = "MD" + mysqlInfo.mMySQLName;
	string headerMacro = "_MD" + nameToUpper(mysqlInfo.mMySQLName) + "_H_";
	line(header, "#ifndef " + headerMacro);
	line(header, "#define " + headerMacro);
	line(header, "");
	line(header, "#include \"MySQLData.h\"");
	line(header, "");
	line(header, "class MySQLTable;");
	line(header, "class " + className + " : public MySQLData");
	line(header, "{");
	line(header, "public:");
	uint memberCount = mysqlInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		line(header, "\tCOL(" + mysqlInfo.mMemberList[i].mTypeName + ", " + mysqlInfo.mMemberList[i].mMemberName + ");");
	}
	line(header, "public:");
	line(header, "\tstatic void fillColName(MySQLTable* table);");
	line(header, "\tvoid resultRowToTableData(myMap<const char*, char*>& resultRow) override;");
	line(header, "\tvoid paramList(char* params, uint size) override;");
	line(header, "\tvoid resetProperty() override;");
	line(header, "};");
	line(header, "");
	line(header, "#endif", false);

	// Դ�ļ�
	string source;
	line(source, "#include \"" + className + ".h\"");
	line(source, "#include \"Utility.h\"");
	line(source, "#include \"MySQLTable.h\"");
	line(source, "");
	// �ֶξ�̬��������
	FOR_I(memberCount)
	{
		line(source, "COL_DEFINE(" + className + ", " + mysqlInfo.mMemberList[i].mMemberName + ");");
	}
	// fillColName����
	line(source, "");
	line(source, "void " + className + "::fillColName(MySQLTable* table)");
	line(source, "{");
	FOR_I(memberCount)
	{
		line(source, "\ttable->addColName(" + mysqlInfo.mMemberList[i].mMemberName + ");");
	}
	line(source, "}");
	line(source, "");
	// resultRowToTableData����
	line(source, "void " + className + "::resultRowToTableData(myMap<const char*, char*>& resultRow)");
	line(source, "{");
	FOR_I(memberCount)
	{
		line(source, "\tPARSE(" + mysqlInfo.mMemberList[i].mMemberName + ");");
	}
	line(source, "}");
	line(source, "");
	// paramList����
	line(source, "void " + className + "::paramList(char* params, uint size)");
	line(source, "{");
	FOR_I(memberCount)
	{
		if (i != memberCount - 1)
		{
			if (mysqlInfo.mMemberList[i].mTypeName == "string")
			{
				string isUTF8Str = mysqlInfo.mMemberList[i].mUTF8 ? "true" : "false";
				line(source, "\tAPPEND_STRING(" + mysqlInfo.mMemberList[i].mMemberName + ", " + isUTF8Str + ");");
			}
			else
			{
				line(source, "\tAPPEND_VALUE(" + mysqlInfo.mMemberList[i].mMemberName + ");");
			}
		}
		else
		{
			if (mysqlInfo.mMemberList[i].mTypeName == "string")
			{
				string isUTF8Str = mysqlInfo.mMemberList[i].mUTF8 ? "true" : "false";
				line(source, "\tAPPEND_STRING_END(" + mysqlInfo.mMemberList[i].mMemberName + ", " + isUTF8Str + ");");
			}
			else
			{
				line(source, "\tAPPEND_VALUE_END(" + mysqlInfo.mMemberList[i].mMemberName + ");");
			}
		}
	}
	line(source, "}");
	line(source, "");
	// resetProperty����
	line(source, "void " + className + "::resetProperty()");
	line(source, "{");
	line(source, "\tMySQLData::resetProperty();");
	FOR_I(memberCount)
	{
		if (mysqlInfo.mMemberList[i].mTypeName == "string")
		{
			line(source, "\tm" + mysqlInfo.mMemberList[i].mMemberName + ".clear();");
		}
		else if (mysqlInfo.mMemberList[i].mTypeName == "float")
		{
			line(source, "\tm" + mysqlInfo.mMemberList[i].mMemberName + " = 0.0f;");
		}
		else
		{
			line(source, "\tm" + mysqlInfo.mMemberList[i].mMemberName + " = 0;");
		}
	}
	line(source, "}", false);
	validPath(filePath);
	header = ANSIToUTF8(header.c_str(), true);
	source = ANSIToUTF8(source.c_str(), true);
	writeFile(filePath + className + ".h", header);
	writeFile(filePath + className + ".cpp", source);
}

// TDSQLite.h��TDSQLite.cpp,SQLiteTable.h�ļ�
void CodeGenerator::generateCppSQLiteDataFile(const SQLiteInfo& sqliteInfo, string dataFilePath, string tableFilePath)
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
void CodeGenerator::generateCppSQLiteTotalHeaderFile(const myVector<SQLiteInfo>& sqliteList, string filePath)
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
void CodeGenerator::generateCppSQLiteRegisteFile(const myVector<SQLiteInfo>& sqliteList, string filePath)
{
	// MySQLRegiste.h
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

// MySQLHeader.h�ļ�
void CodeGenerator::generateCppMySQLTotalHeaderFile(const myVector<MySQLInfo>& mysqlList, string filePath)
{
	string str0;
	line(str0, "#ifndef _MYSQL_HEADER_H_");
	line(str0, "#define _MYSQL_HEADER_H_");
	line(str0, "");
	uint packetCount = mysqlList.size();
	FOR_I(packetCount)
	{
		line(str0, "#include \"MD" + mysqlList[i].mMySQLName + ".h\"");
	}
	line(str0, "");
	FOR_I(packetCount)
	{
		line(str0, "#include \"MySQL" + mysqlList[i].mMySQLName + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif", false);
	validPath(filePath);
	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "MySQLHeader.h", str0);
}

// MySQLRegiste.h��MySQLRegiste.cpp�ļ�
void CodeGenerator::generateCppMySQLRegisteFile(const myVector<MySQLInfo>& mysqlList, string filePath)
{
	// MySQLRegiste.h
	string str0;
	line(str0, "#ifndef _MYSQL_REGISTER_H_");
	line(str0, "#define _MYSQL_REGISTER_H_");
	line(str0, "");
	line(str0, "#include \"GameBase.h\"");
	line(str0, "");
	line(str0, "class MySQLRegister : public GameBase");
	line(str0, "{");
	line(str0, "public:");
	line(str0, "\tstatic void registeAll();");
	line(str0, "};");
	line(str0, "");
	line(str0, "#endif", false);
	validPath(filePath);
	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "MySQLRegister.h", str0);

	string str1;
	line(str1, "#include \"MySQLRegister.h\"");
	line(str1, "#include \"MySQLDataBase.h\"");
	line(str1, "#include \"MySQLHeader.h\"");
	line(str1, "");
	line(str1, "#define REGISTE_MYSQL(classType, tableName) m##classType = mMySQLDataBase->registeTable<classType>(tableName);");
	line(str1, "");
	line(str1, "void MySQLRegister::registeAll()");
	line(str1, "{");
	uint count = mysqlList.size();
	FOR_I(count)
	{
		line(str1, "\tREGISTE_MYSQL(MySQL" + mysqlList[i].mMySQLName + ", \"" + mysqlList[i].mMySQLName + "\");");
	}
	line(str1, "}", false);
	str1 = ANSIToUTF8(str1.c_str(), true);
	writeFile(filePath + "MySQLRegister.cpp", str1);
}

// CommandHeader.h�ļ�
void CodeGenerator::generateCppCmdTotalHeaderFile(const myVector<string>& cmdList, string filePath)
{
	string str0;
	line(str0, "#ifndef _COMMAND_HEADER_H_");
	line(str0, "#define _COMMAND_HEADER_H_");
	line(str0, "");
	line(str0, "#include \"CommandHeaderBase.h\"");
	line(str0, "");
	uint count = cmdList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + cmdList[i] + ".h\"");
	}
	line(str0, "");
	line(str0, "#include \"StringDefine.h\"");
	line(str0, "");
	line(str0, "#endif", false);
	validPath(filePath);
	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "CommandHeader.h", str0);
}

// StateHeader.h
void CodeGenerator::generateCppStateTotalHeaderFile(const myVector<string>& stateList, string filePath)
{
	string str0;
	line(str0, "#ifndef _STATE_HEADER_H_");
	line(str0, "#define _STATE_HEADER_H_");
	line(str0, "");
	uint count = stateList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + stateList[i] + ".h\"");
	}
	line(str0, "");
	line(str0, "#include \"StateInterfaceHeader.h\"");
	line(str0, "");
	line(str0, "#endif", false);
	validPath(filePath);
	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "StateHeader.h", str0);
}

// CharacterSkillHeader.h
void CodeGenerator::generateCppSkillTotalHeaderFile(const myVector<string>& skillList, string filePath)
{
	string str0;
	line(str0, "#ifndef _CHARACTER_SKILL_HEADER_H_");
	line(str0, "#define _CHARACTER_SKILL_HEADER_H_");
	line(str0, "");
	uint count = skillList.size();
	FOR_I(count)
	{
		line(str0, "#include \"" + skillList[i] + ".h\"");
	}
	line(str0, "");
	line(str0, "#include \"StateInterfaceHeader.h\"");
	line(str0, "");
	line(str0, "#endif", false);
	validPath(filePath);
	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "CharacterSkillHeader.h", str0);
}

// PacketHeader.h��PacketDeclareHeader.h�ļ�
void CodeGenerator::generateCppPacketTotalHeaderFile(const myVector<PacketInfo>& packetList, string filePath)
{
	// PacketHeader.h
	string str0;
	line(str0, "#ifndef _PACKET_HEADER_H_");
	line(str0, "#define _PACKET_HEADER_H_");
	line(str0, "");
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			line(str0, "#include \"" + packetList[i].mPacketName + ".h\"");
		}
	}
	line(str0, "");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			line(str0, "#include \"" + packetList[i].mPacketName + ".h\"");
		}
	}
	line(str0, "#include \"StringDefine.h\"");
	line(str0, "");
	line(str0, "#endif", false);
	validPath(filePath);
	str0 = ANSIToUTF8(str0.c_str(), true);
	writeFile(filePath + "PacketHeader.h", str0);

	// PacketDeclareHeader.h
	string str1;
	line(str1, "#ifndef _PACKET_DECLARE_HEADER_H_");
	line(str1, "#define _PACKET_DECLARE_HEADER_H_");
	line(str1, "");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			line(str1, "#include \"" + packetList[i].mPacketName + "_Declare.h\"");
		}
	}
	line(str1, "");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			line(str1, "#include \"" + packetList[i].mPacketName + "_Declare.h\"");
		}
	}
	line(str1, "");
	line(str1, "#endif", false);
	validPath(filePath);
	str1 = ANSIToUTF8(str1.c_str(), true);
	writeFile(filePath + "PacketDeclareHeader.h", str1);
}

// PacketDefine.h�ļ�
void CodeGenerator::generateCppPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath)
{
	string str;
	line(str, "#ifndef _PACKET_DEFINE_H_");
	line(str, "#define _PACKET_DEFINE_H_");
	line(str, "");
	line(str, "#include \"ServerDefine.h\"");
	line(str, "");
	line(str, "enum class PACKET_TYPE : ushort");
	line(str, "{");
	line(str, "\tMIN,");
	line(str, "");
	line(str, "\tCS_MIN = 10000,");
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			line(str, "\t" + packetNameToUpper(packetList[i].mPacketName) + ",");
		}
	}
	line(str, "\tCS_MAX,");
	line(str, "");
	line(str, "\tSC_MIN = 20000,");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			line(str, "\t" + packetNameToUpper(packetList[i].mPacketName) + ",");
		}
	}
	line(str, "\tSC_MAX,");
	line(str, "};");
	line(str, "");
	line(str, "#endif", false);
	validPath(filePath);
	str = ANSIToUTF8(str.c_str(), true);
	writeFile(filePath + "PacketDefine.h", str);
}

// PacketRegister.cpp�ļ�
void CodeGenerator::generateCppPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath)
{
	string str;
	line(str, "#include \"PacketHeader.h\"");
	line(str, "#include \"GameLog.h\"");
	line(str, "#include \"NetServer.h\"");
	line(str, "#include \"PacketFactoryManager.h\"");
	line(str, "#include \"PacketRegister.h\"");
	line(str, "");
	line(str, "#define PACKET_FACTORY(packet, type) mPacketFactoryManager->addFactory<packet>(PACKET_TYPE::type, NAME(packet));");
	line(str, "");
	line(str, "void PacketRegister::registeAll()");
	line(str, "{");
	line(str, "\tuint preCount = mPacketFactoryManager->getFactoryCount();");
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			line(str, "\tPACKET_FACTORY(" + packetList[i].mPacketName + ", " + packetNameToUpper(packetList[i].mPacketName) + ");");
		}
	}
	line(str, "\tmPacketFactoryManager->checkRegisteCount(preCount, (int)PACKET_TYPE::CS_MAX - (int)PACKET_TYPE::CS_MIN - 1, \"CS\");");
	line(str, "");
	line(str, "\tpreCount = mPacketFactoryManager->getFactoryCount();");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			line(str, "\tPACKET_FACTORY(" + packetList[i].mPacketName + ", " + packetNameToUpper(packetList[i].mPacketName) + ");");
		}
	}
	line(str, "\tmPacketFactoryManager->checkRegisteCount(preCount, (int)PACKET_TYPE::SC_MAX - (int)PACKET_TYPE::SC_MIN - 1, \"SC\");");
	line(str, "};");
	validPath(filePath);
	str = ANSIToUTF8(str.c_str(), true);
	writeFile(filePath + "PacketRegister.cpp", str);
}

// Packet_Declare.h�ļ�
void CodeGenerator::generateCppPacketDeclareFile(const PacketInfo& packetInfo, string filePath)
{
	string headerMacro = "_" + packetNameToUpper(packetInfo.mPacketName) + "_DECLARE_H_";
	string file;
	line(file, "#ifndef " + headerMacro);
	line(file, "#define " + headerMacro);
	line(file, "");
	line(file, "#define " + packetInfo.mPacketName + "_Declare \\");
	line(file, "public:\\");
	// ע���Ա����
	uint memberCount = packetInfo.mMemberList.size();
	if (memberCount != 0)
	{
		line(file, "\tvoid fillParams() override\\");
		line(file, "\t{\\");
		FOR_I(memberCount)
		{
			line(file, cppPushParamString(packetInfo.mMemberList[i]));
		}
		line(file, "\t}\\");
	}
	else
	{
		line(file, "\tvoid fillParams() override{}\\");
	}
	// ��Ա����������
	line(file, "public:\\");
	FOR_I(memberCount)
	{
		line(file, cppMemberDeclareString(packetInfo.mMemberList[i]));
	}
	removeLast(file, '\\');
	line(file, "");
	line(file, "#endif", false);
	validPath(filePath);
	file = ANSIToUTF8(file.c_str(), true);
	writeFile(filePath + packetInfo.mPacketName + "_Declare.h", file);
}

// StringDefineCmd.h��StringDefineCmd.cpp
void CodeGenerator::generateStringDefineCmd(const myVector<string>& cmdList, string filePath)
{
	// ͷ�ļ�
	string header;
	uint cmdCount = cmdList.size();
	FOR_I(cmdCount)
	{
		line(header, "DECLARE_STRING(" + cmdList[i] + ");");
	}
	validPath(filePath);
	header = ANSIToUTF8(header.c_str(), true);
	writeFile(filePath + "StringDefineCmd.h", header);

	// Դ�ļ�
	string source;
	line(source, "#include \"StringDefine.h\"");
	line(source, "#include \"CommandHeader.h\"");
	line(source, "");
	FOR_I(cmdCount)
	{
		line(source, "DEFINE_STRING(" + cmdList[i] + ");");
	}
	source = ANSIToUTF8(source.c_str(), true);
	writeFile(filePath + "StringDefineCmd.cpp", source);
}

// StringDefineSkill.h��StringDefineSkill.cpp
void CodeGenerator::generateStringDefineSkill(const myVector<string>& skillList, string filePath)
{
	// ͷ�ļ�
	string header;
	uint cmdCount = skillList.size();
	FOR_I(cmdCount)
	{
		line(header, "DECLARE_STRING(" + skillList[i] + ");");
	}
	validPath(filePath);
	header = ANSIToUTF8(header.c_str(), true);
	writeFile(filePath + "StringDefineSkill.h", header);

	// Դ�ļ�
	string source;
	line(source, "#include \"StringDefine.h\"");
	line(source, "#include \"CharacterSkillHeader.h\"");
	line(source, "");
	FOR_I(cmdCount)
	{
		line(source, "DEFINE_STRING(" + skillList[i] + ");");
	}
	source = ANSIToUTF8(source.c_str(), true);
	writeFile(filePath + "StringDefineSkill.cpp", source);
}

// StringDefineMySQL.h��StringDefineMySQL.cpp
void CodeGenerator::generateStringDefineMySQL(const myVector<string>& mysqlList, string filePath)
{
	// ͷ�ļ�
	string header;
	uint cmdCount = mysqlList.size();
	FOR_I(cmdCount)
	{
		line(header, "DECLARE_STRING(MD" + mysqlList[i] + ");");
	}
	validPath(filePath);
	header = ANSIToUTF8(header.c_str(), true);
	writeFile(filePath + "StringDefineMySQL.h", header);

	// Դ�ļ�
	string source;
	line(source, "#include \"StringDefine.h\"");
	line(source, "#include \"MySQLHeader.h\"");
	line(source, "");
	FOR_I(cmdCount)
	{
		line(source, "DEFINE_STRING(MD" + mysqlList[i] + ");");
	}
	source = ANSIToUTF8(source.c_str(), true);
	writeFile(filePath + "StringDefineMySQL.cpp", source);
}

// StringDefineState.h��StringDefineState.cpp
void CodeGenerator::generateStringDefineState(const myVector<string>& stateList, string filePath)
{
	// ͷ�ļ�
	string header;
	uint cmdCount = stateList.size();
	FOR_I(cmdCount)
	{
		line(header, "DECLARE_STRING(" + stateList[i] + ");");
	}
	validPath(filePath);
	header = ANSIToUTF8(header.c_str(), true);
	writeFile(filePath + "StringDefineState.h", header);

	// Դ�ļ�
	string source;
	line(source, "#include \"StringDefine.h\"");
	line(source, "#include \"StateHeader.h\"");
	line(source, "");
	FOR_I(cmdCount)
	{
		line(source, "DEFINE_STRING(" + stateList[i] + ");");
	}
	source = ANSIToUTF8(source.c_str(), true);
	writeFile(filePath + "StringDefineState.cpp", source);
}

// StringDefinePacket.h��StringDefinePacket.cpp
void CodeGenerator::generateStringDefinePacket(const myVector<string>& packetList, string filePath)
{
	// ͷ�ļ�
	string header;
	uint cmdCount = packetList.size();
	FOR_I(cmdCount)
	{
		line(header, "DECLARE_STRING(" + packetList[i] + ");");
	}
	validPath(filePath);
	header = ANSIToUTF8(header.c_str(), true);
	writeFile(filePath + "StringDefinePacket.h", header);

	// Դ�ļ�
	string source;
	line(source, "#include \"StringDefine.h\"");
	line(source, "#include \"PacketHeader.h\"");
	line(source, "");
	FOR_I(cmdCount)
	{
		line(source, "DEFINE_STRING(" + packetList[i] + ");");
	}
	source = ANSIToUTF8(source.c_str(), true);
	writeFile(filePath + "StringDefinePacket.cpp", source);
}

// TDSQLite.cs��SQLiteTable.cs�ļ�
void CodeGenerator::generateCSharpSQLiteDataFile(const SQLiteInfo& sqliteInfo, string dataFilePath, string tableFilePath)
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
	line(file, "public class " + dataClassName + " : TableData");
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
	line(table, "\tpublic " + tableClassName + "()");
	line(table, "\t\t:base(typeof(" + dataClassName + ")) {}");
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
void CodeGenerator::generateCSharpSQLiteRegisteFileFile(const myVector<SQLiteInfo>& sqliteInfo, string filePath)
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
			line(file, "\t\tregisteTable(ref mSQLite" + sqliteInfo[i].mSQLiteName + ", \"" + sqliteInfo[i].mSQLiteName + "\");");
		}
	}
	line(file, "\t\tmSQLite.linkAllTable();");
	line(file, "\t}");
	line(file, "\t//-------------------------------------------------------------------------------------------------------------");
	line(file, "\tprotected static void registeTable<T>(ref T table, string tableName) where T : SQLiteTable, new()");
	line(file, "\t{");
	line(file, "\t\ttable = mSQLite.registeTable<T>(tableName);");
	line(file, "\t}");
	line(file, "}", false);
	validPath(filePath);
	file = ANSIToUTF8(file.c_str(), true);
	writeFile(filePath + "SQLiteRegister.cs", file);
}

// PacketDefine.cs�ļ�
void CodeGenerator::generateCSharpPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath)
{
	string str;
	line(str, "using System;");
	line(str, "using System.Collections.Generic;");
	line(str, "");
	line(str, "public enum PACKET_TYPE : ushort");
	line(str, "{");
	line(str, "\tMIN,");
	line(str, "");
	line(str, "\tCS_MIN = 10000,");
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			line(str, "\t" + packetNameToUpper(packetList[i].mPacketName) + ",");
		}
	}
	line(str, "\tCS_MAX,");
	line(str, "");
	line(str, "\tSC_MIN = 20000,");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			line(str, "\t" + packetNameToUpper(packetList[i].mPacketName) + ",");
		}
	}
	line(str, "\tSC_MAX,");
	line(str, "};", false);
	validPath(filePath);
	str = ANSIToUTF8(str.c_str(), true);
	writeFile(filePath + "PacketDefine.cs", str);
}

// PacketRegister.cs�ļ�
void CodeGenerator::generateCSharpPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath)
{
	string str;
	line(str, "using System;");
	line(str, "using System.Collections;");
	line(str, "using System.Collections.Generic;");
	line(str, "");
	line(str, "public class PacketRegister : GameBase");
	line(str, "{");
	line(str, "\tpublic static void registeAll()");
	line(str, "\t{");
	line(str, "\t\tint preCount = mSocketFactory.getPacketTypeCount();");
	uint packetCount = packetList.size();
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "CS"))
		{
			line(str, "\t\tregistePacket<" + packetList[i].mPacketName + ">(PACKET_TYPE." + packetNameToUpper(packetList[i].mPacketName) + ");");
		}
	}
	line(str, "\t\tmSocketFactory.checkRegisteCount(PACKET_TYPE.CS_MAX - PACKET_TYPE.CS_MIN - 1, preCount, \"CS\");");
	line(str, "");
	line(str, "\t\tpreCount = mSocketFactory.getPacketTypeCount();");
	FOR_I(packetCount)
	{
		if (startWith(packetList[i].mPacketName, "SC"))
		{
			line(str, "\t\tregistePacket<" + packetList[i].mPacketName + ">(PACKET_TYPE." + packetNameToUpper(packetList[i].mPacketName) + ");");
		}
	}
	line(str, "\t\tmSocketFactory.checkRegisteCount(PACKET_TYPE.SC_MAX - PACKET_TYPE.SC_MIN - 1, preCount, \"SC\");");
	line(str, "\t}");
	line(str, "\tprotected static void registePacket<T>(PACKET_TYPE type) where T : SocketPacket, new()");
	line(str, "\t{");
	line(str, "\t\tmSocketFactory.registePacket<T>(type);");
	line(str, "\t}");
	line(str, "}", false);
	validPath(filePath);
	str = ANSIToUTF8(str.c_str(), true);
	writeFile(filePath + "PacketRegister.cs", str);
}

// Packet_Declare.cs�ļ�
void CodeGenerator::generateCSharpFile(const PacketInfo& packetInfo, string filePath)
{
	const int prefixLength = 2;
	if (packetInfo.mPacketName.substr(0, prefixLength) != "CS" && packetInfo.mPacketName.substr(0, prefixLength) != "SC")
	{
		ERROR("����ǰ׺����");
		return;
	}
	string file;
	line(file, "using System;");
	line(file, "using System.Collections;");
	line(file, "using System.Collections.Generic;");
	line(file, "");
	line(file, "public partial class " + packetInfo.mPacketName + " : SocketPacket");
	line(file, "{");
	uint memberCount = packetInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		line(file, cSharpMemberDeclareString(packetInfo.mMemberList[i]));
	}
	if (memberCount > 0)
	{
		line(file, "\tprotected override void fillParams()");
		line(file, "\t{");
		FOR_I(memberCount)
		{
			line(file, cSharpPushParamString(packetInfo.mMemberList[i]));
		}
		line(file, "\t}");
	}
	line(file, "}", false);
	validPath(filePath);
	file = ANSIToUTF8(file.c_str(), true);
	writeFile(filePath + packetInfo.mPacketName + "_Declare.cs", file);
}

MySQLMember CodeGenerator::parseMySQLMemberLine(string line)
{
	MySQLMember memberInfo;
	// �ֶ����ͺ��ֶ���
	myVector<string> memberStrList;
	split(line.c_str(), " ", memberStrList);
	if (findString(memberStrList[0].c_str(), "(utf8)", NULL))
	{
		strReplaceAll(memberStrList[0], "(utf8)", "");
		memberInfo.mUTF8 = true;
	}
	else
	{
		memberInfo.mUTF8 = false;
	}
	memberInfo.mTypeName = memberStrList[0];
	memberInfo.mMemberName = memberStrList[1];
	return memberInfo;
}

SQLiteMember CodeGenerator::parseSQLiteMemberLine(string line)
{
	SQLiteMember memberInfo;
	// ���ֶ����ڿͻ��˻��Ƿ�����
	int rectStartIndex = line.find_first_of('[');
	int rectEndIndex = line.find_first_of(']', rectStartIndex);
	if (rectStartIndex >= 0 && rectEndIndex >= 0)
	{
		string owner = line.substr(rectStartIndex, rectEndIndex - rectStartIndex + 1);
		if (owner == "[Client]")
		{
			memberInfo.mOwner = SQLITE_OWNER::CLIENT_ONLY;
		}
		else if (owner == "[Server]")
		{
			memberInfo.mOwner = SQLITE_OWNER::SERVER_ONLY;
		}
		else
		{
			memberInfo.mOwner = SQLITE_OWNER::BOTH;
		}
		line.erase(rectStartIndex, rectEndIndex - rectStartIndex + 1);
	}
	else
	{
		memberInfo.mOwner = SQLITE_OWNER::BOTH;
	}
	// ���ֶ������ı��
	int roundStartIndex = line.find_first_of('(');
	int roundEndIndex = line.find_first_of(')');
	if (roundStartIndex >= 0 && roundEndIndex >= 0)
	{
		memberInfo.mLinkTable = line.substr(roundStartIndex + 1, roundEndIndex - roundStartIndex - 1);
		line.erase(roundStartIndex, roundEndIndex - roundStartIndex + 1);
	}
	// �ֶ����ͺ��ֶ���
	myVector<string> memberStrList;
	split(line.c_str(), " ", memberStrList);
	memberInfo.mTypeName = memberStrList[0];
	memberInfo.mMemberName = memberStrList[1];
	return memberInfo;
}

PacketMember CodeGenerator::parseMemberLine(const string& line)
{
	PacketMember memberInfo;
	// �����Ա����
	if (line.find_first_of('[') != -1)
	{
		// ���������,�����ȴ���[]�ڲ��ĳ���
		int lengthMarcoStart = line.find_first_of('[');
		int lengthMarcoEnd = line.find_first_of(']');
		string lengthMacro = line.substr(lengthMarcoStart + 1, lengthMarcoEnd - lengthMarcoStart - 1);
		strReplaceAll(lengthMacro, " ", "");
		split(lengthMacro.c_str(), "*", memberInfo.mArrayLengthMacro);
		// ������������,���жϱ������ͺͱ�����
		string memberLine = line.substr(0, lengthMarcoStart);
		myVector<string> memberStrList;
		split(memberLine.c_str(), " ", memberStrList);
		memberInfo.mTypeName = memberStrList[0];
		strReplaceAll(memberInfo.mTypeName, "\t", "");
		memberInfo.mIsArray = true;
		memberInfo.mMemberName = memberStrList[1];
		// ֻ�������Ա�����Ż��е���������
		if (memberStrList.size() == 3)
		{
			memberInfo.mVariableLength = stringToBool(memberStrList[2]);
		}
		else
		{
			memberInfo.mVariableLength = true;
		}
	}
	// ��ͨ��Ա����
	else
	{
		myVector<string> memberStrList;
		split(line.c_str(), " ", memberStrList);
		if (memberStrList.size() != 2 && memberStrList.size() != 3)
		{
			ERROR("��Ա�����д���:" + line);
			return PacketMember();
		}
		memberInfo.mTypeName = memberStrList[0];
		strReplaceAll(memberInfo.mTypeName, "\t", "");
		memberInfo.mIsArray = false;
		memberInfo.mMemberName = memberStrList[1];
	}
	return memberInfo;
}

string CodeGenerator::packetNameToUpper(const string& packetName)
{
	// ���ݴ�д��ĸ���
	myVector<string> macroList;
	int length = packetName.length();
	const int prefixLength = 2;
	if (packetName.substr(0, prefixLength) != "CS" && packetName.substr(0, prefixLength) != "SC")
	{
		ERROR("����ǰ׺����");
		return "";
	}
	int lastIndex = prefixLength;
	// ��3��ʼ,��Ϊ��0,1����ǰ׺,��2��ʼ�ն��Ǵ�д,���ȡ�����ַ���
	for (int i = prefixLength + 1; i < length; ++i)
	{
		// �Ѵ�д��ĸΪ�ָ���
		if (packetName[i] >= 'A' && packetName[i] <= 'Z')
		{
			macroList.push_back(packetName.substr(lastIndex, i - lastIndex));
			lastIndex = i;
		}
	}
	macroList.push_back(packetName.substr(lastIndex, length - lastIndex));
	string headerMacro = packetName.substr(0, prefixLength);
	FOR_VECTOR_CONST(macroList)
	{
		headerMacro += "_" + toUpper(macroList[i]);
	}
	return headerMacro;
}

string CodeGenerator::nameToUpper(const string& sqliteName)
{
	// ���ݴ�д��ĸ���
	myVector<string> macroList;
	int length = sqliteName.length();
	int lastIndex = 0;
	// ��1��ʼ,��Ϊ��0��ʼ�ն��Ǵ�д,���ȡ�����ַ���
	for (int i = 1; i < length; ++i)
	{
		// �Ѵ�д��ĸΪ�ָ���
		if (sqliteName[i] >= 'A' && sqliteName[i] <= 'Z')
		{
			macroList.push_back(sqliteName.substr(lastIndex, i - lastIndex));
			lastIndex = i;
		}
	}
	macroList.push_back(sqliteName.substr(lastIndex, length - lastIndex));
	string headerMacro;
	FOR_VECTOR_CONST(macroList)
	{
		headerMacro += "_" + toUpper(macroList[i]);
	}
	return headerMacro;
}

string CodeGenerator::cppPushParamString(const PacketMember& memberInfo)
{
	string str;
	if (memberInfo.mIsArray)
	{
		string lengthMacro;
		uint macroCount = memberInfo.mArrayLengthMacro.size();
		FOR_I(macroCount)
		{
			lengthMacro += "GameDefine::" + memberInfo.mArrayLengthMacro[i];
			if (i != macroCount - 1)
			{
				lengthMacro += " * ";
			}
		}
		str = "\t\tpushParam(" + memberInfo.mMemberName + ", " + lengthMacro + ", " + boolToString(memberInfo.mVariableLength) + ");\\";
	}
	else
	{
		str = "\t\tpushParam(" + memberInfo.mMemberName + ");\\";
	}
	return str;
}

string CodeGenerator::cppMemberDeclareString(const PacketMember& memberInfo)
{
	string str;
	if (memberInfo.mIsArray)
	{
		string lengthMacro;
		uint macroCount = memberInfo.mArrayLengthMacro.size();
		FOR_I(macroCount)
		{
			lengthMacro += "GameDefine::" + memberInfo.mArrayLengthMacro[i];
			if (i != macroCount - 1)
			{
				lengthMacro += " * ";
			}
		}
		str = "\t" + memberInfo.mTypeName + " " + memberInfo.mMemberName + "[" + lengthMacro + "];\\";
	}
	else
	{
		str = "\t" + memberInfo.mTypeName + " " + memberInfo.mMemberName + ";\\";
	}
	return str;
}

string CodeGenerator::cSharpPushParamString(const PacketMember& memberInfo)
{
	string str;
	if (memberInfo.mIsArray)
	{
		str = "\t\tpushParam(" + memberInfo.mMemberName + ", " + boolToString(memberInfo.mVariableLength) + ");";
	}
	else
	{
		str = "\t\tpushParam(" + memberInfo.mMemberName + ");";
	}
	return str;
}

string CodeGenerator::cSharpMemberDeclareString(const PacketMember& memberInfo)
{
	// c#���治��char,ʹ��byte,Ҳû��ullong,ʹ��long
	string typeName = memberInfo.mTypeName;
	if (typeName == "char")
	{
		typeName = "byte";
	}
	else if (typeName == "ullong")
	{
		typeName = "long";
	}
	string str;
	if (memberInfo.mIsArray)
	{
		typeName = toUpper(typeName) + "S";
		string lengthMacro;
		uint macroCount = memberInfo.mArrayLengthMacro.size();
		FOR_I(macroCount)
		{
			lengthMacro += "GameDefine." + memberInfo.mArrayLengthMacro[i];
			if (i != macroCount - 1)
			{
				lengthMacro += " * ";
			}
		}
		str = "\tpublic " + typeName + " " + memberInfo.mMemberName + " = new " + typeName + "(" + lengthMacro + ");";
	}
	else
	{
		typeName = toUpper(typeName);
		str = "\tpublic " + typeName + " " + memberInfo.mMemberName + " = new " + typeName + "();";
	}
	return str;
}