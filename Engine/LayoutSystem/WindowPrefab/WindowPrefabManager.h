#ifndef _WINDOW_PREFAB_MANAGER_H_
#define _WINDOW_PREFAB_MANAGER_H_

#include "txEngineDefine.h"
#include "txEngineBase.h"

class WindowPrefab;
class LayoutManager;
class Window;
class WindowPrefabManager : txEngineBase
{
public:
	WindowPrefabManager(){}
	virtual ~WindowPrefabManager(){ destroy(); }
	void init();
	void destroy();
	void loadAllPrefab();
	void loadPrefab(const std::string& fileName);
	// ����һ���յ�Ԥ��
	WindowPrefab* createPrefab(const std::string& name, const bool& sendEvent = true);
	// ����һ�����ڴ���һ��Ԥ��,�Ὣ�ô����е������Ӵ��ڿ�����Ԥ����
	WindowPrefab* createPrefab(Window* window, const std::string& name);
	Window* createWindow(const std::string& typeName, const std::string& name);
	void destroyPrefab(const std::string& name);
	void destroyPrefab(WindowPrefab* prefab);
	WindowPrefab* getPrefab(const std::string& name);
	txMap<std::string, WindowPrefab*>& getPrefabList() { return mPrefabList; }
protected:
	void addChildToList(Window* window);
	void removeChildFromList(Window* window);
protected:
	txMap<std::string, WindowPrefab*> mPrefabList;
	txMap<std::string, Window*> mWindowList;	// ���е�Ԥ���е��Ӵ���,������Ԥ��,Ҳ������Ԥ���Ӵ����е��ڲ��Ӵ���
};

#endif