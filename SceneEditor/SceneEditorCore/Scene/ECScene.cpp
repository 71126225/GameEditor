#include "txEngineRoot.h"
#include "txScene.h"
#include "txSceneManager.h"
#include "txSubMesh.h"
#include "txSubEntity.h"
#include "txEntity.h"
#include "txComponent.h"
#include "txProperty.h"
#include "txStaticMeshProperty.h"
#include "txDynamicMeshProperty.h"
#include "json/json.h"
#include "opcode/Opcode.h"
#include "opcode/Ice/IceIndexedTriangle.h"

#include "ECScene.h"
#include "EditorCoreRoot.h"
#include "ECEditorManager.h"
#include "ECEditor.h"
#include "ECAxis.h"
#include "ECGrid.h"
#include "ECSceneManager.h"
#include "CoreCommandHeader.h"

static void collosionCallback(const Opcode::CollisionFace& hit, void* user_data);

ECScene::ECScene(const std::string& name)
:
txComponentOwner(name),
mSceneRectDirty(true),
mHeightMap(NULL),
mHeightFlag(NULL),
mScene(NULL),
mSceneVisible(true),
mEditorAxisVisible(true),
mEditorGridVisible(true),
mSceneGridVisible(true)
{
	// ���������������
	mEditorAxis = TRACE_NEW(ECAxis, mEditorAxis, "editor axis");
	mEditorAxis->init();
	mEditorGrid = TRACE_NEW(ECGrid, mEditorGrid, "editor grid");
	mEditorGrid->init();
	mEditorGrid->initGrid(32, 32);
	mSceneGrid = TRACE_NEW(ECGrid, mSceneGrid, "scene grid");
	mSceneGrid->init();
	mSceneGrid->initGrid(64, 64);
	mSceneGrid->setColour(VECTOR4(0.3f, 0.7f, 0.8f, 1.0f));
	mSceneGrid->setAutoShow(false);
}

void ECScene::update(float elapsedTime)
{
	if (mEditorAxis != NULL)
	{
		mEditorAxis->update(elapsedTime);
	}
	if (mEditorGrid != NULL)
	{
		mEditorGrid->update(elapsedTime);
	}
}

void ECScene::destroy()
{
	closeScene();
	TRACE_DELETE(mEditorAxis);
	TRACE_DELETE(mEditorGrid);
	TRACE_DELETE(mSceneGrid);
}

bool ECScene::rename(const std::string& newName)
{
	if (mName == newName)
	{
		return false;
	}
	std::string oldName = mName;
	mName = newName;
	// ���ȸ������г���������,�������ʧ��,��ָ�����,Ȼ�󷵻�
	bool sceneRet = mScene->rename(newName);
	if (!sceneRet)
	{
		mName = oldName;
		return false;
	}
	// ֪ͨ�����������Լ������ָı���
	bool ret = mEditorSceneManager->notifySceneRenamed(oldName, this);
	// ������������������,�����ָֻ�
	if (!ret)
	{
		mName = oldName;
		return false;
	}
	return true;
}

void ECScene::renderHelper(txCamera* camera)
{
	// axis�ᱻ����entity��Ⱦ,����ֻ��Ҫ�ֶ���Ⱦ����
	mEditorGrid->render(camera);
	mSceneGrid->render(camera);
}

ECEditor* ECScene::createEditor(const std::string& name)
{
	CommandEditorManagerCreateEditor* cmdCreateEditor = NEW_CMD(cmdCreateEditor);
	cmdCreateEditor->mName = name;
	mCommandSystem->pushCommand(cmdCreateEditor, mEditorManager);
	// ���������ı༭������б�
	ECEditor* editor = mEditorManager->getEditor(name);
	editor->setScene(this);
	mEditorList.insert(editor->getName(), editor);
	return editor;
}

void ECScene::destroyEditor(const std::string& name)
{
	auto iter = mEditorList.find(name);
	if (iter != mEditorList.end())
	{
		// ������ٵ���������ҽӵ�����,��Ͽ��ҽ�
		if (iter->second == mEditorAxis->getAttachTarget())
		{
			mEditorAxis->attachTarget(NULL);
		}
		if (iter->second == mEditorGrid->getAttachTarget())
		{
			mEditorGrid->attachTarget(NULL);
		}
		CommandEditorManagerDestroyEditor* cmdDestroyEditor = NEW_CMD(cmdDestroyEditor);
		cmdDestroyEditor->mEditor = iter->second;
		mCommandSystem->pushCommand(cmdDestroyEditor, mEditorManager);
		mEditorList.erase(iter);
	}
	setRectDirty(true);
}

