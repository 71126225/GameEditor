#include "txObject.h"
#include "txNode.h"
#include "txEntity.h"
#include "txEntityManager.h"
#include "txEngineRoot.h"

#include "ECAxis.h"
#include "ECEditor.h"
#include "txComponentHeader.h"

void ECAxis::initComponents()
{
	txComponentStaticMesh* staticModel = static_cast<txComponentStaticMesh*>(addComponent("axis", TOSTRING(txComponentStaticMesh)));
	staticModel->initEntityByFileName(P_MODEL_STATIC_PATH + "zuobiao.mesh", false, true);
	staticModel->setActive(false);
}

void ECAxis::update(float elapsedTime)
{
	txMovableObject::update(elapsedTime);
	if (mAttachTarget != NULL)
	{
		setPosition(mAttachTarget->getPosition(), false);
		setRotation(mAttachTarget->getRotation(), true);
	}
}

void ECAxis::attachTarget(ECEditor* editor)
{
	mAttachTarget = editor;
	// �Զ����ػ�����ʾʱ,û�йҽ��κα༭��ʱ,����������
	if (mAutoShow)
	{
		setVisible(mAttachTarget != NULL);
	}
}

void ECAxis::setVisible(bool visible)
{
	txComponent* component = getFirstComponentByBaseType(TOSTRING(txComponentMesh));
	if (component != NULL)
	{
		component->setActive(visible);
	}
}

bool ECAxis::isVisible()
{
	txComponent* component = getFirstComponentByBaseType(TOSTRING(txComponentMesh));
	if (component != NULL)
	{
		return component->isActive();
	}
	return false;
}

void ECAxis::setAutoShow(bool autoShow)
{
	mAutoShow = autoShow; 
	// �������Ϊ�Զ���ʾ,���жϵ�ǰ�Ƿ�Ӧ����ʾ
	if (mAutoShow)
	{
		setVisible(mAttachTarget != NULL);
	}
}