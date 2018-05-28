#include "WindowHeader.h"
#include "Layout.h"
#include "WindowProperty.h"
#include "LayoutManager.h"

#include "ECUndoDeleteWindow.h"
#include "LayoutEditorCore.h"

int ECUndoDeleteWindow::mDeleteLayoutCount = 0;

ECUndoDeleteWindow::~ECUndoDeleteWindow()
{
	if (mTempLayout != NULL)
	{
		mLayoutManager->destroyTempLayout(mTempLayout->getName());
	}
}

void ECUndoDeleteWindow::setWindow(Window* window)
{
	if (mWindow != NULL)
	{
		return;
	}
	mTempLayout = mLayoutManager->createTempLayout("UndoDeleteWindow" + txStringUtility::intToString(mDeleteLayoutCount++));
	mWindow = mTempLayout->createWindow(window->getTypeName(), window->getName());
	// �ȰѴ��ڵ��������Կ�������ʱ����,û�������κ�Ԥ��ʱ,�ſ����Ӵ���
	window->copyProperty(mWindow);
	if (!window->hasLinkedPrefab(false))
	{
		// Ȼ��Ѵ��ڵ������Ӵ��ڿ�������ʱ����
		window->copyChildren(mWindow, false, EMPTY_STRING);
	}
	if (window->getParent() != NULL)
	{
		mOldParentName = window->getParent()->getName();
		mOldWindowPos = window->getParent()->getChildPos(window);
	}
	if (window->getLayout() != NULL)
	{
		mOldLayoutName = window->getLayout()->getName();
	}
}

void ECUndoDeleteWindow::undo()
{
	// ����ɾ������,�������½����������ȥ
	// ���Ȳ�����������
	Layout* oldLayout = mLayoutManager->getLayout(mOldLayoutName);
	if (oldLayout == NULL)
	{
		return;
	}
	// ������
	Window* parentWindow = oldLayout->getWindow(mOldParentName);
	// ֱ�ӽ���ʱ������ӵ�ԭ���ĸ�������,��ʱ�����µ������Ӵ���Ҳ��һ�����
	mLayoutEditorCore->addWindow(oldLayout, parentWindow, mWindow, mOldWindowPos);
}