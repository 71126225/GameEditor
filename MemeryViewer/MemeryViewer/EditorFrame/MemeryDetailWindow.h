#ifndef _MEMERY_DETAIL_WINDOW_H_
#define _MEMERY_DETAIL_WINDOW_H_

#include <map>

#include "MemeryViewer.h"
#include "EditorHeader.h"

class MemeryDetailWindow : public wxPanel
{
public:
	MemeryDetailWindow(wxWindow* parent, long style);
	~MemeryDetailWindow();
	void init(MemeryViewer* viewer);
	void update(float elapsedTime);
	void setClassType(std::string type) { mClassType = type; }
	DECLARE_EVENT_TABLE()
	void onSearchTextChanged(wxCommandEvent& event);
	void onSearchTextEntered(wxCommandEvent& event);
protected:
	wxTextCtrl* mSearchText;
	wxStaticText* mMemeryCountLabel;
	wxListCtrl* mMemeryInfoListCtrl;
	MemeryViewer* mMemeryViewer;
	std::string mClassType;	// ֻ��ʾָ�����͵���ϸ��Ϣ,���ȫ����ʾ,��̫����
	std::map<void*, MemeryInfo> mClassList;
};

#endif