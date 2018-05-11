#ifndef _TX_QUATERNION_H_
#define _TX_QUATERNION_H_

namespace txMath
{
	class txMatrix4;
	class txMatrix3;
	class txVector3;
	class txQuaternion
	{
	public:
		txQuaternion();
		txQuaternion(float fx, float fy, float fz, float fw);
		txQuaternion(float yaw, float pitch, float roll);
		txQuaternion(const txQuaternion& other);
		txQuaternion(const txMatrix3& other);
		txQuaternion(const txMatrix4& other);
		~txQuaternion();
		void initWithMatrix3(txMatrix3 mat3);
		txQuaternion& operator *= (const txQuaternion& other);
		txQuaternion& operator /= (float s);
		txQuaternion& operator *= (float s);
		txQuaternion& operator += (const txQuaternion& other);
		float length();
		void normalize();
		float getRadianYaw();
		float getRadianPitch();
		float getRadianRoll();
		txQuaternion cross(txQuaternion other);
		txQuaternion conjugate(); // ������Ԫ��
		txQuaternion inverse(); // ������Ԫ��
		txMatrix3 toMatrix3(); // ת��Ϊ3�׾���
	public:
		float x;
		float y;
		float z;
		float w;
	};
}

#endif