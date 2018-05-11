#include "txSubEntity.h"
#include "txMesh.h"
#include "txEntity.h"
#include "txSkinBone.h"
#include "txBone.h"
#include "txEngineRoot.h"
#include "txEngineLog.h"

txSubEntity::txSubEntity(txEntity* parent)
:
mOrimesh(NULL),
mParent(parent),
mSkin(NULL),
boneIndices(NULL),
boneWeights(NULL)
{
	;
}

txSubEntity::~txSubEntity()
{
	// �򵥿���ʱ,�������ݶ������Լ���,���Բ����ͷ�
	if (mCopyType != ECT_SIMPLE && mCopyType != ECT_ANIM_SIMPLE)
	{
		//�ͷŴ洢����Ƥ��Ϣ
		TRACE_DELETE_ARRAY(boneIndices);
		TRACE_DELETE_ARRAY(boneWeights);
		TRACE_DELETE(mSkin);
		TRACE_DELETE(mOrimesh);
	}
	boneIndices = NULL;
	boneWeights = NULL;
	mSkin = NULL;
	mOrimesh = NULL;
}

void txSubEntity::init(txMesh* mesh, const bool& bHasSkeleton)
{
	mOrimesh = mesh;
	if (bHasSkeleton)
	{
		mOrimesh->isSkeletonMesh = true;
	}

	mOrimesh->setAutoDestroyTexture(mParent->getCopyType() == ECT_NONE);
}

void txSubEntity::GenerateSubEntityTextures()
{
	mOrimesh->GenerateTextures();
}

txVector<txMaterial*> txSubEntity::getAllMaterials()
{
	if (mOrimesh != NULL)
	{
		return mOrimesh->getAllMaterials();
	}
	return txVector<txMaterial*>();
}

void txSubEntity::SetMeshSkin(txSkin* pSkin)
{
	mSkin = pSkin;
	if (mSkin != NULL)
	{
		BuildSkinArray();
	}
}

void txSubEntity::CopySubEntity(txSubEntity* pSubEntity, const ENTITY_COPY_TYPE& copyType)
{
	mCopyType = copyType;
	if (mCopyType == ECT_DEEP)
	{
		// ����mesh����
		txMesh* pMesh = TRACE_NEW(txMesh, pMesh);
		mOrimesh->CopyMesh(pMesh);
		pSubEntity->init(pMesh, !mParent->GetSkeleton()->IsSkeletonEmpty());

		// �������Ƥ,��Ҫ������Ƥ
		pSubEntity->mSkin = NULL;
		if (mSkin != NULL)
		{
			pSubEntity->mSkin = TRACE_NEW(txSkin, pSubEntity->mSkin);
			mSkin->CopySkin(pSubEntity->mSkin);
		}

		// ��Ϊ������Ȩ�����ݲ���Ҫ�����ټ���,���Բ�����txEntity��CreateSubEntity��������ʵ��
		int texBufferSize = mOrimesh->GetTexCoordBufferSize();
		pSubEntity->boneIndices = TRACE_NEW_ARRAY(GLfloat, texBufferSize * 2, pSubEntity->boneIndices);
		pSubEntity->boneWeights = TRACE_NEW_ARRAY(GLfloat, texBufferSize * 2, pSubEntity->boneWeights);
		memcpy(pSubEntity->boneIndices, boneIndices, texBufferSize * 2 * sizeof(GLfloat));
		memcpy(pSubEntity->boneWeights, boneWeights, texBufferSize * 2 * sizeof(GLfloat));

		auto iter = mBoneMap.begin();
		auto iterEnd = mBoneMap.end();
		FOR_STL(mBoneMap, ; iter != iterEnd; ++iter)
		{
			pSubEntity->mBoneMap.insert(iter->first, iter->second);
		}
		END_FOR_STL(mBoneMap);
		pSubEntity->mBoneMatrixNum = mBoneMatrixNum;
	}
	else if (mCopyType == ECT_SIMPLE || mCopyType == ECT_ANIM_SIMPLE)
	{
		pSubEntity->init(mOrimesh, !mParent->GetSkeleton()->IsSkeletonEmpty());
		pSubEntity->mSkin = mSkin;
		pSubEntity->boneIndices = boneIndices;
		pSubEntity->boneWeights = boneWeights;
		pSubEntity->mBoneMap = mBoneMap;
		pSubEntity->mBoneMatrixNum = mBoneMatrixNum;
	}
}

