#ifndef _LAYOUT_SCRIPT_H_
#define _LAYOUT_SCRIPT_H_

#include "txCommandReceiver.h"
#include "txEngineBase.h"
#include "WindowHeader.h"

class Layout;
class LayoutScript : public txCommandReceiver
{
public:
	LayoutScript(const std::string& name, Layout* layout)
		:
		txCommandReceiver(name),
		mLayout(layout)
	{}
	virtual ~LayoutScript(){ destroy(); }
	virtual void assignWindow() = 0;
	virtual void init(){}
	void destroy(){}
	virtual void update(const float& elapsedTime){}
	// �ڿ�ʼ��ʾ֮ǰ,��Ҫ�����е�״̬�����õ�����ʱ��״̬
	virtual void onReset() { }
	// ���ò���״̬��,�ٸ��ݵ�ǰ��Ϸ״̬���ò�����ʾǰ��״̬
	virtual void onGameState() { }
	virtual void onShow(const bool& immediately, const std::string& param){}
	virtual void onHide(const bool& immediately, const std::string& param){}
	// ֪ͨ�ű���ʼ��ʾ������,�ж�ȫ������
	void notifyStartShowOrHide();
	void addDelayCmd(txCommand* cmd);
	void interruptCommand(const int& assignID);
	Layout* getLayout(){ return mLayout; }
	void setRoot(Window* root) { mRoot = root; }
	Window* getRoot() { return mRoot; }
	bool connectWindowEvent(const WINDOW_EVENT& event, const std::string& windowName, WindowEventCallback callback, void* userData = NULL);
protected:
	static void onCmdStarted(txCommand* cmd, void* userdata);
protected:
	Layout* mLayout;
	Window* mRoot;
	txSet<int> mDelayCmdList;  // ������ʾ������ʱ���ӳ������б�,������ִ��ʱ,����б����Ƴ�������
};

#endif