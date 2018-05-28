#ifndef _COMPONENT_EDITOR_CORE_H_
#define _COMPONENT_EDITOR_CORE_H_

#include "CommonDefine.h"
#include "EditorCoreBase.h"
#include "txEngineBase.h"

class txComponent;
class ComponentEditorCore : public EditorCoreBase, public txEngineBase
{
public:
	ComponentEditorCore(){}
	~ComponentEditorCore(){}
	virtual void init(){}
	virtual void update(const float& elapsedTime){}
	virtual void render(){}
	txVector<txComponent*>& getComponentSelections() { return mComponentMultiSelections; }
	void setComponentSelection(txComponent* component);
	void setComponentSelection(txVector<txComponent*>& selections);
	void reselectComponent();
	void clearComponentSelections();
protected:
	void notifyComponentSelectionChanged(const bool& notifyEvent = true);
protected:
	txVector<txComponent*> mComponentMultiSelections;	// ���б�ѡ�е����,������˳��֮��
	txSet<txComponent*> mComponentMultiSelSet;			// ���б�ѡ�е����,�������ж��Ƿ�ѡ������ͬ�����
};

#endif