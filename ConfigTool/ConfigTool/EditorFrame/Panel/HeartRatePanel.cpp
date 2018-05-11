#include "txUtility.h"
#include "ConfigToolCore.h"
#include "HeartRateData.h"
#include "SpeedDataManager.h"

#include "HeartRatePanel.h"
#include "EditorUtility.h"

enum
{
	ID_DATA_LIST,
	ID_CONFIRM_TIME_BUTTON,
};

BEGIN_EVENT_TABLE(HeartRatePanel, EditorPanel)
EVT_BUTTON(ID_CONFIRM_TIME_BUTTON, OnConfirmTimeOut)
END_EVENT_TABLE()

HeartRatePanel::HeartRatePanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name)
:
EditorPanel(parent, position, size, style, name)
{
	setup();
}

HeartRatePanel::~HeartRatePanel()
{
	;
}

void HeartRatePanel::setup()
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* m_staticText1 = new wxStaticText(this, wxID_ANY, wxT("����������ʱ:"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(m_staticText1, 0, wxALL | wxALIGN_BOTTOM, 5);

	mHeartRateTimeOutEdit = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(mHeartRateTimeOutEdit, 0, wxALL | wxALIGN_BOTTOM, 5);

	wxStaticText* m_staticText2 = new wxStaticText(this, wxID_ANY, wxT("��"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(m_staticText2, 0, wxALL | wxALIGN_BOTTOM, 5);

	mConfirmTimeOut = new wxButton(this, ID_CONFIRM_TIME_BUTTON, wxT("ȷ��"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(mConfirmTimeOut, 0, wxALL | wxALIGN_BOTTOM, 5);

	wxStaticText* m_staticText3 = new wxStaticText(this, wxID_ANY, wxT("��ǰ��ʱʱ��:"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(m_staticText3, 0, wxALL | wxALIGN_BOTTOM, 5);

	mCurHeartRateTimeOut = new wxStaticText(this, wxID_ANY, wxT("1��"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(mCurHeartRateTimeOut, 0, wxALL | wxALIGN_BOTTOM, 5);
	bSizer1->Add(bSizer2, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxVERTICAL);

	mHeartRateDataList = new wxListCtrl(this, ID_DATA_LIST, wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER | wxLC_REPORT);
	bSizer4->Add(mHeartRateDataList, 1, wxALL | wxEXPAND, 5);

	bSizer1->Add(bSizer4, 10, wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();

	// Ϊ�б������
	const int columnCount = 3;
	std::string column[columnCount] = { "���ʴ����", "�ֽ�����", "����" };
	int width[columnCount] = { 200, 300, 100};
	for (int i = 0; i < columnCount; ++i)
	{
		mHeartRateDataList->InsertColumn(i, column[i], 0, width[i]);
	}
}

void HeartRatePanel::showData(std::map<std::string, HeartRateData*>& dataList)
{
	int itemCount = mHeartRateDataList->GetItemCount();
	int shownCount = dataList.size();
	if (itemCount < shownCount)
	{
		resetListItemCount(shownCount);
	}
	// ��û�����ݵ���ʾ�������
	for (int i = shownCount; i < itemCount; ++i)
	{
		if (mHeartRateDataList->GetItemText(i, 0) != "")
		{
			int columnCount = mHeartRateDataList->GetColumnCount();
			for (int j = 0; j < columnCount; ++j)
			{
				mHeartRateDataList->SetItem(i, j, wxT(""));
			}
		}
	}

	// ������ʾ����
	std::map<std::string, HeartRateData*>::iterator iterData = dataList.begin();
	std::map<std::string, HeartRateData*>::iterator iterDataEnd = dataList.end();
	for (int i = 0; iterData != iterDataEnd; ++iterData, ++i)
	{
		HeartRateData* data = iterData->second;
		std::string number = data->mNumber;
		std::string byteStr = txUtility::charArrayToHexString(data->mData, data->mDataSize);
		std::string heartRateStr = txUtility::intToString(data->mHeartRate);
		updateItem(i, 0, number);			// ���ʴ����
		updateItem(i, 1, byteStr);			// �ֽ�ʮ����������
		updateItem(i, 2, heartRateStr);		// ����
	}
}

void HeartRatePanel::refreshTime()
{
	mCurHeartRateTimeOut->SetLabel(txUtility::floatToString(mSpeedDataManager->getHeartRateTimeOut(), 1) + "��");
}

void HeartRatePanel::resetListItemCount(int count)
{
	int curItemCount = mHeartRateDataList->GetItemCount();
	int deltaCount = curItemCount - count;
	// ��������,��ɾ�������
	if (deltaCount > 0)
	{
		for (int i = 0; i < deltaCount; ++i)
		{
			mHeartRateDataList->DeleteItem(curItemCount - i - 1);
		}
	}
	// ��������,�����
	else if (deltaCount < 0)
	{
		deltaCount = -deltaCount;
		for (int i = 0; i < deltaCount; ++i)
		{
			mHeartRateDataList->InsertItem(curItemCount + i, wxT(""), -1);
		}
	}
	else
	{
		return;
	}
}

void HeartRatePanel::updateItem(const int& item, const int& column, const std::string& text)
{
	if (mHeartRateDataList->GetItemText(item, column) != text)
	{
		mHeartRateDataList->SetItem(item, column, text);
	}
}

void HeartRatePanel::OnConfirmTimeOut(wxCommandEvent& e)
{
	mSpeedDataManager->setHeartRateTimeOut(txUtility::stringToFloat(mHeartRateTimeOutEdit->GetValue().ToStdString()));
}