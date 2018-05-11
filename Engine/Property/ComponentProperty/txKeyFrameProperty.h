#ifndef _TX_KEY_FRAME_PROPERTY_H_
#define _TX_KEY_FRAME_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(txComponentKeyFrame)

// ���ļ�
class txKeyFramePropertyTrembling : public txComponentKeyFrameProperty<txStringProperty, std::string>
{
public:
	txKeyFramePropertyTrembling();
	virtual void setRealValue(const std::string& value, txComponentKeyFrame* component);
	virtual std::string getRealValue(txComponentKeyFrame* component);
	virtual txVector<std::string>* getChoices() { return &mChoices; }
	static void setChoices(txVector<std::string>& choices) { mChoices = choices; }
protected:
	static txVector<std::string> mChoices;
};

//--------------------------------------------------------------------------------------------------------------
// ���ٶ�
class txKeyFramePropertyOnceLength : public txComponentKeyFrameProperty<txFloatProperty, float>
{
public:
	txKeyFramePropertyOnceLength();
	virtual void setRealValue(const float& value, txComponentKeyFrame* component);
	virtual float getRealValue(txComponentKeyFrame* component);
};

//--------------------------------------------------------------------------------------------------------------
// �𶯷���
class txKeyFramePropertyAmplitude : public txComponentKeyFrameProperty<txFloatProperty, float>
{
public:
	txKeyFramePropertyAmplitude();
	virtual void setRealValue(const float& value, txComponentKeyFrame* component);
	virtual float getRealValue(txComponentKeyFrame* component);
};

//--------------------------------------------------------------------------------------------------------------
// ��ʱ��
class txKeyFramePropertyCurrentTime : public txComponentKeyFrameProperty<txFloatProperty, float>
{
public:
	txKeyFramePropertyCurrentTime();
	virtual void setRealValue(const float& value, txComponentKeyFrame* component);
	virtual float getRealValue(txComponentKeyFrame* component);
};

//--------------------------------------------------------------------------------------------------------------
// ��ƫ��
class txKeyFramePropertyOffset : public txComponentKeyFrameProperty<txFloatProperty, float>
{
public:
	txKeyFramePropertyOffset();
	virtual void setRealValue(const float& value, txComponentKeyFrame* component);
	virtual float getRealValue(txComponentKeyFrame* component);
};

//--------------------------------------------------------------------------------------------------------------
// �𶯲���״̬
class txKeyFramePropertyCurState : public txComponentKeyFrameProperty<txEnumProperty<PLAY_STATE>, PLAY_STATE>
{
public:
	txKeyFramePropertyCurState();
	virtual void setRealValue(const PLAY_STATE& value, txComponentKeyFrame* component);
	virtual PLAY_STATE getRealValue(txComponentKeyFrame* component);
};

//--------------------------------------------------------------------------------------------------------------
// ��ѭ��
class txKeyFramePropertyLoop : public txComponentKeyFrameProperty<txBoolProperty, bool>
{
public:
	txKeyFramePropertyLoop();
	virtual void setRealValue(const bool& value, txComponentKeyFrame* component);
	virtual bool getRealValue(txComponentKeyFrame* component);
};

//--------------------------------------------------------------------------------------------------------------
// ������һ��
class txKeyFramePropertyFullOnce : public txComponentKeyFrameProperty<txBoolProperty, bool>
{
public:
	txKeyFramePropertyFullOnce();
	virtual void setRealValue(const bool& value, txComponentKeyFrame* component);
	virtual bool getRealValue(txComponentKeyFrame* component);
};

#endif