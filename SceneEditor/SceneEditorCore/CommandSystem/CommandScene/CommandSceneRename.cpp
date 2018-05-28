#include "CoreCommandHeader.h"
#include "ECScene.h"
#include "SceneEditorCore.h"
#include "EditorCoreRoot.h"

void CommandSceneRename::execute()
{
	ECScene* scene = CMD_CAST<ECScene*>(mReceiver);
	// �˴�����ʹ������
	std::string oldName = scene->getName();
	bool result = scene->rename(mNewName);
	// ��������ɹ�,����Ҫ���Ͳ��ָ������¼�
	if (result)
	{
		txVector<std::string> params;
		params.push_back(oldName);
		params.push_back(mNewName);
		mEditorCoreRoot->sendEvent(CE_SCENE_RENAMED, params);

		// ��¼�����иı�
		mSceneEditorCore->setSceneModified(true);
	}
	if (mResult != NULL)
	{
		*mResult = result;
	}
}

std::string CommandSceneRename::showDebugInfo()
{
	COMMAND_DEBUG("new name : %s", mNewName.c_str());
}