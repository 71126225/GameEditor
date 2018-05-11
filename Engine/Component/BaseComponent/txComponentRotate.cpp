#include "txComponentHeader.h"
#include "txRotateProperty.h"
#include "txNode.h"

void txComponentRotate::initProperty()
{
	txComponentKeyFrame::initProperty();
	addProperty<txRotatePropertyTargetAngle>();
	addProperty<txRotatePropertyStartAngle>();
}

void txComponentRotate::applyKeyFrame(const txVector<float>& value)
{
	VECTOR3 rot = mStartAngle + (mTargetAngle - mStartAngle) * value[0];
	MATRIX3 mat = txMath::eulerAngleToMatrix3(rot);
	// ����ǵ�һ�����,��Ӱ��任���
	if (mParent == NULL)
	{
		txComponentTransform* transform = static_cast<txComponentTransform*>(mComponentOwner->getFirstActiveComponentByBaseType(TOSTRING(txComponentTransform)));
		if (transform != NULL)
		{
			transform->setRotation(mat, true);
		}
	}
	// ���ӵ�и����,��ֻӰ�츸���
	else
	{
		if (mParent->getNode() != NULL)
		{
			mParent->getNode()->SetRotationMatrix(mat, true);
		}
	}
}