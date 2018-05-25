#include "ExpressionEditor.h"
#include "PropertyWindow.h"

enum
{
	ID_OK_BUTTON,
	ID_CANCEL_BUTTON,
};

BEGIN_EVENT_TABLE(ExpressionEditor, EditorDialog)
EVT_SHOW(ExpressionEditor::OnShow)
EVT_BUTTON(ID_OK_BUTTON, ExpressionEditor::OnOKButton)
EVT_BUTTON(ID_CANCEL_BUTTON, ExpressionEditor::OnCancelButton)
END_EVENT_TABLE()

ExpressionEditor::ExpressionEditor(wxWindow* parent, const wxString& title, const wxPoint& position, const wxSize& size, long style, const wxString& name)
	:
	EditorDialog(parent, title, position, size, style, name)
{
	Setup();
	mOK = false;
	mPropertyWindow = static_cast<PropertyWindow*>(parent);
}

void ExpressionEditor::Setup()
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	mLabel0 = new wxStaticText(this, wxID_ANY, wxT("���ʽ:"), wxDefaultPosition, wxDefaultSize, 0);
	mLabel0->Wrap(-1);
	bSizer2->Add(mLabel0, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	mExpressionText = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(mExpressionText, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);


	bSizer1->Add(bSizer2, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxHORIZONTAL);

	mFloatRadio = new wxRadioButton(this, wxID_ANY, wxT("���������㷨��"), wxDefaultPosition, wxDefaultSize, 0);
	mFloatRadio->SetValue(true);
	bSizer3->Add(mFloatRadio, 0, wxALL, 5);

	mIntRadio = new wxRadioButton(this, wxID_ANY, wxT("�������㷨��"), wxDefaultPosition, wxDefaultSize, 0);
	mIntRadio->SetValue(false);
	bSizer3->Add(mIntRadio, 0, wxALL, 5);


	bSizer1->Add(bSizer3, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer(wxHORIZONTAL);
	wxString tipText = "�޸���������ʱ,ʹ��\\()�������ʽ,���ʽ��i�ᱻ�滻Ϊѡ�е��±�,\n";
	tipText += "�޸�������ֵ����ʱ, ֻ��������ʽ, ���ʽ�е�i�ᱻ�滻Ϊѡ�е��±�,\n";
	tipText += "���������ʽ֧�ּӼ��˳�����,�������ʽ֧�ּӼ��˳�����ȡ��.";
	mTipText = new wxStaticText(this, wxID_ANY, tipText, wxDefaultPosition, wxDefaultSize, 0);
	mTipText->Wrap(-1);
	bSizer7->Add(mTipText, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	bSizer1->Add(bSizer7, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer(wxVERTICAL);

	mButtonOK = new wxButton(this, ID_OK_BUTTON, wxT("ȷ��"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer5->Add(mButtonOK, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);


	bSizer4->Add(bSizer5, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer(wxVERTICAL);

	mButtonCancel = new wxButton(this, ID_CANCEL_BUTTON, wxT("ȡ��"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer6->Add(mButtonCancel, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);


	bSizer4->Add(bSizer6, 1, wxEXPAND, 5);


	bSizer1->Add(bSizer4, 1, wxEXPAND, 5);


	this->SetSizer(bSizer1);
	this->Layout();
}

void ExpressionEditor::OnOKButton(wxCommandEvent& event)
{
	mOK = true;
	Show(false);
}

void ExpressionEditor::OnCancelButton(wxCommandEvent& event)
{
	mOK = false;
	Show(false);
}

void ExpressionEditor::OnShow(wxShowEvent& event)
{
	// ͨ�����ȷ�϶��رմ���ʱ
	if (!event.IsShown() && mOK)
	{
		mPropertyWindow->setSpecialPropertyValue(mExpressionText->GetValue().ToStdString(), mFloatRadio->GetValue());
	}
	// ������ʾʱ��Ҫ����OK��ʶ
	else
	{
		mOK = false;
	}
}