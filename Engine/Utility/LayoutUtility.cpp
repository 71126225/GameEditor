#include "txEngineRoot.h"
#include "txRenderWindow.h"
#include "txURect.h"
#include "txRendererInstance.h"
#include "txTexture.h"

#include "LayoutUtility.h"
#include "WindowHeader.h"
#include "LayoutManager.h"

txMap<WINDOW_TYPE, std::string> LayoutUtility::mWindowTypeToTypeNameMap;
txMap<std::string, WINDOW_TYPE> LayoutUtility::mTypeNameToWindowTypeMap;

LayoutUtility::LayoutUtility()
{
	mWindowTypeToTypeNameMap.insert(WT_PREFAB, PREFAB_TYPE_NAME);
	mWindowTypeToTypeNameMap.insert(WT_WINDOW, "Window");
	mWindowTypeToTypeNameMap.insert(WT_BUTTON, "Button");
	mWindowTypeToTypeNameMap.insert(WT_EDITBOX, "EditBox");
	mWindowTypeToTypeNameMap.insert(WT_CHECKBOX, "CheckBox");
	mWindowTypeToTypeNameMap.insert(WT_NUMBER_WINDOW, "NumberWindow");
	mWindowTypeToTypeNameMap.insert(WT_PROGRESS_WINDOW, "ProgressWindow");
	mWindowTypeToTypeNameMap.insert(WT_TEXTURE_WINDOW, "TextureWindow");
	mWindowTypeToTypeNameMap.insert(WT_TEXTURE_ANIM_WINDOW, "TextureAnimWindow");
	mWindowTypeToTypeNameMap.insert(WT_TEXT_WINDOW, "TextWindow");
	mWindowTypeToTypeNameMap.insert(WT_VIDEO_WINDOW, "VideoWindow");

	auto iterType = mWindowTypeToTypeNameMap.begin();
	auto iterTypeEnd = mWindowTypeToTypeNameMap.end();
	FOR_STL(mWindowTypeToTypeNameMap, ; iterType != iterTypeEnd; ++iterType)
	{
		mTypeNameToWindowTypeMap.insert(iterType->second, iterType->first);
	}
	END_FOR_STL(mWindowTypeToTypeNameMap);
	if (mTypeNameToWindowTypeMap.size() < WT_MAX)
	{
		ENGINE_ERROR("error : not all window type registered!");
	}
}

VECTOR2 LayoutUtility::getWindowPosition(Window* pWindow)
{
	if (pWindow == NULL)
	{
		return txMath::VEC2_ZERO;
	}
	else
	{
		VECTOR2 parentPosition = getWindowPosition(pWindow->getParent());
		VECTOR2 parentSize = getWindowSize(pWindow->getParent());
		const txPoint& windowPointPos = pWindow->getPosition();
		VECTOR2 position = txMath::pointToVector2(windowPointPos, parentSize) + parentPosition;
		return position;
	}
}

VECTOR2 LayoutUtility::getRelativeWindowPosition(Window* pWindow)
{
	VECTOR2 parentSize = getWindowSize(pWindow->getParent());
	const txPoint& windowPoint = pWindow->getPosition();
	VECTOR2 position = txMath::pointToVector2(windowPoint, parentSize);
	return position;
}

