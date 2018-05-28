#ifndef _SCENE_EDITOR_CORE_H_
#define _SCENE_EDITOR_CORE_H_

#include "CommonDefine.h"
#include "EditorCoreBase.h"
#include "txEngineBase.h"

class ECScene;
class ECEditor;
class SceneEditorCore : public EditorCoreBase, public txEngineBase
{
public:
	SceneEditorCore();
	~SceneEditorCore();
	virtual void init(){}
	virtual void update(const float& elapsedTime);
	virtual void render();
	ECScene* getCurScene() { return mCurScene; }
	txVector<ECEditor*>& getEditorSelections() { return mEditorMultiSelections; }
	void setEditorSelection(ECEditor* selection);
	void setEditorSelection(txVector<ECEditor*>& selection);
	void reselectEditor();
	void clearEditorSelection();
	void newScene(const std::string& newSceneName);
	void openAdvanceScene(const std::string& sceneName);
	void saveAdvanceScene();
	// �򿪳���,�����ǳ�����
	void openScene(const std::string& sceneName, const bool& binary);
	// ת��json�����ļ��������Ƴ����ļ�
	bool transSceneFile(const std::string& fullFileName, const bool& tranMeshFile);
	// ת��json��̬ģ���ļ��������ƶ�̬ģ���ļ�,transAllFileΪ�Ƿ�ת���ļ�����Ŀ¼�е����ж�̬ģ��
	bool transSkinMeshFile(const std::string& fullFileName, const bool& transAllFile);
	void destroyScene(){ destroyScene(mCurScene); }
	void destroyScene(ECScene* scene);
	// force��ʾ�Ƿ�ǿ������״̬,��ʹ״̬��ͬҲҪ����,���ҷ���״̬�ı���¼�
	void setSceneModified(bool modified, bool force = false, bool notifyEvent = true);
	bool isSceneModified() { return mSceneModified; }
protected:
	bool askSaveScene(const std::string& info);
	void notifyEditorSelectionChanged(const bool& notifyEvent = true);
protected:
	txVector<ECEditor*> mEditorMultiSelections;			// ���б�ѡ�е�����,������˳��֮��
	txSet<ECEditor*> mEditorMultiSelSet;				// ���б�ѡ�е�����,�������ж��Ƿ�ѡ������ͬ������
	ECScene* mCurScene;									// ��ǰ����
	bool mSceneModified;								// ��ǰ�ĳ����Ƿ�ı��
};

#endif