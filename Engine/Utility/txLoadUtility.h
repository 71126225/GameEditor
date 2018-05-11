#ifndef _TX_LOAD_UTILITY_H_
#define _TX_LOAD_UTILITY_H_

#include "txEngineDefine.h"
#include "txEngineBase.h"

class txMesh;
class txSkeleton;
class txAnimation;
struct SkinMeshPair;
class txEntity;
class txTexture;
class txLoadUtility : public txEngineBase
{
public:
	// ���ؾ�̬ģ��
	static bool loadMeshFromFile(const std::string& fileName, txMesh* pMesh, const bool& loadTexture = true, txVector<txTexture*>* textureList = NULL);
	// ���ض�̬ģ��
	static bool loadSkeletonMeshFromFile(const std::string& fileName, txVector<SkinMeshPair>& skinMeshPairList, txEntity* pEntity, const bool& loadTexture = true, txVector<txTexture*>* textureList = NULL);
	// json��ʽ�ĳ����ļ�ת��Ϊ�����Ƶĳ����ļ�,fullFileNameΪ������·�����ļ���,�����Ǿ���·��,Ҳ������media�µ����·��
	// ���ص�Ҳ�Ǵ�����·�����ļ���,transMeshFileΪ�Ƿ���ģ���ļ�Ҳһ��ת��Ϊ�����Ƶ�
	static bool jsonSceneFileToBinarySceneFile(const std::string& fullFileName, std::string& newFileName, const bool& transMeshFile);
	// json��ʽ��ģ���ļ�ת��Ϊ�����Ƶ�ģ���ļ�,fullFileNameΪ������·�����ļ���,�����Ǿ���·��,Ҳ������media�µ����·��
	// ���ص�Ҳ�Ǵ�����·�����ļ���
	static bool jsonMeshFileToBinaryMeshFile(const std::string& fullFileName, std::string& newFileName);
	static bool jsonSkeletonMeshFileToBinarySkeletonMeshFile(const std::string& fullFileName, std::string& newFileName);
	static bool jsonSkeletonFileToBinarySkeletonFile(const std::string& fullFileName, std::string& newFileName);
	static bool jsonAnimationFileToBinaryAnimationFile(const std::string& fullFileName, std::string& newFileName);
protected:
	static bool loadJsonMesh(const std::string& fileName, txMesh* pMesh, const bool& loadTexture = true, txVector<txTexture*>* textureList = NULL);
	static bool loadBinaryMesh(const std::string& fileName, txMesh* pMesh, const bool& loadTexture = true, txVector<txTexture*>* textureList = NULL);
	static bool loadJsonSkeletonMesh(const std::string& fileName, txVector<SkinMeshPair>& skinMeshPairList, txEntity* pEntity, const bool& loadTexture = true, txVector<txTexture*>* textureList = NULL);
	static bool loadBinarySkeletonMesh(const std::string& fileName, txVector<SkinMeshPair>& skinMeshPairList, txEntity* pEntity, const bool& loadTexture = true, txVector<txTexture*>* textureList = NULL);
	static bool loadJsonSkeleton(const std::string& fileName, txSkeleton* pSkeleton);
	static bool loadBinarySkeleton(const std::string& fileName, txSkeleton* pSkeleton);
	static bool loadJsonAnimation(const std::string& fileName, txAnimation* pAnimation);
	static bool loadBinaryAnimation(const std::string& fileName, txAnimation* pAnimation);
};

#endif