#include "txCommandHeader.h"
#include "GameScene.h"
#include "SceneProcedure.h"

void CommandGameSceneChangeProcedure::execute()
{
	GameScene* gameScene = CMD_CAST<GameScene*>(mReceiver);
	// ����������׼����ת����ʱ,����������ת
	SceneProcedure* curProcedure = gameScene->getCurSceneProcedure();
	if (curProcedure != NULL && curProcedure->isPreparingExit())
	{
		ENGINE_ERROR("procedure is preparing to change, can not change again!");
	}
	else
	{
		gameScene->changeProcedure(mProcedure, mIntent);
	}
}

std::string CommandGameSceneChangeProcedure::showDebugInfo()
{
	COMMAND_DEBUG("procedure : %d, intent : %s", mProcedure, mIntent.c_str());
}