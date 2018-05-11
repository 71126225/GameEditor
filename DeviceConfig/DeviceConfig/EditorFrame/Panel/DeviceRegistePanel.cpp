#include "DeviceRegistePanel.h"
#include "GymInfo.h"
#include "EditorCore.h"
#include "EditorFrame.h"
#include "DeviceInfoPanel.h"

enum
{
	ID_REGISTE_BUTTON,
};

BEGIN_EVENT_TABLE(DeviceRegistePanel, EditorPanel)
EVT_BUTTON(ID_REGISTE_BUTTON, onRegisteButton)
END_EVENT_TABLE()


DeviceRegistePanel::DeviceRegistePanel(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
EditorPanel(parent, position, size, style, name)
{
	setup();
}

DeviceRegistePanel::~DeviceRegistePanel()
{
	;
}

void DeviceRegistePanel::setup()
{
	wxBoxSizer* bSizer18 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer19 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bSizer22 = new wxBoxSizer(wxVERTICAL);
	mGymNameLabel = new wxStaticText(this, wxID_ANY, wxT("��������:"), wxDefaultPosition, wxDefaultSize, 0);
	mGymNameLabel->Wrap(-1);
	mGymNameLabel->SetFont(wxFont(20, 70, 90, 90, false, wxEmptyString));
	bSizer22->Add(mGymNameLabel, 0, wxALL | wxALIGN_RIGHT, 5);
	bSizer19->Add(bSizer22, 1, wxALIGN_CENTER_VERTICAL, 5);
	wxBoxSizer* bSizer23 = new wxBoxSizer(wxVERTICAL);
	mGymNameEdit = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	mGymNameEdit->SetFont(wxFont(20, 70, 90, 90, false, wxEmptyString));
	bSizer23->Add(mGymNameEdit, 0, wxALL | wxEXPAND, 5);
	bSizer19->Add(bSizer23, 2, wxALIGN_CENTER_VERTICAL, 5);
	bSizer18->Add(bSizer19, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer20 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bSizer24 = new wxBoxSizer(wxVERTICAL);
	mDeviceCodeLabel = new wxStaticText(this, wxID_ANY, wxT("�豸��:"), wxDefaultPosition, wxDefaultSize, 0);
	mDeviceCodeLabel->Wrap(-1);
	mDeviceCodeLabel->SetFont(wxFont(20, 70, 90, 90, false, wxEmptyString));
	bSizer24->Add(mDeviceCodeLabel, 0, wxALL | wxALIGN_RIGHT, 5);
	bSizer20->Add(bSizer24, 1, wxALIGN_CENTER_VERTICAL, 5);
	wxBoxSizer* bSizer25 = new wxBoxSizer(wxVERTICAL);
	mDeviceCodeEdit = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	mDeviceCodeEdit->SetFont(wxFont(20, 70, 90, 90, false, wxEmptyString));
	bSizer25->Add(mDeviceCodeEdit, 0, wxALL | wxEXPAND, 5);
	bSizer20->Add(bSizer25, 2, wxALIGN_CENTER_VERTICAL, 5);
	bSizer18->Add(bSizer20, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer21 = new wxBoxSizer(wxVERTICAL);
	mConfirm = new wxButton(this, ID_REGISTE_BUTTON, wxT("ע��"), wxDefaultPosition, wxDefaultSize, 0);
	mConfirm->SetFont(wxFont(20, 70, 90, 90, false, wxEmptyString));
	bSizer21->Add(mConfirm, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	mTipLabel = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
	mTipLabel->Wrap(-1);
	mTipLabel->SetFont(wxFont(20, 70, 90, 90, false, wxEmptyString));
	bSizer21->Add(mTipLabel, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	bSizer18->Add(bSizer21, 2, wxEXPAND, 5);

	this->SetSizer(bSizer18);
	this->Layout();
}

void DeviceRegistePanel::refreshState()
{
	// �豸�������
	GymInfo* info = mEditorCore->getGymInfo();
	mTipLabel->SetLabel("");
	mDeviceCodeEdit->SetValue(info->mDeviceCode);
	mGymNameEdit->SetValue(info->mGymName);
}
//----------------------------------------------------------------------------------------------------
void DeviceRegistePanel::onRegisteButton(wxCommandEvent& event)
{
	// ��������Ѿ�ע����,�����ٴ�ע��
	GymInfo* info = mEditorCore->getGymInfo();
	if (info->mRegisted)
	{
		wxMessageBox("�ý����Ѿ�ע��,�����ٴ�ע��!");
		return;
	}
	// ������в����Ƿ񶼲�Ϊ��
	if (mGymNameEdit->GetValue() == "")
	{
		wxMessageBox("�����뽡������!");
		return;
	}
	// ���ý�����Ϣ,Ȼ�������������ע��
	info->mGymName = mGymNameEdit->GetValue();
	mTipLabel->SetLabel("����ע��!");
	info->registeDevice();
}

void DeviceRegistePanel::onRegisteResult(int ret)
{
	if (ret == 0)
	{
		// ����Ϊ�����Ѿ�ע��,����ע����Ϣҳ��
		GymInfo* info = mEditorCore->getGymInfo();
		mEditorFrame->getDeviceInfoPanel()->refreshInfo();
		mTipLabel->SetLabel("ע��ɹ�!");
	}
	else if (ret == 1)
	{
		mTipLabel->SetLabel("�޷����ӷ�����!");
	}
	else if (ret == 2)
	{
		mTipLabel->SetLabel("������������!");
	}
}