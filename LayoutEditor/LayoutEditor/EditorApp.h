#ifndef _EDITOR_APP_H_
#define _EDITOR_APP_H_

#include "EditorUtility.h"

class EditorFrame;
class EditorBusyInfo;
class EditorApp : public wxApp
{
public:
	EditorApp();
	virtual ~EditorApp();
	EditorFrame* GetMainFrame() { return mMainFrame; }
	
	virtual bool OnInit();
	EditorBusyInfo* getBusyInfo();
	// �ú���ֻ������EditorFrame�е���
	void destroyBusyInfo();
	static LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException);

	DECLARE_EVENT_TABLE()
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
protected:
	// ���㴰�ڻᱻwxWidgets�Զ�����,����Ҫ�Լ��ֶ�����
	EditorFrame* mMainFrame;
	EditorBusyInfo* mBusyInfo;
};

DECLARE_APP(EditorApp)

#endif