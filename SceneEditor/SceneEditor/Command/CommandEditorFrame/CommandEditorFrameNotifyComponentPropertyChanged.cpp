#include "PropertyHeader.h"

#include "EditorCoreRoot.h"
#include "ECScene.h"
#include "ECEditor.h"
#include "SceneEditorCore.h"

#include "EditorCommandHeader.h"
#include "EditorFrame.h"
#include "PropertyWindow.h"
#include "ComponentHierarchy.h"

void CommandEditorFrameNotifyComponentPropertyChanged::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	ECScene* scene = mSceneEditorCore->getCurScene();
	if (scene == NULL)
	{
		return;
	}
	ECEditor* editor = scene->getEditor(mEditorName);
	if (editor == NULL)
	{
		return;
	}
	// ������������Դ����иı�Ĵ�������,��ˢ�����Դ����е���ʾ
	if (mChanger != mPropertyWindow)
	{
		mPropertyWindow->showProperty(editor->getComponent(mComponentName));
	}
	// ������������״̬�ı���
	if (mPropertyName == txProperty::getPropertyName<txComponentPropertyActive>())
	{
		mComponentHierarchy->setActiveIcon(editor->getComponent(mComponentName));
	}
}

std::string CommandEditorFrameNotifyComponentPropertyChanged::showDebugInfo()
{
	COMMAND_DEBUG("editor name : %s, component name : %s, property name : %s, old value : %s, new value : %s", 
		mEditorName.c_str(), mComponentName.c_str(), mPropertyName.c_str(), mOldPropertyValue.c_str(), mPropertyValue.c_str());
}