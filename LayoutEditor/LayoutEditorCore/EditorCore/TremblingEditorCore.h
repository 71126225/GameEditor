#ifndef _TREMBLING_EDITOR_H_
#define _TREMBLING_EDITOR_H_

#include "txEngineBase.h"
#include "CommonDefine.h"
#include "EditorCoreBase.h"

class txTrembling;
class txTremblingNode;
class txTremblingKeyFrame;
class TremblingEditorCore : public txEngineBase, public EditorCoreBase
{
public:
	TremblingEditorCore();
	virtual void init(){}
	virtual void update(float elapsedTime){}
	void createKeyFrameTrembling(const std::string& name = EMPTY_STRING);
	void loadKeyFrameTrembling(const std::string& tremblingName);
	void selectKeyFrameDimension(const std::string& dimensionName);
	void addDimension(const std::string& dimensionName = EMPTY_STRING);
	void deleteDimension(const std::string& dimensionName);
	void setKeyFrameModified(const bool& modified, const bool& force = false, const bool& sendEvt = true);
	void saveKeyFrameTrebling();
	void selectKeyFrame(const int& index);
	void setKeyFrameTime(const float& time);
	void setKeyFrameValue(const float& value);
	void setTremblingInfo(const std::string& info);
	void setTremblingFormat(const std::string& format);
	void setTremblingNodeInfo(const std::string& info);
	void addKeyFrame(const int& index, const float& time = -1.0f, const float& offset = 0.0f);
	void deleteKeyFrame(const int& index);
	txTrembling* getCurTrembling()				{ return mCurTrembling; }
	txTremblingNode* getCurTremblingNode()		{ return mCurTremblingNode; }
	txTremblingKeyFrame* getCurKeyFrame()		{ return mCurKeyFrame; }
protected:
	bool askSaveTrembling(const std::string& info);
protected:
	bool						mTremblingModified;		// ��ǰ�𶯹ؼ�֡�Ƿ��Ѿ��޸Ĺ���
	txTrembling*				mCurTrembling;			// ��ǰ���ڱ༭���𶯹ؼ�֡
	txTremblingNode*			mCurTremblingNode;		// ��ǰ���ڱ༭�Ĺؼ�֡ά��
	txTremblingKeyFrame*		mCurKeyFrame;			// ��ǰ���ڱ༭�Ĺؼ�֡
};

#endif