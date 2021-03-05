#include "CodeMySQL.h"

void CodeMySQL::generate()
{
	string cppDataPath = cppGamePath + "DataBase/MySQL/Data/";
	string cppTablePath = cppGamePath + "DataBase/MySQL/Table/";

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
		// ȥ�������Ʊ��,�ֺ�
		removeAll(line, '\t', ';');
		// ��Ա�����б���ʼ
		if (line == "{")
		{
			packetStart = true;
			string tableTitle = lines[i - 1];
			removeAll(tableTitle, '\t', ';');
			myVector<string> titleVector;
			split(tableTitle.c_str(), ":", titleVector);
			if (titleVector.size() != 2)
			{
				ERROR("mysql���ĸ�ʽ����: " + lines[i - 1]);
				return;
			}
			tempInfo.mMySQLClassName = titleVector[0];
			tempInfo.mMySQLTableName = titleVector[1];
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
	FOR_VECTOR_CONST(mysqlInfoList)
	{
		// ���ɴ����ļ�
		generateCppMySQLDataFile(mysqlInfoList[i], cppDataPath);
		generateCppMySQLTableFile(mysqlInfoList[i], cppTablePath);
	}
	// ��һ��Ŀ¼����MySQLHeader.h
	string totalHeaderPath = cppDataPath;
	if (endWith(totalHeaderPath, "/") || endWith(totalHeaderPath, "\\"))
	{
		totalHeaderPath = totalHeaderPath.substr(0, totalHeaderPath.length() - 1);
	}
	totalHeaderPath = getFilePath(totalHeaderPath) + "/";
	generateCppMySQLTotalHeaderFile(mysqlInfoList, totalHeaderPath);
	generateCppMySQLRegisteFile(mysqlInfoList, totalHeaderPath);
	generateStringDefineMySQL(mysqlInfoList, cppStringDefinePath);
	generateMySQLInstanceDeclare(mysqlInfoList, totalHeaderPath);
}

// ����MySQLData.h��MySQLData.cpp�ļ�
void CodeMySQL::generateCppMySQLDataFile(const MySQLInfo& mysqlInfo, string filePath)
{
	// ͷ�ļ�
	string header;
	string className = "MD" + mysqlInfo.mMySQLClassName;
	string headerMacro = "_MD" + nameToUpper(mysqlInfo.mMySQLClassName) + "_H_";
	line(header, "#ifndef " + headerMacro);
	line(header, "#define " + headerMacro);
	line(header, "");
	line(header, "#include \"MySQLData.h\"");
	line(header, "");
	line(header, "class " + className + " : public MySQLData");
	line(header, "{");
	line(header, "\tBASE_CLASS(MySQLData);");
	line(header, "public:");
	uint memberCount = mysqlInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		line(header, "\tstatic const char* " + mysqlInfo.mMemberList[i].mMemberName + ";");
	}
	line(header, "public:");
	FOR_I(memberCount)
	{
		line(header, "\t" + mysqlInfo.mMemberList[i].mTypeName + " m" + mysqlInfo.mMemberList[i].mMemberName + ";");
	}
	line(header, "public:");
	line(header, "\tstatic void fillColName(MySQLTable* table);");
	line(header, "\tvoid parseResult(myMap<const char*, char*>& resultRow) override;");
	line(header, "\tvoid paramList(char* params, uint size) const override;");
	line(header, "\tvoid resetProperty() override;");
	line(header, "};");
	line(header, "");
	line(header, "#endif", false);

	// Դ�ļ�
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	// �ֶξ�̬��������
	FOR_I(memberCount)
	{
		line(source, "const char* " + className + "::" + mysqlInfo.mMemberList[i].mMemberName + " = STR(" + mysqlInfo.mMemberList[i].mMemberName + ");");
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
	line(source, "void " + className + "::parseResult(myMap<const char*, char*>& resultRow)");
	line(source, "{");
	FOR_I(memberCount)
	{
		const string& typeName = mysqlInfo.mMemberList[i].mTypeName;
		const string& memberName = mysqlInfo.mMemberList[i].mMemberName;
		if (typeName == "int")
		{
			line(source, "\tparseInt(&m" + memberName + ", resultRow.get(" + memberName + ", NULL));");
		}
		else if (typeName == "uint")
		{
			line(source, "\tparseUInt(&m" + memberName + ", resultRow.get(" + memberName + ", NULL));");
		}
		else if (typeName == "byte")
		{
			line(source, "\tparseByte(&m" + memberName + ", resultRow.get(" + memberName + ", NULL));");
		}
		else if (typeName == "char")
		{
			line(source, "\tparseChar(&m" + memberName + ", resultRow.get(" + memberName + ", NULL));");
		}
		else if (typeName == "short")
		{
			line(source, "\tparseShort(&m" + memberName + ", resultRow.get(" + memberName + ", NULL));");
		}
		else if (typeName == "ushort")
		{
			line(source, "\tparseUShort(&m" + memberName + ", resultRow.get(" + memberName + ", NULL));");
		}
		else if (typeName == "float")
		{
			line(source, "\tparseFloat(&m" + memberName + ", resultRow.get(" + memberName + ", NULL));");
		}
		else if (typeName == "ullong")
		{
			line(source, "\tparseULLong(&m" + memberName + ", resultRow.get(" + memberName + ", NULL));");
		}
		else if (typeName == "string")
		{
			line(source, "\tparseString(&m" + memberName + ", resultRow.get(" + memberName + ", NULL));");
		}
	}
	line(source, "}");
	line(source, "");

	// paramList����
	line(source, "void " + className + "::paramList(char* params, uint size) const");
	line(source, "{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		string addComma = i != memberCount - 1 ? "true" : "false";
		if (typeName == "string")
		{
			line(source, "\tappendValueString(params, size, m" + memberName + ".c_str(), " + (memberInfo.mUTF8 ? "true" : "false") + ", " + addComma + ");");
		}
		else if (typeName == "int")
		{
			line(source, "\tappendValueInt(params, size, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "uint")
		{
			line(source, "\tappendValueInt(params, size, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "byte")
		{
			line(source, "\tappendValueInt(params, size, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "char")
		{
			line(source, "\tappendValueInt(params, size, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "short")
		{
			line(source, "\tappendValueInt(params, size, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "ushort")
		{
			line(source, "\tappendValueInt(params, size, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "float")
		{
			line(source, "\tappendValueFloat(params, size, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "ullong")
		{
			line(source, "\tappendValueULLong(params, size, m" + memberName + ", " + addComma + ");");
		}
	}
	line(source, "}");
	line(source, "");
	// resetProperty����
	line(source, "void " + className + "::resetProperty()");
	line(source, "{");
	line(source, "\tbase::resetProperty();");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "string")
		{
			line(source, "\tm" + memberName + ".clear();");
		}
		else if (typeName == "float")
		{
			line(source, "\tm" + memberName + " = 0.0f;");
		}
		else
		{
			line(source, "\tm" + memberName + " = 0;");
		}
	}
	line(source, "}", false);

	writeFile(filePath + className + ".h", ANSIToUTF8(header.c_str(), true));
	writeFile(filePath + className + ".cpp", ANSIToUTF8(source.c_str(), true));
}

// ����MySQLTable.h��MySQLTable.cpp�ļ�
void CodeMySQL::generateCppMySQLTableFile(const MySQLInfo& mysqlInfo, string filePath)
{
	string tableClassName = "MySQL" + mysqlInfo.mMySQLClassName;
	string dataClassName = "MD" + mysqlInfo.mMySQLClassName;

	// ͷ�ļ�
	string header;
	string headerMacro = "_MYSQL" + nameToUpper(mysqlInfo.mMySQLClassName) + "_H_";
	line(header, "#ifndef " + headerMacro);
	line(header, "#define " + headerMacro);
	line(header, "");
	line(header, "#include \"MySQLTable.h\"");
	line(header, "");
	line(header, "class " + tableClassName + " : public MySQLTable");
	line(header, "{");
	line(header, "\tBASE_CLASS(MySQLTable);");
	line(header, "public:");
	line(header, "\t" + tableClassName + "(const char* tableName)");
	line(header, "\t\t:MySQLTable(tableName) {}");
	line(header, "\tvoid init(MYSQL * mysql) override;");
	line(header, "protected:");
	line(header, "\tMySQLData* createData() override;");
	line(header, "protected:");
	line(header, "};");
	line(header, "");
	line(header, "#endif", false);

	// Դ�ļ�
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	line(source, "void " + tableClassName + "::init(MYSQL * mysql)");
	line(source, "{");
	line(source, "\tMySQLTable::init(mysql);");
	line(source, "\t" + dataClassName + "::fillColName(this);");
	line(source, "}");
	line(source, "");
	line(source, "MySQLData* " + tableClassName + "::createData()");
	line(source, "{");
	line(source, "\treturn mMySQLDataBase->createData<" + dataClassName + ">(NAME(" + dataClassName + "));");
	line(source, "}", false);

	writeFile(filePath + tableClassName + ".h", ANSIToUTF8(header.c_str(), true));
	writeFile(filePath + tableClassName + ".cpp", ANSIToUTF8(source.c_str(), true));
}

// MySQLHeader.h�ļ�
void CodeMySQL::generateCppMySQLTotalHeaderFile(const myVector<MySQLInfo>& mysqlList, string filePath)
{
	string str0;
	line(str0, "#ifndef _MYSQL_HEADER_H_");
	line(str0, "#define _MYSQL_HEADER_H_");
	line(str0, "");
	uint packetCount = mysqlList.size();
	FOR_I(packetCount)
	{
		line(str0, "#include \"MD" + mysqlList[i].mMySQLClassName + ".h\"");
	}
	line(str0, "");
	FOR_I(packetCount)
	{
		line(str0, "#include \"MySQL" + mysqlList[i].mMySQLClassName + ".h\"");
	}
	line(str0, "");
	line(str0, "#endif", false);

	writeFile(filePath + "MySQLHeader.h", ANSIToUTF8(str0.c_str(), true));
}

// MySQLRegiste.h��MySQLRegiste.cpp�ļ�
void CodeMySQL::generateCppMySQLRegisteFile(const myVector<MySQLInfo>& mysqlList, string filePath)
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
	writeFile(filePath + "MySQLRegister.h", ANSIToUTF8(str0.c_str(), true));

	string str1;
	line(str1, "#include \"GameHeader.h\"");
	line(str1, "");
	line(str1, "#define REGISTE_MYSQL(classType, tableName) m##classType = mMySQLDataBase->registeTable<classType>(tableName);");
	line(str1, "");
	line(str1, "void MySQLRegister::registeAll()");
	line(str1, "{");
	uint count = mysqlList.size();
	FOR_I(count)
	{
		line(str1, "\tREGISTE_MYSQL(MySQL" + mysqlList[i].mMySQLClassName + ", \"" + mysqlList[i].mMySQLTableName + "\");");
	}
	line(str1, "}", false);
	writeFile(filePath + "MySQLRegister.cpp", ANSIToUTF8(str1.c_str(), true));
}

// StringDefineMySQL.h��StringDefineMySQL.cpp
void CodeMySQL::generateStringDefineMySQL(const myVector<MySQLInfo>& mysqlList, string filePath)
{
	// ͷ�ļ�
	string header;
	uint count = mysqlList.size();
	FOR_I(count)
	{
		line(header, stringDeclare("MD" + mysqlList[i].mMySQLClassName));
	}
	writeFile(filePath + "StringDefineMySQL.h", ANSIToUTF8(header.c_str(), true));

	// Դ�ļ�
	string source;
	line(source, "#include \"GameHeader.h\"");
	line(source, "");
	FOR_I(count)
	{
		line(source, stringDefine("MD" + mysqlList[i].mMySQLClassName));
	}
	writeFile(filePath + "StringDefineMySQL.cpp", ANSIToUTF8(source.c_str(), true));
}

// MySQLInstanceDeclare.h��MySQLInstanceDeclare.cpp
void CodeMySQL::generateMySQLInstanceDeclare(const myVector<MySQLInfo>& mysqlList, string filePath)
{
	// ͷ�ļ�
	string header;
	line(header, "// auto generated file, so it looks might be strange");
	line(header, "");
	uint count = mysqlList.size();
	FOR_I(count)
	{
		line(header, "static MySQL" + mysqlList[i].mMySQLClassName + "* mMySQL" + mysqlList[i].mMySQLClassName + ";");
	}
	writeFile(filePath + "MySQLInstanceDeclare.h", ANSIToUTF8(header.c_str(), true));

	string cpp;
	line(cpp, "// auto generated file, so it looks might be strange");
	line(cpp, "");
	line(cpp, "#include \"GameBase.h\"");
	line(cpp, "");
	FOR_I(count)
	{
		line(cpp, "MySQL" + mysqlList[i].mMySQLClassName + "* GameBase::mMySQL" + mysqlList[i].mMySQLClassName + ";");
	}
	writeFile(filePath + "MySQLInstanceDeclare.cpp", ANSIToUTF8(cpp.c_str(), true));
}