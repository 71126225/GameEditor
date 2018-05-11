#ifndef _TX_PHYSICS_UTILITY_H_
#define _TX_PHYSICS_UTILITY_H_

#include "ode.h"
#include "txMathUtility.h"

class txPhysicsUtility
{
public:
	static VECTOR3 odeVector3ToGLMVector3(dVector3 odeVec3)
	{
		return VECTOR3((float)odeVec3[0], (float)odeVec3[1], (float)odeVec3[2]);
	}
	// ŷ����ת��Ϊode�����׾���
	static void eulerAngleToODEMatrix3(const VECTOR3& eulerAngle, dMatrix3 odeRot)
	{
		glmMatrix3ToODEMatrix3(txMath::eulerAngleToMatrix3(eulerAngle), odeRot);
	}
	// ������ָ������תָ���ǶȻ��һ��ode�����׾���
	static void axisAngleToODEMatrix3(const VECTOR3& axis, const float& angle, dMatrix3 rot)
	{
		dRFromAxisAndAngle(rot, axis.x, axis.y, axis.z, angle);
	}
	// glm����Ԫ��תode����Ԫ��
	static void glmQuatToODEQuat(const QUATERNION& quaternion, dQuaternion q)
	{
		q[0] = quaternion.w;
		q[1] = quaternion.x;
		q[2] = quaternion.y;
		q[3] = quaternion.z;
	}
	// ode����Ԫ��תglm����Ԫ��
	static QUATERNION odeQuatToGLMQuat(dQuaternion q)
	{
		QUATERNION quaternion;
		quaternion.w = (float)q[0];
		quaternion.x = (float)q[1];
		quaternion.y = (float)q[2];
		quaternion.z = (float)q[3];
		return quaternion;
	}
	// glm��������ת����תode��������ת����
	static void glmMatrix3ToODEMatrix3(const MATRIX3& rot, dMatrix3 odeRot)
	{
		odeRot[4 * 0 + 0] = rot[0].x;
		odeRot[4 * 1 + 0] = rot[0].y;
		odeRot[4 * 2 + 0] = rot[0].z;

		odeRot[4 * 0 + 1] = rot[1].x;
		odeRot[4 * 1 + 1] = rot[1].y;
		odeRot[4 * 2 + 1] = rot[1].z;

		odeRot[4 * 0 + 2] = rot[2].x;
		odeRot[4 * 1 + 2] = rot[2].y;
		odeRot[4 * 2 + 2] = rot[2].z;
	}
	// ode��������ת����תglm��������ת����
	static MATRIX3 odeMatrix3ToGLMMatrix3(dMatrix3 odeRot)
	{
		MATRIX3 rot;
		rot[0].x = (float)odeRot[4 * 0 + 0];
		rot[0].y = (float)odeRot[4 * 1 + 0];
		rot[0].z = (float)odeRot[4 * 2 + 0];

		rot[1].x = (float)odeRot[4 * 0 + 1];
		rot[1].y = (float)odeRot[4 * 1 + 1];
		rot[1].z = (float)odeRot[4 * 2 + 1];

		rot[2].x = (float)odeRot[4 * 0 + 2];
		rot[2].y = (float)odeRot[4 * 1 + 2];
		rot[2].z = (float)odeRot[4 * 2 + 2];
		return rot;
	}
};

#endif