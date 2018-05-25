#include "Layout.h"

#include "LayoutEditorCore.h"

#include "EditorFrame.h"
#include "LayoutHierarchy.h"
#include "PropertyWindow.h"
#include "LoadedLayout.h"
#include "EditorCommandHeader.h"

void CommandEditorFrameNotifyLayoutDestroied::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	// ������ٵ��ǵ�ǰ���ڱ༭�Ĳ���(Ϊ�����ʾ��ǰ�����Ѿ���������)
	if (mLayoutEditorCore->getCurLayout() == NULL || mLayoutName == mLayoutEditorCore->getCurLayout()->getName())
	{
		// ˢ��layout�Ĳ㼶��ͼ
		mLayoutHierarchy->displayLayout(NULL);
		// ������Դ���
		mPropertyWindow->clearProperty();
	}
	// ˢ���Ѽ��ز����б�
	mLoadedLayout->refreshList();
}

std::string CommandEditorFrameNotifyLayoutDestroied::showDebugInfo()
{
	COMMAND_DEBUG("layout name : %s", mLayoutName.c_str());
}