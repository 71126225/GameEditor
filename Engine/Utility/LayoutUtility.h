#ifndef _LAYOUT_UTILITY_H_
#define _LAYOUT_UTILITY_H_

#include "txRect.h"
#include "txEngineBase.h"

class txTexture;
class LayoutManager;
class Window;
class LayoutUtility : public txEngineBase
{
public:
	LayoutUtility();
	void init(){}
	static WINDOW_TYPE getWindowTypeFromTypeName(const std::string& typeName)
	{
		auto iter = mTypeNameToWindowTypeMap.find(typeName);
		if (iter != mTypeNameToWindowTypeMap.end())
		{
			return iter->second;
		}
		return WT_MAX;
	}
	static const std::string& getTypeNameFromWindowType(const WINDOW_TYPE& type)
	{
		auto iter = mWindowTypeToTypeNameMap.find(type);
		if (iter != mWindowTypeToTypeNameMap.end())
		{
			return iter->second;
		}
		return EMPTY_STRING;
	}
	static VECTOR2 getWindowPosition(Window* pWindow);
	static VECTOR2 getRelativeWindowPosition(Window* pWindow); // �õ�����ڸ����ڵ���������
	static VECTOR2 getWindowSize(Window* pWindow);
	static txRect getWindowRect(Window* pWindow);
	static VECTOR3 getPickupRayDirection(const VECTOR2& screenPoint);	// ͨ����Ļ�����������ѡ�����߷���
	static const int& getScreenWidth();
	static const int& getScreenHeight();
	static VECTOR2 getScreenSize();
	// ����Ļ��������ת��Ϊ�������е������������,parentPositionʱ�����ڵ���Ļ����
	static VECTOR2 screenToLocalPosition(const VECTOR2& screenPixel, const VECTOR2& parentPosition) { return screenPixel - parentPosition; }
	// �õ���������Ļ�е���������
	static VECTOR2 getWindowPositionInScreen(Window* window);
	// ���㴰�ڵ���Ⱦ��������
	// isAbsoluteCenterΪ�����ʾtransformCenter�����з������Ǿ���ֵ,��������Ļ����Ϊԭ�������ϵ,������Դ��ڿ�߼������յı任����
	// Ϊ�����ʾtransformCenter��x,y�����ֵ,z�Ǿ���ֵ,����Դ��ڿ�߲����ϴ���λ�ü������յı任����
	// һ�������,��һ����������Ҫ��Ⱦ���ʱ,��ֵ����Ϊtrue,һ������ֻ��Ҫ��Ⱦһ��ʱ,��ֵ����Ϊfalse
	// directVerticesΪ�����ʾ�Ǽ���ֱ����Ⱦ����Ļ�ϵĶ�������,Ϊ�����ʾ�Ǽ�����ʵ����ά�ռ��е�һ��ƽ��Ķ�������
	static void generateVertices(GLfloat* vertices, const VECTOR2& windowPosition, const VECTOR2& windowSize, const MATRIX4& transform,
		const VECTOR3& transformCenter, const bool& isAbsoluteCenter, const float& fovy, const bool& directVertices);
	// ���㴰�ڵ���������
	static void generateTexCoord(txTexture* tex, GLfloat* texcoords, SYMMETRY_MODE& symmetryMode, const VECTOR2& texCoord, const VECTOR2& texSize);
	// �����߿������ϵ��������
	static void generateWireframeCoordinate(GLfloat* wireframeVertices, GLfloat* coordinateVertices, GLfloat* vertices);
	// ����UIƽ���Z�����,screenHeightʵ�����ӿڵĸ߶�,fovy�ǹ���ͶӰ����ʱ��fovy
	static float generateUIClipZ(const int& screenHeight, const float& fovy)
	{
		return screenHeight / 2.0f / std::tan(fovy / 2.0f);
	}
protected:
	static txMap<WINDOW_TYPE, std::string> mWindowTypeToTypeNameMap;
	static txMap<std::string, WINDOW_TYPE> mTypeNameToWindowTypeMap;
};

#endif