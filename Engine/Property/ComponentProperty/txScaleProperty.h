#ifndef _TX_SCALE_PROPERTY_H_
#define _TX_SCALE_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(txComponentScale)

//---------------------------------------------------------------------------------------------------------
// ��ʼ����ֵ
class txScalePropertyStartScale : public txComponentScaleProperty<txVector3Property, VECTOR3>
{
public:
	txScalePropertyStartScale();
	virtual void setRealValue(const VECTOR3& value, txComponentScale* component);
	virtual VECTOR3 getRealValue(txComponentScale* component);
};

//---------------------------------------------------------------------------------------------------------
// Ŀ������ֵ
class txScalePropertyTargetScale : public txComponentScaleProperty<txVector3Property, VECTOR3>
{
public:
	txScalePropertyTargetScale();
	virtual void setRealValue(const VECTOR3& value, txComponentScale* component);
	virtual VECTOR3 getRealValue(txComponentScale* component);
};

#endif