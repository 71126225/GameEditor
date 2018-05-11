#ifndef _PROGRESS_WINDOW_H_
#define _PROGRESS_WINDOW_H_

#include "ComplexWindow.h"

class TextureWindow;
class ProgressWindow : public ComplexWindow
{
public:
	ProgressWindow(const std::string& name, const WINDOW_TYPE& type);
	virtual ~ProgressWindow(){}
	virtual void init(Layout* layout);
	void setProgress(const float& percent, const bool& refreshNow = false);
	const float& getProgress() { return mProgress; }
	TextureWindow* getBackgroundWindow() { return mBackground; }
	TextureWindow* getSlotWindow() { return mSlot; }
	TextureWindow* getBarWindow() { return mBar; }
	void setOriginBarWindowSize(const txPoint& size);
	void setOriginBarWindowPosition(const txPoint& position);
	const VECTOR2& getOriginBarTextureSize() { return mOriginBarTextureSize; }
	const txPoint& getOriginBarWindowSize() { return mOriginBarWindowSize; }
	const txPoint& getOriginBarWindowPosition() { return mOriginBarWindowPosition; }
	void setLeftToRight(const bool& leftToRight);
	const bool& getLeftToRight(){ return mLeftToRight; }
	virtual void notifyChildRectChanged(Window* child);
	virtual void notifyChildTextureSizeChanged(Window* child);
	virtual void notifyChildTextureChanged(Window* child);
	virtual void saveWindow(std::string& windowStream);
	virtual void saveTemplate(std::string& windowStream);
protected:
	virtual void initProperty();
	virtual void renameInnerChildren(const std::string& namePrefix);
protected:
	VECTOR2 mOriginBarTextureSize;
	txPoint mOriginBarWindowSize;
	txPoint mOriginBarWindowPosition;
	TextureWindow* mBackground; // �������ı���
	TextureWindow* mSlot;		// �������Ĳ�
	TextureWindow* mBar;		// ����������
	bool mLeftToRight;			// �������Ǵ�����(true)���Ǵ��ҵ���(false)
	float mProgress;
	bool mChangindChildRect;	// �Ƿ����ڸı��Ӵ��ڵĴ��ھ���
	bool mChangingChildTextureSize;	// �Ƿ����ڸı��Ӵ��ڵ�ͼƬ��С
};

#endif
