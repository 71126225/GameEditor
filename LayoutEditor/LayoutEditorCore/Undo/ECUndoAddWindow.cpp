#include "ECUndoAddWindow.h"
#include "LayoutEditorCore.h"

void ECUndoAddWindow::undo()
{
	// ������Ӵ���,���ǽ���ӵĴ���ɾ��
	mLayoutEditorCore->deleteWindow(mWindowName);
}