#include "CodeMySQL.h"

void CodeMySQL::generate()
{
	print("正在生成MySQL");
	// 解析模板文件
	// 整个文件是否已经开始解析
	bool fileStart = false;
	// 是否已经开始解析一个表格的数据体
	bool packetStart = false;
	// 是否已经开始解析一个表格中的索引
	bool indexStart = false;
	myVector<string> lines = openTxtFileLines("MySQL.txt");
	if (lines.size() == 0)
	{
		ERROR("未找到表格格式文件MySQL.txt");
		return;
	}
	myVector<MySQLInfo> gameMySQLInfoList;
	myVector<MySQLInfo> gameCoreMySQLInfoList;
	MySQLInfo tempInfo;
	FOR_VECTOR(lines)
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
		// 去除所有制表符,分号
		removeAll(line, '\t', ';');
		// 成员变量列表起始
		if (line == "{")
		{
			indexStart = false;
			packetStart = true;
			string comment;
			// 表格注释,只取一行,不支持多行
			if (i >= 2 && startWith(lines[i - 2], "//"))
			{
				comment = lines[i - 2].substr(strlen("//"));
				removeStartAll(comment, ' ');
			}
			string tableTitle = lines[i - 1];
			removeAll(tableTitle, '\t', ';');
			myVector<string> titleVector;
			split(tableTitle.c_str(), ":", titleVector);
			if (titleVector.size() != 2)
			{
				ERROR("mysql表格的格式错误: " + lines[i - 1]);
				return;
			}
			string mysqlName = titleVector[1];
			MYSQL_SERVER_OWNER owner = MYSQL_SERVER_OWNER::NONE;
			if (findSubstr(mysqlName, "[Game]"))
			{
				if (owner != MYSQL_SERVER_OWNER::NONE)
				{
					ERROR("不能重复设置MySQL标签," + mysqlName);
				}
				owner = MYSQL_SERVER_OWNER::GAME;
			}
			if (findSubstr(mysqlName, "[GameCore]"))
			{
				if (owner != MYSQL_SERVER_OWNER::NONE)
				{
					ERROR("不能重复设置MySQL标签," + mysqlName);
				}
				owner = MYSQL_SERVER_OWNER::GAME_CORE;
			}
			if (owner == MYSQL_SERVER_OWNER::NONE)
			{
				ERROR("标签缺失," + mysqlName);
			}
			// 移除标签
			replaceAll(mysqlName, "[Game]", "");
			replaceAll(mysqlName, "[GameCore]", "");
			tempInfo.init(titleVector[0], mysqlName, comment, owner);
			continue;
		}
		// 成员变量列表结束
		if (line == "}")
		{
			if (tempInfo.mOwner == MYSQL_SERVER_OWNER::GAME)
			{
				gameMySQLInfoList.push_back(tempInfo);
			}
			else if (tempInfo.mOwner == MYSQL_SERVER_OWNER::GAME_CORE)
			{
				gameCoreMySQLInfoList.push_back(tempInfo);
			}
			packetStart = false;
			continue;
		}
		if (line == "index:")
		{
			indexStart = true;
			continue;
		}
		if (packetStart)
		{
			// 当前是在解析索引
			if (indexStart)
			{
				tempInfo.mIndexList.push_back(line);
			}
			// 当前是在解析列表的字段
			else
			{
				tempInfo.mMemberList.push_back(parseMySQLMemberLine(line));
			}
		}
	}
	// Game
	string cppGameDataPath = cppGamePath + "DataBase/MySQL/Data/";
	string cppGameTablePath = cppGamePath + "DataBase/MySQL/Table/";
	deleteFolder(cppGameDataPath);
	for (const MySQLInfo& info : gameMySQLInfoList)
	{
		// 生成代码文件
		generateCppMySQLDataFile(info, cppGameDataPath);
		generateCppMySQLTableFile(info, cppGameTablePath);
	}
	// 上一层目录生成MySQLHeader.h
	string totalHeaderGamePath = cppGameDataPath;
	removeEnd(totalHeaderGamePath, '/');
	totalHeaderGamePath = getFilePath(totalHeaderGamePath) + "/";
	const string gameBaseHeaderPath = cppGamePath + "Common/GameBase.h";
	const string gameBaseSourcePath = cppGamePath + "Common/GameBase.cpp";
	generateCppMySQLGameRegisteFile(gameMySQLInfoList, totalHeaderGamePath);
	myVector<string> newGameList;
	for (const MySQLInfo& info : gameMySQLInfoList)
	{
		newGameList.push_back("MD" + info.mMySQLClassName);
	}
	generateMySQLInstanceDeclare(gameMySQLInfoList, gameBaseHeaderPath);
	generateMySQLInstanceDefine(gameMySQLInfoList, gameBaseSourcePath);
	generateMySQLInstanceClear(gameMySQLInfoList, gameBaseSourcePath);

	// GameCore
	string cppGameCoreDataPath = cppGameCorePath + "DataBase/MySQL/Data/";
	string cppGameCoreTablePath = cppGameCorePath + "DataBase/MySQL/Table/";
	deleteFolder(cppGameCoreDataPath);
	for (const MySQLInfo& info : gameCoreMySQLInfoList)
	{
		// 生成代码文件
		generateCppMySQLDataFile(info, cppGameCoreDataPath);
		generateCppMySQLTableFile(info, cppGameCoreTablePath);
	}
	// 上一层目录生成MySQLHeader.h
	string totalHeaderGameCorePath = cppGameCoreDataPath;
	removeEnd(totalHeaderGameCorePath, '/');
	totalHeaderGameCorePath = getFilePath(totalHeaderGameCorePath) + "/";
	const string gameCoreBaseHeaderPath = cppGameCorePath + "Common/GameCoreBase.h";
	const string gameCoreBaseSourcePath = cppGameCorePath + "Common/GameCoreBase.cpp";
	generateCppMySQLGameCoreRegisteFile(gameCoreMySQLInfoList, totalHeaderGameCorePath);
	myVector<string> newCoreGameList;
	for (const MySQLInfo& info : gameCoreMySQLInfoList)
	{
		newCoreGameList.push_back("MD" + info.mMySQLClassName);
	}
	generateMySQLInstanceDeclare(gameCoreMySQLInfoList, gameCoreBaseHeaderPath);
	generateMySQLInstanceDefine(gameCoreMySQLInfoList, gameCoreBaseSourcePath);
	generateMySQLInstanceClear(gameCoreMySQLInfoList, gameCoreBaseSourcePath);
	print("完成生成MySQL");
	print("");
}

