#include "Utility.h"
#include "json/json.h"
#include "txEngineRoot.h"
#include "txBaseProperty.h"

#include "WindowPrefabManager.h"
#include "WindowHeader.h"
#include "WindowFactory.h"
#include "LayoutManager.h"
#include "Layout.h"
#include "WindowProperty.h"

void WindowPrefabManager::init()
{
	loadAllPrefab();
}

void WindowPrefabManager::destroy()
{
	// �ȸ���һ��prefab�б�,Ȼ��������Ƴ����б����������е�prefab
	auto prefabList = mPrefabList;
	auto iter = prefabList.begin();
	auto iterEnd = prefabList.end();
	FOR_STL(prefabList, ; iter != iterEnd; ++iter)
	{
		destroyPrefab(iter->first);
	}
	END_FOR_STL(prefabList);
}

void WindowPrefabManager::loadAllPrefab()
{
	txVector<std::string> fileList;
	txFileUtility::findFiles(P_UI_WINDOW_PREFAB_PATH, fileList, PREFAB_SUFFIX);
	int fileCount = fileList.size();
	FOR_STL(fileList, int i = 0; i < fileCount; ++i)
	{
		loadPrefab(fileList[i]);
	}
	END_FOR_STL(fileList);
	mEngineRoot->sendEvent(EE_PREFAB_RESOURCE_CHANGED);
}

void WindowPrefabManager::loadPrefab(const std::string& fileName)
{
	Json::Reader jRead;
	Json::Value arrVal;
	bool ret = jRead.parse(txFileUtility::openTxtFile(fileName), arrVal);
	if (ret)
	{
		if (arrVal.type() == Json::arrayValue)
		{
			auto valueIte = arrVal.begin();
			auto valueIteEnd = arrVal.end();
			for (; valueIte != valueIteEnd; ++valueIte)
			{
				std::string windowType = (*valueIte)[TYPE_PROPERTY_NAME].asString();
				std::string windowName = (*valueIte)[NAME_PROPERTY_NAME].asString();
				Window* window = NULL;
				if (windowType == PREFAB_TYPE_NAME)
				{
					// ����Ԥ��
					window = createPrefab(windowName, false);
					WindowPrefab* windowPrefab = static_cast<WindowPrefab*>(window);
					windowPrefab->setFileName(txStringUtility::getFileName(fileName));
				}
				else
				{
					// ���ô��ڵĸ�����
					std::string parentName = (*valueIte)[PARENT_PROPERTY_NAME].asString();
					bool isInnerChild = txStringUtility::stringToBool((*valueIte)[txProperty::getPropertyName<WindowPropertyInnerChild>()].asString());
					// ������ڲ��Ӵ���,�򲻴�������,��Ϊ�������ڴ���ʱ�Ѿ����ڲ��Ӵ��ڴ���������
					if (isInnerChild)
					{
						if (parentName != EMPTY_STRING)
						{
							// �ڲ��Ӵ���һ������Ԥ����Ӵ���,���Բ���Ҫ��Ԥ���б��в��Ҹ�����
							auto iter = mWindowList.find(windowName);
							if (iter != mWindowList.end())
							{
								window = iter->second;
							}
							else
							{
								ENGINE_ERROR("error : can not find inner child! child name : %s", windowName.c_str());
							}
						}
						else
						{
							ENGINE_ERROR("error : inner child's parent is empty! child name : %s", windowName.c_str());
						}
					}
					else
					{
						// ��������
						window = createWindow(windowType, windowName);
						if (parentName != EMPTY_STRING)
						{
							Window* parentWindow = NULL;
							auto it = mWindowList.find(parentName);
							// �ڴ����б����Ҳ�������Ԥ���б��в���
							if (it != mWindowList.end())
							{
								parentWindow = it->second;
							}
							else
							{
								auto iterPrefab = mPrefabList.find(parentName);
								if (iterPrefab != mPrefabList.end())
								{
									parentWindow = iterPrefab->second;
								}
							}
							if (parentWindow != NULL)
							{
								parentWindow->addChild(window);
							}
							else
							{
								ENGINE_ERROR("error : can not find parent window, window name : %s, parent name : %s", window->getName().c_str(), parentName.c_str());
							}
						}
					}
				}

				// �������ڵ���������
				auto& propertyMap = window->getPropertyMap();
				auto iter = propertyMap.begin();
				auto iterEnd = propertyMap.end();
				FOR_STL(propertyMap, ; iter != iterEnd; ++iter)
				{
					txProperty* property = iter->second;
					std::string propertyValue = (*valueIte)[property->getName()].asString();
					if (propertyValue != EMPTY_STRING)
					{
						property->set(propertyValue, window);
					}
				}
				END_FOR_STL(propertyMap);
			}
		}
	}
	else
	{
		ENGINE_ERROR("error : parse prefab : %s failed! reason : %s", fileName.c_str(), jRead.getFormattedErrorMessages().c_str());
	}
}

