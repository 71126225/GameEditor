#ifndef _HUMAN_IMAGE_H_
#define _HUMAN_IMAGE_H_

#include "ServerDefine.h"
#include "ImageDefine.h"

class HumanImage
{
public:
	int mPosX;
	int mPosY;
	std::string mOriginFileName;
	int mIndexInGroup;		// �ڵ�ǰ����е��±�
	std::string mActionName;
	int mDirection;
	int mFrameIndex;		// �ڵ�ǰ�����е��±�
	int mClothID;
	int mActionIndex;		// �����������е��±�
public:
	void moveImage(const std::string& prePath);
	void setIndexInGroup(int index);
	bool isValidImage() const { return mFrameIndex < Action[mActionIndex].mFrameCount; }
};

#endif