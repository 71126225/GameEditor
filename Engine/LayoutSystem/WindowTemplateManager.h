#ifndef _WINDOW_TEMPLATE_MANAGER_H_
#define _WINDOW_TEMPLATE_MANAGER_H_

#include "txEngineDefine.h"
#include "txEngineBase.h"

class Window;
class LayoutManager;
class WindowTemplateManager : txEngineBase
{
public:
	WindowTemplateManager(){}
	virtual ~WindowTemplateManager(){ destroy(); }
	virtual void init();
	virtual void destroy();
	void loadAllTemplate();
	void loadTemplate(const std::string& fileName);
	// ����һ���յ�ģ��
	Window* createTemplate(const std::string& typeName, const std::string& name, const bool& sendEvent = true);
	// ����һ�����ڴ���һ��ģ��,���Ը����ں������Ӵ���,ֻ������������
	Window* createTemplate(Window* window, const std::string& name);
	bool destroyTemplate(const std::string& name);
	bool destroyTemplate(Window* windowTemplate);
	Window* getTemplate(const std::string& name);
	const std::string& getTemplateFileName(const std::string& name);
	bool renameTemplate(const std::string& oldName, const std::string& newName);
	txMap<std::string, std::pair<Window*, std::string> >& getTemplateList() { return mTemplateList; }
protected:
	void setFileName(const std::string& templateName, const std::string& fileName);
	void addChildToList(Window* window);
	void removeChildFromList(Window* window);
protected:
	txMap<std::string, std::pair<Window*, std::string> > mTemplateList;
	txMap<std::string, Window*> mWindowList;
};

#endif