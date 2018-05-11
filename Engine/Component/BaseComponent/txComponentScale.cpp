#include "txComponentHeader.h"
#include "txScaleProperty.h"
#include "txNode.h"

void txComponentScale::initProperty()
{
	txComponentKeyFrame::initProperty();
	addProperty<txScalePropertyStartScale>();
	addProperty<txScalePropertyTargetScale>();
}

void txComponentScale::applyKeyFrame(const txVector<float>& value)
{
	VECTOR3 scale = mStartScale + (mTargetScale - mStartScale) * value[0];
	if (mParent == NULL)
	{
		txComponentTransform* transform = static_cast<txComponentTransform*>(mComponentOwner->getFirstActiveComponentByBaseType(TOSTRING(txComponentTransform)));
		if (transform != NULL)
		{
			transform->setScale(scale, true);
		}
	}
	// ���ӵ�и����,��ֻӰ�츸���
	else
	{
		if (mParent->getNode() != NULL)
		{
			mParent->getNode()->SetScale(scale, true);
		}
	}
}