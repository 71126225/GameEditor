#ifndef _WINDOW_COMPONENT_MOVE_RELATIVE_H_
#define _WINDOW_COMPONENT_MOVE_RELATIVE_H_

#include "txComponentKeyFrame.h"
#include "txPoint.h"

// �����һ��������˶�
class WindowComponentMoveRelative : public txComponentKeyFrame
{
public:
	WindowComponentMoveRelative(const std::string& type, const std::string& name)
		:
		txComponentKeyFrame(type, name)
	{}
	virtual ~WindowComponentMoveRelative(){ destroy(); }
	void destroy(){}
	void setStartPos(const txPoint& startPos) { mStartPos = startPos; }
	virtual void applyKeyFrame(const txVector<float>& value);
	virtual bool isType(const std::string& type)
	{
		return txComponentKeyFrame::isType(type) || type == TOSTRING(WindowComponentMoveRelative);
	}
protected:
	txPoint mStartPos;	// �ƶ���ʼʱ��λ��
};

#endif