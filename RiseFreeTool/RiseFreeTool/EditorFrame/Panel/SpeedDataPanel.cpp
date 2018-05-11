#include "RiseFreeToolCore.h"
#include "ComManager.h"
#include "SpeedDataManager.h"
#include "DataKey.h"

#include "SpeedDataPanel.h"
#include "EditorUtility.h"

const int FRICTION_COUNT = 10;

enum
{
	ID_DEVICE_LIST_COMBO,
	ID_LIETEN_BUTTON,
	ID_CANCEL_LISTEN_BUTTON,
	ID_FRICTION_0,
	ID_DATA_LIST = ID_FRICTION_0 + FRICTION_COUNT,
};

BEGIN_EVENT_TABLE(SpeedDataPanel, wxPanel)
EVT_BUTTON(ID_LIETEN_BUTTON, OnListenButton)
EVT_BUTTON(ID_CANCEL_LISTEN_BUTTON, OnCancelListenButton)
END_EVENT_TABLE()

SpeedDataPanel::SpeedDataPanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name)
:
EditorPanel(parent, position, size, style, name)
{
	setup();
}

SpeedDataPanel::~SpeedDataPanel()
{
	int frictionCount = mFrictionRadioButtonList.size();
	for (int i = 0; i < frictionCount; ++i)
	{
		mFrictionRadioButtonList[i]->Disconnect(wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(SpeedDataPanel::OnFrictionChanged), NULL, this);
	}
}