void ECScene::destroyEditor(ECEditor* editor)
{
	if (editor != NULL)
	{
		destroyEditor(editor->getName());
	}
}

void ECScene::saveAdvanceScene()
{
	std::string stream;
	txStringUtility::jsonStartArray(stream, 0, true);
	// �������еı༭��
	auto iterEditor = mEditorList.begin();
	auto iterEditorEnd = mEditorList.end();
	FOR_STL(mEditorList, ; iterEditor != iterEditorEnd; ++iterEditor)
	{
		iterEditor->second->saveEditor(stream, 1);
	}
	END_FOR_STL(mEditorList);
	txStringUtility::jsonEndArray(stream, 0, true);
	txStringUtility::removeLastComma(stream);
	txFileUtility::writeFile(P_SCENE_PATH + mName + "/" + mName + ".Ascene", stream.length(), stream.c_str());
}

void ECScene::setSceneVisible(bool visible)
{
	if (mScene != NULL)
	{
		mScene->setVisible(visible);
	}
	mSceneVisible = visible;
}

void ECScene::calculateSceneHeightMap(int precision)
{
	if (mScene == NULL)
	{
		return;
	}

	time_t time0 = time(0);

	// ����ո߶�ͼ����
	mScene->clearHeightMap();
	// Ȼ����㳡�����
	if (isRectDirty())
	{
		generateSceneRect();
	}

	TRACE_DELETE_ARRAY(mHeightMap);
	TRACE_DELETE_ARRAY(mHeightFlag);
	mScene->setHeightMapPrecision(precision);

	int heightMapSize = mScene->getHeightMapSize();
	mHeightMap = TRACE_NEW_ARRAY(float, heightMapSize, mHeightMap);
	mHeightFlag = TRACE_NEW_ARRAY(bool, heightMapSize, mHeightFlag);
	for (int i = 0; i < heightMapSize; ++i)
	{
		mHeightMap[i] = 0.0f;
		mHeightFlag[i] = false;
	}

	// �Ƚ�������Ҫ����ʵ�����ʵ���б���
	txVector<txEntity*> entityList;
	auto& objectList = mScene->getObjectList();
	if (objectList.size() > 0)
	{
		auto iter = objectList.begin();
		auto iterEnd = objectList.end();
		FOR_STL(objectList, ; iter != iterEnd; ++iter)
		{
			if (iter->second->getType() == OT_ENTITY)
			{
				txEntity* entity = static_cast<txEntity*>(iter->second);
				// ���Ե�ģ�Ͳ�����߶ȼ���б���
				if (!isIgnoreEntity(entity->getName()))
				{
					entityList.push_back(entity);
				}
			}
		}
		END_FOR_STL(objectList);
	}
	else
	{
		auto iter = mEditorList.begin();
		auto iterEnd = mEditorList.end();
		FOR_STL(mEditorList, ; iter != iterEnd; ++iter)
		{
			ECEditor* editor = iter->second;
			auto& meshList = editor->getComponentsByBaseType(TOSTRING(txComponentMesh));
			if (meshList.size() > 0)
			{
				auto iterCom = meshList.begin();
				auto iterComEnd = meshList.end();
				FOR_STL(meshList, ; iterCom != iterComEnd; ++iterCom)
				{
					txComponentMesh* meshComponent = static_cast<txComponentMesh*>(iterCom->second);
					txEntity* entity = meshComponent->getEntity();
					// ���Ե�ģ�Ͳ�����߶ȼ���б���
					if (entity != NULL && !isIgnoreEntity(entity->getName()))
					{
						entityList.push_back(entity);
					}
				}
				END_FOR_STL(meshList);
			}
		}
		END_FOR_STL(mEditorList);
	}

	// �Ƚ��������ݷ���opcode
	txVector<std::pair<Opcode::Model*, IceMaths::Matrix4x4> > opcodeModelList;
	int entityCount = entityList.size();
	FOR_STL(entityList, int ii = 0; ii < entityCount; ++ii)
	{
		txEntity* entity = entityList[ii];
		const MATRIX4& worldTransform = entity->GetParent()->GetFinalMatrix();
		auto& subEntityList = entity->getSubEntityList();
		int subEntityCount = subEntityList.size();
		FOR_STL(subEntityList, int i = 0; i < subEntityCount; ++i)
		{
			txMesh* pMesh = subEntityList[i]->GetOriMesh();
			GLfloat* veretxBuffer = pMesh->GetVertexBuffer();
			int vertexCount = pMesh->GetVertexBufferSize() / 3;
			int subMeshCount = pMesh->getSubMeshNum();
			for (int j = 0; j < subMeshCount; ++j)
			{
				txSubMesh* subMesh = pMesh->getSubMesh(j);
				GLushort* indexBuffer = subMesh->getIndexPtr();
				int triangleCount = subMesh->getIndexCount() / 3;

				// ����opcode����
				Opcode::OPCODECREATE data;
				Opcode::MeshInterface* opcodeMesh = new Opcode::MeshInterface();
				opcodeMesh->SetNbTriangles(triangleCount);
				opcodeMesh->SetNbVertices(vertexCount);
				IceMaths::IndexedTriangle* triangleBuffer = new IceMaths::IndexedTriangle[triangleCount];
				for (int k = 0; k < triangleCount; ++k)
				{
					int index0 = indexBuffer[k * 3 + 0];
					int index1 = indexBuffer[k * 3 + 1];
					int index2 = indexBuffer[k * 3 + 2];
					IceMaths::IndexedTriangle triangle(index0, index1, index2);
					triangleBuffer[k] = triangle;
				}
				IceMaths::Point* pointBuffer = new IceMaths::Point[vertexCount];
				for (int k = 0; k < vertexCount; ++k)
				{
					float vertexX = veretxBuffer[k * 3 + 0];
					float vertexY = veretxBuffer[k * 3 + 1];
					float vertexZ = veretxBuffer[k * 3 + 2];
					VECTOR3 vertex(vertexX, vertexY, vertexZ);
					vertex = txMath::transformVector3(vertex, worldTransform);
					IceMaths::Point point(vertex.x, vertex.y, vertex.z);
					pointBuffer[k] = point;
				}
				opcodeMesh->SetPointers(triangleBuffer, pointBuffer);
				data.mIMesh = opcodeMesh;
				Opcode::Model* model = new Opcode::Model();
				model->Build(data);
				IceMaths::Matrix4x4 worldMat;
				worldMat.Identity();
				opcodeModelList.push_back(std::pair<Opcode::Model*, IceMaths::Matrix4x4>(model, worldMat));
			}
		}
		END_FOR_STL(subEntityList);
	}
	END_FOR_STL(entityList);

	int heightMapWidth = mScene->getHeightMapWidth();
	const VECTOR3& sceneOri = mScene->getSceneOrigin();
	VECTOR3 ori(0.0f, 10000.0f, 0.0f);
	VECTOR3 dir(-txMath::AXIS_Y);
	// ����ģ��,����������߼��
	for (int i = 0; i < heightMapSize; ++i)
	{
		mHeightIndex = i;
		int x = i % heightMapWidth;
		int y = i / heightMapWidth;
		ori.x = (float)x / (float)precision + sceneOri.x;
		ori.z = (float)y / (float)precision + sceneOri.z;
		IceMaths::Ray ray(IceMaths::Point(ori.x, ori.y, ori.z), IceMaths::Point(dir.x, dir.y, dir.z));
		float height = 0.0f;
		bool hasHeight = false;
		int opcodeModelCount = opcodeModelList.size();
		FOR_STL(opcodeModelList, int j = 0; j < opcodeModelCount; ++j)
		{
			Opcode::Model* model = opcodeModelList[j].first;
			const IceMaths::Matrix4x4& worldMat = opcodeModelList[j].second;
			RayCallBackUserData userData;
			userData.mRay = &ray;
			userData.mScene = this;
			Opcode::RayCollider rayCollider;
			rayCollider.SetHitCallback(collosionCallback);
			rayCollider.SetUserData(&userData);
			rayCollider.SetFirstContact(false);
			rayCollider.SetMaxDist(20000.0f);
			static udword cache;
			rayCollider.Collide(ray, *model, &worldMat, &cache);
		}
		END_FOR_STL(opcodeModelList);
	}
	mScene->setHeightMapData(mHeightMap, heightMapSize);
	TRACE_DELETE_ARRAY(mHeightMap);
	TRACE_DELETE_ARRAY(mHeightFlag);
	mHeightIndex = 0;

	// �ͷ�opcodeģ��

	// �����¼�֪ͨ�����߶�ͼ�Ѿ��������
	time_t deltaTime = time(0) - time0;

	txVector<std::string> paramList;
	paramList.push_back(mName);
	paramList.push_back(txStringUtility::intToString((int)deltaTime));
	mEditorCoreRoot->sendEvent(CE_HEIGHT_MAP_DONE, paramList);
}