// 生成MySQLData.h和MySQLData.cpp文件
void CodeMySQL::generateCppMySQLDataFile(const MySQLInfo& mysqlInfo, const string& filePath)
{
	// 头文件
	string header;
	string className = "MD" + mysqlInfo.mMySQLClassName;
	line(header, "#pragma once");
	line(header, "");
	line(header, "#include \"MySQLData.h\"");
	line(header, "");
	line(header, "// " + mysqlInfo.mComment);
	if (mysqlInfo.mOwner == MYSQL_SERVER_OWNER::GAME)
	{
		line(header, "class " + className + " : public MySQLData");
	}
	else
	{
		line(header, "class MICRO_LEGEND_CORE_API " + className + " : public MySQLData");
	}
	line(header, "{");
	line(header, "\ttypedef MySQLData base;");
	line(header, "public:");
	line(header, "\tstatic const int ID;");
	const uint memberCount = mysqlInfo.mMemberList.size();
	FOR_I(memberCount)
	{
		line(header, "\tstatic const int " + mysqlInfo.mMemberList[i].mMemberName + ";");
	}

	line(header, "\tstatic const string Name_ID;");
	FOR_I(memberCount)
	{
		line(header, "\tstatic const string Name_" + mysqlInfo.mMemberList[i].mMemberName + ";");
	}
	line(header, "public:");
	FOR_I(memberCount)
	{
		string memberLine;
		const string& typeName = mysqlInfo.mMemberList[i].mTypeName;
		if (typeName == "int" || typeName == "uint" || typeName == "byte" || typeName == "char" || typeName == "short" || typeName == "ushort" || typeName == "llong")
		{
			memberLine = "\t" + mysqlInfo.mMemberList[i].mTypeName + " m" + mysqlInfo.mMemberList[i].mMemberName + " = 0;";
		}
		else if (typeName == "bool")
		{
			memberLine = "\t" + mysqlInfo.mMemberList[i].mTypeName + " m" + mysqlInfo.mMemberList[i].mMemberName + " = false;";
		}
		else if (typeName == "float")
		{
			memberLine = "\t" + mysqlInfo.mMemberList[i].mTypeName + " m" + mysqlInfo.mMemberList[i].mMemberName + " = 0.0f;";
		}
		else if (typeName == "string")
		{
			memberLine = "\t" + mysqlInfo.mMemberList[i].mTypeName + " m" + mysqlInfo.mMemberList[i].mMemberName + ";";
		}
		const uint tabCount = generateAlignTableCount(memberLine, 48);
		FOR_I(tabCount)
		{
			memberLine += '\t';
		}
		memberLine += "// " + mysqlInfo.mMemberList[i].mComment;
		line(header, memberLine);
	}
	line(header, "public:");
	line(header, "\tstatic void fillColName(MySQLTable* table);");
	line(header, "\tvoid parseResult(const HashMap<int, char*>& resultRow) override;");
	line(header, "\tvoid paramList(string& params) const override;");
	line(header, "\tvoid generateUpdate(string& params, ullong flag) const override;");
	line(header, "\tvoid clone(MySQLData* target) const override;");
	line(header, "\tvoid cloneWithFlag(MySQLData* target, ullong flag) const override;");
	line(header, "\tvoid resetProperty() override;");
	line(header, "\tbool updateBool(bool value, int index) override;");
	line(header, "\tbool updateInt(int value, int index) override;");
	line(header, "\tbool updateFloat(float value, int index) override;");
	line(header, "\tbool updateLLong(llong value, int index) override;");
	line(header, "\tbool updateString(const string& value, int index) override;");
	line(header, "\tbool hasBool(bool value, int index) override;");
	line(header, "\tbool hasInt(int value, int index) override;");
	line(header, "\tbool hasFloat(float value, int index) override;");
	line(header, "\tbool hasLLong(llong value, int index) override;");
	line(header, "\tbool hasString(const string& value, int index) override;");
	line(header, "\tbool getBool(int index) override;");
	line(header, "\tint getInt(int index) override;");
	line(header, "\tfloat getFloat(int index) override;");
	line(header, "\tllong getLLong(int index) override;");
	line(header, "\tconst string& getString(int index) override;");
	line(header, "};", false);

	// 源文件
	string source;
	line(source, "#include \"GameCoreHeader.h\"");
	line(source, "");
	// 字段静态变量定义
	line(source, "const int " + className + "::ID = 0;");
	FOR_I(memberCount)
	{
		line(source, "const int " + className + "::" + mysqlInfo.mMemberList[i].mMemberName + " = " + StringUtility::intToString(i + 1) + ";");
	}
	line(source, "const string " + className + "::Name_ID = \"ID\";");
	FOR_I(memberCount)
	{
		line(source, "const string " + className + "::Name_" + mysqlInfo.mMemberList[i].mMemberName + " = \"" + mysqlInfo.mMemberList[i].mMemberName + "\";");
	}
	// fillColName函数
	line(source, "");
	line(source, "void " + className + "::fillColName(MySQLTable* table)");
	line(source, "{");
	line(source, "\ttable->addColName(Name_ID);");
	FOR_I(memberCount)
	{
		line(source, "\ttable->addColName(Name_" + mysqlInfo.mMemberList[i].mMemberName + ");");
	}
	line(source, "}");
	line(source, "");
	// parseResult函数
	line(source, "void " + className + "::parseResult(const HashMap<int, char*>& resultRow)");
	line(source, "{");
	line(source, "\tparseLLong(mID, resultRow.get(ID, nullptr));");
	FOR_I(memberCount)
	{
		const string& typeName = mysqlInfo.mMemberList[i].mTypeName;
		const string& memberName = mysqlInfo.mMemberList[i].mMemberName;
		if (typeName == "int")
		{
			line(source, "\tparseInt(m" + memberName + ", resultRow.get(" + memberName + ", nullptr));");
		}
		else if (typeName == "uint")
		{
			line(source, "\tparseUInt(m" + memberName + ", resultRow.get(" + memberName + ", nullptr));");
		}
		else if (typeName == "bool")
		{
			line(source, "\tparseBool(m" + memberName + ", resultRow.get(" + memberName + ", nullptr));");
		}
		else if (typeName == "byte")
		{
			line(source, "\tparseByte(m" + memberName + ", resultRow.get(" + memberName + ", nullptr));");
		}
		else if (typeName == "char")
		{
			line(source, "\tparseChar(m" + memberName + ", resultRow.get(" + memberName + ", nullptr));");
		}
		else if (typeName == "short")
		{
			line(source, "\tparseShort(m" + memberName + ", resultRow.get(" + memberName + ", nullptr));");
		}
		else if (typeName == "ushort")
		{
			line(source, "\tparseUShort(m" + memberName + ", resultRow.get(" + memberName + ", nullptr));");
		}
		else if (typeName == "float")
		{
			line(source, "\tparseFloat(m" + memberName + ", resultRow.get(" + memberName + ", nullptr));");
		}
		else if (typeName == "llong")
		{
			line(source, "\tparseLLong(m" + memberName + ", resultRow.get(" + memberName + ", nullptr));");
		}
		else if (typeName == "string")
		{
			line(source, "\tparseString(m" + memberName + ", resultRow.get(" + memberName + ", nullptr));");
		}
	}
	line(source, "}");
	line(source, "");

	// paramList函数
	line(source, "void " + className + "::paramList(string& params) const");
	line(source, "{");
	line(source, "\tUtility::sqlAddLLong(params, mID, " + (memberCount > 0 ? string("true") : string("false")) + ");");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		string addComma = i != memberCount - 1 ? "true" : "false";
		if (typeName == "string")
		{
			if (memberInfo.mUTF8)
			{
				line(source, "\tUtility::sqlAddStringUTF8(params, m" + memberName + ", " + addComma + ");");
			}
			else
			{
				line(source, "\tUtility::sqlAddString(params, m" + memberName + ", " + addComma + ");");
			}
		}
		else if (typeName == "int")
		{
			line(source, "\tUtility::sqlAddInt(params, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "uint")
		{
			line(source, "\tUtility::sqlAddInt(params, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "bool")
		{
			line(source, "\tUtility::sqlAddInt(params, m" + memberName + " ? 1 : 0, " + addComma + ");");
		}
		else if (typeName == "byte")
		{
			line(source, "\tUtility::sqlAddInt(params, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "char")
		{
			line(source, "\tUtility::sqlAddInt(params, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "short")
		{
			line(source, "\tUtility::sqlAddInt(params, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "ushort")
		{
			line(source, "\tUtility::sqlAddInt(params, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "float")
		{
			line(source, "\tUtility::sqlAddFloat(params, m" + memberName + ", " + addComma + ");");
		}
		else if (typeName == "llong")
		{
			line(source, "\tUtility::sqlAddLLong(params, m" + memberName + ", " + addComma + ");");
		}
	}
	line(source, "}");
	line(source, "");

	// generateUpdate函数
	line(source, "void " + className + "::generateUpdate(string& params, const ullong flag) const");
	line(source, "{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		line(source, "\tif (Utility::hasBit(flag, " + memberName + "))");
		line(source, "\t{");
		if (typeName == "string")
		{
			if (memberInfo.mUTF8)
			{
				line(source, "\t\tUtility::sqlUpdateStringUTF8(params, Name_" + memberName + ", m" + memberName + ");");
			}
			else
			{
				line(source, "\t\tUtility::sqlUpdateString(params, Name_" + memberName + ", m" + memberName + ");");
			}
		}
		else if (typeName == "int")
		{
			line(source, "\t\tUtility::sqlUpdateInt(params, Name_" + memberName + ", m" + memberName + ");");
		}
		else if (typeName == "uint")
		{
			line(source, "\t\tUtility::sqlUpdateInt(params, Name_" + memberName + ", m" + memberName + ");");
		}
		else if (typeName == "bool")
		{
			line(source, "\t\tUtility::sqlUpdateBool(params, Name_" + memberName + ", m" + memberName + ");");
		}
		else if (typeName == "byte")
		{
			line(source, "\t\tUtility::sqlUpdateInt(params, Name_" + memberName + ", m" + memberName + ");");
		}
		else if (typeName == "char")
		{
			line(source, "\t\tUtility::sqlUpdateInt(params, Name_" + memberName + ", m" + memberName + ");");
		}
		else if (typeName == "short")
		{
			line(source, "\t\tUtility::sqlUpdateInt(params, Name_" + memberName + ", m" + memberName + ");");
		}
		else if (typeName == "ushort")
		{
			line(source, "\t\tUtility::sqlUpdateInt(params, Name_" + memberName + ", m" + memberName + ");");
		}
		else if (typeName == "float")
		{
			line(source, "\t\tUtility::sqlUpdateFloat(params, Name_" + memberName + ", m" + memberName + ");");
		}
		else if (typeName == "llong")
		{
			line(source, "\t\tUtility::sqlUpdateLLong(params, Name_" + memberName + ", m" + memberName + ");");
		}
		line(source, "\t}");
	}
	line(source, "\t// 去除最后的逗号");
	line(source, "\tconst int length = (int)params.length();");
	line(source, "\tif (length > 0)");
	line(source, "\t{");
	line(source, "\t\tparams.erase(length - 1);");
	line(source, "\t}");
	line(source, "}");
	line(source, "");

	// clone函数
	line(source, "void " + className + "::clone(MySQLData* target) const");
	line(source, "{");
	line(source, "\tbase::clone(target);");
	if (memberCount > 0)
	{
		line(source, "\tauto* targetData = static_cast<" + className + "*>(target);");
		FOR_I(memberCount)
		{
			const string& memberName = mysqlInfo.mMemberList[i].mMemberName;
			line(source, "\ttargetData->m" + memberName + " = m" + memberName + ";");
		}
	}
	line(source, "}");
	line(source, "");

	// cloneWithFlag函数
	line(source, "void " + className + "::cloneWithFlag(MySQLData* target, const ullong flag) const");
	line(source, "{");
	line(source, "\tbase::cloneWithFlag(target, flag);");
	if (memberCount > 0)
	{
		line(source, "\tauto* targetData = static_cast<" + className + "*>(target);");
		FOR_I(memberCount)
		{
			const string& memberName = mysqlInfo.mMemberList[i].mMemberName;
			line(source, "\tif (Utility::hasBit(flag, " + memberName + "))");
			line(source, "\t{");
			line(source, "\t\ttargetData->m" + memberName + " = m" + memberName + ";");
			line(source, "\t}");
		}
	}
	line(source, "}");
	line(source, "");

	// resetProperty函数
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
	line(source, "}");
	line(source, "");

	// updateBool函数
	line(source, "bool " + className + "::updateBool(const bool value, const int index)");
	line(source, "{");
	line(source, "\tbase::updateBool(value, index);");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "bool")
		{
			line(source, "\tcase " + memberName + ": m" + memberName + " = value; return true;");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// updateInt函数
	line(source, "bool " + className + "::updateInt(const int value, const int index)");
	line(source, "{");
	line(source, "\tbase::updateInt(value, index);");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "int" || typeName == "ushort" || typeName == "short" || typeName == "byte" || typeName == "char")
		{
			line(source, "\tcase " + memberName + ": m" + memberName + " = value; return true;");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// updateFloat函数
	line(source, "bool " + className + "::updateFloat(const float value, const int index)");
	line(source, "{");
	line(source, "\tbase::updateFloat(value, index);");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "float")
		{
			line(source, "\tcase " + memberName + ": m" + memberName + " = value; return true;");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// updateLLong函数
	line(source, "bool " + className + "::updateLLong(const llong value, const int index)");
	line(source, "{");
	line(source, "\tbase::updateLLong(value, index);");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "llong")
		{
			line(source, "\tcase " + memberName + ": m" + memberName + " = value; return true;");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// updateString函数
	line(source, "bool " + className + "::updateString(const string& value, const int index)");
	line(source, "{");
	line(source, "\tbase::updateString(value, index);");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "string")
		{
			line(source, "\tcase " + memberName + ": m" + memberName + " = value; return true;");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");

	// hasBool函数
	line(source, "bool " + className + "::hasBool(const bool value, const int index)");
	line(source, "{");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "bool")
		{
			line(source, "\tcase " + memberName + ": return m" + memberName + " == value;");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// hasInt函数
	line(source, "bool " + className + "::hasInt(const int value, const int index)");
	line(source, "{");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "int" || typeName == "ushort" || typeName == "short" || typeName == "byte" || typeName == "char")
		{
			line(source, "\tcase " + memberName + ": return m" + memberName + " == value;");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// hasFloat函数
	line(source, "bool " + className + "::hasFloat(const float value, const int index)");
	line(source, "{");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "float")
		{
			line(source, "\tcase " + memberName + ": return m" + memberName + " == value;");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// hasLLong函数
	line(source, "bool " + className + "::hasLLong(const llong value, const int index)");
	line(source, "{");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "llong")
		{
			line(source, "\tcase " + memberName + ": return m" + memberName + " == value;");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// hasString函数
	line(source, "bool " + className + "::hasString(const string& value, const int index)");
	line(source, "{");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "string")
		{
			line(source, "\tcase " + memberName + ": return m" + memberName + " == value;");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// getBool函数
	line(source, "bool " + className + "::getBool(const int index)");
	line(source, "{");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "bool")
		{
			line(source, "\tcase " + memberName + ": return m" + memberName + ";");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return false;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn false;");
	line(source, "}");
	line(source, "");

	// getInt函数
	line(source, "int " + className + "::getInt(const int index)");
	line(source, "{");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "int" || typeName == "ushort" || typeName == "short" || typeName == "byte" || typeName == "char")
		{
			line(source, "\tcase " + memberName + ": return m" + memberName + ";");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return 0;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn 0;");
	line(source, "}");
	line(source, "");

	// getFloat函数
	line(source, "float " + className + "::getFloat(const int index)");
	line(source, "{");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "float")
		{
			line(source, "\tcase " + memberName + ": return m" + memberName + ";");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return 0.0f;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn 0.0f;");
	line(source, "}");
	line(source, "");

	// getLLong函数
	line(source, "llong " + className + "::getLLong(const int index)");
	line(source, "{");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "llong")
		{
			line(source, "\tcase " + memberName + ": return m" + memberName + ";");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return 0;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn 0;");
	line(source, "}");
	line(source, "");

	// getString函数
	line(source, "const string& " + className + "::getString(const int index)");
	line(source, "{");
	line(source, "\tswitch (index)");
	line(source, "\t{");
	FOR_I(memberCount)
	{
		const MySQLMember& memberInfo = mysqlInfo.mMemberList[i];
		const string& typeName = memberInfo.mTypeName;
		const string& memberName = memberInfo.mMemberName;
		if (typeName == "string")
		{
			line(source, "\tcase " + memberName + ": return m" + memberName + ";");
		}
		else
		{
			line(source, "\tcase " + memberName + ": return FrameDefine::EMPTY;");
		}
	}
	line(source, "\t}");
	line(source, "\treturn FrameDefine::EMPTY;");
	line(source, "}", false);

	writeFile(filePath + className + ".h", ANSIToUTF8(header.c_str(), true));
	writeFile(filePath + className + ".cpp", ANSIToUTF8(source.c_str(), true));
}

// 生成MySQLTable.h和MySQLTable.cpp文件
void CodeMySQL::generateCppMySQLTableFile(const MySQLInfo& mysqlInfo, const string& filePath)
{
	string tableClassName = "MySQL" + mysqlInfo.mMySQLClassName;
	string dataClassName = "MD" + mysqlInfo.mMySQLClassName;

	// 头文件
	string header;
	line(header, "#pragma once");
	line(header, "");
	line(header, "#include \"MySQLTable.h\"");
	line(header, "");
	if (mysqlInfo.mOwner == MYSQL_SERVER_OWNER::GAME)
	{
		line(header, "class " + tableClassName + " : public MySQLTable");
	}
	else
	{
		line(header, "class MICRO_LEGEND_CORE_API " + tableClassName + " : public MySQLTable");
	}
	line(header, "{");
	line(header, "\ttypedef MySQLTable base;");
	line(header, "public:");
	line(header, "\texplicit " + tableClassName + "(const char* tableName);");
	line(header, "\tvoid init() override;");
	if (mysqlInfo.mIndexList.size() > 0)
	{
		line(header, "\tvoid lateInit() override;");
	}
	line(header, "\tMySQLData* createData() override;");
	line(header, "\tvoid createDataList(Vector<MySQLData*>& dataList, const int count) override;");
	line(header, "protected:");
	line(header, "};", false);

	// 源文件
	string source;
	line(source, "#include \"GameCoreHeader.h\"");
	line(source, "");
	line(source, tableClassName + "::" + tableClassName + "(const char* tableName):");
	line(source, "\tMySQLTable(tableName)");
	line(source, "{}");
	line(source, "");
	line(source, "void " + tableClassName + "::init()");
	line(source, "{");
	line(source, "\tbase::init();");
	line(source, "\t" + dataClassName + "::fillColName(this);");
	line(source, "}");
	line(source, "");
	if (mysqlInfo.mIndexList.size() > 0)
	{
		line(source, "void " + tableClassName + "::lateInit()");
		line(source, "{");
		line(source, "\tbase::lateInit();");
		for (const string& indexName : mysqlInfo.mIndexList)
		{
			line(source, "\texecuteNonQuery((string(\"ALTER TABLE \") + mTableName + \" ADD INDEX " + indexName + "(\" + " + dataClassName + "::Name_" + indexName + " + \")\").c_str(), false);");
		}
		line(source, "}");
		line(source, "");
	}
	line(source, "MySQLData* " + tableClassName + "::createData()");
	line(source, "{");
	line(source, "\treturn FrameBase::mMySQLDataPool->newClass<" + dataClassName + ">();");
	line(source, "}");
	line(source, "void " + tableClassName + "::createDataList(Vector<MySQLData*>& dataList, const int count)");
	line(source, "{");
	line(source, "\treturn FrameBase::mMySQLDataPool->newClassList<" + dataClassName + ">(dataList, count);");
	line(source, "}", false);

	writeFile(filePath + tableClassName + ".h", ANSIToUTF8(header.c_str(), true));
	writeFile(filePath + tableClassName + ".cpp", ANSIToUTF8(source.c_str(), true));
}

// MySQLRegiste.h和MySQLRegiste.cpp文件
void CodeMySQL::generateCppMySQLGameRegisteFile(const myVector<MySQLInfo>& mysqlList, const string& filePath)
{
	// MySQLRegiste.h
	string str0;
	line(str0, "#pragma once");
	line(str0, "");
	line(str0, "#include \"GameBase.h\"");
	line(str0, "");
	line(str0, "class GameMySQLRegister");
	line(str0, "{");
	line(str0, "public:");
	line(str0, "\tstatic void registeAll();");
	line(str0, "};", false);
	writeFile(filePath + "GameMySQLRegister.h", ANSIToUTF8(str0.c_str(), true));

	string str1;
	line(str1, "#include \"GameHeader.h\"");
	line(str1, "");
	line(str1, "void GameMySQLRegister::registeAll()");
	line(str1, "{");
	uint count = mysqlList.size();
	FOR_I(count)
	{
		const string& mysqlClassName = mysqlList[i].mMySQLClassName;
		line(str1, "\tGameBase::mMySQL" + mysqlClassName + " = FrameBase::mMySQLManager->registeTable<MySQL" + mysqlClassName + ">(\"" + mysqlList[i].mMySQLTableName + "\");");
	}
	line(str1, "}", false);
	writeFile(filePath + "GameMySQLRegister.cpp", ANSIToUTF8(str1.c_str(), true));
}

void CodeMySQL::generateCppMySQLGameCoreRegisteFile(const myVector<MySQLInfo>& mysqlList, const string& filePath)
{
	// MySQLRegiste.h
	string str0;
	line(str0, "#pragma once");
	line(str0, "");
	line(str0, "#include \"GameCoreBase.h\"");
	line(str0, "");
	line(str0, "class MICRO_LEGEND_CORE_API GameCoreMySQLRegister");
	line(str0, "{");
	line(str0, "public:");
	line(str0, "\tstatic void registeAll();");
	line(str0, "};", false);
	writeFile(filePath + "GameCoreMySQLRegister.h", ANSIToUTF8(str0.c_str(), true));

	string str1;
	line(str1, "#include \"GameCoreHeader.h\"");
	line(str1, "");
	line(str1, "void GameCoreMySQLRegister::registeAll()");
	line(str1, "{");
	uint count = mysqlList.size();
	FOR_I(count)
	{
		const string& mysqlClassName = mysqlList[i].mMySQLClassName;
		line(str1, "\tGameCoreBase::mMySQL" + mysqlClassName + " = FrameBase::mMySQLManager->registeTable<MySQL" + mysqlClassName + ">(\"" + mysqlList[i].mMySQLTableName + "\");");
	}
	line(str1, "}", false);
	writeFile(filePath + "GameCoreMySQLRegister.cpp", ANSIToUTF8(str1.c_str(), true));
}

void CodeMySQL::generateMySQLInstanceDeclare(const myVector<MySQLInfo>& mysqlList, const string& gameBaseHeaderFile)
{
	// 更新GameBase.h的特定部分
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(gameBaseHeaderFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// MySQL"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "};"); }))
	{
		return;
	}
	for (const MySQLInfo& info : mysqlList)
	{
		codeList.insert(++lineStart, "\tstatic MySQL" + info.mMySQLClassName + "* mMySQL" + info.mMySQLClassName + ";");
	}
	writeFile(gameBaseHeaderFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

void CodeMySQL::generateMySQLInstanceDefine(const myVector<MySQLInfo>& mysqlList, const string& gameBaseSourceFile)
{
	// GameBase的类名与文件名一致
	const string gameBaseClassName = getFileNameNoSuffix(gameBaseSourceFile, true);
	// 更新GameBase.cpp的特定部分
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(gameBaseSourceFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// MySQL Define"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "};"); }))
	{
		return;
	}
	for (const MySQLInfo& info : mysqlList)
	{
		codeList.insert(++lineStart, "MySQL" + info.mMySQLClassName + "* " + gameBaseClassName + "::mMySQL" + info.mMySQLClassName + ";");
	}
	writeFile(gameBaseSourceFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}

void CodeMySQL::generateMySQLInstanceClear(const myVector<MySQLInfo>& mysqlList, const string& gameBaseSourceFile)
{
	// 更新GameBase.cpp的特定部分
	myVector<string> codeList;
	int lineStart = -1;
	if (!findCustomCode(gameBaseSourceFile, codeList, lineStart,
		[](const string& codeLine) { return endWith(codeLine, "// MySQL Clear"); },
		[](const string& codeLine) { return codeLine.length() == 0 || findSubstr(codeLine, "};"); }))
	{
		return;
	}

	for (const MySQLInfo& info : mysqlList)
	{
		codeList.insert(++lineStart, "\tmMySQL" + info.mMySQLClassName + " = nullptr;");
	}
	writeFile(gameBaseSourceFile, ANSIToUTF8(codeListToString(codeList).c_str(), true));
}