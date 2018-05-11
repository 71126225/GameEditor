#include "txTexture.h"

#include "WindowHeader.h"
#include "ProgressWindowProperty.h"
#include "Layout.h"

ProgressWindow::ProgressWindow(const std::string& name, const WINDOW_TYPE& type)
	:
	ComplexWindow(name, type),
	mProgress(1.0f),
	mLeftToRight(true),
	mChangindChildRect(false),
	mChangingChildTextureSize(false)
{}

void ProgressWindow::init(Layout* layout)
{
	ComplexWindow::init(layout);
	mBackground = (TextureWindow*)createInnerChild("TextureWindow", mName + "_background");
	mSlot = (TextureWindow*)createInnerChild("TextureWindow", mName + "_slot");
	mBar = (TextureWindow*)createInnerChild("TextureWindow", mName + "_bar");
	addInnerChild(mBackground);
	mBackground->addChild(mSlot);
	mBar->setAutoSize(false);
	mSlot->addChild(mBar);

	mOriginBarWindowPosition = mBar->getPosition();
	mOriginBarWindowSize = mBar->getSize();
	mOriginBarTextureSize = mBar->getTextureSize();
}

void ProgressWindow::setProgress(const float& percent, const bool& refreshNow)
{
	mProgress = percent;
	txMath::clamp(mProgress, 0.0f, 1.0f);

	txTexture* tex = mBar->getTexture();
	if (tex == NULL)
	{
		return;
	}
	mChangindChildRect = true;
	mChangingChildTextureSize = true;
	if (mLeftToRight)
	{
		// ���������Ҫ��ˮƽ��ת
		if (mBar->getSymmetryMode() == SM_HORI_INVERSE)
		{
			mBar->setTextureCoord(VECTOR2(mOriginBarTextureSize.x - mOriginBarTextureSize.x * mProgress, 0.0f), false);
			mBar->setTextureSize(VECTOR2(mOriginBarTextureSize.x * mProgress, mOriginBarTextureSize.y), refreshNow);
		}
		// ������ת���߶Գ�,��ֹ����Ϊ���Ҹ���,���Ҿ��������ֱ��ת
		else
		{
			mBar->setTextureCoord(txMath::VEC2_ZERO, false);
			mBar->setTextureSize(VECTOR2(mOriginBarTextureSize.x * mProgress, mOriginBarTextureSize.y), refreshNow);
		}
		txPoint point = mOriginBarWindowSize;
		point.x = mOriginBarWindowSize.x * mProgress;
		mBar->setSize(point);
		mBar->setPosition(mOriginBarWindowPosition, refreshNow);
	}
	else
	{
		// ���������Ҫ��ˮƽ��ת
		if (mBar->getSymmetryMode() == SM_HORI_INVERSE)
		{
			mBar->setTextureCoord(txMath::VEC2_ZERO, false);
			mBar->setTextureSize(VECTOR2(mOriginBarTextureSize.x * mProgress, mOriginBarTextureSize.y), refreshNow);
		}
		// ������ת���߶Գ�,��ֹ����Ϊ���Ҹ���,���Ҿ��������ֱ��ת
		else
		{
			mBar->setTextureCoord(VECTOR2(mOriginBarTextureSize.x * (1.0f - mProgress), 0.0f), false);
			mBar->setTextureSize(VECTOR2(mOriginBarTextureSize.x * mProgress, mOriginBarTextureSize.y), true);
		}
		txPoint size = mOriginBarWindowSize;
		size.x = mOriginBarWindowSize.x * mProgress;
		mBar->setSize(size);
		txPoint pos = mOriginBarWindowPosition;
		pos.x = mOriginBarWindowPosition.x + mOriginBarWindowSize.x * (1.0f - mProgress);
		mBar->setPosition(pos, refreshNow);
	}
	mChangindChildRect = false;
	mChangingChildTextureSize = false;
}

void ProgressWindow::initProperty()
{
	ComplexWindow::initProperty();
	addProperty<ProgressWindowPropertyPercent>();
	addProperty<ProgressWindowPropertyLeftToRight>();
}

void ProgressWindow::setOriginBarWindowSize(const txPoint& size)
{
	mOriginBarWindowSize = size;
	if (NULL != mBar)
	{
		mBar->setSize(mOriginBarWindowSize);
	}
}

void ProgressWindow::setOriginBarWindowPosition(const txPoint& position)
{
	mOriginBarWindowPosition = position;
	if (NULL != mBar)
	{
		mBar->setPosition(mOriginBarWindowPosition);
	}
}

void ProgressWindow::setLeftToRight(const bool& leftToRight)
{
	mLeftToRight = leftToRight;
	setProgress(mProgress);
}

// ���ڿ��Ե������ý������ڲ��Ӵ�����������,������Ҫ����������ʱ֪ͨ����������!
void ProgressWindow::notifyChildRectChanged(Window* child)
{
	// ���ʱ�����Լ��ı��Ӵ��ڵĴ�С�������֪ͨ,��������
	if (mChangindChildRect)
	{
		return;
	}
	// ֻ�������Ĵ�С�ı��¼�
	if (child == mBar)
	{
		mOriginBarWindowPosition = mBar->getPosition();
		mOriginBarWindowSize = mBar->getSize();
	}
}

void ProgressWindow::notifyChildTextureSizeChanged(Window* child)
{
	if (mChangingChildTextureSize)
	{
		return;
	}
	if (child == mBar)
	{
		mOriginBarTextureSize = mBar->getTextureSize();
	}
}

void ProgressWindow::notifyChildTextureChanged(Window* child)
{
	if (child == mBar)
	{
		if (mBar->getTexture() != NULL)
		{
			mOriginBarTextureSize = mBar->getTexture()->getTextureSize();
		}
		else
		{
			mOriginBarTextureSize = txMath::VEC2_ZERO;
		}
	}
}

void ProgressWindow::saveWindow(std::string& windowStream)
{
	setProgress(1.0f);
	mBar->setAutoSize(false);
	ComplexWindow::saveWindow(windowStream);
}

void ProgressWindow::saveTemplate(std::string& windowStream)
{
	setProgress(1.0f);
	mBar->setAutoSize(false);
	ComplexWindow::saveTemplate(windowStream);
}

void ProgressWindow::renameInnerChildren(const std::string& namePrefix)
{
	mBackground->rename(namePrefix + "_background");
	mSlot->rename(namePrefix + "_slot");
	mBar->rename(namePrefix + "_bar");
}