bool txSubEntity::rayIntersect(const VECTOR3& ori, const VECTOR3& dir, VECTOR3& intersectPoint)
{
	if (mOrimesh != NULL)
	{
		return mOrimesh->rayIntersect(ori, dir, intersectPoint, mParent->GetParent()->GetFinalMatrix());
	}
	return false;
}

void txSubEntity::BuildSkinArray()
{
	if (mOrimesh != NULL && mSkin != NULL)
	{
		boneIndices = TRACE_NEW_ARRAY(GLfloat, mOrimesh->GetTexCoordBufferSize() * 2, boneIndices);
		memset(boneIndices, 0, sizeof(GLfloat)* mOrimesh->GetTexCoordBufferSize() * 2);

		boneWeights = TRACE_NEW_ARRAY(GLfloat, mOrimesh->GetTexCoordBufferSize() * 2, boneWeights);
		memset(boneWeights, 0, sizeof(GLfloat)* mOrimesh->GetTexCoordBufferSize() * 2);

		//ѭ��ÿ����Ƥ����
		auto& skinBoneList = mSkin->getSkinBoneList();
		auto iterSkinBone = skinBoneList.begin();
		auto iterSkinBoneEnd = skinBoneList.end();
		int nBoneIndex = 0;
		FOR_STL(skinBoneList, ; iterSkinBone != iterSkinBoneEnd; ++iterSkinBone)
		{
			txSkinBone* sb = iterSkinBone->second;
			auto& weightList = sb->getWeightList();
			auto iterWeight = weightList.begin();
			auto iterWeightEnd = weightList.end();
			FOR_STL(weightList, ; iterWeight != iterWeightEnd; ++iterWeight)
			{
				//��ȡ�����index��Ȩ��
				int index = iterWeight->first;
				float weight = iterWeight->second;
				if (weight < 0.001f)
				{
					continue;
				}
				if (boneIndices[index * 4] <= 0)
				{
					boneIndices[index * 4] = (float)nBoneIndex;
					boneWeights[index * 4] = weight;
				}
				else if (boneIndices[index * 4 + 1] <= 0)
				{
					boneIndices[index * 4 + 1] = (float)nBoneIndex;
					boneWeights[index * 4 + 1] = weight;
				}
				else if (boneIndices[index * 4 + 2] <= 0)
				{
					boneIndices[index * 4 + 2] = (float)nBoneIndex;
					boneWeights[index * 4 + 2] = weight;
				}
				else
				{
					boneIndices[index * 4 + 3] = (float)nBoneIndex;
					boneWeights[index * 4 + 3] = weight;
				}
			}
			END_FOR_STL(weightList);
			++nBoneIndex;
		}
		END_FOR_STL(skinBoneList);
		mBoneMatrixNum = nBoneIndex;
	}
}

void txSubEntity::BuildBoneMatrixArray()
{
	if (mSkin != NULL && mParent != NULL && mParent->GetSkeleton() != NULL)
	{
		// ѭ��ÿ����Ƥ����
		auto& skinBoneList = mSkin->getSkinBoneList();
		auto iterSkinBone = skinBoneList.begin();
		auto iterSkinBoneEnd = skinBoneList.end();
		int nBoneIndex = 0;
		FOR_STL(skinBoneList, ; iterSkinBone != iterSkinBoneEnd; ++iterSkinBone)
		{
			if (nBoneIndex >= RENDER_BONE_MAXNUM)
			{
				ENGINE_ERROR("error : bone count out of range! max bone count : %d", RENDER_BONE_MAXNUM);
				break;
			}
			txSkinBone* sb = iterSkinBone->second;
			if (sb != NULL)
			{
				txBone* pBone = mParent->GetSkeleton()->GetBone(iterSkinBone->first.c_str());
				if (pBone != NULL)
				{
					boneMatrices[nBoneIndex] = pBone->GetFinalMatrix() * sb->getOffsetMat();
					++nBoneIndex;
				}
			}
		}
		END_FOR_STL(skinBoneList);
		mBoneMatrixNum = nBoneIndex;
	}
}