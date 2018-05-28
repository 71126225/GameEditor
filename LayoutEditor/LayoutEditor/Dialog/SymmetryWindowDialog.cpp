#include "Layout.h"
#include "WindowProperty.h"
#include "LayoutUtility.h"

#include "LayoutEditorCore.h"
#include "CommandHeader.h"
#include "WindowHeader.h"

#include "SymmetryWindowDialog.h"
#include "EditorApp.h"
#include "EditorFrame.h"

enum
{
	ID_OK_BUTTON,
	ID_CLOSE_BUTTON,
};

BEGIN_EVENT_TABLE(SymmetryWindowDialog, EditorDialog)
EVT_BUTTON(ID_OK_BUTTON, SymmetryWindowDialog::OnOKButton)
EVT_BUTTON(ID_CLOSE_BUTTON, SymmetryWindowDialog::OnCloseButton)
END_EVENT_TABLE()

SymmetryWindowDialog::SymmetryWindowDialog(wxWindow* parent, const wxString& title, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
EditorDialog(parent, title, wxDefaultPosition, size, style, name)
{
	setup();
}

void SymmetryWindowDialog::setup()
{
	wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer8 = new wxBoxSizer(wxVERTICAL);
	wxStaticText* m_staticText5 = new wxStaticText(this, wxID_ANY, wxT("��������λ��,ʹ֮��Դ�����ڸ���������һ����ʽ�Գ�,������Դ���ڱ�������ͬһ��������"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer8->Add(m_staticText5, 0, wxALL, 5);
	bSizer1->Add(bSizer8, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer2 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* m_staticText1 = new wxStaticText(this, wxID_ANY, wxT("����:"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(m_staticText1, 0, wxALL, 5);
	mOperateWindowText = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(mOperateWindowText, 1, wxALL, 5);
	bSizer1->Add(bSizer2, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer3 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* m_staticText2 = new wxStaticText(this, wxID_ANY, wxT("�Գ�Դ����:"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(m_staticText2, 0, wxALL, 5);
	mSourceWindowText = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(mSourceWindowText, 1, wxALL, 5);
	bSizer1->Add(bSizer3, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer4 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bSizer6 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* m_staticText3 = new wxStaticText(this, wxID_ANY, wxT("X�ԳƷ�ʽ:"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer6->Add(m_staticText3, 0, wxALL, 5);
	mXSymmetryModeCombo = new wxComboBox(this, wxID_ANY, wxT("���������ĶԳ�"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0);
	mXSymmetryModeCombo->Append(wxT("��Դ������ͬ"));
	mXSymmetryModeCombo->Append(wxT("���������ĶԳ�"));
	mXSymmetryModeCombo->Append(wxT("��"));
	mXSymmetryModeCombo->SetSelection(1);
	bSizer6->Add(mXSymmetryModeCombo, 0, wxALL, 5);
	mConsiderWindowSizeXCheck = new wxCheckBox(this, wxID_ANY, wxT("���Ǵ���X��С"), wxDefaultPosition, wxDefaultSize, 0);
	mConsiderWindowSizeXCheck->SetValue(true);
	bSizer6->Add(mConsiderWindowSizeXCheck, 0, wxALL, 5);
	bSizer4->Add(bSizer6, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer7 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* m_staticText4 = new wxStaticText(this, wxID_ANY, wxT("Y�ԳƷ�ʽ:"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer7->Add(m_staticText4, 0, wxALL, 5);
	mYSymmetryModeCombo = new wxComboBox(this, wxID_ANY, wxT("��Դ������ͬ"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0);
	mYSymmetryModeCombo->Append(wxT("��Դ������ͬ"));
	mYSymmetryModeCombo->Append(wxT("���������ĶԳ�"));
	mYSymmetryModeCombo->Append(wxT("��"));
	mYSymmetryModeCombo->SetSelection(0);
	bSizer7->Add(mYSymmetryModeCombo, 0, wxALL, 5);
	mConsiderWindowSizeYCheck = new wxCheckBox(this, wxID_ANY, wxT("���Ǵ���Y��С"), wxDefaultPosition, wxDefaultSize, 0);
	mConsiderWindowSizeYCheck->SetValue(true);
	bSizer7->Add(mConsiderWindowSizeYCheck, 0, wxALL, 5);
	bSizer4->Add(bSizer7, 1, wxEXPAND, 5);
	bSizer1->Add(bSizer4, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer5 = new wxBoxSizer(wxHORIZONTAL);
	mOKButton = new wxButton(this, ID_OK_BUTTON, wxT("ȷ��"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer5->Add(mOKButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	mCloseButton = new wxButton(this, ID_CLOSE_BUTTON, wxT("�ر�"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer5->Add(mCloseButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	bSizer1->Add(bSizer5, 1, wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();
}

void SymmetryWindowDialog::OnOKButton(wxCommandEvent& event)
{
	::Layout* layout = mLayoutEditorCore->getCurLayout();
	if (layout != NULL)
	{
		// ��ò����Ĵ���
		std::string windowName = mOperateWindowText->GetValue().ToStdString();
		Window* window = layout->getWindow(windowName);
		if (window == NULL)
		{
			mEditorFrame->logError("error : can not find window : " + windowName + " in current layout!");
			return;
		}
		std::string sourceWindowName = mSourceWindowText->GetValue().ToStdString();
		Window* sourceWindow = layout->getWindow(sourceWindowName);
		if (sourceWindow == NULL)
		{
			mEditorFrame->logError("error : can not find start window : " + sourceWindowName + " in current layout!");
			return;
		}

		txPoint windowPos = window->getPosition();
		const txPoint& sourcePos = sourceWindow->getPosition();
		// x��λ��
		std::string xSymmetry = mXSymmetryModeCombo->GetValue().ToStdString();
		if (xSymmetry == "��Դ������ͬ")
		{
			windowPos.x = sourcePos.x;
		}
		else if (xSymmetry == "���������ĶԳ�")
		{
			windowPos.x = txDim::IDENTITY - sourcePos.x;
			if (mConsiderWindowSizeXCheck->GetValue())
			{
				windowPos.x -= window->getSize().x;
			}
		}
		else if (xSymmetry == "��")
		{
			;
		}
		else
		{
			return;
		}

		// y��λ��
		std::string ySymmetry = mYSymmetryModeCombo->GetValue().ToStdString();
		if (ySymmetry == "��Դ������ͬ")
		{
			windowPos.y = sourcePos.y;
		}
		else if (ySymmetry == "���������ĶԳ�")
		{
			windowPos.y = txDim::IDENTITY - sourcePos.y;
			if (mConsiderWindowSizeYCheck->GetValue())
			{
				windowPos.y -= window->getSize().y;
			}
		}
		else if (ySymmetry == "��")
		{
			;
		}
		else
		{
			return;
		}
		CommandWindowSetProperty* cmdProperty = NEW_CMD(cmdProperty);
		cmdProperty->mPropertyName = txProperty::getPropertyName<WindowPropertyPosition>();
		cmdProperty->mPropertyValue = txStringUtility::pointToString(windowPos);
		cmdProperty->mOperator = this;
		mCommandSystem->pushCommand(cmdProperty, window);
	}
}

void SymmetryWindowDialog::OnCloseButton(wxCommandEvent& event)
{
	Show(false);
}