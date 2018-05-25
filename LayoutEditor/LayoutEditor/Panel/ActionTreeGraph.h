#ifndef _ACTION_TREE_GRAPH_H_
#define _ACTION_TREE_GRAPH_H_

#include "GraphPanelBase.h"

class ActionTreeGraph : public GraphPanelBase
{
public:
	ActionTreeGraph(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~ActionTreeGraph(){}
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
	void OnDeleteAction(wxCommandEvent& event);
protected:
	void drawAction(wxDC* dc);	// �������йؼ�֡
	void drawCoord(wxDC* dc);	// ����������Ϳ̶���
	void drawAction(wxDC* dc, txAction* action, const int& posY, const int& parentPosY);
	txAction* hitAction(const wxPoint& point);	// ͨ��һ���������һ���ؼ�֡
	txAction* hitAction(txAction* action, const wxPoint& point, const int& posY, const int& parentY);
	float positionXToTime(const int& posX){ return (posX - mHoriStart) / (float)mPixelPerUnitHori; }
	int timeToPositionX(const float& time) { return time * mPixelPerUnitHori + mHoriStart; }
	const VECTOR3& getColour(const std::string& actionName);
protected:
	float mPixelPerUnitHori;		// ������ٸ�����Ϊһ����λ�ľ���,�������������
	int mHoriStart;
	int mVertStart;
	wxPoint mMouseOffset;
	wxPoint mMiddlePosOffset;		// �м�����ʱ���λ�����������ϵ����ƫ��
	wxPoint mMenuPosition;			// �����˵�ʱ����λ��
	txMap<std::string, VECTOR3> mActionColourList;
};

#endif