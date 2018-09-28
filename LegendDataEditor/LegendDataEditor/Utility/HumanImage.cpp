#include "HumanImage.h"
#include "Utility.h"

void HumanImage::moveImage(const std::string& prePath)
{
	// ֻ�ƶ���ЧͼƬ
	if (isValidImage())
	{
		std::string animSetName = mActionName + "_dir" + StringUtility::intToString(mDirection);
		std::string fileName = animSetName + "_" + StringUtility::intToString(mFrameIndex) + ".png";
		std::string path = prePath + "/human_cloth" + StringUtility::intToString(mClothID) + "_" + animSetName + "/";
		FileUtility::moveFile(mOriginFileName, path + fileName);
	}
}

void HumanImage::setIndexInGroup(int index)
{
	mIndexInGroup = index;
	int i = 0;
	while (true)
	{
		if (index - Action[i].mMaxFrame * DIRECTION_COUNT < 0)
		{
			break;
		}
		index -= Action[i].mMaxFrame * DIRECTION_COUNT;
		++i;
	}
	mActionIndex = i;
	// ��Ϊһ�鶯����Դ������8�������ϵ����ж���,���Կ��Ը����±��������������֡�±�
	mDirection = index / Action[mActionIndex].mMaxFrame;
	mFrameIndex = index % Action[mActionIndex].mMaxFrame;
	mActionName = Action[mActionIndex].mName;
}