#ifndef _TX_DYNAMIC_MESH_PROPERTY_H_
#define _TX_DYNAMIC_MESH_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(txComponentDynamicMesh)

// ��̬ģ��
class txDynamicMeshPropertyDynamicMesh : public txComponentDynamicMeshProperty<txStringProperty, std::string>
{
public:
	txDynamicMeshPropertyDynamicMesh();
	virtual void setRealValue(const std::string& value, txComponentDynamicMesh* component);
	virtual std::string getRealValue(txComponentDynamicMesh* component);
	virtual txVector<std::string>* getChoices() { return &mChoices; }
	static void setChoices(txVector<std::string>& choices) { mChoices = choices; }
protected:
	static txVector<std::string> mChoices;
};

//---------------------------------------------------------------------------------------------------------
// ����
class txDynamicMeshPropertyAnimation : public txComponentDynamicMeshProperty<txStringProperty, std::string>
{
public:
	txDynamicMeshPropertyAnimation();
	virtual void setRealValue(const std::string& value, txComponentDynamicMesh* component);
	virtual std::string getRealValue(txComponentDynamicMesh* component);
	virtual txVector<std::string>* getChoices() { return &mChoices; }
	static void setChoices(txVector<std::string>& choices) { mChoices = choices; }
protected:
	static txVector<std::string> mChoices;
};

//---------------------------------------------------------------------------------------------------------
// ��������״̬
class txDynamicMeshPropertyAnimationState : public txComponentDynamicMeshProperty<txEnumProperty<PLAY_STATE>, PLAY_STATE>
{
public:
	txDynamicMeshPropertyAnimationState();
	virtual void setRealValue(const PLAY_STATE& value, txComponentDynamicMesh* component);
	virtual PLAY_STATE getRealValue(txComponentDynamicMesh* component);
};

//---------------------------------------------------------------------------------------------------------
// ��������,first������,second��֡��
class txDynamicMeshPropertyAnimationLength : public txComponentDynamicMeshProperty<txVector2Property, VECTOR2>
{
public:
	txDynamicMeshPropertyAnimationLength();
	virtual void setRealValue(const VECTOR2& value, txComponentDynamicMesh* component);
	virtual VECTOR2 getRealValue(txComponentDynamicMesh* component);
};

//---------------------------------------------------------------------------------------------------------
// ���������ٶ�
class txDynamicMeshPropertyAnimationSpeed : public txComponentDynamicMeshProperty<txFloatProperty, float>
{
public:
	txDynamicMeshPropertyAnimationSpeed();
	virtual void setRealValue(const float& value, txComponentDynamicMesh* component);
	virtual float getRealValue(txComponentDynamicMesh* component);
};

//---------------------------------------------------------------------------------------------------------
// �������ŵ���ʼ֡����ֹ֡
class txDynamicMeshPropertyAnimationClip : public txComponentDynamicMeshProperty<txVector2Property, VECTOR2>
{
public:
	txDynamicMeshPropertyAnimationClip();
	virtual void setRealValue(const VECTOR2& value, txComponentDynamicMesh* component);
	virtual VECTOR2 getRealValue(txComponentDynamicMesh* component);
};

#endif