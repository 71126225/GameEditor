#include "EditorApp.h"
#include "EditorFrame.h"
#include "MainViewerWindow.h"
#include "EditorUtility.h"
#include "Data.h"
#include "DataBase.h"
#include "DataTemplate.h"
#include "txUtility.h"

enum
{
	ID_DATA_INFO_VIEW,
	ID_INSERT_COUNT_TEXT,
	ID_FROM_ITEM_TEXT,
	ID_TO_ITEM_TEXT,
	ID_TOOL_DELETE_ITEM,
	ID_TOOL_INSERT_ITEM_FRONT,
	ID_TOOL_INSERT_ITEM_BELOW,
	ID_TOOL_RADIO_INT_EXPRESSION,
	ID_TOOL_RADIO_FLOAT_EXPRESSION,
	ID_TOOL_RADIO_NUMBER_EXPRESSION,
	ID_TOOL_RADIO_STRING_EXPRESSION,
	ID_BUTTON_SET_ITEM,
};

BEGIN_EVENT_TABLE(MainViewerWindow, wxPanel)

EVT_DATAVIEW_ITEM_EDITING_DONE(ID_DATA_INFO_VIEW, OnDataItemEditDone)
EVT_DATAVIEW_SELECTION_CHANGED(ID_DATA_INFO_VIEW, OnDataItemSelectionChanged)
EVT_DATAVIEW_COLUMN_HEADER_CLICK(ID_DATA_INFO_VIEW, OnDataColumnSelected)
EVT_TOOL(ID_TOOL_DELETE_ITEM, OnToolDeleteItem)
EVT_TOOL(ID_TOOL_INSERT_ITEM_FRONT, OnToolInsertItemFront)
EVT_TOOL(ID_TOOL_INSERT_ITEM_BELOW, OnToolInsertItemBelow)
EVT_TEXT(ID_INSERT_COUNT_TEXT, OnInsertCountText)
EVT_TEXT(ID_FROM_ITEM_TEXT, OnFromItemText)
EVT_TEXT(ID_TO_ITEM_TEXT, OnToItemText)
EVT_BUTTON(ID_BUTTON_SET_ITEM, OnSetItemButton)
EVT_TOOL(ID_TOOL_RADIO_INT_EXPRESSION, OnIntExpressionCheck)
EVT_TOOL(ID_TOOL_RADIO_FLOAT_EXPRESSION, OnFloatExpressionCheck)
EVT_TOOL(ID_TOOL_RADIO_NUMBER_EXPRESSION, OnNumberExpressionCheck)
EVT_TOOL(ID_TOOL_RADIO_STRING_EXPRESSION, OnStringExpressionCheck)

END_EVENT_TABLE()

MainViewerWindow::MainViewerWindow(wxWindow* parent, long style)
:
wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, style),
mSelectColumn(-1),
mIntOrFloat(true),
mNumberOrString(true)
{
	mEditorFrame = wxGetApp().GetMainFrame();
	setup();
}

