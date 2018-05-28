#include "txEngineRoot.h"
#include "txPoint.h"
#include "txPropertyManager.h"

#include "WindowHeader.h"
#include "Layout.h"
#include "LayoutUtility.h"
#include "LayoutManager.h"
#include "WindowProperty.h"
#include "TextureWindowProperty.h"

#include "LayoutEditorCore.h"
#include "ECUtility.h"
#include "EditorCommandHeader.h"

#include "PropertyWindow.h"
#include "CustomProperty.h"
#include "EditorApp.h"
#include "EditorFrame.h"
#include "LayoutHierarchy.h"
#include "ExpressionEditor.h"

enum 
{
	ID_MENU_TO_ABSOLUTE, 
	ID_MENU_TO_RELATIVE,
	ID_MENU_USE_TEXTURE_SIZE,
	ID_MENU_SCALE_APPLY_TO_SIZE,
};

BEGIN_EVENT_TABLE(PropertyWindow, PropertyPanelBase)
EVT_MENU(ID_MENU_TO_ABSOLUTE, PropertyWindow::OnToAbsolute)
EVT_MENU(ID_MENU_TO_RELATIVE, PropertyWindow::OnToRelative)
EVT_MENU(ID_MENU_USE_TEXTURE_SIZE, PropertyWindow::OnUseTextureSize)
EVT_MENU(ID_MENU_SCALE_APPLY_TO_SIZE, PropertyWindow::OnScaleApplyToSize)
END_EVENT_TABLE()

