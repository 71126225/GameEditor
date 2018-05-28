#ifndef _EC_UNDO_DELETE_WINDOW_H_
#define _EC_UNDO_DELETE_WINDOW_H_

#include "ECUndo.h"

class Window;
class Layout;
class ECUndoDeleteWindow : public ECUndo
{
public:
	ECUndoDeleteWindow()
		:
		mWindow(NULL),
		mTempLayout(NULL)
	{}
	virtual ~ECUndoDeleteWindow();
	void setWindow(Window* window);
	virtual void undo();
protected:
	Window* mWindow;
	Layout* mTempLayout;
	std::string mOldLayoutName; // ��ɾ�����ڵ��������ֵ�����
	std::string mOldParentName;	// ��ɾ�����ڵĸ����ڵ�����
	int mOldWindowPos;			// ��ɾ���Ĵ����ڸ������е�˳���±�
	static int mDeleteLayoutCount;
};

#endif