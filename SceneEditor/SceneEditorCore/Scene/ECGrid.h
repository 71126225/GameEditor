#ifndef _EC_GRID_H_
#define _EC_GRID_H_

#include "txMovableObject.h"

class ECEditor;
class txCamera;
class ECGrid : public txMovableObject
{
public:
	ECGrid(const std::string& name);
	virtual ~ECGrid(){ destroy(); }
	virtual void initComponents(){}
	// ����Ŀ�������ż��
	void initGrid(int width, int height);
	virtual void update(float elapsedTime);
	void destroy(){}
	void render(txCamera* camera);
	void setVisible(bool visible) { mVisible = visible; }
	bool isVisible() { return mVisible; }
	void setAutoShow(bool autoShow);
	bool getAutoShow() { return mAutoShow; }
	void setColour(const VECTOR4& colour) { mGridColour = colour; }
	void attachTarget(ECEditor* editor);
	ECEditor* getAttachTarget() { return mAttachTarget; }
protected:
	ECEditor* mAttachTarget;
	bool mVisible;
	bool mAutoShow;			// Ϊtrueʱ,�ڹҽӵ�������ʱ���Զ���ʾ,��������ȡ��ʱ���Զ�����,false�򲻻��Զ���ʾ������
	VECTOR4 mGridColour;
	GLfloat* mVertices;
	GLushort* mIndices;
	int mIndexCount;
};

#endif