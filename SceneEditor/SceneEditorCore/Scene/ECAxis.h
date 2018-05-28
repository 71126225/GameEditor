#ifndef _EC_AXIS_H_
#define _EC_AXIS_H_

#include "txMovableObject.h"

class ECEditor;
class ECAxis : public txMovableObject
{
public:
	ECAxis(const std::string& name)
		:
		txMovableObject(name),
		mAttachTarget(NULL),
		mAutoShow(true)
	{}
	virtual ~ECAxis(){ destroy(); }
	virtual void initComponents();
	virtual void update(float elapsedTime);
	void destroy(){}
	void attachTarget(ECEditor* editor);
	void setVisible(bool visible);
	bool isVisible();
	void setAutoShow(bool autoShow);
	bool getAutoShow() { return mAutoShow; }
	ECEditor* getAttachTarget() { return mAttachTarget; }
protected:
	ECEditor* mAttachTarget;
	bool mAutoShow;				// Ϊtrueʱ,�ڹҽӵ�������ʱ���Զ���ʾ,��������ȡ��ʱ���Զ�����,false�򲻻��Զ���ʾ������
};

#endif