#include "txComponentHeader.h"
#include "txAlphaProperty.h"
#include "txEntity.h"

void txComponentAlpha::initProperty()
{
	txComponentKeyFrame::initProperty();
	addProperty<txAlphaPropertyStartAlpha>();
	addProperty<txAlphaPropertyTargetAlpha>();
}

void txComponentAlpha::applyKeyFrame(const txVector<float>& value)
{
	if (value.size() == 0)
	{
		return;
	}
	if (mComponentOwner != NULL)
	{
		float alpha = mStartAlpha + (mTargetAlpha - mStartAlpha) * value[0];
		// ����ǵ�һ�����,��Ӱ�����е�ģ�����
		if (mParent == NULL)
		{
			// ȡ��ģ������б�,����͸����
			auto& componentList = mComponentOwner->getComponentsByBaseType(TOSTRING(txComponentMesh));
			auto iterDynamic = componentList.begin();
			auto iterDynamicEnd = componentList.end();
			FOR_STL(componentList, ; iterDynamic != iterDynamicEnd; ++iterDynamic)
			{
				txComponentMesh* componentMesh = static_cast<txComponentMesh*>(iterDynamic->second);
				if (componentMesh->isActive() && componentMesh->getEntity() != NULL)
				{
					componentMesh->getEntity()->setAlpha(alpha);
				}
			}
			END_FOR_STL(componentList);
		}
		// ���ӵ�и����,��ֻӰ�츸���
		else
		{
			if (mParent->getBaseType() == TOSTRING(txComponentMesh))
			{
				txComponentMesh* componentMesh = static_cast<txComponentMesh*>(mParent);
				if (componentMesh->isActive() && componentMesh->getEntity() != NULL)
				{
					componentMesh->getEntity()->setAlpha(alpha);
				}
			}
		}
	}
}