VECTOR2 LayoutUtility::getWindowSize(Window* pWindow)
{
	if (pWindow == NULL)
	{
		return getScreenSize();
	}
	else
	{
		VECTOR2 parentSize = getWindowSize(pWindow->getParent());
		txPoint windowSize = pWindow->getSize();
		// ���Զ�������,��ֱ�ӷ���ʵ�ʵĿ��
		if (pWindow->getAutoSizeType() == AST_NONE)
		{
			VECTOR2 size;
			size.x = parentSize.x * windowSize.x.mRelative + windowSize.x.mAbsolute;
			size.y = parentSize.y * windowSize.y.mRelative + windowSize.y.mAbsolute;
			return size;
		}
		// �Զ�����߶�
		else if (pWindow->getAutoSizeType() == AST_AUTO_HEIGHT)
		{
			VECTOR2 size;
			size.x = parentSize.x * windowSize.x.mRelative + windowSize.x.mAbsolute;
			const txDim& autoHeight = pWindow->getAutoHeight();
			size.y = size.x * autoHeight.mRelative + autoHeight.mAbsolute;
			return size;
		}
		// �Զ�������
		else if (pWindow->getAutoSizeType() == AST_AUTO_WIDTH)
		{
			VECTOR2 size;
			size.y = parentSize.y * windowSize.y.mRelative + windowSize.y.mAbsolute;
			const txDim& autoWidth = pWindow->getAutoWidth();
			size.x = size.y * autoWidth.mRelative + autoWidth.mAbsolute;
			return size;
		}
		return txMath::VEC2_ZERO;
	}
}

txRect LayoutUtility::getWindowRect(Window* pWindow)
{
	if (pWindow == NULL)
	{
		return txRect(txMath::VEC2_ZERO, getScreenSize());
	}
	else
	{
		txRect parentRect = getWindowRect(pWindow->getParent());
		const txURect& windowRect = pWindow->getRect();
		txRect rect;
		rect.mMin.x = parentRect.mMin.x * windowRect.mMin.x.mRelative + windowRect.mMin.x.mAbsolute;
		rect.mMin.y = parentRect.mMin.y * windowRect.mMin.y.mRelative + windowRect.mMin.y.mAbsolute;

		// ���Զ�������,��ֱ�ӷ���ʵ�ʵĿ��
		if (pWindow->getAutoSizeType() == AST_NONE)
		{
			rect.mMax.x = parentRect.mMax.x * windowRect.mMax.x.mRelative + windowRect.mMax.x.mAbsolute;
			rect.mMax.y = parentRect.mMax.y * windowRect.mMax.y.mRelative + windowRect.mMax.y.mAbsolute;
		}
		// �Զ�����߶�
		else if (pWindow->getAutoSizeType() == AST_AUTO_HEIGHT)
		{
			rect.mMax.x = parentRect.mMax.x * windowRect.mMax.x.mRelative + windowRect.mMax.x.mAbsolute;
			const txDim& autoHeight = pWindow->getAutoHeight();
			rect.mMax.y = rect.mMax.x * autoHeight.mRelative + autoHeight.mAbsolute;
		}
		// �Զ�������
		else if (pWindow->getAutoSizeType() == AST_AUTO_WIDTH)
		{
			rect.mMax.y = parentRect.mMax.y * windowRect.mMax.y.mRelative + windowRect.mMax.y.mAbsolute;
			const txDim& autoWidth = pWindow->getAutoWidth();
			rect.mMax.x = rect.mMax.y * autoWidth.mRelative + autoWidth.mAbsolute;
		}
		return rect;
	}
}

VECTOR3 LayoutUtility::getPickupRayDirection(const VECTOR2& screenPoint)
{
	// ����Ļ�����ת����-1~1֮��
	const int& renderWindowWdith = mRenderWindow->getWidth();
	const int& renderWindowHeight = mRenderWindow->getHeight();
	VECTOR2 relativeScreenPos = screenPoint;
	relativeScreenPos.x -= renderWindowWdith / 2.0f;
	relativeScreenPos.x /= renderWindowWdith / 2.0f;
	relativeScreenPos.y -= renderWindowHeight / 2.0f;
	relativeScreenPos.y /= renderWindowHeight / 2.0f;
	// Ȼ�󾭹�ͶӰ�������ͼ����������
	VECTOR3 worldPoint = txMath::transformVector3(VECTOR3(relativeScreenPos.x, relativeScreenPos.y, 1.0f), mLayoutManager->getInversePerspectiveMatrix(), true);
	worldPoint = txMath::transformVector3(worldPoint, mLayoutManager->getInverseUIViewMatrix(), true);
	return txMath::normalize(worldPoint);
}

