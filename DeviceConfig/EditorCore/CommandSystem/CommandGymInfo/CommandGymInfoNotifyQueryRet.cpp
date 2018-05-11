#include "CommandHeader.h"
#include "GymInfo.h"
#include "EditorCore.h"

void CommandGymInfoNotifyQueryRet::execute()
{
	GymInfo* gymInfo = static_cast<GymInfo*>(mReceiver);
	if (mResult == 0)
	{
		gymInfo->mRegisted = mRegisted;
		gymInfo->mRegisteDate = mRegisteDate;
		gymInfo->mGymName = mGymName;
		// �ɹ���ע����д��ע���
		if (mRegisted)
		{
			EditorCoreUtility::writeRegistryValue(COMPANY_NAME, GAME_NAME, gymInfo->mRegisteCode, KEY_NAME);
		}
		mEditorCore->sendEvent(CET_GYM_QUERY_RET);
	}
}