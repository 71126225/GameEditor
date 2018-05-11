#ifndef _TX_COMPONENT_EFFECT_H_
#define _TX_COMPONENT_EFFECT_H_

#include "txComponent.h"

class txNode;
class txEntity;
class txObjectBoneHook;
class txComponentEffect : public txComponent
{
public:
	txComponentEffect(const std::string& type, const std::string& name)
		:
		txComponent(type, name),
		mHook(NULL)
	{}
	virtual ~txComponentEffect(){ destroy(); }
	virtual void initProperty();
	virtual void setBaseType(){ mBaseType = TOSTRING(txComponentEffect); }
	void destroy();
	virtual void update(float elapsedTime);
	virtual bool isType(const std::string& type){return type == TOSTRING(txComponentEffect);}
	virtual void setActive(const bool& active)
	{
		txComponent::setActive(active);
		if (!active)
		{
			stop();
		}
	}
	void setRotate(const MATRIX3& rotate);
	void setPosition(const VECTOR3& translate);
	void setScale(const VECTOR3& scale);
	// ����1�ǹ�ͷ��,����2�ǽ�ɫ�����
	bool createHook(const std::string& boneName, txEntity* hookEntity);
	// ����1ʱ���ӵ��ļ���,����·��
	// ����2�����ӽڵ�ĸ��ڵ�,ֻ�е�����û�йҽӵ��κιҵ���ʱ����Ч,�������ӽڵ�ĸ��ڵ��ǹҵ�����ģ�͵Ľڵ�
	bool createEffect(const std::string& fileName, txNode* parentNode);
	void SetEmittersUpdateMode(const bool& loop);
	void play();
	void stop();
	const std::string& getEffectFileName() { return mEffectFileName; }
protected:
	std::string mEffectFileName;
	txObjectBoneHook* mHook;
};

#endif