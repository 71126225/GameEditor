#ifndef _HUMAN_IMAGE_H_
#define _HUMAN_IMAGE_H_

#include "ServerBase.h"
#include "ImageDefine.h"

class HumanImage : public ServerBase
{
public:
	string mLabel;
	int mPosX;
	int mPosY;
	string mActionName;
	int mDirection;
	int mFrameIndex;		// �ڵ�ǰ�����е��±�
	int mClothID;
	int mActionIndex;		// �����������е��±�
public:
	void setFileName(const string& fileName);
};

#endif