PropertyWindow::PropertyWindow(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
PropertyPanelBase(parent, position, size, style, name),
mSpecialValueProperty(NULL),
mRightProperty(NULL)
{
	mExpressionEditor = TRACE_NEW(ExpressionEditor, mExpressionEditor, this, wxT("���ʽ�༭"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, wxT("ExpressionEditor"));
	mExpressionEditor->SetSize(450, 250);
}

PropertyWindow::~PropertyWindow()
{
	TRACE_DELETE(mExpressionEditor);
}

void PropertyWindow::onRefreshProperty()
{
	mLayoutEditorCore->reselect();
}

void PropertyWindow::populateProperties(txVector<txPropertyReceiver*>& receiverList)
{
	int windowCount = receiverList.size();
	if (windowCount == 0)
	{
		return;
	}
	Window* firstWindow = static_cast<Window*>(receiverList[0]);
	// ֻ��ʾһ�����ڵ�����ʱ�Ż���ʾ����
	if (windowCount == 1)
	{
		const std::string& typeName = firstWindow->getTypeName();
		mPropertyStandPage->Append(new wxStringProperty(TYPE_PROPERTY_NAME, TYPE_PROPERTY_NAME, wxString(typeName.c_str())))->Enable(false);
		mPropertyStandPage->SetPropertyHelpString(wxString(TYPE_PROPERTY_NAME), wxT("��������"));
	}

	// ��ʾ��һ�����ڵ�����
	const std::string& windowName = firstWindow->getName();
	mPropertyStandPage->Append(new wxStringProperty(NAME_PROPERTY_NAME, NAME_PROPERTY_NAME, wxString(windowName.c_str())));
	mPropertyStandPage->SetPropertyHelpString(wxString(NAME_PROPERTY_NAME), wxT("��������"));

	PropertyPanelBase::populateProperties(receiverList);
}

void PropertyWindow::onPropertyValueChanged(const std::string& propertyName, const std::string& propertyValue)
{
	if (mPropertyReceiverList.size() == 0)
	{
		return;
	}
	if (propertyName == NAME_PROPERTY_NAME)
	{
		setWindowName(propertyValue);
	}
	else
	{
		PropertyPanelBase::onPropertyValueChanged(propertyName, propertyValue);
	}
}

void PropertyWindow::onPropertyRightClick(wxPGProperty* prop)
{
	mRightProperty = prop;
	std::string propertyName = mRightProperty->GetName().ToStdString();

	wxMenu menu;
	if (propertyName == txProperty::getPropertyName<WindowPropertyPosition>())
	{
		menu.Append(ID_MENU_TO_ABSOLUTE, wxT("ת��Ϊ��������"));
		menu.Append(ID_MENU_TO_RELATIVE, wxT("ת��Ϊ�������"));
	}
	else if (propertyName == txProperty::getPropertyName<WindowPropertySize>())
	{
		menu.Append(ID_MENU_TO_ABSOLUTE, wxT("ת��Ϊ��������"));
		menu.Append(ID_MENU_TO_RELATIVE, wxT("ת��Ϊ�������"));
		menu.Append(ID_MENU_USE_TEXTURE_SIZE, wxT("ʹ��ͼƬ��С"));
	}
	else if (propertyName == txProperty::getPropertyName<WindowPropertyScale>())
	{
		menu.Append(ID_MENU_SCALE_APPLY_TO_SIZE, wxT("����Ӧ�õ��ߴ�"), wxT("���ݴ�������ֵ�������ڴ�С,���ҵ�������λ�������ִ��ڵ���Ⱦλ�ò���"));
	}
	if (menu.GetMenuItemCount() > 0)
	{
		// ����Ҽ�����
		wxPoint ptWx = wxGetMousePosition();
		wxPoint ptWxScreen = ScreenToClient(ptWx);
		PopupMenu(&menu, ptWxScreen);
	}
}

void PropertyWindow::onPropertyDoubleClick(wxPGProperty* prop)
{
	mSpecialValueProperty = prop;
	mExpressionEditor->Show(true);
}

void  PropertyWindow::setWindowName(const std::string& propertyValue, bool replaceKeyword, bool floatOrInt)
{
	int windowCount = mPropertyReceiverList.size();
	FOR_STL(mPropertyReceiverList, int i = 0; i < windowCount; ++i)
	{
		std::string finalValue = propertyValue;
		if (replaceKeyword)
		{
			txMath::replaceStringKeyword(finalValue, "i", i, floatOrInt);
		}
		CommandWindowRename* cmdRename = NEW_CMD(cmdRename);
		cmdRename->mNewName = finalValue;
		mCommandSystem->pushCommand(cmdRename, static_cast<Window*>(mPropertyReceiverList[i]));
	}
	END_FOR_STL(mPropertyReceiverList);
}

void PropertyWindow::setProperty(txPropertyReceiver* receiver, const std::string& propertyName, const std::string& propertyValue)
{
	CommandWindowSetProperty* cmdProperty = NEW_CMD(cmdProperty);
	cmdProperty->mPropertyName = propertyName;
	cmdProperty->mPropertyValue = propertyValue;
	cmdProperty->mOperator = this;
	mCommandSystem->pushCommand(cmdProperty, static_cast<Window*>(receiver));
}

void PropertyWindow::setSpecialPropertyValue(const std::string& value, bool floatOrInt)
{
	std::string propertyName = mSpecialValueProperty->GetName().ToStdString();
	if (propertyName == NAME_PROPERTY_NAME)
	{
		setWindowName(value, true, floatOrInt);
	}
	else
	{
		if (propertyName.find_last_of('.') == -1)
		{
			setReceiverProperty(propertyName, value, true, floatOrInt);
		}
		else
		{
			setSubPropertyValue(mSpecialValueProperty, value, floatOrInt);
		}
	}
}

void PropertyWindow::OnToAbsolute(wxCommandEvent& event)
{
	if (NULL != mRightProperty)
	{
		std::string name = mRightProperty->GetName().ToStdString();
		PROPERTY_TYPE gwpt = getSelectPropertyType(name);
		if (PT_POINT == gwpt)
		{
			// ��Ҫ����һ���б�,��Ϊ��ѭ���лᷢ�¼��޸��б�
			txVector<txPropertyReceiver*> windowList = mPropertyReceiverList;
			int windowCount = windowList.size();
			FOR_STL(windowList, int i = 0; i < windowCount; ++i)
			{
				Window* window = static_cast<Window*>(windowList[i]);
				txPointProperty* propety = static_cast<txPointProperty*>(window->getProperty(name));
				if (NULL != propety)
				{
					Window* prarent = window->getParent();
					VECTOR2 parentSize;
					if (prarent != NULL)
					{
						parentSize = prarent->getFinalSize();
					}
					else
					{
						parentSize = LayoutUtility::getScreenSize();
					}
					txPoint pt = propety->getValue(window);
					txPoint ptAbsolute = txMath::pointToAbsolutePoint(pt, parentSize);
					CommandWindowSetProperty* cmdProperty = NEW_CMD(cmdProperty);
					cmdProperty->mPropertyName = name;
					cmdProperty->mPropertyValue = txStringUtility::pointToString(ptAbsolute);
					mCommandSystem->pushCommand(cmdProperty, window);
				}
			}
			END_FOR_STL(windowList);
		}
	}
}

void PropertyWindow::OnToRelative(wxCommandEvent& event)
{
	if (NULL != mRightProperty)
	{
		std::string name = mRightProperty->GetName().ToStdString();
		PROPERTY_TYPE gwpt = getSelectPropertyType(name);
		if (PT_POINT == gwpt)
		{
			// ��Ҫ����һ���б�,��Ϊ��ѭ���лᷢ�¼��޸��б�
			txVector<txPropertyReceiver*> windowList = mPropertyReceiverList;
			int windowCount = windowList.size();
			FOR_STL(windowList, int i = 0; i < windowCount; ++i)
			{
				Window* window = static_cast<Window*>(windowList[i]);
				txPointProperty* propety = static_cast<txPointProperty*>(window->getProperty(name));
				if (NULL != propety)
				{
					Window* prarent = window->getParent();
					VECTOR2 parentSize;
					if (prarent != NULL)
					{
						parentSize = prarent->getFinalSize();
					}
					else
					{
						parentSize = LayoutUtility::getScreenSize();
					}
					txPoint pt = propety->getValue(window);
					txPoint ptRelative = txMath::pointToRelotivePoint(pt, parentSize);
					CommandWindowSetProperty* cmdProperty = NEW_CMD(cmdProperty);
					cmdProperty->mPropertyName = name;
					cmdProperty->mPropertyValue = txStringUtility::pointToString(ptRelative);
					mCommandSystem->pushCommand(cmdProperty, window);
				}
			}
			END_FOR_STL(windowList);
		}
	}
}

void PropertyWindow::OnUseTextureSize(wxCommandEvent& event)
{
	if (NULL != mRightProperty)
	{
		std::string name = mRightProperty->GetName().ToStdString();
		if (name == txProperty::getPropertyName<WindowPropertySize>())
		{
			int windowCount = mPropertyReceiverList.size();
			FOR_STL(mPropertyReceiverList, int i = 0; i < windowCount; ++i)
			{
				Window* window = static_cast<Window*>(mPropertyReceiverList[i]);
				TextureWindowPropertyTextureSize* propety = window->getProperty<TextureWindowPropertyTextureSize>();
				if (NULL != propety)
				{
					VECTOR2 value = propety->getValue(window);
					txPoint ptAbsolute(txDim(0.0f, (int)value.x), txDim(0.0f, (int)value.y));
					CommandWindowSetProperty* cmdProperty = NEW_CMD(cmdProperty);
					cmdProperty->mPropertyName = name;
					cmdProperty->mPropertyValue = txStringUtility::pointToString(ptAbsolute);
					mCommandSystem->pushCommand(cmdProperty, window);
				}
			}
			END_FOR_STL(mPropertyReceiverList);
		}
	}
}

void PropertyWindow::OnScaleApplyToSize(wxCommandEvent& event)
{
	if (NULL != mRightProperty)
	{
		std::string name = mRightProperty->GetName().ToStdString();
		if (name == txProperty::getPropertyName<WindowPropertyScale>())
		{
			std::string posPropertyName = txProperty::getPropertyName<WindowPropertyPosition>();
			std::string sizePropertyName = txProperty::getPropertyName<WindowPropertySize>();
			int windowCount = mPropertyReceiverList.size();
			FOR_STL(mPropertyReceiverList, int i = 0; i < windowCount; ++i)
			{
				Window* window = static_cast<Window*>(mPropertyReceiverList[i]);
				txPoint windowSize = window->getSize();
				txPoint newSize;
				newSize.x = windowSize.x * window->getMatrixScale().x;
				newSize.y = windowSize.y * window->getMatrixScale().y;
				txPoint posMove = newSize - windowSize;
				posMove.x *= window->getMatrixTransformCenter().x;
				posMove.y *= window->getMatrixTransformCenter().y;
				txPoint newPos = window->getPosition() - posMove;

				// ��������λ��,��Ϊ�������Ĳ�ͬ,һ����������Դ�����������,���ǳߴ�ı����Դ������½�Ϊԭ��
				// ����Ϊ�˱�֤���ڵ�λ�ò���,��Ҫ��������λ��
				CommandWindowSetProperty* cmdPropertyPos = NEW_CMD(cmdPropertyPos);
				cmdPropertyPos->mPropertyName = posPropertyName;
				cmdPropertyPos->mPropertyValue = txStringUtility::pointToString(newPos);
				mCommandSystem->pushCommand(cmdPropertyPos, window);

				// ���ô��ڳߴ�
				CommandWindowSetProperty* cmdPropertySize = NEW_CMD(cmdPropertySize);
				cmdPropertySize->mPropertyName = sizePropertyName;
				cmdPropertySize->mPropertyValue = txStringUtility::pointToString(newSize);
				mCommandSystem->pushCommand(cmdPropertySize, window);

				// �ָ�����
				CommandWindowSetProperty* cmdPropertyScale = NEW_CMD(cmdPropertyScale);
				cmdPropertyScale->mPropertyName = name;
				cmdPropertyScale->mPropertyValue = txStringUtility::vector2ToString(txMath::VEC2_ONE);
				mCommandSystem->pushCommand(cmdPropertyScale, window);
			}
			END_FOR_STL(mPropertyReceiverList);
		}
	}
}