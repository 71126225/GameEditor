#ifndef _TX_SUBENTITY_H_
#define _TX_SUBENTITY_H_

#include "txEngineDefine.h"

class txMesh;
class txEntity;
class txMaterial;
class txSkin;
class txSubEntity
{
public:
	txSubEntity(txEntity* parent);
	virtual ~txSubEntity();

	void init(txMesh* mesh, const bool& bHasSkeleton = false);

	// ��ʼ����ʵ�������
	void GenerateSubEntityTextures();
	txVector<txMaterial*> getAllMaterials();
	txMesh* GetOriMesh(){ return mOrimesh; }
	txEntity* GetParent(){ return mParent; }
	void SetParent(txEntity* ent){ mParent = ent; }
	void SetMeshSkin(txSkin* pSkin);
	txSkin* GetMeshSkin() { return mSkin; }
	void BuildBoneMatrixArray();
	MATRIX4* GetBoneMatrixArray(){ return &boneMatrices[0]; }
	const int& GetBoneMatrixNum(){ return mBoneMatrixNum; }
	GLfloat* GetBoneIndicesArray(){ return boneIndices; }
	GLfloat* GetBoneWeightsArray(){ return boneWeights; }
	// ������ʵ�������,parent(����ʱ��ȷ����),dataindex(ʵ���Ͽ���ʵ�������ÿ���඼û�п��ǹ����dataindex)����
	// boneMatrices����Ҫ����,���ڲ��Ŷ���ʱ�����
	void CopySubEntity(txSubEntity* pSubEntity, const ENTITY_COPY_TYPE& copyType);
	bool rayIntersect(const VECTOR3& ori, const VECTOR3& dir, VECTOR3& intersectPoint);
protected:
	void BuildSkinArray();
protected:
	// skin���ⲿ����,Ȼ����subEntity����
	txSkin* mSkin;
	// mesh���ⲿ����,Ȼ����subEntity����,��skinһ��
	txMesh* mOrimesh;
	txEntity* mParent;
	MATRIX4 boneMatrices[RENDER_BONE_MAXNUM];
	txMap<std::string, int> mBoneMap;
	int mBoneMatrixNum;
	GLfloat* boneIndices;
	GLfloat* boneWeights;
	ENTITY_COPY_TYPE mCopyType;
};

#endif