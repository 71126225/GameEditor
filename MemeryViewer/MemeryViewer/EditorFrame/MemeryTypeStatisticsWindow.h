#ifndef _MEMERY_TYPE_STATISTICS_WINDOW_H_
#define _MEMERY_TYPE_STATISTICS_WINDOW_H_

#include <vector>

#include "EditorHeader.h"
#include "MemeryViewer.h"

class MemeryViewer;
class KeywordDialog;
class MemeryTypeStatisticsWindow : public wxPanel
{
public:
	MemeryTypeStatisticsWindow(wxWindow* parent, long style, wxSize size);
	~MemeryTypeStatisticsWindow();
	void init(MemeryViewer* viewer);
	void update(float elapsedTime);
	void destroy();
	void setMaxCount(int count);
	DECLARE_EVENT_TABLE()
	void OnFilterTextChanged(wxCommandEvent& event);
	void OnSearchHideItem(wxCommandEvent& event);
	void OnOpenKeywordList(wxCommandEvent& event);
protected:
	wxListCtrl* mTypeList;				// �ڴ������б�
	wxListCtrl* mSearchList;			// �ڴ���������б�
	wxTextCtrl* mFilterText;			// �ؼ���������
	wxCheckBox* mSearchHideItem;		// �Ƿ���������������
	wxButton* mOpenIgnoreKeywordList;	// �򿪹ؼ��ֹ����б�
	wxStaticText* mMemeryCountLabel;	// �ڴ�������
	wxStaticText* mMemerySizeLabel;		// �ڴ��ܴ�С
	wxStaticText* mMemeryTypeCountLabel;// �ڴ����͸���
	MemeryViewer* mMemeryViewer;
	std::vector<MemeryType> mIgnoreMemeryType;
	KeywordDialog* mKeywordListDialog;
};

#endif