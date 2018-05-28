#ifndef _KEY_FRAME_GRAPH_H_
#define _KEY_FRAME_GRAPH_H_

#include "GraphPanelBase.h"

class txTremblingKeyFrame;
class KeyFrameGraph : public GraphPanelBase
{
public:
	KeyFrameGraph(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~KeyFrameGraph(){}
	virtual void setup(){}
	virtual void render(wxDC* dc);
	virtual void notifyMouseWheel(wxMouseEvent& event);
	virtual void notifyMouseLeftUp(wxMouseEvent& event);
	virtual void notifyMouseRightUp(wxMouseEvent& event);
	virtual void notifyMiddleDown(wxMouseEvent& event);
	virtual void notifyMouseMove(wxMouseEvent& event);
	virtual void notifyKeyDown(wxKeyEvent& key);
	virtual void notifyKeyUp(wxKeyEvent& key);
	DECLARE_EVENT_TABLE()
	void OnInsertKeyFrame(wxCommandEvent& event);
	void OnDeleteKeyFrame(wxCommandEvent& event);
protected:
	void drawKeyFrame(wxDC* dc);	// �������йؼ�֡
	void drawCoord(wxDC* dc);		// ����������Ϳ̶���
	txTremblingKeyFrame* hitKeyFrame(const wxPoint& point, wxPoint& offset);	// ͨ��һ���������һ���ؼ�֡,offsetʱ�������λ������ڹؼ�֡�������ĵ��ƫ��
	float positionXToTime(const int& posX){ return (posX - mHoriStart) / (float)mPixelPerUnitHori; }
	float positionYToOffset(const int& posY){ return (posY - mVertStart) / (float)mPixelPerUnitVert; }
	int timeToPositionX(const float& time) { return time * mPixelPerUnitHori + mHoriStart; }
	int offsetToPositionY(const float& offset) { return offset * mPixelPerUnitVert + mVertStart; }
protected:
	float mPixelPerUnitHori;		// ������ٸ�����Ϊһ����λ�ľ���,�������������
	float mPixelPerUnitVert;		// ������ٸ�����Ϊһ����λ�ľ���,�������������
	int mHoriStart;
	int mVertStart;
	bool mIsShiftDown;
	wxPoint mMouseOffset;
	wxPoint mMiddlePosOffset;		// �м�����ʱ���λ�����������ϵ����ƫ��
	wxPoint mMenuPosition;			// �����˵�ʱ����λ��
};

#endif