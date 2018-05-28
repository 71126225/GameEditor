#ifndef _LAYOUT_EDITOR_CORE_H_
#define _LAYOUT_EDITOR_CORE_H_

#include "txEngineBase.h"
#include "CommonDefine.h"
#include "EditorCoreBase.h"

class Layout;
class Window;
class ECRenderWindowCoordinate;
class LayoutEditorCore : public EditorCoreBase, public txEngineBase
{
public:
	LayoutEditorCore();
	virtual void init();
	virtual void update(float elapsedTime);
	// ����һ���յ�Layout 
	Layout* createLayout(const std::string& layoutName = "layout");
	Layout* loadLayout(const std::string& fileName);
	void closeLayout(){ closeLayout(mCurLayout); }
	void closeLayout(const std::string& layoutName);
	void closeLayout(Layout* layout);
	void destroyLayout() { destroyLayout(mCurLayout); }
	void destroyLayout(const std::string& layoutName);
	void destroyLayout(Layout* layout);
	void saveLayout();
	void editPrefab(const std::string& prefabName);
	// ��ָ�����ڱ���ΪԤ��
	void savePrefab(const std::string& windowName);
	// ��ָ�����ڱ���Ϊģ��
	void saveTemplate(const std::string& windowName);
	void setLayoutVisible(Layout* layout, const bool& visible);
	void setLayoutVisible(const std::string& layoutName, const bool& visible);
	// force��ʾ�Ƿ�ǿ������״̬,��ʹ״̬��ͬҲҪ����,���ҷ���״̬�ı���¼�
	void setLayoutModified(const bool& modified, const bool& force = false, const bool& notifyEvent = true);
	const bool& isLayoutModified() { return mLayoutModified; }
	void setWireframeVisible(const bool& visible);
	void setSelectionWindowCoordinateVisible(const bool& visible);
	void setRenderWindowCoordinateVisible(const bool& visible);
	const bool& getWireframeVisible(){ return mWireframeVisible; }
	const bool& getSelectionWindowCoordinateVisible(){ return mCoordinateVisible; }
	const bool& getRenderWindowCoordinateVisible();
	void setScreenDensity(const float& density);
	void pickupAllResources(const std::string& targetPath);
	// ƽ��ѡ�еĴ���
	void moveSelectedWindow(const VECTOR2& moveDelta);
	// ɾ������
	void deleteWindow(const std::string& name, const std::string& layoutName = EMPTY_STRING);
	// ͨ����Layout����Window,����ӵ�layout��
	Window* addWindow(Layout* parent, const std::string& typeName, const std::string& windowName = EMPTY_STRING, const int& childPos = -1);
	// ����һ��Window���Ӵ���
	Window* addWindow(Window* parent, const std::string& typeName, const std::string& windowName = EMPTY_STRING, const int& childPos = -1);
	// ��parent���һ���Ӵ���,ֱ�ӽ�child��ӵ�parent�Ӵ����б���
	// child�µ������Ӵ���Ҳ��һ����ӻ��Ƶ�parent��
	void addWindow(Layout* layout, Window* parent, Window* child, const int& childPos);
	txVector<Window*>& getSelections() { return mMultiSelections; }
	void setSelection(Window* selection);
	void setSelection(txVector<Window*>& selection);
	void reselect();
	void clearSelection(const bool& sendEvt = true);
	// ����һ������,copyChildָ���Ƿ����Ӵ���һ�𿽱�,cutWindowָ���Ƿ����,����ʱǿ����ͬ�Ӵ���һ�����
	void copyWindow(const std::string& name, const bool& copyChild = false, const bool& cutWindow = false);
	void pasteWindow(const std::string& destParent, const int& pos = -1);
	// �����Ҫ�ı�layout������,�������ô˺���,����ֱ�ӵ���layout�ĸ���
	bool renameLayout(const std::string& newName);
	// �ƶ������ڸ������е���Ⱦ˳��
	void moveWindowPos(Window* window, const int& destPos);
	void notifyMouseEnterWindow(const std::string& windowName);
	void notifyMouseLeaveWindow(const std::string& windowName);
	Layout* getCurLayout()					{ return mCurLayout; }
	ECRenderWindowCoordinate* getRenderWindowCoordinate(){ return mRenderWindowCoordinate; }
protected:
	// ��һ��ѯ���Ƿ񱣴沼�ֵĶԻ���,����ֵ��ʾ�Ƿ����ִ����ȥ
	bool askSaveLayout(const std::string& info);
	void notifySelectionChanged(const bool& notifyEvent = true);
	void processKey();
protected:
	txVector<Window*>			mMultiSelections;		// ���б�ѡ�еĴ���,������˳��֮��
	txSet<Window*>				mMultiSelSet;			// ���б�ѡ�еĴ���,�������ж��Ƿ�ѡ������ͬ�Ĵ���
	Window*						mCopyWindow;			// �����Ĵ���
	Layout*						mCopySrcLayout;			// ������Դ���������Ĳ���
	bool						mCutWindow;				// ����ʱ�Ƿ���Ҫɾ��ԭ����
	std::string					mCutWindowName;			// ��Ҫ���еĴ�����
	bool						mLayoutModified;		// ��ǰ�Ĳ����Ƿ�ı��
	Layout*						mCurLayout;				// ��ǰ���ڱ༭�Ĳ���
	bool						mWireframeVisible;		// �ǲ���ʾ�߿�
	bool						mCoordinateVisible;		// �Ƿ���ʾ��������
	ECRenderWindowCoordinate*	mRenderWindowCoordinate;// ��Ļ����ϵ
	VECTOR4						mPreWireframeColor;		// ��һ�ε��߿���ɫ
	VECTOR4						mPreCoordinateColor;	// ��һ�ε�����ϵ��ɫ
};

#endif