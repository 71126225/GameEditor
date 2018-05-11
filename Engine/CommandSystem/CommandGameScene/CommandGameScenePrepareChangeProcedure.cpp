#include "txCommandHeader.h"
#include "GameScene.h"
#include "SceneProcedure.h"

void CommandGameScenePrepareChangeProcedure::execute()
{
	GameScene* gameScene = CMD_CAST<GameScene*>(mReceiver);
	// ׼��ʱ��������0
	SceneProcedure* curProcedure = gameScene->getCurSceneProcedure();
	if (mPrepareTime <= 0.0f)
	{
		ENGINE_ERROR("preapare time must be larger than 0!");
	}
	// ����׼����תʱ,�������ٴ�׼����ת
	else if (curProcedure->isPreparingExit())
	{
		ENGINE_ERROR("procedure is preparing to exit, can not prepare again!");
	}
	else
	{
		gameScene->prepareChangeProcedure(mProcedure, mPrepareTime, mIntent);
	}
}

std::string CommandGameScenePrepareChangeProcedure::showDebugInfo()
{
	COMMAND_DEBUG("procedure : %d, prepare time : %.2f, intent : %s", mProcedure, mPrepareTime, mIntent.c_str());
}