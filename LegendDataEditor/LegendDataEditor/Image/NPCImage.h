#ifndef _NPC_IMAGE_H_
#define _NPC_IMAGE_H_

#include "ServerBase.h"
#include "ImageDefine.h"

class NPCImage : public ServerBase
{
public:
	int mPosX;
	int mPosY;
	string mActionName;
	int mDirection;
	int mFrameIndex;		// �ڵ�ǰ�����е��±�
	int mID;
	int mActionIndex;		// �����������е��±�
public:
	void setFileName(const string& fileName);
	bool isValidImage() const { return mFrameIndex < NPC_ACTION[mActionIndex].mFrameCount; }
};

#endif