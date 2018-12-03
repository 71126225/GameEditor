﻿#include "SQLiteNPCFrame.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string NPCFrameData::COL_ID = "ID";
string NPCFrameData::COL_DIRECTION = "Direction";
string NPCFrameData::COL_ACTION = "Action";
string NPCFrameData::COL_FRAME_COUNT = "FrameCount";
string NPCFrameData::COL_POSX = "PosX";
string NPCFrameData::COL_POSY = "PosY";

void SQLiteNPCFrame::query(txVector<NPCFrameData>& dataList)
{
	string queryStr = "SELECT * FROM " + mTableName;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
bool SQLiteNPCFrame::insert(const NPCFrameData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}