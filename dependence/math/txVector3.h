#ifndef _TX_VECTOR3_H_
#define _TX_VECTOR3_H_

namespace txMath
{
	class txVector3
	{
	public:
		txVector3();
		txVector3(float fx, float fy, float fz);
		txVector3(const txVector3& other);
		~txVector3();
		txVector3& operator = (const txVector3& other);
		txVector3& operator += (const txVector3& other);
		txVector3& operator -= (const txVector3& other);
		txVector3& operator *= (float scale);
		txVector3& operator /= (float scale);
		float dot(txVector3 other) const; // ���
		txVector3 cross(txVector3 other) const; // ���
		float length() const;
		void normalize(); // ��λ������
		txVector3 product(txVector3 other) const; // ����ά�����ĸ��������ֱ����

	public:
		float x;
		float y;
		float z;
	};
}

#endif