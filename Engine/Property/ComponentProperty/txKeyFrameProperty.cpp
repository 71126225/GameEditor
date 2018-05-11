#include "txKeyFrameProperty.h"

txVector<std::string> txKeyFramePropertyTrembling::mChoices;
txKeyFramePropertyTrembling::txKeyFramePropertyTrembling()
{
	mPropertyName = "KeyFrameName";
	mDescription = "�ؼ�֡�ļ�";
}
void txKeyFramePropertyTrembling::setRealValue(const std::string& value, txComponentKeyFrame* component)
{
	component->setTrembling(value);
}
std::string txKeyFramePropertyTrembling::getRealValue(txComponentKeyFrame* component)
{
	return component->getTremblingName();
}

//--------------------------------------------------------------------------------------------------------------
txKeyFramePropertyOnceLength::txKeyFramePropertyOnceLength()
{
	mPropertyName = "KeyFrameSpeed";
	mDescription = "ִ��һ�ιؼ�֡��ʱ��";
}
void txKeyFramePropertyOnceLength::setRealValue(const float& value, txComponentKeyFrame* component)
{
	component->setOnceLength(value);
}
float txKeyFramePropertyOnceLength::getRealValue(txComponentKeyFrame* component)
{
	return component->getOnceLength();
}

//--------------------------------------------------------------------------------------------------------------
txKeyFramePropertyAmplitude::txKeyFramePropertyAmplitude()
{
	mPropertyName = "KeyFrameAmplitude";
	mDescription = "�ؼ�֡����";
}
void txKeyFramePropertyAmplitude::setRealValue(const float& value, txComponentKeyFrame* component)
{
	component->setAmplitude(value);
}
float txKeyFramePropertyAmplitude::getRealValue(txComponentKeyFrame* component)
{
	return component->getAmplitude();
}

//--------------------------------------------------------------------------------------------------------------
txKeyFramePropertyCurrentTime::txKeyFramePropertyCurrentTime()
{
	mPropertyName = "KeyFrameCurrentTime";
	mDescription = "��ǰ�ؼ�֡ʱ��";
}
void txKeyFramePropertyCurrentTime::setRealValue(const float& value, txComponentKeyFrame* component)
{
	component->setCurrentTime(value);
}
float txKeyFramePropertyCurrentTime::getRealValue(txComponentKeyFrame* component)
{
	return component->getCurrentTime();
}

//--------------------------------------------------------------------------------------------------------------
txKeyFramePropertyOffset::txKeyFramePropertyOffset()
{
	mPropertyName = "KeyFrameOffset";
	mDescription = "�ؼ�֡ƫ��";
}
void txKeyFramePropertyOffset::setRealValue(const float& value, txComponentKeyFrame* component)
{
	component->setOffset(value);
}
float txKeyFramePropertyOffset::getRealValue(txComponentKeyFrame* component)
{
	return component->getOffset();
}

//--------------------------------------------------------------------------------------------------------------
txKeyFramePropertyCurState::txKeyFramePropertyCurState()
{
	mPropertyName = "KeyFrameState";
	mDescription = "�ؼ�֡����״̬";
	ADD_ENUM(PS_PLAY);
	ADD_ENUM(PS_PAUSE);
	ADD_ENUM(PS_STOP);
}
void txKeyFramePropertyCurState::setRealValue(const PLAY_STATE& value, txComponentKeyFrame* component)
{
	component->setState(value);
}
PLAY_STATE txKeyFramePropertyCurState::getRealValue(txComponentKeyFrame* component)
{
	return component->getState();
}

//--------------------------------------------------------------------------------------------------------------
txKeyFramePropertyLoop::txKeyFramePropertyLoop()
{
	mPropertyName = "KeyFrameLoop";
	mDescription = "�ؼ�֡ѭ��";
}
void txKeyFramePropertyLoop::setRealValue(const bool& value, txComponentKeyFrame* component)
{
	component->setLoop(value);
}
bool txKeyFramePropertyLoop::getRealValue(txComponentKeyFrame* component)
{
	return component->getLoop();
}

//--------------------------------------------------------------------------------------------------------------
txKeyFramePropertyFullOnce::txKeyFramePropertyFullOnce()
{
	mPropertyName = "KeyFrameFullOnce";
	mDescription = "�ؼ�֡�Ƿ�������һ��";
}
void txKeyFramePropertyFullOnce::setRealValue(const bool& value, txComponentKeyFrame* component)
{
	component->setFullOnce(value);
}
bool txKeyFramePropertyFullOnce::getRealValue(txComponentKeyFrame* component)
{
	return component->getFullOnce();
}