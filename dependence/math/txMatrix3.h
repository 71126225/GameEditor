#ifndef _TX_MATRIX3_H_
#define _TX_MATRIX3_H_

namespace txMath
{
	class txVector3;
	class txMatrix4;
	// �о���
	class txMatrix3
	{
	public:
		txMatrix3();
		txMatrix3(const txMatrix3& other);
		txMatrix3(txVector3 xAxis, txVector3 yAxis, txVector3 zAxis);
		~txMatrix3();
		txMatrix3 inverse();
		void initWithMatrix4(txMatrix4 other);
		void normalizeAxis(); // ȥ�������е�����ֵ
		txVector3 getNormalizedAxisX() const; // �õ���λ���Ժ��X��
		txVector3 getNormalizedAxisY() const; // �õ���λ���Ժ��Y��
		txVector3 getNormalizedAxisZ() const; // �õ���λ���Ժ��Z��
		txVector3 getScale() const; // �õ������е�����ֵ
		void setScale(txVector3 scale); // ���þ����е�����ֵ
		void scale(txVector3 scale); // ���þ����е�����ֵΪԭ����ֵ���Դ��������ֵ
		txMatrix3 getNormalizedRotation(); // �õ������е���תֵ,����������
		txVector3 getCol(int col) const;
		txVector3 getRow(int row) const;

		txMatrix3& operator *= (const txMatrix3& other);
	public:
		float m[3][3];
		// ����������
		//------------------------------
		// m[0][0], m[1][0], m[2][0],
		// m[0][1], m[1][1], m[2][1],
		// m[0][2], m[1][2], m[2][2],
		//------------------------------
		// ����m[0][0], m[0][1], m[0][2]Ϊx��
	};
}

#endif