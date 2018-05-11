#include "txPropertyManager.h"
#include "txProperty.h"
#include "txEngineDefine.h"
#include "txEngineLog.h"
#include "PropertyHeader.h"

txPropertyManager::~txPropertyManager()
{
	destroy();
}

void txPropertyManager::init()
{
	// �����������
	registeProperty<txComponentPropertyActive>();
	// �任�������
	registeProperty<txTransformPropertyPosition>();
	registeProperty<txTransformPropertyRotation>();
	registeProperty<txTransformPropertyScale>();
	// ģ���������
	registeProperty<txMeshPropertyMeshPosition>();
	registeProperty<txMeshPropertyMeshRotation>();
	registeProperty<txMeshPropertyMeshScale>();
	// ��̬ģ���������
	registeProperty<txStaticMeshPropertyStaticMesh>();
	// ��̬ģ���������
	registeProperty<txDynamicMeshPropertyDynamicMesh>();
	registeProperty<txDynamicMeshPropertyAnimation>();
	registeProperty<txDynamicMeshPropertyAnimationState>();
	registeProperty<txDynamicMeshPropertyAnimationLength>();
	registeProperty<txDynamicMeshPropertyAnimationSpeed>();
	registeProperty<txDynamicMeshPropertyAnimationClip>();
	// ��Ч�������
	registeProperty<txEffectPropertyParticle>();
	// �����������
	registeProperty<txPhysicsPropertyShowAllGeometry>();
	registeProperty<txPhysicsPropertyKinematic>();
	registeProperty<txPhysicsPropertyLinearVelocity>();
	registeProperty<txPhysicsPropertyAngularVelocity>();
	registeProperty<txPhysicsPropertyConstantForce>();
	registeProperty<txPhysicsPropertyInstantForce>();
	registeProperty<txPhysicsPropertyConstantTorque>();
	registeProperty<txPhysicsPropertyInstantTorque>();
	// ��ײ���������
	registeProperty<txCollisionPropertyGeometryMass>();
	registeProperty<txCollisionPropertyGeometryShowGeometry>();
	registeProperty<txCollisionPropertyGeometryRelativePosition>();
	registeProperty<txCollisionPropertyGeometryRelativeRotation>();
	// ������ײ���������
	registeProperty<txCollisionBoxPropertyBoxWidth>();
	registeProperty<txCollisionBoxPropertyBoxHeight>();
	registeProperty<txCollisionBoxPropertyBoxLength>();
	// ������ײ���������
	registeProperty<txCollisionCapsulePropertyCenterCylinderHeight>();
	registeProperty<txCollisionCapsulePropertyHalfSphereRadius>();
	// Բ������ײ���������
	registeProperty<txCollisionCylinderPropertyCylinderHeight>();
	registeProperty<txCollisionCylinderPropertyBottomRadius>();
	// ������ײ���������
	registeProperty<txCollisionSpherePropertySphereRadius>();
	// ƽ����ײ���������
	registeProperty<txCollisionPlanePropertyPlaneWidthX>();
	registeProperty<txCollisionPlanePropertyPlaneLengthZ>();
	// ͸���������
	registeProperty<txAlphaPropertyStartAlpha>();
	registeProperty<txAlphaPropertyTargetAlpha>();
	// �����������
	registeProperty<txScalePropertyStartScale>();
	registeProperty<txScalePropertyTargetScale>();
	// ��ת�������
	registeProperty<txRotatePropertyTargetAngle>();
	registeProperty<txRotatePropertyStartAngle>();
	// ���ٻ������ת����ת���
	registeProperty<txRotateSpeedPropertyRotateSpeed>();
	registeProperty<txRotateSpeedPropertyRotateAcceleration>();
	registeProperty<txRotateSpeedPropertyRotateSpeedPlayState>();
	// HSL���
	registeProperty<txHSLPropertyStartHSL>();
	registeProperty<txHSLPropertyTargetHSL>();
	// �ؼ�֡���
	registeProperty<txKeyFramePropertyTrembling>();
	registeProperty<txKeyFramePropertyOnceLength>();
	registeProperty<txKeyFramePropertyAmplitude>();
	registeProperty<txKeyFramePropertyCurrentTime>();
	registeProperty<txKeyFramePropertyOffset>();
	registeProperty<txKeyFramePropertyCurState>();
	registeProperty<txKeyFramePropertyLoop>();
	registeProperty<txKeyFramePropertyFullOnce>();

	// ������������
	registeProperty<WindowPropertyPosition>();
	registeProperty<WindowPropertySize>();
	registeProperty<WindowPropertyPrefab>();
	registeProperty<WindowPropertyVisible>();
	registeProperty<WindowPropertyHandleInput>();
	registeProperty<WindowPropertyAlpha>();
	registeProperty<WindowPropertyColour>();
	registeProperty<WindowPropertyColourBlendMode>();
	registeProperty<WindowPropertySymmetryMode>();
	registeProperty<WindowPropertyWaterEffect>();
	registeProperty<WindowPropertyRotation>();
	registeProperty<WindowPropertyScale>();
	registeProperty<WindowPropertyTransform>();
	registeProperty<WindowPropertyTransformCenter>();
	registeProperty<WindowPropertyHSL>();
	registeProperty<WindowPropertyRotateAround>();
	registeProperty<WindowPropertyInnerChild>();
	registeProperty<WindowPropertyAutoSizeType>();
	registeProperty<WindowPropertyHeightDim>();
	registeProperty<WindowPropertyWidthDim>();
	registeProperty<WindowPropertyDiriveParentRotate>();
	// �ı���������
	registeProperty<TextWindowPropertyText>();
	registeProperty<TextWindowPropertyTextDockingPosition>();
	registeProperty<TextWindowPropertyFont>();
	registeProperty<TextWindowPropertyTextRotateWithWindow>();
	registeProperty<TextWindowPropertyTextSpace>();
	registeProperty<TextWindowPropertyMaxTextCount>();
	// ��̬ͼƬ��������
	registeProperty<TextureWindowPropertyTexture>();
	registeProperty<TextureWindowPropertyMaskTexture>();
	registeProperty<TextureWindowPropertyAutoSize>();
	registeProperty<TextureWindowPropertyTextureCoord>();
	registeProperty<TextureWindowPropertyTextureSize>();
	registeProperty<TextureWindowPropertyArcProgress>();
	registeProperty<TextureWindowPropertyLinearDodge>();
	// ����֡��������
	registeProperty<TextureAnimWindowPropertyTextureAnimSet>();
	registeProperty<TextureAnimWindowPropertyFrameCount>();
	registeProperty<TextureAnimWindowPropertyInterval>();
	registeProperty<TextureAnimWindowPropertyLoopMode>();
	registeProperty<TextureAnimWindowPropertyPlayState>();
	registeProperty<TextureAnimWindowPropertyStartIndex>();
	registeProperty<TextureAnimWindowPropertyEndIndex>();
	// ��������������
	registeProperty<ProgressWindowPropertyPercent>();
	registeProperty<ProgressWindowPropertyLeftToRight>();
	// ���ִ�������
	registeProperty<NumberWindowPropertyNumber>();
	registeProperty<NumberWindowPropertyNumberStyle>();
	registeProperty<NumberWindowPropertyNumberInterval>();
	registeProperty<NumberWindowPropertyMaxNumberCount>();
	registeProperty<NumberWindowPropertyNumberDockingPosition>();
	registeProperty<NumberWindowPropertyRotateWithWindow>();
	// �༭�򴰿�����
	registeProperty<EditBoxPropertyShowCaret>();
	registeProperty<EditBoxPropertyMaxTextLength>();
	registeProperty<EditBoxPropertyEditText>();
	// ��ѡ�򴰿�����
	registeProperty<CheckBoxPropertyCheck>();
	// ��ͨ��ť
	registeProperty<ButtonPropertyNormalTexture>();
	registeProperty<ButtonPropertyHoverTexture>();
	registeProperty<ButtonPropertyPushedTexture>();
	registeProperty<ButtonPropertyDisabledTexture>();
	// ��Ƶ��������
	registeProperty<VideoWindowPropertyVideo>();
	registeProperty<VideoWindowPropertyPlayState>();
	registeProperty<VideoWindowPropertyVideoLoop>();
	registeProperty<VideoWindowPropertyRate>();
	registeProperty<VideoWindowPropertyVideoLength>();
	registeProperty<VideoWindowPropertyPlayPercent>();
	registeProperty<VideoWindowPropertyPlayTime>();
	registeProperty<VideoWindowPropertyVideoSize>();
	registeProperty<VideoWindowPropertyVideoFPS>();

	// ���ڻ�����Ϊ
	registeProperty<ActionWindowPropertyActionWindow>();
	registeProperty<ActionWindowPropertyExecuteTime>();
	// ͸���ȱ仯
	registeProperty<ActionWindowAlphaPropertyStartAlpha>();
	registeProperty<ActionWindowAlphaPropertyTargetAlpha>();
	// hsl�仯
	registeProperty<ActionWindowHSLPropertyStartHSL>();
	registeProperty<ActionWindowHSLPropertyTargetHSL>();
	// �ƶ�
	registeProperty<ActionWindowMovePropertyStartPos>();
	registeProperty<ActionWindowMovePropertyTargetPos>();
	// ��ת
	registeProperty<ActionWindowRotatePropertyStartRotation>();
	registeProperty<ActionWindowRotatePropertyTargetRotation>();
	// ����
	registeProperty<ActionWindowScalePropertyStartScale>();
	registeProperty<ActionWindowScalePropertyTargetScale>();
}

