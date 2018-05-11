#ifndef _TEXTURE_WINDOW_H_
#define _TEXTURE_WINDOW_H_

#include "Window.h"

class txTexture;
class TextureWindow : public Window
{
public:
	TextureWindow(const std::string& name, const WINDOW_TYPE& type)
		:
		Window(name, type),
		mTexture(NULL),
		mMaskTexture(NULL),
		mTextureCoord(txMath::VEC2_ZERO),
		mTextureSize(txMath::VEC2_ONE),
		mAutoSize(true),
		mArcProgress(1.0f),
		mLinearDodge(false),
		mUseMipmap(true)
	{}
	virtual ~TextureWindow(){}
	virtual void render();
		
	txTexture* getTexture()						{ return mTexture; }
	txTexture* getMaskTexture()					{ return mMaskTexture; }
	const VECTOR2& getTextureCoord()			{ return mTextureCoord; }
	const VECTOR2& getTextureSize()				{ return mTextureSize; }
	const bool& getAutoSize()					{ return mAutoSize; }
	const bool& getLinearDodge()				{ return mLinearDodge; }
	const float& getArcProgress()				{ return mArcProgress; }

	void setArcProgress(const float& progress)	{ mArcProgress = progress; }
	void setMaskTexture(txTexture* texture)		{ mMaskTexture = texture; }
	void setLinearDodge(const bool& linearDodge){ mLinearDodge = linearDodge; }
	void setTextureCoord(const VECTOR2& textureCoord, const bool& refreshNow)
	{ 
		// ֻ�ڷ��Զ���ȡ�����Сʱ����Ч
		if (!mAutoSize)
		{
			mTextureCoord = textureCoord;
			if (refreshNow)
			{
				notifyTexCoordNeedUpdated();
			}
			else
			{
				setTexCoordsDirty(true);
			}
		}
	}
	void setTextureSize(const VECTOR2& textureSize, const bool& refreshNow)
	{
		// ֻ�ڷ��Զ���ȡ�����Сʱ����Ч
		if (!mAutoSize)
		{
			mTextureSize = textureSize;
			if (refreshNow)
			{
				notifyTexCoordNeedUpdated();
			}
			else
			{
				setTexCoordsDirty(true);
			}
			selfTextureSizeChanged();
		}
	}
	void setAutoSize(const bool& autoSize);
	// refreshNow��ʾ�Ƿ�����ˢ����������,�ڴ��ڵ�update�е��øú���ʱ��Ҫ����Ϊtrue
	void setTexture(txTexture* texture, const bool& refreshNow = false);
	virtual void finishAsycLoad();
	virtual void getUsedResourceList(txVector<std::string>& resourceList);
	virtual void notifyTexCoordNeedUpdated();
protected:
	virtual void initProperty();
	void selfTextureSizeChanged();
	void selfTextureChanged();
protected:
	txTexture* mTexture;			// ��Ⱦ������
	txTexture* mMaskTexture;		// ͸���ڵ�����,�ڵ�����Ĵ�С��Ҫ����Ⱦ����Ĵ�Сһ��,�����һ��,���ڵ�����ᱻ����
	VECTOR2 mTextureCoord;			// ��Ҫ��Ⱦ��������ʼ����
	VECTOR2 mTextureSize;			// ��Ҫ��Ⱦ������ߴ�
	bool mAutoSize;					// �Ƿ��Զ���ȡͼƬ�Ŀ��
	float mArcProgress;				// ���μ��еĽǶȰٷֱ�
	bool mLinearDodge;				// �Ƿ�ʹ�����Լ������ӷ�ʽ
	bool mUseMipmap;				// �Ƿ�ʹ�ö༶��������
};

#endif