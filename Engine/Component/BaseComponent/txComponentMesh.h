#ifndef _TX_COMPONENT_MESH_H_
#define _TX_COMPONENT_MESH_H_

#include "txAABox.h"
#include "txComponent.h"

class txNode;
class txEntity;
class txObjectBoneHook;
// ģ�����
class txComponentMesh : public txComponent
{
public:
	txComponentMesh(const std::string& type, const std::string& name)
		:
		txComponent(type, name),
		mEntity(NULL),
		mIsOutEntity(false),
		mHook(NULL)
	{}
	virtual ~txComponentMesh(){ destroy(); }
	virtual void setBaseType() { mBaseType = TOSTRING(txComponentMesh); }
	virtual void initProperty();
	virtual void init(txComponentOwner* owner);
	virtual void update(float elapsedTime);
	virtual bool isType(const std::string& type){ return type == TOSTRING(txComponentMesh); }
	void destroy(){ destroyEntity(); }
	void destroyEntity();
	virtual bool initEntityByFileName(const std::string& filePath, const bool& hasAnim, const bool& loadTexture);
	virtual bool initEntityByEntityName(const std::string& sourceEntityName, const bool& hasAnim, const bool& loadTexture);
	// resetNodeΪ�Ƿ��������ø��ڵ�, �����������,��ʵ��ڵ�ĸ��ڵ�����ΪparentNode
	virtual bool initEntityByOutEntity(const std::string& sourceEntityName, const bool& resetNode);
	const std::string& getModelFileName() { return mModelFileName; }
	txEntity* getEntity(){ return mEntity; }
	virtual void setActive(const bool& active);
	void generateTexture();
	const VECTOR3& getPosition();
	const VECTOR3& getScale();
	const MATRIX3& getRotation();
	void setPosition(const VECTOR3& pos);
	void setScale(const VECTOR3& scale);
	void setRotation(const MATRIX3& rot);
	void resetRotation();
	virtual txAABox getWorldAABB();
	virtual txAABox getLocalAABB();
	void setShowAABB(const bool& show);
	bool getShowAABB();
	void roll(const float& angle);
	void pitch(const float& angle);
	void yaw(const float& angle);
	void setHook(const std::string& hookName);
protected:
	txEntity* mEntity;			// ʵ��
	bool mIsOutEntity;			// entity�Ƿ������ⲿ������,�������ɾ�̬ģ�����������
	std::string mModelFileName; // ����·����ģ���ļ���
	txObjectBoneHook* mHook;	// ģ��������ҽӵĹҵ�,ֻ�е�ģ������ҽӵ�һ����̬ģ������²Ż��йҵ�
};

#endif