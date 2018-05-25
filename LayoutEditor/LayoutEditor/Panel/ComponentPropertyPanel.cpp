#include "txEngineRoot.h"
#include "txPoint.h"
#include "txPropertyManager.h"
#include "txComponent.h"

#include "WindowHeader.h"
#include "Layout.h"
#include "LayoutUtility.h"
#include "LayoutManager.h"
#include "WindowProperty.h"
#include "TextureWindowProperty.h"

#include "ComponentEditorCore.h"
#include "ECUtility.h"
#include "EditorCommandHeader.h"

#include "ComponentPropertyPanel.h"
#include "CustomProperty.h"
#include "EditorApp.h"
#include "EditorFrame.h"
#include "LayoutHierarchy.h"

ComponentPropertyPanel::ComponentPropertyPanel(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
PropertyPanelBase(parent, position, size, style, name)
{}

void ComponentPropertyPanel::populateProperties(txVector<txPropertyReceiver*>& receiverList)
{
	int componentCount = receiverList.size();
	if (componentCount == 0)
	{
		return;
	}
	txComponent* firstComponent = static_cast<txComponent*>(receiverList[0]);
	// ֻ��ʾһ�����ڵ�����ʱ�Ż���ʾ����
	if (componentCount == 1)
	{
		const std::string& typeName = firstComponent->getType();
		mPropertyStandPage->Append(new wxStringProperty("Type", "Type", wxString(typeName.c_str())))->Enable(false);
		mPropertyStandPage->SetPropertyHelpString(wxString("Type"), wxT("����"));
		const std::string& baseTypeName = firstComponent->getBaseType();
		mPropertyStandPage->Append(new wxStringProperty("BaseType", "BaseType", wxString(baseTypeName.c_str())))->Enable(false);
		mPropertyStandPage->SetPropertyHelpString(wxString("BaseType"), wxT("��������"));
	}

	// ��ʾ��һ�����������
	const std::string& componentName = firstComponent->getName();
	mPropertyStandPage->Append(new wxStringProperty("Name", "Name", wxString(componentName.c_str())));
	mPropertyStandPage->SetPropertyHelpString(wxString("Name"), wxT("�������"));

	PropertyPanelBase::populateProperties(receiverList);
}

void ComponentPropertyPanel::onRefreshProperty()
{
	mComponentEditorCore->reselectComponent();
}

void ComponentPropertyPanel::setProperty(txPropertyReceiver* receiver, const std::string& propertyName, const std::string& propertyValue)
{
	txComponent* component = static_cast<txComponent*>(receiver);
	// ��������ֵ
	CommandWindowSetComponentProperty* cmdProperty = NEW_CMD(cmdProperty);
	cmdProperty->mComponentName = component->getName();
	cmdProperty->mPropertyName = propertyName;
	cmdProperty->mPropertyValue = propertyValue;
	cmdProperty->mOperator = this;
	mCommandSystem->pushCommand(cmdProperty, component->getOwner());
}