void collosionCallback(const Opcode::CollisionFace& hit, void* user_data)
{
	RayCallBackUserData userData = *((RayCallBackUserData*)user_data);
	ECScene* scene = (ECScene*)userData.mScene;
	const IceMaths::Ray& ray = *((IceMaths::Ray*)userData.mRay);
	IceMaths::Point point = ray.mOrig + ray.mDir * hit.mDistance;
	float curHeight = scene->getHeightMap()[scene->getHeightIndex()];
	if (!scene->getHeightFlag()[scene->getHeightIndex()] || point.y > curHeight)
	{
		scene->getHeightMap()[scene->getHeightIndex()] = point.y;
		scene->getHeightFlag()[scene->getHeightIndex()] = true;
	}
}

void ECScene::readSceneFile(bool binary)
{
	// ���������еĳ���
	closeScene();
	// Ȼ������µĳ���
	bool ret = mSceneManager->loadSceneFromFile(mName.c_str(), binary);
	mScene = mSceneManager->getScene(mName);
	mSceneGrid->setVisible(true);
	if (ret)
	{
		// ��ˢ��һ�鳡��,Ȼ����㳡�����
		mSceneManager->update(0.0f);
		mScene->generateSceneRect();
		mSceneManager->createSceneTexture(mName);
		// ������ԭ������������ϵԭ�����,���Ǹ߶Ȳ�Ҫ�ı�
		mSceneManager->alignedToWorldOrigin(mName);
	}
}

