#ifndef _CONFIG_DATA_DIALOG_H_
#define _CONFIG_DATA_DIALOG_H_

#include "DataDefine.h"
#include "EditorHeader.h"

class EditorFrame;
class DataTemplate;
class ConfigDataDialog : public wxDialog
{
public:
	ConfigDataDialog(wxWindow* parent, const wxString& title, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~ConfigDataDialog();
	void setup();
	void refresh();
	void editTable(DataTemplate* dataTemplate);
	DECLARE_EVENT_TABLE()
	void OnTypeChoiceChanged(wxCommandEvent& event);
	void OnArrayLengthChanged(wxCommandEvent& event);
	void OnAddButton(wxCommandEvent& event);
	void OnMoveUp(wxCommandEvent& event);
	void OnMoveDown(wxCommandEvent& event);
	void OnMoveTop(wxCommandEvent& event);
	void OnMoveBottom(wxCommandEvent& event);
	void OnDeleteItem(wxCommandEvent& event);
	void OnListItemSelected(wxListEvent& event);
	void OnRightUp(wxMouseEvent& event);
	void OnRightDown(wxMouseEvent& event);
	void OnListItemKeyDown(wxListEvent& event);
	void OnCreateTableButton(wxCommandEvent& event);
	void OnClearTableButton(wxCommandEvent& event);
protected:
	void deleteItem(int index);
	void moveItem(int destPos);
	void addItem(const std::string& paramType, const std::string& describe, int count, int paramPos);
protected:
	// �������ʱ�����������ɾ���ƶ��ֶ�,���һ���Ѵ��ڵı��ʱ,�����޸ı������,ÿ��һ�����ɾ���ƶ���������Ҫ����һ��,���Ҳ��ܳ���
	EditorFrame* mEditorFrame;
	wxTextCtrl* mDescribeTextCtrl;
	wxChoice* mTypeChoice;
	wxTextCtrl* mArrayLengthTextCtrl;
	wxListCtrl* mDataViewList;
	wxTextCtrl* mTableNameTextCtrl;
	wxButton* mCreateTableButton;
	wxButton* mClearButton;
	std::vector<std::vector<std::string> > mDataParamList;
	int mCurSelectIndex;
	DataTemplate* mDataTemplate;
};

#endif