const int& LayoutUtility::getScreenWidth()
{
	return mRenderWindow->getWidth();
}

const int& LayoutUtility::getScreenHeight()
{
	return mRenderWindow->getHeight();
}

VECTOR2 LayoutUtility::getScreenSize()
{
	return VECTOR2((float)getScreenWidth(), (float)getScreenHeight());
}

VECTOR2 LayoutUtility::getWindowPositionInScreen(Window* window)
{
	// ���Լ�û�и�����ʱ,�����Լ�����Ļ�е���������
	if (window->getParent() == NULL)
	{
		return getWindowPosition(window);
	}
	// ���Լ��и�����ʱ,���ȼ��㸸��������Ļ�е���������,Ȼ���ټ����Լ��ڸ������е���������
	else
	{
		return getWindowPositionInScreen(window->getParent()) + getWindowPosition(window);
	}
}

void LayoutUtility::generateVertices(GLfloat* vertices, const VECTOR2& windowPosition, const VECTOR2& windowSize, const MATRIX4& transform,
	const VECTOR3& transformCenter, const bool& isAbsoluteCenter, const float& fovy, const bool& directVertices)
{
	const int& screenWidth = mRenderWindow->getWidth();
	const int& screenHeight = mRenderWindow->getHeight();
	VECTOR3 point[UI_VERTEX_COUNT];
	VECTOR2 windowPos = windowPosition;
	if (directVertices)
	{
		// ����������ת��������Ļ����Ϊԭ�������
		windowPos -= VECTOR2(screenWidth / 2.0f, screenHeight / 2.0f);
		point[0] = VECTOR3(windowPos.x, windowPos.y, 0.0f);									// ��Ļ���½�
		point[1] = VECTOR3(windowPos.x + windowSize.x, windowPos.y, 0.0f);					// ��Ļ���½�
		point[2] = VECTOR3(windowPos.x + windowSize.x, windowPos.y + windowSize.y, 0.0f);	// ��Ļ���Ͻ�
		point[3] = VECTOR3(windowPos.x, windowPos.y + windowSize.y, 0.0f);					// ��Ļ���Ͻ�
		VECTOR3 wSize = point[2] - point[0];
		VECTOR3 windowCenter;
		// �������յı任����
		if (!isAbsoluteCenter)
		{
			// z�����Ǿ��Ե�ֵ,����ֻ�����
			windowCenter = point[0] + VECTOR3(wSize.x * transformCenter.x, wSize.y * transformCenter.y, wSize.z + transformCenter.z);
		}
		else
		{
			windowCenter = transformCenter;
		}
		for (int i = 0; i < UI_VERTEX_COUNT; ++i)
		{
			// ����Ĭ��ת���Դ�������Ϊԭ�������,Ȼ���������任,Ȼ����ת����Ļ���½�Ϊԭ�������,�������еı任�����Դ�������Ϊ׼
			point[i] = point[i] - windowCenter;
			point[i] = txMath::transformVector3(point[i], transform);
			point[i] = point[i] + windowCenter;
			// ���������ŵ�-1.0f��1.0f֮��
			point[i].x /= (screenWidth / 2.0f);
			point[i].y /= (screenHeight / 2.0f);
		}
	}
	else
	{
		// ����UIƽ�浽������ľ���
		float uiClipDis = -generateUIClipZ(screenHeight, fovy);
		point[0] = VECTOR3(windowPos.x, windowPos.y, uiClipDis); // ��Ļ���½�
		point[1] = VECTOR3(windowPos.x + windowSize.x, windowPos.y, uiClipDis);// ��Ļ���½�
		point[2] = VECTOR3(windowPos.x + windowSize.x, windowPos.y + windowSize.y, uiClipDis);// ��Ļ���Ͻ�
		point[3] = VECTOR3(windowPos.x, windowPos.y + windowSize.y, uiClipDis);// ��Ļ���Ͻ�
		// �Ƚ���������ƽ��(-width / 2.0f, -height / 2.0f),��֤���ڶ�����������ϵԭ��
		for (int i = 0; i < 4; ++i)
		{
			point[i] -= VECTOR3(screenWidth / 2.0f, screenHeight / 2.0f, 0.0f);
		}
		VECTOR3 wSize = point[2] - point[0];
		VECTOR3 curTransformCenter = transformCenter;
		// �������յı任����
		if (!isAbsoluteCenter)
		{
			// z�����Ǿ��Ե�ֵ,����ֻ�����
			curTransformCenter = VECTOR3(wSize.x * transformCenter.x, wSize.y * transformCenter.y, wSize.z + transformCenter.z);
		}
		VECTOR3 windowCenter = point[0] + curTransformCenter;
		for (int i = 0; i < UI_VERTEX_COUNT; ++i)
		{
			// ����Ĭ��ת���Դ�������Ϊԭ�������,Ȼ���������任,Ȼ����ת����Ļ���½�Ϊԭ�������,�������еı任�����Դ�������Ϊ׼
			point[i] = point[i] - windowCenter;
			point[i] = txMath::transformVector3(point[i], transform);
			point[i] = point[i] + windowCenter;
		}
	}
	// ��������
	for (int i = 0; i < UI_VERTEX_COUNT; ++i)
	{
		vertices[i * 3 + 0] = point[i].x;
		vertices[i * 3 + 1] = point[i].y;
		vertices[i * 3 + 2] = point[i].z;
	}
}

