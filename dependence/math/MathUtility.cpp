#include "MathUtility.h"

namespace txMath
{
	void getYawPitchFromDirection(txVector3 direction, float& yaw, float& pitch)
	{
		direction.normalize();
		// ���ȼ��㸩����,��������������X-Zƽ��ļн�,������Ϊ��,������Ϊ��
		pitch = asin(direction.y);

		// �ټ��㺽���,���������������X-Zƽ���ϵ�ͶӰ��Z��������ļн�,�������¿���˳ʱ��Ϊ��,��ʱ��Ϊ��
		txVector3 projectionXZ(direction.x, 0.0f, direction.z);
		float len = projectionXZ.length();
		// ���ͶӰ�ĳ���Ϊ0,���ʾ������Ϊ90�����-90��,�����Ϊ0
		if (isFloatZero(len))
		{
			yaw = 0.0f;
		}
		else
		{
			projectionXZ.normalize();
			yaw = acos(projectionXZ.dot(txVector3(0.0f, 0.0f, 1.0f)));
			// �жϺ���ǵ�����,���xΪ��,�����Ϊ��,���xΪ��,�����Ϊ��
			if (direction.x < 0.0f)
			{
				yaw = -yaw;
			}
		}
	}

	txMatrix3 getMatrix3FromDirection(txVector3 direction)
	{
		float yaw, pitch;
		getYawPitchFromDirection(direction, yaw, pitch);
		return getMatrix3FromYawPitchRoll(yaw, pitch);
	}

	txMatrix3 getMatrix3FromYawPitchRoll(float yaw, float pitch, float roll)
	{
		txMatrix3 yawMat = getMatrix3FromYaw(yaw);
		txMatrix3 pitchMat = getMatrix3FromPitch(pitch);
		txMatrix3 rollMat = getMatrix3FromRoll(roll);
		txMatrix3 ret = pitchMat * yawMat;
		ret = rollMat * ret;
		return ret;
	}

	txQuaternion GetQuaternionFromDirection(txVector3 direction)
	{
		txMatrix3 mat = getMatrix3FromDirection(direction);
		return matrix3ToQuaternion(mat);
	}

	txVector3 GetDirectionFromYawPitch(float yaw, float pitch)
	{
		// ���pitchΪ90�����-90��,��ֱ�ӷ�������,��ʱ���ۺ����Ϊ����,����������ֱ���ϻ�����ֱ����
		if (isFloatZero(pitch - PI / 2.0f))
		{
			return txVector3(0.0f, 1.0f, 0.0f);
		}
		else if (isFloatZero(pitch + PI / 2.0f))
		{
			return txVector3(0.0f, -1.0f, 0.0f);
		}
		else
		{
			txVector3 dir;
			dir.z = cos(yaw);
			dir.x = sin(yaw);
			if (yaw < -PI / 2.0f || yaw > PI / 2.0f)
			{
				dir.z = -dir.z;
			}
			dir.y = tan(pitch);
			dir.normalize();
			return dir;
		}
	}

	txVector3 getDirectionFromMatrix3(txMatrix3 mat)
	{
		txVector3 zDir(0.0f, 0.0f, 1.0f);
		txVector3 dir = zDir * mat;
		return dir;
	}

	txVector3 getDirectionFromQuaternion(txQuaternion qua)
	{
		txVector3 zDir(0.0f, 0.0f, 1.0f);
		txVector3 dir = zDir * qua;
		return dir;
	}

	void adjustAngle360(float& radianAngle)
	{
		// ���С��0,��ѭ������PI * 2
		while (radianAngle < 0.0f)
		{
			radianAngle += PI * 2.0f;
		}
		// �������PI * 2,��ѭ����ȥPI * 2
		while (radianAngle > PI * 2.0f)
		{
			radianAngle -= PI * 2.0f;
		}
	}

	void adjustAngle180(float& radianAngle)
	{
		// ���С��-PI,��ѭ������PI * 2
		while (radianAngle < -PI)
		{
			radianAngle += PI * 2.0f;
		}
		// �������PI,��ѭ����ȥPI * 2
		while (radianAngle > PI)
		{
			radianAngle -= PI * 2.0f;
		}
	}

