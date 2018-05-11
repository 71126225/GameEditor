#ifndef _TX_SPRING_H_
#define _TX_SPRING_H_

#include "txEngineDefine.h"

// ����
class txSpring
{
public:
	txSpring();
	virtual ~txSpring(){}
	void update(float fElaspedTime);
	// �������� ���Ϊ������ѹ�����ɵķ���,Ϊ�����쵯�ɵķ���
	float calculateElasticForce()
	{
		float elasticForce = (mCurLength - mNormalLength) * mSpringK;
		return elasticForce;
	}
	void setNormaLength(const float& length) { mNormalLength = length; }
	void setMass(const float& mass) { mObjectMass = mass; }
	void setSpringk(const float& k) { mSpringK = k; }
	void setSpeed(const float& speed) { mObjectSpeed = speed; }
	void setForce(const float& force) { mForce = force; }
	void setCurLength(const float& length) { mCurLength = length; }
	const float& getSpeed(){ return mObjectSpeed; }
	const float& getLength() { return mCurLength; }
	const float& getNomalLength(){ return mNormalLength; }
protected:
	float mNormalLength;
	float mCurLength;
	float mSpringK;
	float mObjectMass;
	float mMinLength;
	// �����ٶ� ֻ������û�з���,�������������쵯�ɵķ���,��ֵѹ�����ɵķ���
	float mForce;
	float mObjectSpeed;
	float mPreAcce;
};

#endif