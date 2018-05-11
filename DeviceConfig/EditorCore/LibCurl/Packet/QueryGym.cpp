#include "QueryGym.h"
#include "json/json.h"
#include "json/reader.h"
#include "json/value.h"
#include "CommandHeader.h"
#include "EditorCore.h"
#include "GymInfo.h"

void QueryGym::write(std::string& stream)
{
	stream = "GymID=" + mRegisteCode;
}

void QueryGym::execute(const std::string& value)
{
	Json::Reader reader;
	Json::Value arrVal;

	// 0��ʾ�ɹ�,1��ʾ�޷����ӷ�����,2��ʾ���ִ���
	int result = 0;
	bool registed = false;
	std::string gymName;
	std::string registeDate;
	// ����JSON��ʽ����
	if (!reader.parse(value, arrVal) || !arrVal.isObject())
	{
		result = 1;
	}
	else
	{
		if (arrVal["result"].asString() == "success")
		{
			// ����ɹ��᷵�ؽ���������
			if (mEditorCore->getGymInfo()->mRegisteCode != arrVal["GymID"].asString())
			{
				result = 2;
			}
			else
			{
				registed = true;
				gymName = txUtility::UTF8ToANSI(arrVal["GymName"].asString());
				registeDate = arrVal["Date"].asString();
			}
		}
	}
	CommandGymInfoNotifyQueryRet cmdQueryRet(COMMAND_PARAM);
	cmdQueryRet.mResult = result;
	cmdQueryRet.mRegisted = registed;
	cmdQueryRet.mGymName = gymName;
	cmdQueryRet.mRegisteDate = registeDate;
	mCommandSystem->pushCommand(&cmdQueryRet, mEditorCore->getGymInfo());
}