	void checkInt(float& value)
	{
		// ���ж��Ƿ�Ϊ0
		if (isFloatZero(value))
		{
			value = 0.0f;
			return;
		}
		int intValue = (int)value;
		// ����0
		if (value > 0.0f)
		{
			// ���ԭֵ��ȥ����ֵС��0.5f,���ʾԭֵ���ܽӽ�������ֵ
			if (value - (float)intValue < 0.5f)
			{
				if (isFloatZero(value - intValue))
				{
					value = (float)intValue;
				}
			}
			// ���ԭֵ��ȥ����ֵ����0.5f, ���ʾԭֵ���ܽӽ�������ֵ+1
			else
			{
				if (isFloatZero(value - (intValue + 1)))
				{
					value = (float)(intValue + 1);
				}
			}
		}
		// С��0
		else if (value < 0.0f)
		{
			// ���ԭֵ��ȥ����ֵ�Ľ���ľ���ֵС��0.5f,���ʾԭֵ���ܽӽ�������ֵ
			if (abs(value - (float)intValue) < 0.5f)
			{
				if (isFloatZero(value - intValue))
				{
					value = (float)intValue;
				}
			}
			else
			{
				// ���ԭֵ��ȥ����ֵ�Ľ���ľ���ֵ����0.5f, ���ʾԭֵ���ܽӽ�������ֵ-1
				if (isFloatZero(value - (intValue - 1)))
				{
					value = (float)(intValue - 1);
				}
			}
		}

	}

	bool isFloatZero(float value)
	{
		return value >= -MIN_DELTA && value <= MIN_DELTA;
	}

	txMatrix3 operator * (const txMatrix3& m1, const txMatrix3& m2)
	{
		txVector3 thisRow0 = m1.getRow(0);
		txVector3 thisRow1 = m1.getRow(1);
		txVector3 thisRow2 = m1.getRow(2);
		txVector3 otherCol0 = m2.getCol(0);
		txVector3 otherCol1 = m2.getCol(1);
		txVector3 otherCol2 = m2.getCol(2);

		txMatrix3 temp;
		temp.m[0][0] = thisRow0.dot(otherCol0);
		temp.m[0][1] = thisRow1.dot(otherCol0);
		temp.m[0][2] = thisRow2.dot(otherCol0);
		temp.m[1][0] = thisRow0.dot(otherCol1);
		temp.m[1][1] = thisRow1.dot(otherCol1);
		temp.m[1][2] = thisRow2.dot(otherCol1);
		temp.m[2][0] = thisRow0.dot(otherCol2);
		temp.m[2][1] = thisRow1.dot(otherCol2);
		temp.m[2][2] = thisRow2.dot(otherCol2);
		return temp;
	}

	txVector3 operator * (const txVector3& v1, const txMatrix3& m1)
	{
		txVector3 temp;
		temp.x = v1.dot(m1.getRow(0));
		temp.y = v1.dot(m1.getRow(1));
		temp.z = v1.dot(m1.getRow(2));
		return temp;
	}

	txMatrix3 getMatrix3FromYaw(float yaw)
	{
		// �Ƚ��Ƕ����Ƶ�-180��180֮��
		adjustAngle180(yaw);

		// �������Y�᲻��,��תZ���X��,�ȼ���Z��,Ȼ���˵õ�X��
		txVector3 zAxis;
		zAxis.x = sin(yaw);
		checkInt(zAxis.x);
		zAxis.y = 0.0f;
		zAxis.z = cos(yaw);
		checkInt(zAxis.z);
		txVector3 yAxis(0.0f, 1.0f, 0.0f);
		txVector3 xAxis = yAxis.cross(zAxis);
		return txMatrix3(xAxis, yAxis, zAxis);
	}

	txMatrix3 getMatrix3FromPitch(float pitch)
	{
		// �Ƚ��Ƕ����Ƶ�-180��180֮��
		adjustAngle180(pitch);

		// ��������X�᲻��,��תZ���Y��,�ȼ���Y��,Ȼ���˵õ�Z��
		txVector3 yAxis;
		yAxis.x = 0.0f;
		yAxis.y = cos(pitch);
		checkInt(yAxis.y);
		yAxis.z = sin(-pitch);
		checkInt(yAxis.z);
		txVector3 xAxis(1.0f, 0.0f, 0.0f);
		txVector3 zAxis = xAxis.cross(yAxis);
		return txMatrix3(xAxis, yAxis, zAxis);
	}

	txMatrix3 getMatrix3FromRoll(float roll)
	{
		// �Ƚ��Ƕ����Ƶ�-180��180֮��
		adjustAngle180(roll);

		// ��������Z�᲻��,��תX���Y��,�ȼ���X��,Ȼ���˵õ�Y��
		txVector3 xAxis;
		xAxis.x = cos(roll);
		checkInt(xAxis.x);
		xAxis.y = sin(roll);
		checkInt(xAxis.y);
		xAxis.z = 0.0f;
		txVector3 zAxis(0.0f, 0.0f, 1.0f);
		txVector3 yAxis = zAxis.cross(xAxis);
		return txMatrix3(xAxis, yAxis, zAxis);
	}