void ECScene::readAdvanceScene(bool async)
{
	// ���������еĳ���
	closeScene();
	mScene = mSceneManager->newScene(mName);
	mSceneGrid->setVisible(true);
	Json::Reader jRead;
	Json::Value arrVal;
	bool ret = jRead.parse(txFileUtility::openTxtFile(P_SCENE_PATH + mName + "/" + mName + ".Ascene"), arrVal);
	if (ret)
	{
		txVector<std::string> propertyList;
		if (arrVal.type() == Json::arrayValue)
		{
			auto iterEditor = arrVal.begin();
			auto iterEditorEnd = arrVal.end();
			for (; iterEditor != iterEditorEnd; ++iterEditor)
			{
				ECEditor* editor = NULL;
				if (iterEditor->type() == Json::arrayValue)
				{
					auto iterComponent = iterEditor->begin();
					auto iterComponentEnd = iterEditor->end();
					for (int i = 0; iterComponent != iterComponentEnd; ++iterComponent, ++i)
					{
						// �����еĵ�0��Ԫ���Ǳ༭��Ļ�������,�������Ǳ༭����������
						if (i > 0)
						{
							std::string componentName = (*iterComponent)["name"].asString();
							std::string componentType = (*iterComponent)["type"].asString();
							std::string parentName = (*iterComponent)["parent"].asString();
							txComponent* parentComponent = NULL;
							if (parentName != EMPTY_STRING)
							{
								parentComponent = editor->getComponent(parentName);
							}
							// Ϊ�༭��������,���Ҳ��Ҹ����,������ҵ����������
							// �Ȳ��ұ༭���Ƿ��Ѿ����˸����,�����޸�����,û�еĲ����
							txComponent* component = editor->getComponent(componentName);
							if (component == NULL)
							{
								component = editor->addComponent(componentName, componentType);
							}
							auto& propertyMap = component->getPropertyMap();
							auto iterProp = propertyMap.begin();
							auto iterPropEnd = propertyMap.end();
							FOR_STL(propertyMap, ; iterProp != iterPropEnd; ++iterProp)
							{
								txProperty* prop = iterProp->second;
								prop->setAsync(async);
								std::string propertyValue = (*iterComponent)[prop->getName()].asString();
								if (propertyValue != EMPTY_STRING)
								{
									editor->setComponentProperty(component, prop, propertyValue);
								}
								// �������Ժ���Ҫ�ָ��첽���,ȷ������Ӱ�������ط���ʹ��
								prop->setAsync(false);
							}
							END_FOR_STL(propertyMap);
							// ֪ͨ���������Ѿ��������
							component->notifyPropertyDone();
							component->detachOwnerParentComponent();
							component->attachOwnerParentComponent(editor, parentComponent, -1);
						}
						else
						{
							// �����༭��
							editor = createEditor((*iterComponent)["name"].asString());
						}
					}
				}
			}
		}
	}
	setRectDirty(true);
}

