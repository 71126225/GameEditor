#ifndef _RENDER_CANVAS_H_
#define _RENDER_CANVAS_H_

#include "EditorUtility.h"
#include "EditorBase.h"
#include "EditorCoreBase.h"
#include "txEngineBase.h"
#include "txIPoint.h"

class RenderCanvas : public wxPanel, public EditorBase, public EditorCoreBase, public txEngineBase
{
public:
	RenderCanvas(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~RenderCanvas(){}
	void notifyAdjustMousePosition(bool adjust){ mAdjustMousePosition = adjust; }
	txIPoint getMouseEventPosition(const wxMouseEvent& event);
	DECLARE_EVENT_TABLE()
public:
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnChar(wxKeyEvent& event);
	void OnMouseLeftDown(wxMouseEvent& event);
	void OnMouseLeftUp(wxMouseEvent& event);
	void OnMouseRightDown(wxMouseEvent& event);
	void OnMouseRightUp(wxMouseEvent& event);
	void OnMouseMove(wxMouseEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
	void OnShow(wxShowEvent& event);
protected:
	wxPoint mMouseRightLastPos; // �����һ֡������
	bool mAdjustMousePosition;	// ��Ⱦ�������õ����ֱ���ʱ,ʵ�ʻ��С�ͻ����Ĵ�С,����������������,������Ҫ����
};

#endif