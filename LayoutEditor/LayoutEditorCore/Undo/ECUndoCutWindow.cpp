#include "WindowHeader.h"
#include "Layout.h"
#include "WindowProperty.h"
#include "LayoutManager.h"

#include "ECUndoCutWindow.h"
#include "LayoutEditorCore.h"

void ECUndoCutWindow::undo()
{
	// �������д���,�������½����ڹҽӵ�ԭ���ĸ�������
	Layout* layout = mLayoutEditorCore->getCurLayout();
	if (layout == NULL)
	{
		return;
	}
	// ������
	Window* parentWindow = layout->getWindow(mOldParentName);
	mLayoutEditorCore->copyWindow(mWindowName, true, true);
	mLayoutEditorCore->pasteWindow(mOldParentName, mOldWindowPos);
}