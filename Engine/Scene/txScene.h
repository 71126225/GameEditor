#ifndef _TX_SCENE_H_
#define _TX_SCENE_H_

#include "txEngineDefine.h"
#include "txAABox.h"
#include "txEngineBase.h"

// ���ڼ��غ͹����������ļ�,һ�������ļ���Ӧһ��txScene
class txSceneManager;
class txObject;
class txTexture;
class txCamera;
class txRouteRender;
class txScene : public txEngineBase
{
public:
	txScene(const std::string& name);
	virtual ~txScene();
	const std::string& getName() { return mSceneName; }
	bool rename(const std::string& newName);
	// ���س����ļ�
	bool loadScene(const bool& binary);
	void saveScene();
	// �����°汾�ĳ����ļ�
	bool loadNewScene();
	void saveNewScene();
	// ��������ʵ�������
	void generaTextures();
	// ��һ���Ѿ����صĳ�������һ�ݳ���
	bool copySceneFrom(txScene* srcScene);
	// �������е�������������������ϵ������ƽ��
	void moveScene(const VECTOR3& delta);
	// ���ó��������������Ƿ�ɼ�
	void setVisible(bool visible);
	void addObject(txObject* object);
	// �õ������е������б�
	txMap<std::string, txObject*>& getObjectList(){ return mSceneObjects; }
	txObject* getObject(const std::string& name)
	{
		auto iter = mSceneObjects.find(name);
		if (iter != mSceneObjects.end())
		{
			return iter->second;
		}
		return NULL;
	}
	// ���ٱ����������о�̬ʵ��
	void destroy();
	// �������߼��,���ص������������������ཻ��
	bool rayIntersect(const VECTOR3& ori, const VECTOR3& dir, VECTOR3& intersectPoint);
	// �õ�������ָ����ĸ߶�, x, z�����������µ�
	bool getHeight(const float& x, const float& z, float& y);
	// ���ָ����ĵ�������
	TERRAIN_TYPE getTerrainType(const float& x, const float& z);
	// �õ�ָ����ĵ������͵ĸ߶�,����ˮ��ĸ߶ȵȵ�
	bool getTerrainTypeHeight(const float& x, const float& z, float& y);
	// ���㳡���Ŀ��
	void generateSceneRect();
	// ���ɸ߶�ͼ��Ⱦ����
	void generateRenderHeightMap();
	// ���߶�ͼ����Ϊ�ļ�
	void saveHeightMapFile(const std::string& fileName);
	// ���ļ����ظ߶�ͼ
	void loadHeightMapFile(const std::string& fileName);
	void setSceneAABB(const txAABox& aabb);
	float getSceneRealWidth() { return (mSceneAABB.getMax() - mSceneAABB.getMin()).x; }
	float getSceneRealHeight() { return (mSceneAABB.getMax() - mSceneAABB.getMin()).z; }
	const int& getSceneWidth() { return mSceneWidth; }
	const int& getSceneHeight() { return mSceneHeight; }
	const VECTOR3& getSceneOrigin() { return mSceneOrigin; }
	const VECTOR3& getSceneMove() { return mSceneMove; }
	// �ָ�������ƽ����
	void resetScenePosition() { moveScene(mSceneOrigin - mSceneMove); }
	void alignedToWorldOrigin(const bool& alignedHeight = false) 
	{
		VECTOR3 moveDelta = -mSceneMove;
		if (!alignedHeight)
		{
			moveDelta.y = 0.0f;
		}
		moveScene(moveDelta);
	}
	// �����߶�ͼ
	const int& getHeightMapPrecision() { return mHeightMapPrecision; }
	int getHeightMapSize() { return getHeightMapWidth() * getHeightMapHeight(); }
	int getHeightMapWidth() { return mSceneWidth * mHeightMapPrecision + 1; }
	int getHeightMapHeight() { return mSceneHeight * mHeightMapPrecision + 1; }
	float* getSceneHeightMap() { return mSceneHeightMap; }
	void setHeightMapData(float* heightMap, const int& heightMapSize);
	void setHeightMapPrecision(const int& precision) { mHeightMapPrecision = precision; }
	GLfloat* getHeightMapVertices(const int& index);
	int getHeightMapBufferCount() { return mHeightMapMeshList.size(); }
	GLushort* getHeightMapIndices(const int& index);
	int getHeightMapVertexCount(const int& index);
	int getHeightMapIndicesCount(const int& index);
	void setShowHeightMap(const bool& show){ mShowHeightMap = show; }
	const bool& getShowHeightMap() { return mShowHeightMap; }
	void clearHeightMap();