void LayoutUtility::generateTexCoord(txTexture* tex, GLfloat* texcoords, SYMMETRY_MODE& symmetryMode, const VECTOR2& texCoord, const VECTOR2& texSize)
{
	if (texcoords != NULL)
	{
		// ����ͼƬ�Ŀ��
		float texWidth = 1.0f, texHeight = 1.0f;
		// ������������ʹ�С
		VECTOR2 curTexCoord = texCoord;
		VECTOR2 curTexSize = texSize;
		if (tex == NULL)
		{
			curTexCoord = txMath::VEC2_ZERO;
			curTexSize = txMath::VEC2_ZERO;
			for (int i = 0; i < UI_TEXTURE_COORD_COUNT * 2; ++i)
			{
				texcoords[i] = 0.0f;
			}
		}
		else
		{
			texWidth = tex->getTextureSize().x;
			texHeight = tex->getTextureSize().y;

			// ���Ҿ���������ҿ���ʱ,����������겻��0,0���߿�߲�����ͼƬ���,���������Գƴ���
			if (symmetryMode == SM_LEFT_COPY_TO_RIGHT || symmetryMode == SM_LEFT_MIRROR_TO_RIGHT)
			{
				if (!txMath::isFloatZero(curTexCoord.x) || !txMath::isFloatZero(curTexCoord.y) ||
					!txMath::isFloatZero(curTexSize.x - texWidth) || !txMath::isFloatZero(curTexSize.y - texHeight))
				{
					symmetryMode = SM_NONE;
				}
			}
			// �����Գƴ���
			if (symmetryMode == SM_NONE)
			{
				int vertexIndex = 0;
				// ��Ļ���½�
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
				++vertexIndex;

				// ��Ļ���½�
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
				++vertexIndex;

				// ��Ļ���Ͻ�
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight;
				++vertexIndex;

				// ��Ļ���Ͻ�
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight;
			}
			// ����ߵ�ֱ�Ӹ��Ƶ��ұ߻���ߵľ����Ƶ��ұ�
			else if (symmetryMode == SM_LEFT_COPY_TO_RIGHT || symmetryMode == SM_LEFT_MIRROR_TO_RIGHT)
			{
				// ����ֻ�������ڵ�����
				int vertexIndex = 0;
				// ��Ļ���½�
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
				++vertexIndex;

				// ��Ļ���½�
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth * 2.0f;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
				++vertexIndex;

				// ��Ļ���Ͻ�
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth * 2.0f;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight;
				++vertexIndex;

				// ��Ļ���Ͻ�
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight;
			}
			// ˮƽ��ת
			else if (symmetryMode == SM_HORI_INVERSE)
			{
				int vertexIndex = 0;
				// ��Ļ���½�
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
				++vertexIndex;

				// ��Ļ���½�
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
				++vertexIndex;

				// ��Ļ���Ͻ�
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight;
				++vertexIndex;

				// ��Ļ���Ͻ�
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight;
			}
			// ��ֱ��ת
			else if (symmetryMode == SM_VERT_INVERSE)
			{
				int vertexIndex = 0;
				// ��Ļ���½�
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight;
				++vertexIndex;

				// ��Ļ���½�
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight;
				++vertexIndex;

				// ��Ļ���Ͻ�
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
				++vertexIndex;

				// ��Ļ���Ͻ�
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
			}
			else if (symmetryMode == SM_HORI_VERT_MIRROR)
			{
				int vertexIndex = 0;
				// ��Ļ���½�
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
				++vertexIndex;

				// ��Ļ���½�
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth * 2.0f;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y) / texHeight;
				++vertexIndex;

				// ��Ļ���Ͻ�
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth * 2.0f;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight * 2.0f;
				++vertexIndex;

				// ��Ļ���Ͻ�
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight * 2.0f;
			}
			else if (symmetryMode == SM_BOTTOM_COPY_TO_TOP || symmetryMode == SM_BOTTOM_MIRROR_TO_TOP)
			{
				// ����ֻ�������ڵ��°��
				int vertexIndex = 0;
				// ��Ļ���½�
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
				++vertexIndex;

				// ��Ļ���½�
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth;
				texcoords[vertexIndex * 2 + 1] = curTexCoord.y / texHeight;
				++vertexIndex;

				// ��Ļ���Ͻ�
				texcoords[vertexIndex * 2 + 0] = (curTexCoord.x + curTexSize.x) / texWidth;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight * 2.0f;
				++vertexIndex;

				// ��Ļ���Ͻ�
				texcoords[vertexIndex * 2 + 0] = curTexCoord.x / texWidth;
				texcoords[vertexIndex * 2 + 1] = (curTexCoord.y + curTexSize.y) / texHeight * 2.0f;
			}
		}
	}
}

