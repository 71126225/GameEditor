#ifndef _COMPLEX_WINDOW_H_
#define _COMPLEX_WINDOW_H_

#include "Window.h"

// ���ϴ��ڻ���,���ര�ڿ��԰�������ڲ��Ӵ���,�Ӷ����һ���µĴ�������
// �ڲ��Ӵ������ֻ���ǷǸ��ϴ���
// �ڲ��Ӵ������ⲿ�����븸������һ������,�����������Ӵ���
class ComplexWindow : public Window
{
public:
	ComplexWindow(const std::string& name, const WINDOW_TYPE& type);
	virtual ~ComplexWindow();
	virtual void render();	// ���ƴ���
	virtual void update(const float& elapsedTime);
	void destroy();
	virtual void copyProperty(Window* window);
	virtual bool rename(const std::string& newName);
	virtual Window* getChild(const std::string& name);
	virtual void detachLayoutParent(const bool& detachLayoutOnly = false);
	// �����벼�ֺ͸����ڵ���ϵ,ʹ�ô��ڳ�Ϊ�����е�һ������,�ô����µ������Ӵ���Ҳ���ؽ��벼�ֵ���ϵ,���ӹ�ϵ��Ȼ����
	virtual void attachLayoutParent(Layout* layout, Window* parent, const int& childPos);
	virtual void setDirty(const bool& dirty, const bool& refreshNow);
	virtual void getUsedResourceList(txVector<std::string>& resourceList);
	virtual void removeChild(const std::string& name);
	virtual void notifyChildNameChanged(const std::string& oldName, Window* window);
	virtual void notifyInnerChildDestroied(const std::string& name);
	virtual void saveWindow(std::string& windowStream);
	virtual void saveTemplate(std::string& windowStream);
	txVector<Window*>& getInnerChildren() { return mInnerChildList; }
	virtual void getAllChildren(txVector<Window*>& children)
	{
		children = mInnerChildList;
		int childCount = mChildren.size();
		FOR_STL(mChildren, int i = 0; i < childCount; ++i)
		{
			children.push_back(mChildren[i]);
		}
		END_FOR_STL(mChildren);
	}
protected:
	void addInnerChild(Window* child);
	Window* createInnerChild(const std::string& windowType, const std::string& name);
	virtual void renameInnerChildren(const std::string& namePrefix) = 0;
	// �ݹ鿽�������Ӵ��ڵ�����
	void copyChildProperty(Window* sourceWindow, Window* destWindow);
protected:
	txMap<std::string, Window*> mInnerChildMap;					// ���ϴ����µĵ�һ���ڲ��Ӵ��ڲ����б�
	txVector<Window*> mInnerChildList;	// ���ϴ����µĵ�һ���ڲ��Ӵ����б�
};

#endif