void txPropertyManager::destroy()
{
	auto iter = mPropertyList.begin();
	auto iterEnd = mPropertyList.end();
	FOR_STL(mPropertyList, ; iter != iterEnd; ++iter)
	{
		auto iterProp = iter->second.begin();
		auto iterPropEnd = iter->second.end();
		FOR_STL(iter->second, ; iterProp != iterPropEnd; ++iterProp)
		{
			TRACE_DELETE(iterProp->second);
		}
		END_FOR_STL(iter->second);
	}
	END_FOR_STL(mPropertyList);
	mPropertyList.clear();
}

txProperty* txPropertyManager::getProperty(const std::string& name, const std::string& receiverType)
{
	auto iter = mPropertyList.find(receiverType);
	if (iter != mPropertyList.end())
	{
		auto iterProp = iter->second.find(name);
		if (iterProp != iter->second.end())
		{
			return iterProp->second;
		}
	}
	return NULL;
}

PROPERTY_TYPE txPropertyManager::getPropertyType(const std::string& name, const std::string& receiverType)
{
	auto iter = mPropertyList.find(receiverType);
	if (iter != mPropertyList.end())
	{
		auto iterProp = iter->second.find(name);
		if (iterProp != iter->second.end())
		{
			return iterProp->second->getType();
		}
	}
	return PT_STRING;
}

bool txPropertyManager::addPropertyToList(txProperty* pro, const std::string& receiverType)
{
	auto iter = mPropertyList.find(receiverType);
	if (iter == mPropertyList.end())
	{
		txMap<std::string, txProperty*> propList;
		propList.insert(pro->getName(), pro);
		mPropertyList.insert(receiverType, propList);
	}
	else
	{
		auto iterProp = iter->second.find(pro->getName());
		if (iterProp != iter->second.end())
		{
			ENGINE_ERROR("error : can not add property to manager! property name : %s", pro->getName().c_str());
			return false;
		}
		else
		{
			iter->second.insert(pro->getName(), pro);
		}
	}
	return true;
}