	txQuaternion matrix3ToQuaternion(txMatrix3 mat3)
	{
		float fourXSquaredMinus1 = mat3.m[0][0] - mat3.m[1][1] - mat3.m[2][2];
		float fourYSquaredMinus1 = mat3.m[1][1] - mat3.m[0][0] - mat3.m[2][2];
		float fourZSquaredMinus1 = mat3.m[2][2] - mat3.m[0][0] - mat3.m[1][1];
		float fourWSquaredMinus1 = mat3.m[0][0] + mat3.m[1][1] + mat3.m[2][2];

		int biggestIndex = 0;
		float fourBiggestSquaredMinus1 = fourWSquaredMinus1;
		if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourXSquaredMinus1;
			biggestIndex = 1;
		}
		if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourYSquaredMinus1;
			biggestIndex = 2;
		}
		if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourZSquaredMinus1;
			biggestIndex = 3;
		}

		float biggestVal = sqrt(fourBiggestSquaredMinus1 + 1.0f) * 0.5f;
		float mult = 0.25f / biggestVal;

		txQuaternion Result;
		switch (biggestIndex)
		{
		case 0:
			Result.w = biggestVal;
			Result.x = (mat3.m[1][2] - mat3.m[2][1]) * mult;
			Result.y = (mat3.m[2][0] - mat3.m[0][2]) * mult;
			Result.z = (mat3.m[0][1] - mat3.m[1][0]) * mult;
			break;
		case 1:
			Result.w = (mat3.m[1][2] - mat3.m[2][1]) * mult;
			Result.x = biggestVal;
			Result.y = (mat3.m[0][1] + mat3.m[1][0]) * mult;
			Result.z = (mat3.m[2][0] + mat3.m[0][2]) * mult;
			break;
		case 2:
			Result.w = (mat3.m[2][0] - mat3.m[0][2]) * mult;
			Result.x = (mat3.m[0][1] + mat3.m[1][0]) * mult;
			Result.y = biggestVal;
			Result.z = (mat3.m[1][2] + mat3.m[2][1]) * mult;
			break;
		case 3:
			Result.w = (mat3.m[0][1] - mat3.m[1][0]) * mult;
			Result.x = (mat3.m[2][0] + mat3.m[0][2]) * mult;
			Result.y = (mat3.m[1][2] + mat3.m[2][1]) * mult;
			Result.z = biggestVal;
			break;
		}
		return Result;
	}

	txMatrix3 quaternionToMatrix3(txQuaternion q)
	{
		txMatrix3 Result;
		float qxx(q.x * q.x);
		float qyy(q.y * q.y);
		float qzz(q.z * q.z);
		float qxz(q.x * q.z);
		float qxy(q.x * q.y);
		float qyz(q.y * q.z);
		float qwx(q.w * q.x);
		float qwy(q.w * q.y);
		float qwz(q.w * q.z);

		Result.m[0][0] = 1 - 2 * (qyy + qzz);
		checkInt(Result.m[0][0]);
		Result.m[0][1] = 2 * (qxy + qwz);
		checkInt(Result.m[0][1]);
		Result.m[0][2] = 2 * (qxz - qwy);
		checkInt(Result.m[0][2]);

		Result.m[1][0] = 2 * (qxy - qwz);
		checkInt(Result.m[1][0]);
		Result.m[1][1] = 1 - 2 * (qxx + qzz);
		checkInt(Result.m[1][1]);
		Result.m[1][2] = 2 * (qyz + qwx);
		checkInt(Result.m[1][2]);

		Result.m[2][0] = 2 * (qxz + qwy);
		checkInt(Result.m[2][0]);
		Result.m[2][1] = 2 * (qyz - qwx);
		checkInt(Result.m[2][1]);
		Result.m[2][2] = 1 - 2 * (qxx + qyy);
		checkInt(Result.m[2][2]);
		Result.normalizeAxis();
		return Result;
	}

	txMatrix4 operator * (const txMatrix4& m, float s)
	{
		txMatrix4 mat4(m);
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				mat4.m[i][j] *= s;
			}
		}
		return mat4;
	}

	txMatrix4 operator * (const txMatrix4& m1, const txMatrix4& m2)
	{
		txVector4 SrcA0 = m1.getCol(0);
		txVector4 SrcA1 = m1.getCol(1);
		txVector4 SrcA2 = m1.getCol(2);
		txVector4 SrcA3 = m1.getCol(3);

		txVector4 SrcB0 = m2.getCol(0);
		txVector4 SrcB1 = m2.getCol(1);
		txVector4 SrcB2 = m2.getCol(2);
		txVector4 SrcB3 = m2.getCol(3);

		txVector4 ret0 = SrcA0 * SrcB0.x + SrcA1 * SrcB0.y + SrcA2 * SrcB0.z + SrcA3 * SrcB0.w;
		txVector4 ret1 = SrcA0 * SrcB1.x + SrcA1 * SrcB1.y + SrcA2 * SrcB1.z + SrcA3 * SrcB1.w;
		txVector4 ret2 = SrcA0 * SrcB2.x + SrcA1 * SrcB2.y + SrcA2 * SrcB2.z + SrcA3 * SrcB2.w;
		txVector4 ret3 = SrcA0 * SrcB3.x + SrcA1 * SrcB3.y + SrcA2 * SrcB3.z + SrcA3 * SrcB3.w;

		return txMatrix4(ret0, ret1, ret2, ret3);
	}

	txMatrix4 getScaleMatrix4(txVector3 scale)
	{
		txMatrix4 mat;
		mat.setScale(scale);
		return mat;
	}

	txMatrix4 getTransformMatrix4(txVector3 transform)
	{
		txMatrix4 mat;
		mat.setTransform(transform);
		return mat;
	}

	txMatrix4 getRotationMatrix4(float yaw, float pitch, float row)
	{
		txMatrix3 yawMatrix = getMatrix3FromYaw(yaw);
		txMatrix3 pitchMatrix = getMatrix3FromPitch(pitch);
		txMatrix3 rowMatrix = getMatrix3FromRoll(row);
		txMatrix4 mat;
		mat.initWithMatrix3(rowMatrix * pitchMatrix * yawMatrix);
		return mat;
	}

	txMatrix4 getViewMatrix4RH(txVector3 eye, txVector3 center, txVector3 up)
	{
		txVector3 f = center - eye;
		f.normalize();
		txVector3 s = f.cross(up);
		s.normalize();
		txVector3 u = s.cross(f);

		txMatrix4 Result;
		Result.m[0][0] = s.x;
		Result.m[1][0] = s.y;
		Result.m[2][0] = s.z;
		Result.m[0][1] = u.x;
		Result.m[1][1] = u.y;
		Result.m[2][1] = u.z;
		Result.m[0][2] = -f.x;
		Result.m[1][2] = -f.y;
		Result.m[2][2] = -f.z;
		Result.m[3][0] = -s.dot(eye);
		Result.m[3][1] = -u.dot(eye);
		Result.m[3][2] = f.dot(eye);
		return Result;
	}

	txMatrix4 getPerspectiveMatrix4RH(float fovy, float aspect, float zNear, float zFar)
	{
		float tanHalfFovy = tan(fovy / 2.0f);

		txMatrix4 Result;
		Result.m[0][0] = 1.0f / (aspect * tanHalfFovy);
		Result.m[1][1] = 1.0f / (tanHalfFovy);
		Result.m[2][2] = -(zFar + zNear) / (zFar - zNear);
		Result.m[2][3] = -1.0f;
		Result.m[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);
		return Result;
	}

	txMatrix4 getOrthoMatrix4(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		txMatrix4 Result;
		Result.m[0][0] = 2.0f / (right - left);
		Result.m[1][1] = 2.0f / (top - bottom);
		Result.m[2][2] = -2.0f / (zFar - zNear);
		Result.m[3][0] = -(right + left) / (right - left);
		Result.m[3][1] = -(top + bottom) / (top - bottom);
		Result.m[3][2] = -(zFar + zNear) / (zFar - zNear);
		return Result;
	}

	txVector3 operator * (const txVector3& v1, const txMatrix4& m1)
	{
		txVector4 temp(v1, 1.0f);
		txVector4 tempRet;
		tempRet.x = temp.dot(m1.getCol(0));
		tempRet.y = temp.dot(m1.getCol(1));
		tempRet.z = temp.dot(m1.getCol(2));
		tempRet.w = temp.dot(m1.getCol(3));
		return tempRet.toWVector3();
	}

	txVector3 operator * (txVector3 v, txQuaternion q)
	{
		txVector3 QuatVector(q.x, q.y, q.z);
		txVector3 uv = QuatVector.cross(v);
		txVector3 uuv = QuatVector.cross(uv);
		txVector3 ret = v + ((uv * q.w) + uuv) * 2.0f;
		checkInt(ret.x);
		checkInt(ret.y);
		checkInt(ret.z);
		return ret;
	}

	float dot(txQuaternion q1, txQuaternion q2)
	{
		return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
	}

	txQuaternion operator / (txQuaternion q, float s)
	{
		return txQuaternion(q.x / s, q.y / s, q.z / s, q.w / s);
	}

	txQuaternion operator * (txQuaternion q, float s)
	{
		return txQuaternion(q.x * s, q.y * s, q.z * s, q.w * s);
	}

	txQuaternion operator * (float s, txQuaternion q)
	{
		return q * s;
	}

	txQuaternion operator * (txQuaternion q1, txQuaternion q2)
	{
		q1 *= q2;
		return q1;
	}

	txQuaternion operator - (txQuaternion q)
	{
		return txQuaternion(-q.x, -q.y, -q.z, -q.w);
	}

	txQuaternion operator + (txQuaternion q1, txQuaternion q2)
	{
		return txQuaternion(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w);
	}

	float mix(float x, float y, float a)
	{
		return x + a * (y - x);
	}

	txQuaternion mix(txQuaternion x, txQuaternion y, float a)
	{
		float cosTheta = dot(x, y);

		// Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
		if (cosTheta > 1.0f)
		{
			// Linear interpolation
			return txQuaternion(
				mix(x.w, y.w, a),
				mix(x.x, y.x, a),
				mix(x.y, y.y, a),
				mix(x.z, y.z, a));
		}
		else
		{
			// Essential Mathematics, page 467
			float angle = acos(cosTheta);
			checkInt(angle);
			float value1 = sin(angle);
			checkInt(value1);
			float value2 = sin(a * angle);
			checkInt(value2);
			float value3 = sin((1.0f - a) * angle);
			checkInt(value3);
			return (value3 * x + value2 * y) / value1;
		}
	}

	txQuaternion lerp(txQuaternion q1, txQuaternion q2, float a)
	{
		return q1 * (1.0f - a) + (q2 * a);
	}

	txQuaternion slerp(txQuaternion q1, txQuaternion q2, float a)
	{
		txQuaternion q3 = q2;

		float cosTheta = dot(q1, q2);

		// If cosTheta < 0, the interpolation will take the long way around the sphere. 
		// To fix this, one quat must be negated.
		if (cosTheta < 0.0f)
		{
			q3 = -q2;
			cosTheta = -cosTheta;
		}

		// Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
		if (cosTheta > 1.0f)
		{
			// Linear interpolation
			return txQuaternion(
				mix(q1.w, q3.w, a),
				mix(q1.x, q3.x, a),
				mix(q1.y, q3.y, a),
				mix(q1.z, q3.z, a));
		}
		else
		{
			// Essential Mathematics, page 467
			float angle = acos(cosTheta);
			checkInt(angle);
			float value1 = sin(angle);
			checkInt(value1);
			float value2 = sin(a * angle);
			checkInt(value2);
			float value3 = sin((1.0f - a) * angle);
			checkInt(value3);
			return (value3 * q1 + value2 * q3) / value1;
		}
	}

	txVector2 operator + (txVector2 vec1, txVector2 vec2)
	{
		return txVector2(vec1.x + vec2.x, vec1.y + vec2.y);
	}

	txVector2 operator - (txVector2 vec1, txVector2 vec2)
	{
		return txVector2(vec1.x - vec2.x, vec1.y - vec2.y);
	}

	txVector2 operator * (txVector2 vec1, float scale)
	{
		return txVector2(vec1.x * scale, vec1.y * scale);
	}

	txVector2 operator / (txVector2 vec1, float scale)
	{
		return txVector2(vec1.x / scale, vec1.y / scale);
	}

	txVector3 operator + (txVector3 vec1, txVector3 vec2)
	{
		return txVector3(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
	}

	txVector3 operator - (txVector3 vec1, txVector3 vec2)
	{
		return txVector3(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z);
	}

	txVector3 operator * (txVector3 vec1, float scale)
	{
		return txVector3(vec1.x * scale, vec1.y * scale, vec1.z * scale);
	}

	txVector3 operator / (txVector3 vec1, float scale)
	{
		return txVector3(vec1.x / scale, vec1.y / scale, vec1.z / scale);
	}

	txVector4 operator + (const txVector4& v1, const txVector4& v2)
	{
		return txVector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
	}

	txVector4 operator - (const txVector4& v1, const txVector4& v2)
	{
		return txVector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
	}
}