WindowPrefab* WindowPrefabManager::createPrefab(const std::string& name, const bool& sendEvent)
{
	auto iter = mPrefabList.find(name);
	if (iter != mPrefabList.end())
	{
		return iter->second;
	}
	WindowPrefab* prefab = static_cast<WindowPrefab*>(Layout::createIndependentWindow(PREFAB_TYPE_NAME, name, true));
	mPrefabList.insert(prefab->getName(), prefab);
	if (sendEvent)
	{
		mEngineRoot->sendEvent(EE_PREFAB_RESOURCE_CHANGED);
	}
	return prefab;
}

WindowPrefab* WindowPrefabManager::createPrefab(Window* window, const std::string& name)
{
	WindowPrefab* prefab = createPrefab(name);
	window->copyProperty(prefab);
	window->copyChildren(prefab, false, EMPTY_STRING);
	return prefab;
}

Window* WindowPrefabManager::createWindow(const std::string& typeName, const std::string& name)
{
	auto iter = mWindowList.find(name);
	if (iter != mWindowList.end())
	{
		ENGINE_ERROR("error : there is a window named : %s, can not create in WindowPrefabManager!", name.c_str());
		return NULL;
	}
	Window* window = Layout::createIndependentWindow(typeName, name, true);
	// �����ڷ��봰���б���
	mWindowList.insert(window->getName(), window);
	// ����Ǹ��ϴ���,����Ҫ�����ϴ����µ������ڲ��Ӵ��ڶ������б���
	if (window->isComplexWindow())
	{
		addChildToList(window);
	}
	return window;
}
void WindowPrefabManager::addChildToList(Window* window)
{
	txVector<Window*> allChildList;
	window->getAllChildren(allChildList);
	int childCount = allChildList.size();
	FOR_STL(allChildList, int i = 0; i < childCount; ++i)
	{
		mWindowList.insert(allChildList[i]->getName(), allChildList[i]);
		addChildToList(allChildList[i]);
	}
	END_FOR_STL(allChildList);
}

void WindowPrefabManager::destroyPrefab(const std::string& name)
{
	auto iter = mPrefabList.find(name);
	if (iter != mPrefabList.end())
	{
		// ������Ԥ���Լ��Ӵ���ǰ,�������б��еĸ�Ԥ���е������Ӵ����Ƴ�
		removeChildFromList(iter->second);

		// �Ƚ�prefab�µ�һ���Ӵ���ȫ������,�˴����������û���Ӵ����б�,��Ϊ�����ٴ��ڹ����л���б�����޸�
		auto children = iter->second->getChildren();
		auto iterChild = children.begin();
		auto iterChildEnd = children.end();
		FOR_STL(children, ; iterChild != iterChildEnd; ++iterChild)
		{
			mLayoutManager->destroyWindow(*iterChild);
		}
		END_FOR_STL(children);

		// Ȼ��������prefab����
		mLayoutManager->destroyWindow(iter->second);
		mPrefabList.erase(iter);
	}
	mEngineRoot->sendEvent(EE_PREFAB_RESOURCE_CHANGED);
}

void WindowPrefabManager::removeChildFromList(Window* window)
{
	// ������Ӵ����Ǹ��ϴ���,����Ҫ�����ϴ��ڵ��ڲ��Ӵ��ڴ��б����Ƴ�
	// ������Ҫ���ǵ�window�Ǹ��ϴ��ڵ����
	txVector<Window*> allChildList;
	window->getAllChildren(allChildList);
	int childCount = allChildList.size();
	FOR_STL(allChildList, int i = 0; i < childCount; ++i)
	{
		auto iterInnerChild = mWindowList.find(allChildList[i]->getName());
		if (iterInnerChild != mWindowList.end())
		{
			mWindowList.erase(iterInnerChild);
		}
		removeChildFromList(allChildList[i]);
	}
	END_FOR_STL(allChildList);
}
	
void WindowPrefabManager::destroyPrefab(WindowPrefab* prefab)
{
	if (prefab == NULL)
	{
		return;
	}
	destroyPrefab(prefab->getName());
}

WindowPrefab* WindowPrefabManager::getPrefab(const std::string& name)
{
	auto iter = mPrefabList.find(name);
	if (iter != mPrefabList.end())
	{
		return iter->second;
	}
	return NULL;
}