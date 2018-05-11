#ifndef _CAMERA_LINKER_SWITCH_LINEAR_H_
#define _CAMERA_LINKER_SWITCH_LINEAR_H_

#include "CameraLinkerSwitch.h"

class CameraLinkerSwitchLinear : public CameraLinkerSwitch
{
public:
	CameraLinkerSwitchLinear(const CAMERA_LINKER_SWITCH& type, CameraLinker* parentLinker)
		:
		CameraLinkerSwitch(type, parentLinker),
		mMovedDistance(0.0f)
	{
		mSpeed = 7.0f;
	}
	virtual ~CameraLinkerSwitchLinear(){ destroy(); }
	virtual void init(const VECTOR3& origin, const VECTOR3& target, const float& speed);
	virtual void update(float elapsedTime);
	virtual void destroy()
	{
		mMovedDistance = 0.0f;
	}
protected:
	float mMovedDistance;
	float mDistance; // �˴�ת�����ܳ���,���ڱ��ⲻ��Ҫ�����������ظ�����
	VECTOR3 mDirection; // �˴�ת���ķ���,���ڱ��ⲻ��Ҫ�������ظ�����
};

#endif