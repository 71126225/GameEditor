#include "txComponentHeader.h"
#include "txNode.h"

void txComponentMove::applyKeyFrame(const txVector<float>& value)
{
	VECTOR3 pos = mStartPosition + (mTargetPosition - mStartPosition) * value[0];
	// ����ǵ�һ�����,��Ӱ��任���
	if (mParent == NULL)
	{
		txComponentTransform* transform = static_cast<txComponentTransform*>(mComponentOwner->getFirstActiveComponentByBaseType(TOSTRING(txComponentTransform)));
		if (transform != NULL)
		{
			transform->setPosition(pos);
		}
	}
	// ���ӵ�и����,��ֻӰ�츸���
	else
	{
		if (mParent->getNode() != NULL)
		{
			mParent->getNode()->SetPosition(pos);
		}
	}
}