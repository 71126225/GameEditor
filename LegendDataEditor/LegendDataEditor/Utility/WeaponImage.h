#ifndef _WEAPON_IMAGE_H_
#define _WEAPON_IMAGE_H_

#include "ServerDefine.h"
#include "ImageDefine.h"

class WeaponImage
{
public:
	std::string mLabel;
	int mPosX;
	int mPosY;
	std::string mActionName;
	int mDirection;
	int mFrameIndex;		// �ڵ�ǰ�����е��±�
	int mWeaponID;
	int mActionIndex;		// �����������е��±�
public:
	void setFileName(const std::string& fileName);
	bool isValidImage() const { return mFrameIndex < HUMAN_ACTION[mActionIndex].mFrameCount; }
};

#endif