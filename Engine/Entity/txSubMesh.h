#ifndef _TX_SUBMESH_H_
#define _TX_SUBMESH_H_

#include "txEngineDefine.h"
#include "txEngineBase.h"

class txMaterial;
class txMesh;
class txSubMesh : public txEngineBase
{
public:
	txSubMesh(txMesh* parent);
	virtual ~txSubMesh();
	void setMaterial(const txMaterial& material);
	txMaterial* getMaterial(){ return m_subMaterial; }
	void pushIndex(const GLushort& index){ m_indices.push_back(index); }
	int getIndexCount(){ return m_indices.size(); }
	void makeIndicesSize(const int& size) { m_indices.resize(size); }
	GLushort* getIndexPtr(){ return m_indices.size() > 0 ? &(m_indices[0]) : NULL; }
	void setNumTriangles(const int& num){ m_nNumTris = num; }
	const int& getNumTriangles(){ return m_nNumTris; }
	void setMaterialIndex(const int& index){ m_materialIndex = index; }
	const int& getMaterialIndex(){ return m_materialIndex; }
	// ��ʼ����ģ�͵�����
	void GenerateTexture();
	// �����������ݵ�pSubMesh��
	void CopySubMesh(txSubMesh* pSubMesh);
	bool rayIntersect(const VECTOR3& ori, const VECTOR3& dir, VECTOR3& point);
	void setAutoDestroyTexture(const bool& autoDestroy) { mAutoDestroyTexture = autoDestroy; }
	const bool& getAutoDestroyTexture() { return mAutoDestroyTexture; }
protected:
	txMaterial* m_subMaterial;
	txVector<GLushort> m_indices;
	int m_nNumTris;
	int m_materialIndex;
	txMesh* mParent;
	// �Ƿ��Զ���������,ֻ�д��ļ����ص�ʵ���Ӧ���Զ�����
	bool mAutoDestroyTexture;
};

#endif