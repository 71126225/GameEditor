#include "LayoutScriptAutoHide.h"
#include "Layout.h"
#include "LayoutTool.h"

void LayoutScriptAutoHide::update(const float& elapsedTime)
{
	LayoutScript::update(elapsedTime);
	// �Ѿ���ʾ���,������Ҫ�Զ�����ʱ�Ż��ʱ
	if (mShowDone && mCurTime >= 0.0f)
	{
		mCurTime += elapsedTime;
		if (mCurTime >= mAutoHideTime)
		{
			LayoutTool::HIDE_LAYOUT(mLayout->getLayoutID());
		}
	}
}

void LayoutScriptAutoHide::hideDone()
{
	mHideDone = true;
	mShowDone = false;
	LayoutTool::HIDE_LAYOUT_FORCE(mLayout->getLayoutID());
}