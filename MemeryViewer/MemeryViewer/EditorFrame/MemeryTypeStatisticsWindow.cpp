#include "MemeryTypeStatisticsWindow.h"
#include "EditorUtility.h"
#include "KeywordDialog.h"

enum
{
	ID_FILTER_TEXT,
	ID_SEARCH_HIDE_CHECK,
	ID_OPEN_LIST_BUTTON,
};

BEGIN_EVENT_TABLE(MemeryTypeStatisticsWindow, wxPanel)

EVT_TEXT(ID_FILTER_TEXT, OnFilterTextChanged)
EVT_CHECKBOX(ID_SEARCH_HIDE_CHECK, OnSearchHideItem)
EVT_BUTTON(ID_OPEN_LIST_BUTTON, OnOpenKeywordList)

END_EVENT_TABLE()

MemeryTypeStatisticsWindow::MemeryTypeStatisticsWindow(wxWindow* parent, long style, wxSize size)
:
wxPanel(parent, wxID_ANY, wxDefaultPosition, size, style)
{
	mKeywordListDialog = new KeywordDialog("�ڴ����͹ؼ���");
}

MemeryTypeStatisticsWindow::~MemeryTypeStatisticsWindow()
{
	delete mKeywordListDialog;
}

void MemeryTypeStatisticsWindow::init(MemeryViewer* viewer)
{
	mMemeryViewer = viewer;

	mKeywordListDialog->init();
	mKeywordListDialog->Hide();

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	mFilterText = new wxTextCtrl(this, ID_FILTER_TEXT, wxT(""), wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER);
	mFilterText->SetMaxLength(100);
	bSizer2->Add(mFilterText, 1, wxALL | wxEXPAND, 5);

	mSearchHideItem = new wxCheckBox(this, ID_SEARCH_HIDE_CHECK, wxT("����������"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(mSearchHideItem, 0, wxALL, 5);

	mOpenIgnoreKeywordList = new wxButton(this, ID_OPEN_LIST_BUTTON, wxT("�����б�"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(mOpenIgnoreKeywordList, 0, wxALL, 5);

	bSizer1->Add(bSizer2, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxHORIZONTAL);

	mMemeryCountLabel = new wxStaticText(this, wxID_ANY, wxT("�ڴ�������:0"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(mMemeryCountLabel, 0, wxALL, 5);
	
	mMemerySizeLabel = new wxStaticText(this, wxID_ANY, wxT("�ڴ��ܴ�С:0KB"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(mMemerySizeLabel, 0, wxALL, 5);

	mMemeryTypeCountLabel = new wxStaticText(this, wxID_ANY, wxT("�ڴ����͸���:0"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(mMemeryTypeCountLabel, 0, wxALL, 5);

	bSizer1->Add(bSizer3, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxHORIZONTAL);

	mTypeList = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxLC_NO_SORT_HEADER | wxLC_REPORT);
	mTypeList->InsertColumn(0, "������");
	mTypeList->SetColumnWidth(0, 450);
	mTypeList->InsertColumn(1, "����");
	mTypeList->SetColumnWidth(1, 80);
	mTypeList->InsertColumn(2, "��С");
	mTypeList->SetColumnWidth(2, 80);
	bSizer4->Add(mTypeList, 1, wxALL | wxEXPAND, 5);

	mSearchList = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxLC_NO_SORT_HEADER | wxLC_REPORT);
	mSearchList->InsertColumn(0, "������");
	mSearchList->SetColumnWidth(0, 450);
	mSearchList->InsertColumn(1, "����");
	mSearchList->SetColumnWidth(1, 80);
	mSearchList->InsertColumn(2, "��С");
	mSearchList->SetColumnWidth(2, 80);
	mSearchList->Hide();
	bSizer4->Add(mSearchList, 1, wxALL | wxEXPAND, 5);

	bSizer1->Add(bSizer4, 1, wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();
}

void MemeryTypeStatisticsWindow::update(float elapsedTime)
{
	if (mMemeryViewer == NULL)
	{
		return;
	}

	// �����ȡ�Ѿ�������ֱ�ӷ���,���ٵȴ�
	if (mMemeryViewer->isLockRead())
	{
		return;
	}

	mMemeryViewer->lockWrite();
	int listMaxCount = mTypeList->GetItemCount();
	int maxIndex = mMemeryViewer->getMaxIndex();
	// ����б��Ѿ���ʾ������,�������б�
	if (listMaxCount < maxIndex)
	{
		setMaxCount(maxIndex);
	}
	mIgnoreMemeryType.clear();
	int totalCount = 0;
	int totalSize = 0;
	int typeCount = 0;
	MemeryType* memeryList = mMemeryViewer->getMemeryList();
	std::set<std::string>& keywordList = mKeywordListDialog->getKeywordList();
	for (int i = 0; i < maxIndex; ++i)
	{
		if (memeryList[i].size > 0)
		{
			totalSize += memeryList[i].size;
		}
		if (memeryList[i].count > 0)
		{
			totalCount += memeryList[i].count;
			++typeCount;
		}
		// ��������0,���Ҳ��Ǳ����Ե�����,����ʾ���б�ָ��λ��
		if (memeryList[i].count > 0 && !EditorUtility::isIgnoreClass(keywordList, memeryList[i].type))
		{
			if (mTypeList->GetItemText(i, 0) != wxString(memeryList[i].type.c_str()))
			{
				mTypeList->SetItem(i, 0, memeryList[i].type.c_str());
			}
			// ��������иı�,��ô��СҲһ�����иı�
			char buf[16];
			sprintf_s(buf, 16, "%d", memeryList[i].count);
			if (mTypeList->GetItemText(i, 1) != wxString(buf))
			{
				mTypeList->SetItem(i, 1, buf);
				sprintf_s(buf, 16, "%d", memeryList[i].size);
				mTypeList->SetItem(i, 2, buf);
			}
		}
		else
		{
			// ����Ǳ����Ե�����,�������������б���
			if (EditorUtility::isIgnoreClass(keywordList, memeryList[i].type))
			{
				mIgnoreMemeryType.push_back(memeryList[i]);
				if (mTypeList->GetItemText(i, 0) != wxString("~~"))
				{
					mTypeList->SetItem(i, 0, "~~");
					mTypeList->SetItem(i, 1, "");
					mTypeList->SetItem(i, 2, "");
				}
			}
			// �������Ϊ0,��ʹ��-��ʾ
			else
			{
				if (mTypeList->GetItemText(i, 0) != wxString("-"))
				{
					mTypeList->SetItem(i, 0, "-");
					mTypeList->SetItem(i, 1, "-");
					mTypeList->SetItem(i, 2, "-");
				}
			}
		}
	}
	mMemeryViewer->unlockWrite();

	std::string totalCountString = std::string("�ڴ�������:") + EditorUtility::intToString(totalCount);
	if (wxString(totalCountString) != mMemeryCountLabel->GetLabel())
	{
		mMemeryCountLabel->SetLabel(totalCountString);
	}

	std::string totalSizeString = std::string("�ڴ��ܴ�С:") + EditorUtility::floatToString(totalSize / 1000.0f) + " KB";
	if (wxString(totalSizeString) != mMemerySizeLabel->GetLabel())
	{
		mMemerySizeLabel->SetLabel(totalSizeString);
	}

	std::string typeCountString = std::string("�ڴ����͸���:") + EditorUtility::intToString(typeCount);
	if (wxString(typeCountString) != mMemeryTypeCountLabel->GetLabel())
	{
		mMemeryTypeCountLabel->SetLabel(typeCountString);
	}
}

void MemeryTypeStatisticsWindow::destroy()
{
	mTypeList->DeleteAllItems();
	mTypeList->DeleteAllColumns();
}

void MemeryTypeStatisticsWindow::setMaxCount(int count)
{
	if (count < 0 || count > 1000)
	{
		return;
	}
	int itemCount = mTypeList->GetItemCount();
	// �����ǰ��ʾ�������Ѿ�����ָ������ʾ����,��ɾ���������
	if (itemCount > count)
	{
		// ���б�β����ʼɾ��
		for (int i = 0; i < itemCount - count; ++i)
		{
			mTypeList->DeleteItem(itemCount - i - 1);
		}
	}
	// �����ǰ��ʾ������С��ָ������ʾ����,�����б�ĩβ��ӿ���
	else if (itemCount < count)
	{
		for (int i = 0; i < count - itemCount; ++i)
		{
			mTypeList->InsertItem(mTypeList->GetItemCount(), "");
		}
	}
}

void MemeryTypeStatisticsWindow::OnFilterTextChanged(wxCommandEvent& event)
{
	// ���������Ϊ��,�����������б�,��ʾ��������б�
	std::string filterText = mFilterText->GetValue().ToStdString();
	if (filterText != wxString(""))
	{
		mTypeList->Hide();
		mSearchList->Show();
		mSearchList->DeleteAllItems();
		int typeItemCount = mTypeList->GetItemCount();
		for (int i = 0; i < typeItemCount; ++i)
		{
			wxString itemText = mTypeList->GetItemText(i, 0);
			if (strstr(itemText.ToStdString().c_str(), filterText.c_str()) != NULL)
			{
				int itemIndex = mSearchList->InsertItem(mSearchList->GetItemCount(), "");
				mSearchList->SetItem(itemIndex, 0, itemText);
				mSearchList->SetItem(itemIndex, 1, mTypeList->GetItemText(i, 1));
				mSearchList->SetItem(itemIndex, 2, mTypeList->GetItemText(i, 2));
			}
		}
		// �����Ҫ����������,���ں����б��м�������
		if (mSearchHideItem->GetValue())
		{
			int listSize = mIgnoreMemeryType.size();
			for (int i = 0; i < listSize; ++i)
			{
				if (strstr(mIgnoreMemeryType[i].type.c_str(), filterText.c_str()) != NULL)
				{
					int itemIndex = mSearchList->InsertItem(mSearchList->GetItemCount(), "");
					mSearchList->SetItem(itemIndex, 0, mIgnoreMemeryType[i].type);
					mSearchList->SetItem(itemIndex, 1, EditorUtility::intToString(mIgnoreMemeryType[i].count));
					mSearchList->SetItem(itemIndex, 2, EditorUtility::intToString(mIgnoreMemeryType[i].size));
				}
			}
		}
	}
	// ���������Ϊ��,����ʾ�����б�,������������б�
	else
	{
		mSearchList->Hide();
		mTypeList->Show();
		mSearchList->DeleteAllItems();
	}
	// ˢ�²���
	Layout();
	event.Skip();
}

void MemeryTypeStatisticsWindow::OnSearchHideItem(wxCommandEvent& event)
{
	event.Skip();
}

void MemeryTypeStatisticsWindow::OnOpenKeywordList(wxCommandEvent& event)
{
	mKeywordListDialog->Show(!mKeywordListDialog->IsShown());
	if (mKeywordListDialog->IsShown())
	{
		mKeywordListDialog->SetPosition(GetScreenPosition());
	}
	event.Skip();
}