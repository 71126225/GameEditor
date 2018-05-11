#ifndef _TX_MATRIX4_H_
#define _TX_MATRIX4_H_

namespace txMath
{
	class txVector3;
	class txMatrix3;
	class txVector4;
	class txQuaternion;
	// �о���
	class txMatrix4
	{
	public:
		txMatrix4();
		txMatrix4(const txMatrix4& other);
		txMatrix4(txVector4 v1, txVector4 v2, txVector4 v3, txVector4 v4);
		~txMatrix4();
		void initWithMatrix3(txMatrix3 other);
		txMatrix3 toMatrix3() const; // ת��Ϊ3�׾���
		txQuaternion toQuaternion(); // ת��Ϊ��Ԫ��
		txVector4 getCol(int col) const;
		txVector4 getRow(int row) const;
		txMatrix4 inverse(); // �������
		txMatrix4 transpose(); // ��ת�þ���
		txVector3 getScale(); // �õ������е�����ֵ
		txMatrix3 getRotation(); // �õ������е���תֵ
		txVector3 getTransform(); // �õ������е�ƽ��ֵ
		void setScale(txVector3 scale); // ��������ֵ,���ı���תֵ��ƽ��ֵ
		void setRotation(txMatrix3 rotation); // ������תֵ,���ı�����ֵ��ƽ��ֵ
		void setTransform(txVector3 transform); // ����ƽ��ֵ,���ı�����ֵ����תֵ
		void scale(txVector3 scale); // ��ԭ�����ŵĻ������ٴ�����
		void rotate(txMatrix3 rotation); // ��ԭ����ת�Ļ������ٴ���ת
		void transform(txVector3 transform); // ��ԭ��ƽ�ƵĻ������ٴ�ƽ��

		txMatrix4& operator *= (const txMatrix4& other);
	public:
		float m[4][4];
		// ����������
		//------------------------------------
		// m[0][0], m[1][0], m[2][0], m[3][0],
		// m[0][1], m[1][1], m[2][1], m[3][1],
		// m[0][2], m[1][2], m[2][2], m[3][2],
		// m[0][3], m[1][3], m[2][3], m[3][3],
		//------------------------------------
		// ����m[0][3], m[1][3], m[2][3]Ϊλ��
	};
}

#endif