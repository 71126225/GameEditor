#include "txDynamicMeshProperty.h"

txVector<std::string> txDynamicMeshPropertyDynamicMesh::mChoices;
txDynamicMeshPropertyDynamicMesh::txDynamicMeshPropertyDynamicMesh()
{
	mReceiverType = TOSTRING(txComponentDynamicMesh);
	mPropertyName = "DynamicMesh";
	mDescription = "��̬ģ��";
}
void txDynamicMeshPropertyDynamicMesh::setRealValue(const std::string& value, txComponentDynamicMesh* component)
{
	component->initEntityByFileName(P_MODEL_DYNAMIC_PATH + value, true, true);
}
std::string txDynamicMeshPropertyDynamicMesh::getRealValue(txComponentDynamicMesh* component)
{
	return component->getModelFileName();
}

//------------------------------------------------------------------------------------------------------------------------------------------------
txVector<std::string> txDynamicMeshPropertyAnimation::mChoices;
txDynamicMeshPropertyAnimation::txDynamicMeshPropertyAnimation()
{
	mReceiverType = TOSTRING(txComponentDynamicMesh);
	mPropertyName = "Animation";
	mDescription = "����";
}
void txDynamicMeshPropertyAnimation::setRealValue(const std::string& value, txComponentDynamicMesh* component)
{
	component->playAnimation(value, true);
}
std::string txDynamicMeshPropertyAnimation::getRealValue(txComponentDynamicMesh* component)
{
	return component->getCurAnimName();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
txDynamicMeshPropertyAnimationState::txDynamicMeshPropertyAnimationState()
{
	mReceiverType = TOSTRING(txComponentDynamicMesh);
	mPropertyName = "AnimationState";
	mDescription = "��������״̬";
	ADD_ENUM(PS_PLAY);
	ADD_ENUM(PS_PAUSE);
	ADD_ENUM(PS_STOP);
}
void txDynamicMeshPropertyAnimationState::setRealValue(const PLAY_STATE& value, txComponentDynamicMesh* component)
{
	component->setPlayState(value);
}
PLAY_STATE txDynamicMeshPropertyAnimationState::getRealValue(txComponentDynamicMesh* component)
{
	return component->getPlayState();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
txDynamicMeshPropertyAnimationLength::txDynamicMeshPropertyAnimationLength()
{
	mReceiverType = TOSTRING(txComponentDynamicMesh);
	mPropertyName = "AnimationLength";
	mDescription = "��������";
	mChildName[0] = "seconds time";
	mChildName[1] = "frame count";
	mChildDescribe[0] = "��ǰ������ʱ�䳤��";
	mChildDescribe[1] = "��ǰ������֡��";
	mReadOnly = true;
}
void txDynamicMeshPropertyAnimationLength::setRealValue(const VECTOR2& value, txComponentDynamicMesh* component)
{
	// ֻ������,��������
}
VECTOR2 txDynamicMeshPropertyAnimationLength::getRealValue(txComponentDynamicMesh* component)
{
	float animFrameLength = component->getCurAnimLength();
	float animTimeLength = animFrameLength / FRAMES_PER_SECOND;
	return VECTOR2(animTimeLength, animFrameLength);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
txDynamicMeshPropertyAnimationSpeed::txDynamicMeshPropertyAnimationSpeed()
{
	mReceiverType = TOSTRING(txComponentDynamicMesh);
	mPropertyName = "AnimationSpeed";
	mDescription = "���������ٶ�";
}
void txDynamicMeshPropertyAnimationSpeed::setRealValue(const float& value, txComponentDynamicMesh* component)
{
	component->setCurAnimSpeed(value);
}
float txDynamicMeshPropertyAnimationSpeed::getRealValue(txComponentDynamicMesh* component)
{
	return component->getCurAnimSpeed();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
txDynamicMeshPropertyAnimationClip::txDynamicMeshPropertyAnimationClip()
{
	mReceiverType = TOSTRING(txComponentDynamicMesh);
	mPropertyName = "AnimationClip";
	mDescription = "��������֡������";
	mChildName[0] = "start frame";
	mChildName[1] = "end frame";
}
void txDynamicMeshPropertyAnimationClip::setRealValue(const VECTOR2& value, txComponentDynamicMesh* component)
{
	component->setStartFrame((int)value.x);
	component->setEndFrame((int)value.y);
}
VECTOR2 txDynamicMeshPropertyAnimationClip::getRealValue(txComponentDynamicMesh* component)
{
	return VECTOR2(component->getStartFrame(), component->getEndFrame());
}