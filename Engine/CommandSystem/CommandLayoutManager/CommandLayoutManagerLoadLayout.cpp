#include "txCommandHeader.h"
#include "LayoutManager.h"
#include "Layout.h"

void CommandLayoutManagerLoadLayout::execute()
{
	LayoutManager* layoutManager = CMD_CAST<LayoutManager*>(mReceiver);
	Layout* layout = layoutManager->createLayout(mLayoutID, mName, mRenderOrder, mName + ".json", mAsync, mCallback, mUserData);
	// ���سɹ�,����ֻ�з��첽����ʱ������������,��Ϊ�첽����ʱ���ֲ�û����ȫ������
	if (layout != NULL && !mAsync)
	{
		// ����ʱ������ʾ״̬����Ҫ������Ч��,��ʾʱ��������Ч,����ʱ��Ҫ����ǿ������,��֪ͨ�ű�
		if (mVisible)
		{
			layout->setVisible(mVisible, true, EMPTY_STRING);
		}
		else
		{
			layout->setVisibleForce(mVisible);
		}
	}
}

std::string CommandLayoutManagerLoadLayout::showDebugInfo()
{
	COMMAND_DEBUG("name : %s, layout id : %d, render order : %d, visible : %s", mName.c_str(), mLayoutID, mRenderOrder, txStringUtility::boolToString(mVisible).c_str());
}