#ifndef _TX_MESH_PROPERTY_H_
#define _TX_MESH_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(txComponentMesh)

// ģ�͵����λ��
class txMeshPropertyMeshPosition : public txComponentMeshProperty<txVector3Property, VECTOR3>
{
public:
	txMeshPropertyMeshPosition();
	virtual void setRealValue(const VECTOR3& value, txComponentMesh* component);
	virtual VECTOR3 getRealValue(txComponentMesh* component);
};

//---------------------------------------------------------------------------------------------------------
// ģ�͵������ת
class txMeshPropertyMeshRotation : public txComponentMeshProperty<txVector3Property, VECTOR3>
{
public:
	txMeshPropertyMeshRotation();
	virtual void setRealValue(const VECTOR3& value, txComponentMesh* component);
	virtual VECTOR3 getRealValue(txComponentMesh* component);
};

//---------------------------------------------------------------------------------------------------------
// ģ�͵��������
class txMeshPropertyMeshScale : public txComponentMeshProperty<txVector3Property, VECTOR3>
{
public:
	txMeshPropertyMeshScale();
	virtual void setRealValue(const VECTOR3& value, txComponentMesh* component);
	virtual VECTOR3 getRealValue(txComponentMesh* component);
};

#endif