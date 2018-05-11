#ifndef _TX_TREMBLING_H_
#define _TX_TREMBLING_H_

#include "txEngineDefine.h"
#include "txEngineBase.h"

class txTremblingNode;

// �𶯽ڵ�洢
class txTrembling : public txEngineBase
{
public:
	txTrembling(const std::string& name)
		:
		mLength(0.0f),
		mName(name)
	{}
	virtual ~txTrembling(){ destroy(); }
	bool init(const std::string& filePath);
	void destroy();
	void saveTrembling(std::string& stream);
	bool readFile(const std::string& filePath);
	txTremblingNode* addNode(const std::string& name, const std::string& info, txMap<float, float>& keyFrameList);
	void deleteNode(const std::string& name);
	txTremblingNode* getNode(const std::string& name);
	void refreshLength();
	void queryValue(const float& time, txVector<float>& valueList, const float& amplitude = 1.0f);
	txVector<float> queryValue(const float& time, const float& amplitude = 1.0f);
	txVector<txTremblingNode*>& getTremblingNodeList()	{ return mTremblingNodeList; }
	const float& getLength()							{ return mLength; }
	const std::string& getName()						{ return mName; }
	const std::string& getInfo()						{ return mInfo; }
	const std::string& getFormat()						{ return mFormat; }
	void setInfo(const std::string& info)				{ mInfo = info; }
	void setFormat(const std::string& format)			{ mFormat = format; }
protected:
	txVector<txTremblingNode*> mTremblingNodeList;
	txMap<std::string, int> mTremblingMap;			// first�ǽڵ������,second�ǽڵ���vector�е��±�
	float mLength;			// �𶯳���
	std::string mName;		// ����
	std::string mInfo;		// �𶯵�����
	std::string mFormat;	// �����ؼ�֡��ʽ
	static int mDimensionNameSeed;
};

#endif