#include "Utility.h"
#include "txMovableObject.h"
#include "txComponentHeader.h"

#include "CameraLinker.h"
#include "GameCamera.h"
#include "CameraLinkerSwitchLinear.h"
#include "CameraLinkerSwitchCircle.h"
#include "CameraLinkerSwitchAroundTarget.h"

void CameraLinker::update(float elapsedTime)
{
	if (mLinkObject != NULL)
	{
		if (mCurSwitch != NULL)
		{
			mCurSwitch->update(elapsedTime);
		}
	}
	txComponent::update(elapsedTime);
}

void CameraLinker::initSwitch()
{
	CameraLinkerSwitchLinear* lineSwitch = TRACE_NEW(CameraLinkerSwitchLinear, lineSwitch, CLS_LINEAR, this);
	mSwitchList.insert(lineSwitch->getType(), lineSwitch);
	CameraLinkerSwitchCircle* circleSwitch = TRACE_NEW(CameraLinkerSwitchCircle, circleSwitch, CLS_CIRCLE, this);
	mSwitchList.insert(circleSwitch->getType(), circleSwitch);
	CameraLinkerSwitchAroundTarget* roateCharacter = TRACE_NEW(CameraLinkerSwitchAroundTarget, roateCharacter, CLS_AROUND_TARGET, this);
	mSwitchList.insert(roateCharacter->getType(), roateCharacter);
}

void CameraLinker::destroySwitch()
{
	if (mSwitchList.size() == 0)
	{
		return;
	}
	auto iter = mSwitchList.begin();
	auto iterEnd = mSwitchList.end();
	FOR_STL(mSwitchList, ; iter != iterEnd; ++iter)
	{
		TRACE_DELETE(iter->second);
	}
	END_FOR_STL(mSwitchList);
	mSwitchList.clear();
	mCurSwitch = NULL;
}

CameraLinkerSwitch* CameraLinker::getSwitch(const CAMERA_LINKER_SWITCH& type)
{
	auto iter = mSwitchList.find(type);
	if (iter != mSwitchList.end())
	{
		return iter->second;
	}
	return NULL;
}

void CameraLinker::setRelativePosition(const VECTOR3& pos, const CAMERA_LINKER_SWITCH& switchType, const bool& useDefaultSwitchSpeed, const float& switchSpeed)
{
	// �����ʹ��ת����,��ֱ������λ��
	if (switchType == CLS_NONE)
	{
		mRelativePosition = pos;
	}
	// ���ʹ��ת����,�������Ӧ��ת����,���ò���
	else
	{
		mCurSwitch = getSwitch(switchType);
		// �Ҳ�����ֱ������λ��
		if (mCurSwitch == NULL)
		{
			mRelativePosition = pos;
		}
		else
		{
			// ���������Ĭ���ٶ�,��ʵ��ת������ǰ���ٶ�,�������µ��ٶ�
			if (!useDefaultSwitchSpeed)
			{
				mCurSwitch->init(mRelativePosition, pos, switchSpeed);
			}
			// ����ͽ�ת������ǰ���ٶ����ý�ȥ
			else
			{
				mCurSwitch->init(mRelativePosition, pos, mCurSwitch->getSwitchSpeed());
			}
		}
	}
}

void CameraLinker::lateUpdate(float elapsedTime)
{
	if (mLinkObject != NULL)
	{
		GameCamera* camera = static_cast<GameCamera*>(mComponentOwner);
		if (mLookatTarget)
		{
			//�����������
			camera->lookAt(mLinkObject->getPosition() + mLookatOffset);
		}
		if (mUseTargetRotation)
		{
			camera->setRotation(mLinkObject->getRotation());
		}
	}
	txComponent::lateUpdate(elapsedTime);
}