void SpeedDataPanel::setup()
{
	wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* m_staticText1 = new wxStaticText(this, wxID_ANY, wxT("�豸�б�"));
	bSizer2->Add(m_staticText1, 0, wxALL, 5);
	mDeviceListCombo = new wxComboBox(this, ID_DEVICE_LIST_COMBO, wxT(""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	bSizer2->Add(mDeviceListCombo, 0, wxALL, 5);
	mListenButton = new wxButton(this, ID_LIETEN_BUTTON, wxT("�򿪴���"));
	bSizer2->Add(mListenButton, 0, wxALL, 5);
	mCancelListenButton = new wxButton(this, ID_CANCEL_LISTEN_BUTTON, wxT("�رմ���"));
	bSizer2->Add(mCancelListenButton, 0, wxALL, 5);
	bSizer1->Add(bSizer2, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer3 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* m_staticText2 = new wxStaticText(this, wxID_ANY, wxT("����"));
	bSizer3->Add(m_staticText2, 0, wxALL, 5);
	for (int i = 0; i < FRICTION_COUNT; ++i)
	{
		wxRadioButton* radioButton = new wxRadioButton(this, ID_FRICTION_0 + i, StringUtility::intToString(i + 1));
		bSizer3->Add(radioButton, 0, wxALL, 5);
		mFrictionRadioButtonList.push_back(radioButton);
	}
	bSizer1->Add(bSizer3, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer4 = new wxBoxSizer(wxVERTICAL);
	mDataListCtrl = new wxListCtrl(this, ID_DATA_LIST, wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER | wxLC_REPORT);
	bSizer4->Add(mDataListCtrl, 1, wxALL | wxEXPAND, 5);

	bSizer1->Add(bSizer4, 1, wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();

	int frictionCount = mFrictionRadioButtonList.size();
	for (int i = 0; i < frictionCount; ++i)
	{
		mFrictionRadioButtonList[i]->Connect(wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(SpeedDataPanel::OnFrictionChanged), NULL, this);
	}

	// Ϊ�б������
	const int columnCount = 6;
	std::string column[columnCount] = { "�ֽ�����", "����", "ת��", "�Ƕ�", "����", "����" };
	int width[columnCount] = { 500, 100, 100, 100, 280, 100 };
	for (int i = 0; i < columnCount; ++i)
	{
		mDataListCtrl->InsertColumn(i, column[i], 0, width[i]);
	}
}

void SpeedDataPanel::update()
{
	DeviceData* deviceData = mSpeedDataManager->getCurDeviceData();
	int itemCount = mDataListCtrl->GetItemCount();
	int dataCount = deviceData != NULL ? deviceData->mDataPacketList.size() : 0;
	if (itemCount < dataCount)
	{
		resetListItemCount(dataCount);
	}
	// ��û�����ݵ���ʾ�������
	for (int i = dataCount; i < itemCount; ++i)
	{
		if (mDataListCtrl->GetItemText(i, 0) != "")
		{
			int columnCount = mDataListCtrl->GetColumnCount();
			for (int j = 0; j < columnCount; ++j)
			{
				mDataListCtrl->SetItem(i, j, wxT(""));
			}
		}
	}
	if (deviceData == NULL)
	{
		return;
	}

	// ������ʾ����
	txMap<int, DataPacket*>::iterator iter = deviceData->mDataPacketList.begin();
	txMap<int, DataPacket*>::iterator iterEnd = deviceData->mDataPacketList.end();
	for (int i = 0; iter != iterEnd; ++iter, ++i)
	{
		DataPacket* dataPacket = iter->second;
		std::string hexStr = StringUtility::charArrayToHexString(dataPacket->mPacketData, dataPacket->mDataCount);
		if (dataPacket->mPayload->mID == CMD_FIT)
		{
			if (dataPacket->mPayload->mDataKey->mKey == KEY_MACHINE)
			{
				MachineKey* key = static_cast<MachineKey*>(dataPacket->mPayload->mDataKey);
				std::string powerStr = StringUtility::intToString(key->mPower);
				std::string rpmStr = StringUtility::intToString(key->mRPM);
				std::string angleStr = StringUtility::intToString(key->mAngle);
				std::string keyStatusStr;
				keyStatusStr += "0 : " + StringUtility::boolToString(key->isKeyDown(0)) + ", ";
				keyStatusStr += "1 : " + StringUtility::boolToString(key->isKeyDown(1)) + ", ";
				keyStatusStr += "2 : " + StringUtility::boolToString(key->isKeyDown(2)) + ", ";
				keyStatusStr += "3 : " + StringUtility::boolToString(key->isKeyDown(3));
				updateItem(0, 0, hexStr);		// �ֽ�����
				updateItem(0, 1, powerStr);		// ����
				updateItem(0, 2, rpmStr);		// ת��
				updateItem(0, 3, angleStr);		// �Ƕ�
				updateItem(0, 4, keyStatusStr);	// ����
				updateItem(0, 5, "");			// ����
			}
		}
		else if (dataPacket->mPayload->mID == CMD_FRICTION)
		{
			if (dataPacket->mPayload->mDataKey->mKey == KEY_FRICTION)
			{
				FrictionKey* key = static_cast<FrictionKey*>(dataPacket->mPayload->mDataKey);
				std::string frictionStr = StringUtility::intToString(key->mFriction);
				updateItem(0, 0, hexStr);		// �ֽ�����
				updateItem(0, 1, "");			// ����
				updateItem(0, 2, "");			// ת��
				updateItem(0, 3, "");			// �Ƕ�
				updateItem(0, 4, "");			// ����
				updateItem(0, 5, frictionStr);	// ����
			}
		}
	}
}

void SpeedDataPanel::notifyDeviceListChanged()
{
	mDeviceListCombo->Clear();
	txVector<std::string> deviceList = mComManager->getDeviceList();
	int count = deviceList.size();
	for (int i = 0; i < count; ++i)
	{
		mDeviceListCombo->Append(deviceList[i]);
	}
}

void SpeedDataPanel::updateItem(const int& item, const int& column, const std::string& text)
{
	if (mDataListCtrl->GetItemText(item, column) != text)
	{
		mDataListCtrl->SetItem(item, column, text);
	}
}

void SpeedDataPanel::resetListItemCount(int count)
{
	int curItemCount = mDataListCtrl->GetItemCount();
	int deltaCount = curItemCount - count;
	// ��������,��ɾ�������
	if (deltaCount > 0)
	{
		for (int i = 0; i < deltaCount; ++i)
		{
			mDataListCtrl->DeleteItem(curItemCount - i - 1);
		}
	}
	// ��������,�����
	else if (deltaCount < 0)
	{
		deltaCount = -deltaCount;
		for (int i = 0; i < deltaCount; ++i)
		{
			mDataListCtrl->InsertItem(curItemCount + i, wxT(""), -1);
		}
	}
}

void SpeedDataPanel::OnListenButton(wxCommandEvent& event)
{
	mComManager->setCurDevice(mDeviceListCombo->GetValue().ToStdString());
}

void SpeedDataPanel::OnCancelListenButton(wxCommandEvent& event)
{
	mComManager->setCurDevice("");
}

void SpeedDataPanel::OnFrictionChanged(wxCommandEvent& event)
{
	int id = event.GetId();
	mComManager->setFriction((id - ID_FRICTION_0 + 1) * 10);
}

void SpeedDataPanel::notifyCurDeviceChanged(const std::string& device)
{
	if (mDeviceListCombo->GetValue() != device)
	{
		mDeviceListCombo->SetValue(device);
	}
}

void SpeedDataPanel::notifyDeviceOpen(bool open)
{
	mListenButton->Enable(!open);
	mCancelListenButton->Enable(open);
}