	// ������������ͼ
	const int& getTerrainTypeMapPrecision() { return mTerrainTypeMapPrecision; }
	int getTerrainTypeMapSize() { return getTerrainTypeMapWidth() * getTerrainTypeMapHeight(); }
	int getTerrainTypeMapWidth() { return mSceneWidth * mTerrainTypeMapPrecision + 1; }
	int getTerrainTypeMapHeight() { return mSceneHeight * mTerrainTypeMapPrecision + 1; }
	int* getTerrainTypeMap() { return mTerrainTypeMap; }
	void setTerrainTypeMapData(int* heightMap, const int& heightMapSize);
	void setTerrainTypeMapPrecision(const int& precision) { mTerrainTypeHeightMapPrecision = precision; }
	GLfloat* getTerrainTypeMapVertices(const int& index);
	int getTerrainTypeMapBufferCount() { return mTerrainTypeMapMeshList.size(); }
	GLushort* getTerrainTypeMapIndices(const int& index);
	int getTerrainTypeMapVertexCount(const int& index);
	int getTerrainTypeMapIndicesCount(const int& index);
	void setShowTerrainTypeMap(const bool& show) { mShowTerrainTypeMap = show; }
	const bool& getShowTerrainTypeMap() { return mShowTerrainTypeMap; }
	void clearTerrainTypeMap();

	// �����������͸߶�ͼ
	const int& getTerrainTypeHeightMapPrecision() { return mTerrainTypeHeightMapPrecision; }
	int getTerrainTypeHeightMapSize() { return getTerrainTypeHeightMapWidth() * getTerrainTypeHeightMapHeight(); }
	int getTerrainTypeHeightMapWidth() { return mSceneWidth * mTerrainTypeHeightMapPrecision + 1; }
	int getTerrainTypeHeightMapHeight() { return mSceneHeight * mTerrainTypeHeightMapPrecision + 1; }
	float* getTerrainTypeHeightMap() { return mTerrainTypeHeightMap; }
	void setTerrainTypeHeightMapData(int* heightMap, const int& heightMapSize);
	void setTerrainTypeHeightMapPrecision(const int& precision) { mTerrainTypeHeightMapPrecision = precision; }
	GLfloat* getTerrainTypeHeightMapVertices(const int& index);
	int getTerrainTypeHeightMapBufferCount() { return mTerrainTypeHeightMapMeshList.size(); }
	GLushort* getTerrainTypeHeightMapIndices(const int& index);
	int getTerrainTypeHeightMapVertexCount(const int& index);
	int getTerrainTypeHeightMapIndicesCount(const int& index);
	void setShowTerrainTypeHeightMap(const bool& show) { mShowTerrainTypeHeightMap = show; }
	const bool& getShowTerrainTypeHeightMap() { return mShowTerrainTypeHeightMap; }
	void clearTerrainTypeHeightMap();
	txMap<int, txRouteRender*>& getRouteRenderList() { return mRouteRender; }
	txRouteRender* getRouteRender(int i) 
	{
		auto iter = mRouteRender.find(i);
		if (iter != mRouteRender.end())
		{
			return iter->second;
		}
		return NULL;
	}
	int addRouteRender(txVector<VECTOR3>& pointList);
	void destroyRouteRender(const int& i);
	void destroyRouteRender(txRouteRender* routeRender);
protected:
	bool loadJsonScene(const std::string& sceneName);
	bool loadBinaryScene(const std::string& sceneName);
protected:
	bool mVisible;
	std::string mSceneName;
	// ��̬ʵ���б�
	txMap<std::string, txObject*> mSceneObjects;
	// �����ڶ��߳��м��ص������б�
	txVector<txTexture*> mNoneAsyncSceneTexturs;
	// ƽ�ƺ󳡾���ԭ��
	VECTOR3 mSceneMove;
	// �����ĳ�ʼԭ��
	VECTOR3 mSceneOrigin;
	// �����İ�Χ��
	txAABox mSceneAABB;
	// ����X��ĳ���,����ȡ��
	int mSceneWidth;
	// ����Z��ĳ���,����ȡ��
	int mSceneHeight;
	// �����߶�ͼ
	float* mSceneHeightMap;
	// �����߶�ͼ�ľ���,ֵԽ��,�򾫶�Խ��
	int mHeightMapPrecision;
	// ��Ⱦ�߶�ͼ���,ÿһ��������������һ���߶�ͼģ��
	txVector<std::pair<GLushort*, GLfloat*> > mHeightMapMeshList;
	// �Ƿ���Ⱦ�߶�ͼ
	bool mShowHeightMap;

	// �����е�������ͼ
	int* mTerrainTypeMap;
	int mTerrainTypeMapPrecision;
	txVector<std::pair<GLushort*, GLfloat*> > mTerrainTypeMapMeshList;
	bool mShowTerrainTypeMap;

	// �����е������͵ĸ߶�ͼ
	float* mTerrainTypeHeightMap;
	int mTerrainTypeHeightMapPrecision;
	txVector<std::pair<GLushort*, GLfloat*> > mTerrainTypeHeightMapMeshList;
	bool mShowTerrainTypeHeightMap;

	static int mRouteIndexSeed;
	txMap<int, txRouteRender*> mRouteRender;
};

#endif