void LayoutUtility::generateWireframeCoordinate(GLfloat* wireframeVertices, GLfloat* coordinateVertices, GLfloat* vertices)
{
	// �߿򶥵�����
	if (wireframeVertices != NULL)
	{
		memcpy(wireframeVertices, vertices, sizeof(GLfloat)* 3 * UI_VERTEX_COUNT);
	}
	// ��������ϵ��������
	if (coordinateVertices != NULL)
	{
		// ���㴰������ϵ
		VECTOR3 point[UI_VERTEX_COUNT];
		for (int i = 0; i < UI_VERTEX_COUNT; ++i)
		{
			point[i] = VECTOR3(vertices[i * 3 + 0], vertices[i * 3 + 1], vertices[i * 3 + 2]);
		}
		VECTOR3 coordinateCenter((point[0] + point[2]) / 2.0f);
		for (int i = 0; i < UI_VERTEX_COUNT; ++i)
		{
			VECTOR3 middlePoint = (point[i] + point[(i + 1) % UI_VERTEX_COUNT]) / 2.0f;
			VECTOR3 relativePoint = middlePoint - coordinateCenter;
			txMath::setLength(relativePoint, 2000.0f);
			middlePoint = coordinateCenter + relativePoint;
			coordinateVertices[i * 3 + 0] = middlePoint.x;
			coordinateVertices[i * 3 + 1] = middlePoint.y;
			coordinateVertices[i * 3 + 2] = middlePoint.z;
		}
	}
}