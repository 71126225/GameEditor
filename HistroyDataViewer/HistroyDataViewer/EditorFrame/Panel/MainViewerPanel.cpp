#include "EditorApp.h"
#include "EditorFrame.h"
#include "MainViewerPanel.h"
#include "EditorUtility.h"
#include "DataHistoryData.h"
#include "DataBase.h"
#include "txUtility.h"

enum
{
	ID_DATA_INFO_VIEW,
};

BEGIN_EVENT_TABLE(MainViewerPanel, wxPanel)
EVT_SHOW(OnShow)
END_EVENT_TABLE()

MainViewerPanel::MainViewerPanel(wxWindow* parent, long style)
:
wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, style)
{
	mEditorFrame = wxGetApp().GetMainFrame();
	setup();

	mItemNames.insert(std::make_pair("date", "����"));
	mItemNames.insert(std::make_pair("machine index", "������"));
	mItemNames.insert(std::make_pair("mileage(km)", "���(����)"));
	mItemNames.insert(std::make_pair("kcal", "��·��(ǧ��)"));
	mItemNames.insert(std::make_pair("max speed(km/h)", "����ٶ�(ǧ��/Сʱ)"));
	mItemNames.insert(std::make_pair("average speed(km/h)", "ƽ���ٶ�(ǧ��/Сʱ)"));
	mItemNames.insert(std::make_pair("cycling time(s)", "����ʱ��(��)"));
	mItemNames.insert(std::make_pair("match time(s)", "ƥ��ʱ��(��)"));
	mItemNames.insert(std::make_pair("match value", "ƥ���"));
}

void MainViewerPanel::setup()
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	mAuiToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 20));
	mAuiToolBar->SetToolBitmapSize(wxSize(16, 16));
	mAuiToolBar->Realize();

	bSizer1->Add(mAuiToolBar, 0, wxALL, 5);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxVERTICAL);

	mDataInfoListCtrl = new wxDataViewListCtrl(this, ID_DATA_INFO_VIEW, wxDefaultPosition, wxSize(-1, -1), wxDV_HORIZ_RULES | wxDV_VERT_RULES | wxDV_MULTIPLE);
	bSizer2->Add(mDataInfoListCtrl, 1, wxALL | wxEXPAND, 5);

	bSizer1->Add(bSizer2, 1, wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();
}

void MainViewerPanel::showData(const std::vector<DataHistoryData*>& dataList)
{
	DataBase* dataBase = mEditorFrame->getDataBase();
	mDataInfoListCtrl->DeleteAllItems();
	mDataInfoListCtrl->ClearColumns();

	int dataCount = dataList.size();
	if (dataCount == 0)
	{
		return;
	}
	
	const std::vector<DataParameter>& parameterList = dataList[0]->getParameterList();
	int paramCount = parameterList.size();
	for (int i = 0; i < paramCount; ++i)
	{
		const DataParameter& dataParam = parameterList[i];
		mDataInfoListCtrl->AppendTextColumn(getItemName(dataParam.mDescribe), wxDATAVIEW_CELL_EDITABLE, 150);
	}

	for (int i = 0; i < dataCount; ++i)
	{
		DataHistoryData* data = dataList[i];
		wxVector<wxVariant> valueList;
		// ���ø��е���������
		const std::vector<DataParameter>& parameterList = data->getParameterList();
		int paramCount = parameterList.size();
		for (int j = 0; j < paramCount; ++j)
		{
			std::string dataString = data->getValueString(j);
			valueList.push_back(dataString);
		}
		mDataInfoListCtrl->AppendItem(valueList);
	}
}

void MainViewerPanel::OnShow(wxShowEvent& event)
{
	mEditorFrame->notifyWindowShow(this, event.IsShown());
}