void MainViewerWindow::setup()
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	mAuiToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 20));
	mAuiToolBar->SetToolBitmapSize(wxSize(16, 16));

	mInsertCountText = new wxTextCtrl(mAuiToolBar, ID_INSERT_COUNT_TEXT, "1");
	mAuiToolBar->AddControl(mInsertCountText);
	mAuiToolBar->AddTool(ID_TOOL_INSERT_ITEM_FRONT, wxT("�����������"), wxBITMAP(IDB_INSERT_FRONT), wxT("�����������"));
	mAuiToolBar->AddTool(ID_TOOL_INSERT_ITEM_BELOW, wxT("�����������"), wxBITMAP(IDB_INSERT_BELOW), wxT("�����������"));
	mAuiToolBar->AddTool(ID_TOOL_DELETE_ITEM, wxT("ɾ��ѡ����"), wxBITMAP(IDB_DELETE_ITEM), wxT("ɾ��ѡ����"));
	mAuiToolBar->AddSeparator();

	mAuiToolBar->AddLabel(wxID_ANY, wxT("  ������:��"));
	mFromItemText = new wxTextCtrl(mAuiToolBar, ID_FROM_ITEM_TEXT, "0");
	mAuiToolBar->AddControl(mFromItemText);
	mAuiToolBar->AddLabel(wxID_ANY, wxT("��"));
	mToItemText = new wxTextCtrl(mAuiToolBar, ID_TO_ITEM_TEXT, "0");
	mAuiToolBar->AddControl(mToItemText);
	mAuiToolBar->AddLabel(wxID_ANY, wxT("��"));
	mAuiToolBar->AddLabel(wxID_ANY, wxT("  ����:"));
	mSelectColumnNameText = new wxTextCtrl(mAuiToolBar, ID_TO_ITEM_TEXT, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	mAuiToolBar->AddControl(mSelectColumnNameText);
	mAuiToolBar->AddLabel(wxID_ANY, wxT("  ֵ:"));
	mItemValueText = new wxTextCtrl(mAuiToolBar, wxID_ANY, "0");
	mAuiToolBar->AddControl(mItemValueText);

	mAuiToolBar->AddTool(ID_TOOL_RADIO_INT_EXPRESSION, wxT("����"), wxBITMAP(IDB_INT_EXPRESSION), wxT("�������ʽ֧�ּӼ��˳�����ȡ��"), wxITEM_RADIO);
	mAuiToolBar->AddTool(ID_TOOL_RADIO_FLOAT_EXPRESSION, wxT("������"), wxBITMAP(IDB_FLOAT_EXPRESSION), wxT("���������ʽ֧�ּӼ��˳�����"), wxITEM_RADIO);
	mAuiToolBar->AddSeparator();
	mAuiToolBar->AddTool(ID_TOOL_RADIO_NUMBER_EXPRESSION, wxT("��ֵ"), wxBITMAP(IDB_NUMBER_EXPRESSION), wxT("���ս��Ϊ��ֵ,ֻ��������ʽ,���ʽ�е�i�ᱻ�滻Ϊѡ�е�˳���±�"), wxITEM_RADIO);
	mAuiToolBar->AddTool(ID_TOOL_RADIO_STRING_EXPRESSION, wxT("�ַ���"), wxBITMAP(IDB_STRING_EXPRESSION), wxT("���ս��Ϊ�ַ���,ʹ��\\()�������ʽ,���ʽ��i�ᱻ�滻Ϊѡ�е�˳���±�"), wxITEM_RADIO);
	if (mIntOrFloat)
	{
		mAuiToolBar->ToggleTool(ID_TOOL_RADIO_INT_EXPRESSION, true);
	}
	else
	{
		mAuiToolBar->ToggleTool(ID_TOOL_RADIO_FLOAT_EXPRESSION, true);
	}
	if (mNumberOrString)
	{
		mAuiToolBar->ToggleTool(ID_TOOL_RADIO_NUMBER_EXPRESSION, true);
	}
	else
	{
		mAuiToolBar->ToggleTool(ID_TOOL_RADIO_STRING_EXPRESSION, true);
	}
	wxButton* setItemButton = new wxButton(mAuiToolBar, ID_BUTTON_SET_ITEM, "ȷ��");
	mAuiToolBar->AddControl(setItemButton);
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

void MainViewerWindow::showData(const std::string& type)
{
	DataBase* dataBase = mEditorFrame->getDataBase();
	// ֪ͨ��ʾ�˱������
	mEditorFrame->notifyOpenTable(type);
	mShowDataType = type;
	mDataInfoListCtrl->DeleteAllItems();
	mDataInfoListCtrl->ClearColumns();

	Data* tempData = dataBase->createData(type);
	mShowDataType = tempData->getType();
	const std::vector<DataParameter>& parameterList = tempData->getParameterList();
	int paramCount = parameterList.size();
	for (int i = 0; i < paramCount; ++i)
	{
		const DataParameter& dataParam = parameterList[i];
		std::string typeString = dataParam.mDataType;
		if (txSerializedData::isArrayType(dataParam.mDataType))
		{
			typeString += txUtility::intToString(dataParam.mDataSize / Data::getTypeSize(dataParam.mDataType));
		}
		mDataInfoListCtrl->AppendTextColumn(dataParam.mDescribe + "(" + typeString + ")", wxDATAVIEW_CELL_EDITABLE, 150);
	}
	dataBase->destroyData(tempData);

	int dataCount = dataBase->getDataCount(type);
	for (int i = 0; i < dataCount; ++i)
	{
		Data* data = dataBase->queryData(type, i);
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

void MainViewerWindow::deleteItem(int index)
{
	if (index >= 0 && index < mDataInfoListCtrl->GetItemCount())
	{
		DataBase* dataBase = mEditorFrame->getDataBase();
		dataBase->deleteData(mShowDataType, index);
		mDataInfoListCtrl->DeleteItem(index);
		mEditorFrame->setDataModified(true);
	}
}

void MainViewerWindow::insertItem(int index)
{
	DataBase* dataBase = mEditorFrame->getDataBase();
	Data* data = dataBase->createData(mShowDataType);
	dataBase->addData(mShowDataType, data, index);
	wxVector<wxVariant> valueList;
	// ���ø��е���������
	const std::vector<DataParameter>& parameterList = data->getParameterList();
	int paramCount = parameterList.size();
	for (int j = 0; j < paramCount; ++j)
	{
		std::string dataString = data->getValueString(j);
		valueList.push_back(dataString);
	}
	if (index >= 0)
	{
		mDataInfoListCtrl->InsertItem(index, valueList);
	}
	else
	{
		mDataInfoListCtrl->AppendItem(valueList);
	}
	mEditorFrame->setDataModified(true);
}

void MainViewerWindow::OnDataItemEditDone(wxDataViewEvent& event)
{
	DataBase* dataBase = mEditorFrame->getDataBase();
	std::string itemText = ((wxString)event.GetValue()).ToStdString();
	int itemIndex = mDataInfoListCtrl->ItemToRow(event.GetItem());
	int columnIndex = event.GetColumn();
	Data* data = dataBase->queryData(mShowDataType, itemIndex);
	data->writeData(itemText, columnIndex);
	mEditorFrame->setDataModified(true);
}

void MainViewerWindow::OnDataItemSelectionChanged(wxDataViewEvent& event)
{
	// ���ݵ�ǰѡ�е������Զ��ж�ѡ���������Ĵӵڼ���ڼ���
	mSelectItemList.clear();
	wxDataViewItemArray itemArray;
	int count = mDataInfoListCtrl->GetSelections(itemArray);
	if (count > 0)
	{
		int lastItem = mDataInfoListCtrl->ItemToRow(itemArray[0]);
		mSelectItemList.push_back(lastItem);
		for (int i = 1; i < count; ++i)
		{
			// ����������������˳�ѭ��
			int curItemIndex = mDataInfoListCtrl->ItemToRow(itemArray[i]);
			if (curItemIndex != lastItem + 1)
			{
				break;
			}
			mSelectItemList.push_back(curItemIndex);
			lastItem = curItemIndex;
		}
	}
	if (mSelectItemList.size() > 0)
	{
		mFromItemText->SetValue(txUtility::intToString(mSelectItemList[0]));
		mToItemText->SetValue(txUtility::intToString(mSelectItemList[mSelectItemList.size() - 1]));
	}
	else
	{
		mFromItemText->SetValue(wxT("0"));
		mToItemText->SetValue(wxT("0"));
	}
}

void MainViewerWindow::OnDataColumnSelected(wxDataViewEvent& event)
{
	const DataTemplate* dataTemplate = mEditorFrame->getDataBase()->getDataTemplate(mShowDataType);
	mSelectColumn = event.GetColumn();
	if (mSelectColumn >= 0 && mSelectColumn < (int)dataTemplate->getParameterList().size())
	{
		const DataParameter& dataParameter = dataTemplate->getParameterList()[mSelectColumn];
		mSelectColumnNameText->SetValue(dataParameter.mDescribe);
		// ����ѡ��������Զ��л������򸡵�������ֵ���ַ���
		if (txUtility::endWith(dataParameter.mDataType, "*"))
		{
			// ����Ϊ�ַ�������,����Ĭ��Ϊ�������㷨��
			setIntOrFloat(true);
			setNumberOrString(false);
		}
		else
		{
			// ����Ϊ��ֵ����
			setNumberOrString(true);
			setIntOrFloat(dataParameter.mDataType != Data::mFloatType);
		}
	}
}

void MainViewerWindow::OnToolDeleteItem(wxCommandEvent& event)
{
	int selectCount = mSelectItemList.size();
	if (selectCount == 0)
	{
		return;
	}
	for (int i = 0; i < selectCount; ++i)
	{
		deleteItem(mSelectItemList[i]);
	}
}

void MainViewerWindow::OnToolInsertItemFront(wxCommandEvent& event)
{
	if (mShowDataType == "")
	{
		return;
	}
	int startItemIndex = 0;
	if (mSelectItemList.size() > 0)
	{
		startItemIndex = mSelectItemList[0];
	}
	int insertCount = txUtility::stringToInt(mInsertCountText->GetValue().ToStdString());
	if (insertCount > 0)
	{
		for (int i = 0; i < insertCount; ++i)
		{
			insertItem(startItemIndex);
		}
	}
	else
	{
		EditorFrame::errorMessage("������Ҫ������е�����");
	}
}

void MainViewerWindow::OnToolInsertItemBelow(wxCommandEvent& event)
{
	if (mShowDataType == "")
	{
		return;
	}
	int startItemIndex = -1;
	if (mSelectItemList.size() > 0)
	{
		startItemIndex = mSelectItemList[0];
	}
	int insertCount = txUtility::stringToInt(mInsertCountText->GetValue().ToStdString());
	if (insertCount > 0)
	{
		for (int i = 0; i < insertCount; ++i)
		{
			insertItem(startItemIndex + 1 + i);
		}
	}
	else
	{
		EditorFrame::errorMessage("������Ҫ������е�����");
	}
}

void MainViewerWindow::OnInsertCountText(wxCommandEvent& event)
{
	// ����ʱ����ַ�,�����������������ַ�,����������ַ�
	if (!EditorUtility::checkNumber(mInsertCountText->GetValue()))
	{
		mInsertCountText->SetValue(wxT(""));
	}
}

void MainViewerWindow::OnFromItemText(wxCommandEvent& event)
{
	// ����ʱ����ַ�,�����������������ַ�,����������ַ�
	if (!EditorUtility::checkNumber(mFromItemText->GetValue()))
	{
		mFromItemText->SetValue("");
	}
}

void MainViewerWindow::OnToItemText(wxCommandEvent& event)
{
	// ����ʱ����ַ�,�����������������ַ�,����������ַ�
	if (!EditorUtility::checkNumber(mToItemText->GetValue()))
	{
		mToItemText->SetValue(wxT(""));
	}
}

void MainViewerWindow::OnSetItemButton(wxCommandEvent& event)
{
	if (mSelectColumn < 0)
	{
		EditorFrame::errorMessage("��ѡ��Ҫ���õ���");
		return;
	}
	// �˴���Ҫ�޸ĵ�ѡ����������б���ѡ�е�������
	// ��mSelectItemList�޹�,ֻ���ı�������д����ʼ�к���ֹ���й�
	// ��Ϊ���޸���ʼ�к���ֹ���ı���ʱ���жϲ��޸�ѡ���б����鷳
	int startItemIndex = txUtility::stringToInt(mFromItemText->GetValue().ToStdString());
	int endItemIndex = txUtility::stringToInt(mToItemText->GetValue().ToStdString());
	int modifyItemCount = endItemIndex - startItemIndex + 1;
	if (modifyItemCount > 0)
	{
		DataBase* dataBase = mEditorFrame->getDataBase();
		for (int i = 0; i < modifyItemCount; ++i)
		{
			std::string finalValue = mItemValueText->GetValue().ToStdString();
			if (mNumberOrString)
			{
				// ������ı��ʽ�д���i,����Ҫ��i�滻Ϊi�����ֵ,Ȼ���������ı��ʽ��ֵ
				int iPos;
				while ((iPos = finalValue.find_first_of("i")) != -1)
				{
					finalValue = txUtility::strReplace(finalValue, iPos, iPos + strlen("i"), txUtility::intToString(i));
				}
				if (mIntOrFloat)
				{
					finalValue = txUtility::intToString(txUtility::calculateInt(finalValue));
				}
				else
				{
					finalValue = txUtility::floatToString(txUtility::calculateFloat(finalValue), 4);
				}
			}
			else
			{
				int expressionBegin = -1;
				int expressionEnd = -1;
				// ����Ѱ��
				while (txUtility::findSubstr(finalValue, std::string("\\("), true, &expressionBegin, 0, false))
				{
					// �ҵ�ƥ��� )
					txUtility::findSubstr(finalValue, std::string(")"), true, &expressionEnd, 0, false);

					// expressionBegin + 1 ȥ�� /
					std::string calculateValue = finalValue.substr(expressionBegin + 1, expressionEnd - (expressionBegin + 1) + 1);

					// ������ı��ʽ�д���i,����Ҫ��i�滻Ϊi�����ֵ,Ȼ���������ı��ʽ��ֵ
					int iPos = 0;
					while (txUtility::findSubstr(calculateValue, std::string("i"), true, &iPos))
					{
						calculateValue = txUtility::strReplace(calculateValue, iPos, iPos + strlen("i"), txUtility::intToString(i));
					}

					if (mIntOrFloat)
					{
						calculateValue = txUtility::intToString(txUtility::calculateInt(calculateValue));
					}
					else
					{
						calculateValue = txUtility::floatToString(txUtility::calculateFloat(calculateValue), 4);
					}
					// �滻�����һ��\\()֮�������
					finalValue = txUtility::strReplace(finalValue, expressionBegin, expressionEnd + 1, calculateValue);
				}
			}
			Data* data = dataBase->queryData(mShowDataType, i + startItemIndex);
			data->writeData(finalValue, mSelectColumn);
			mDataInfoListCtrl->SetTextValue(finalValue, i + startItemIndex, mSelectColumn);
		}
		mEditorFrame->setDataModified(true);
	}
}

void MainViewerWindow::OnIntExpressionCheck(wxCommandEvent& event)
{
	mIntOrFloat = true;
}

void MainViewerWindow::OnFloatExpressionCheck(wxCommandEvent& event)
{
	mIntOrFloat = false;
}

void MainViewerWindow::OnNumberExpressionCheck(wxCommandEvent& event)
{
	mNumberOrString = true;
}

void MainViewerWindow::OnStringExpressionCheck(wxCommandEvent& event)
{
	mNumberOrString = false;
}

void MainViewerWindow::setIntOrFloat(bool intOrFloat)
{
	mIntOrFloat = intOrFloat;
	if (mIntOrFloat && !mAuiToolBar->GetToolToggled(ID_TOOL_RADIO_INT_EXPRESSION))
	{
		mAuiToolBar->ToggleTool(ID_TOOL_RADIO_INT_EXPRESSION, true);
	}
	else if (!mIntOrFloat && !mAuiToolBar->GetToolToggled(ID_TOOL_RADIO_FLOAT_EXPRESSION))
	{
		mAuiToolBar->ToggleTool(ID_TOOL_RADIO_FLOAT_EXPRESSION, true);
	}
	mAuiToolBar->Refresh();
}

void MainViewerWindow::setNumberOrString(bool numberOrString)
{
	mNumberOrString = numberOrString;
	if (mNumberOrString && !mAuiToolBar->GetToolToggled(ID_TOOL_RADIO_NUMBER_EXPRESSION))
	{
		mAuiToolBar->ToggleTool(ID_TOOL_RADIO_NUMBER_EXPRESSION, true);
	}
	else if (!mNumberOrString && !mAuiToolBar->GetToolToggled(ID_TOOL_RADIO_STRING_EXPRESSION))
	{
		mAuiToolBar->ToggleTool(ID_TOOL_RADIO_STRING_EXPRESSION, true);
	}
	mAuiToolBar->Refresh();
}