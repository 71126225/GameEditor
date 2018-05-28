#ifndef _EC_UNDO_CUT_WINDOW_H_
#define _EC_UNDO_CUT_WINDOW_H_

#include "ECUndo.h"

class Window;
class Layout;
class ECUndoCutWindow : public ECUndo
{
public:
	ECUndoCutWindow()
		:
		mOldWindowPos(-1)
	{}
	virtual ~ECUndoCutWindow(){}
	void setCutWindowInfo(const std::string& windowName, const std::string& parentName, const int& pos)
	{
		mWindowName = windowName;
		mOldParentName = parentName;
		mOldWindowPos = pos;
	}
	virtual void undo();
protected:
	std::string mWindowName;
	std::string mOldParentName;	// �����д��ڵĸ����ڵ�����
	int mOldWindowPos;			// �����еĴ����ڸ������е�˳���±�
};

#endif