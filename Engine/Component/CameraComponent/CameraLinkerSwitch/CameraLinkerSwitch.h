#ifndef _CAMERA_LINKER_SWITCH_H_
#define _CAMERA_LINKER_SWITCH_H_

#include "txEngineDefine.h"

// �������������ת����,���ڴ���������������л�ʱ��ת���߼�,Ŀǰ�����ܸ��̶�������ʹ��
// ʵ���Ǽ�����¹̶������������λ��
class CameraLinker;
class CameraLinkerSwitch
{
public:
	CameraLinkerSwitch(const CAMERA_LINKER_SWITCH& type, CameraLinker* parentLinker)
		:
		mType(type),
		mParentLinker(parentLinker)
	{}
	virtual ~CameraLinkerSwitch(){ destroy(); }
	virtual void init(const VECTOR3& origin, const VECTOR3& target, const float& speed)
	{
		mOriginRelative = origin;
		mTargetRelative = target;
		mSpeed = speed;
	}
	virtual void update(float elapsedTime) = 0;
	virtual void destroy(){ mParentLinker = NULL; }
	void setOriginRelative(const VECTOR3& origin) { mOriginRelative = origin; }
	void setTargetRelative(const VECTOR3& target) { mTargetRelative = target; }
	void setSwitchSpeed(const float& speed) { mSpeed = speed; }
	const VECTOR3& getOriginRelative() { return mOriginRelative; }
	const VECTOR3& getTargetRelative() { return mTargetRelative; }
	const float& getSwitchSpeed() { return mSpeed; }
	const CAMERA_LINKER_SWITCH& getType() { return mType; }
	CameraLinker* getLinker() { return mParentLinker; }
protected:
	CAMERA_LINKER_SWITCH mType;
	CameraLinker* mParentLinker;
	VECTOR3 mOriginRelative;
	VECTOR3 mTargetRelative;
	// ת�������ٶ�,��ͬ��ת�����ٶȺ��岻ͬ
	// ֱ��ת������ֱ���ٶ�
	// ����ת�����ǽ��ٶ�
	// ��Ŀ����תת�����ǽ��ٶ�
	float mSpeed;
};

#endif