void ECScene::closeScene()
{
	// ���ٵ�ǰ�����е����б༭��
	auto iterEditor = mEditorList.begin();
	auto iterEditorEnd = mEditorList.end();
	FOR_STL(mEditorList, ; iterEditor != iterEditorEnd; ++iterEditor)
	{
		CommandEditorManagerDestroyEditor* cmdDestroyEditor = NEW_CMD(cmdDestroyEditor);
		cmdDestroyEditor->mEditor = iterEditor->second;
		mCommandSystem->pushCommand(cmdDestroyEditor, mEditorManager);
	}
	END_FOR_STL(mEditorList);
	if (mEditorAxis != NULL)
	{
		mEditorAxis->attachTarget(NULL);
	}
	if (mEditorGrid != NULL)
	{
		mEditorGrid->attachTarget(NULL);
	}
	if (mScene != NULL)
	{
		mSceneManager->destroyScene(mName);
		mScene = NULL;
	}
	if (mSceneGrid != NULL)
	{
		mSceneGrid->setVisible(false);
	}
}

bool ECScene::notifyEditorRenamed(const std::string& oldName, ECEditor* editor)
{
	// �����Ƿ��и����ֵı༭��
	auto iterOld = mEditorList.find(oldName);
	if (iterOld == mEditorList.end())
	{
		return false;
	}
	// �����µ������Ƿ�������
	auto iterNew = mEditorList.find(editor->getName());
	if (iterNew != mEditorList.end())
	{
		return false;
	}
	mEditorList.erase(iterOld);
	mEditorList.insert(editor->getName(), editor);
	return true;
}

void ECScene::notifyEditorComponentProertyChanged(txComponent* component, const std::string& propertyName, const std::string& propertyValue)
{
	if (component == NULL || component->getBaseType() != TOSTRING(txComponentMesh))
	{
		return;
	}
	if (propertyName == txProperty::getPropertyName<txStaticMeshPropertyStaticMesh>()
		|| propertyName == txProperty::getPropertyName<txDynamicMeshPropertyDynamicMesh>())
	{
		setRectDirty(true);
	}
}

void ECScene::notifyEditorDeleteComponent(txComponent* component)
{
	if (component == NULL)
	{
		return;
	}
	if (component->getBaseType() == TOSTRING(txComponentMesh))
	{
		setRectDirty(true);
	}
}

void ECScene::generateSceneRect()
{
	txAABox aabb;
	auto iter = mEditorList.begin();
	auto iterEnd = mEditorList.end();
	FOR_STL(mEditorList, ; iter != iterEnd; ++iter)
	{
		ECEditor* editor = iter->second;
		auto& meshList = editor->getComponentsByBaseType(TOSTRING(txComponentMesh));
		if (meshList.size() > 0)
		{
			auto iterCom = meshList.begin();
			auto iterComEnd = meshList.end();
			FOR_STL(meshList, ; iterCom != iterComEnd; ++iterCom)
			{
				txComponentMesh* meshComponent = static_cast<txComponentMesh*>(iterCom->second);
				txEntity* entity = meshComponent->getEntity();
				if (entity != NULL)
				{
					auto& subEntityList = entity->getSubEntityList();
					int subEntityCount = subEntityList.size();
					FOR_STL(subEntityList, int i = 0; i < subEntityCount; ++i)
					{
						aabb.merge(subEntityList[i]->GetOriMesh()->GetAABox()->transform(entity->GetParent()->GetFinalMatrix()));
					}
					END_FOR_STL(subEntityList);
				}
			}
			END_FOR_STL(meshList);
		}
	}
	END_FOR_STL(mEditorList);
	mScene->setSceneAABB(aabb);
	setRectDirty(false);
}