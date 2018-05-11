#ifndef _MATH_UTILITY_H_
#define _MATH_UTILITY_H_

#include "MathHeader.h"

namespace txMath
{
	const float PI = 3.141593f;
	const float MIN_DELTA = 0.00001f;

	// direction
	void getYawPitchFromDirection(txVector3 direction, float& yaw, float& pitch); // ���ݷ���õ�����Ǻ͸�����
	txMatrix3 getMatrix3FromDirection(txVector3 direction); // ���ݷ���õ���ת����
	txMatrix3 getMatrix3FromYawPitchRoll(float yaw, float pitch, float roll = 0.0f);
	txQuaternion getQuaternionFromDirection(txVector3 direction); // ���ݷ���õ���ת��Ԫ��
	txVector3 getDirectionFromYawPitch(float yaw, float pitch); // ���ݺ���Ǻ͸����ǵõ�����
	txVector3 getDirectionFromMatrix3(txMatrix3 mat); // ����3����ת����õ�����
	txVector3 getDirectionFromQuaternion(txQuaternion qua); // ������Ԫ���õ�����

	// angle
	void adjustAngle360(float& radianAngle);// �������ƵĽǶ�ֵ������0��3.14 * 2֮��
	void adjustAngle180(float& radianAngle);// �������ƵĽǶ�ֵ������-3.14��3.14֮��

	// zero
	void checkInt(float& value); // �жϴ���Ĳ����Ƿ��Ѿ��ӽ�������,����ӽ�������,������Ϊ����
	bool isFloatZero(float value); // �жϴ���Ĳ����Ƿ����0

	// matrix3
	txMatrix3 operator * (const txMatrix3& m1, const txMatrix3& m2);
	txVector3 operator * (const txVector3& v1, const txMatrix3& m1);
	txMatrix3 getMatrix3FromYaw(float yaw); // ���ݻ����Ƶĺ���ǵĵ���ת����,��Y����������Y�Ḻ������ʱ��Ϊ��
	txMatrix3 getMatrix3FromPitch(float pitch); // ���ݻ����Ƶĸ����ǵĵ���ת����, ��X�Ḻ������X��������˳ʱ��Ϊ��
	txMatrix3 getMatrix3FromRoll(float roll); // ���ݻ����ƵĹ����ǵĵ���ת����, ��Z�Ḻ������Z��������˳ʱ��Ϊ��
	txQuaternion matrix3ToQuaternion(txMatrix3 mat3); // ����3�׾���õ���Ԫ��
	txMatrix3 quaternionToMatrix3(txQuaternion q); // ������Ԫ���õ�3�׾���

	// matrix4
	txMatrix4 operator * (const txMatrix4& m, float s);
	txMatrix4 operator * (const txMatrix4& m1, const txMatrix4& m2);
	txVector3 operator * (const txVector3& v1, const txMatrix4& m1);
	txMatrix4 getScaleMatrix4(txVector3 scale);
	txMatrix4 getTransformMatrix4(txVector3 transform);
	txMatrix4 getRotationMatrix4(float yaw, float pitch, float row);
	txMatrix4 getViewMatrix4RH(txVector3 eye, txVector3 center, txVector3 up);
	txMatrix4 getPerspectiveMatrix4RH(float fovy, float aspect, float zNear, float zFar);
	txMatrix4 getOrthoMatrix4(float left, float right, float bottom, float top, float zNear, float zFar);

	// quaternion
	txVector3 operator * (txVector3 v, txQuaternion q);
	float dot(txQuaternion q1, txQuaternion q2);
	txQuaternion operator / (txQuaternion q, float s);
	txQuaternion operator * (txQuaternion q, float s);
	txQuaternion operator * (float s, txQuaternion q);
	txQuaternion operator * (txQuaternion q1, txQuaternion q2);
	txQuaternion operator - (txQuaternion q2);
	txQuaternion operator + (txQuaternion q1, txQuaternion q2);
	float mix(float x, float y, float a);
	txQuaternion mix(txQuaternion x, txQuaternion y, float a);
	txQuaternion lerp(txQuaternion x, txQuaternion y, float a); // ���Բ�ֵ
	txQuaternion slerp(txQuaternion q1, txQuaternion q2, float a); // �������Բ�ֵ

	// vector2
	txVector2 operator + (txVector2 vec1, txVector2 vec2);
	txVector2 operator - (txVector2 vec1, txVector2 vec2);
	txVector2 operator * (txVector2 vec1, float scale);
	txVector2 operator / (txVector2 vec1, float scale);

	// vector3
	txVector3 operator + (txVector3 vec1, txVector3 vec2);
	txVector3 operator - (txVector3 vec1, txVector3 vec2);
	txVector3 operator * (txVector3 vec1, float scale);
	txVector3 operator / (txVector3 vec1, float scale);

	// vector4
	txVector4 operator + (const txVector4& v1, const txVector4& v2);
	txVector4 operator - (const txVector4& v1, const txVector4& v2);
}

#endif