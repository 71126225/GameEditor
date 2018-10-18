#include "Utility.h"
#include "txSerializer.h"
#include "HumanImage.h"
#include "WeaponImage.h"
#include "SQLite.h"
#include "SQLiteCloth.h"
#include "SQLiteClothFrame.h"
#include "SQLiteMonster.h"
#include "SQLiteMonsterFrame.h"
#include "SQLiteWeapon.h"
#include "SQLiteWeaponFrame.h"
#include "SQLiteEffect.h"
#include "SQLiteEffectFrame.h"
#include "HumanAction.h"
#include "WeaponAction.h"
#include "SceneMap.h"
#include "MapHeader.h"
#include "MapTile.h"

void ImageUtility::encodePNG(const std::string& path, char* color, int width, int height, FREE_IMAGE_FORMAT format)
{
	std::string dir = StringUtility::getFilePath(path);
	FileUtility::createFolder(dir);
	FreeImage_Initialise();
	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 32);
	BYTE* bits = FreeImage_GetBits(bitmap);
	memcpy(bits, color, width * height * 4);
	FreeImage_Save(format, bitmap, path.c_str());
	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();
}

bool ImageUtility::readWixFile(const std::string& filePath, WIXFileImageInfo& info)
{
	int fileSize = 0;
	char* fileBuffer = FileUtility::openBinaryFile(filePath, &fileSize);
	if (fileBuffer == NULL)
	{
		return false;
	}
	txSerializer serializer(fileBuffer, fileSize);
	serializer.readBuffer(info.mStrHeader, 44);
	serializer.read(info.mIndexCount);
	for (int i = 0; i < info.mIndexCount; ++i)
	{
		int curStartPos = 0;
		if (serializer.read(curStartPos))
		{
			info.mPositionList.push_back(curStartPos);
		}
	}
	TRACE_DELETE_ARRAY(fileBuffer);
	return true;
}

bool ImageUtility::readWilHeader(const std::string& filePath, WILFileHeader& header)
{
	int fileSize = 0;
	char* fileBuffer = FileUtility::openBinaryFile(filePath, &fileSize);
	if (fileBuffer == NULL)
	{
		return false;
	}
	txSerializer serializer(fileBuffer, fileSize);
	serializer.readBuffer(header.mInfo, 44);
	serializer.readBuffer(header.mPlayInfo, 12);
	for (int i = 0; i < 256; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			serializer.read(header.mColor[i][j]);
		}
	}
	TRACE_DELETE_ARRAY(fileBuffer);
	return true;
}

void ImageUtility::wixWilToPNG(const std::string& wixFileName, const std::string& wilFileName, const std::string& outputPath)
{
	// ��wix�ļ�
	WIXFileImageInfo wixFileHeader;
	if (!readWixFile(wixFileName, wixFileHeader))
	{
		std::cout << "�Ҳ���wix�ļ�" << std::endl;
		return;
	}

	// ��wil�ļ�
	WILFileHeader wilHeader;
	if(!readWilHeader(wilFileName, wilHeader))
	{
		std::cout << "�Ҳ���wil�ļ�" << std::endl;
		return;
	}

	POINT* posList = TRACE_NEW_ARRAY(POINT, wixFileHeader.mPositionList.size(), posList);
	int fileSize = 0;
	char* fileBuffer = FileUtility::openBinaryFile(wilFileName, &fileSize);
	txSerializer serializer(fileBuffer, fileSize);
	for (int i = 0; i < wixFileHeader.mPositionList.size(); ++i)
	{
		// ���±����õ���ǰͼƬ��ʼλ��,���Ҷ�ȡͼƬ��Ϣ
		int startPos = wixFileHeader.mPositionList[i];
		if (startPos == 0)
		{
			continue;
		}
		serializer.setIndex(startPos);
		WILFileImageInfo curInfo;
		// ���,λ��ƫ��
		serializer.read(curInfo.mWidth);
		serializer.read(curInfo.mHeight);
		serializer.read(curInfo.mPosX);
		serializer.read(curInfo.mPosY);
		// ������ɫ�����ڵ�ɫ���л�ȡ��ɫ����
		int pixelCount = curInfo.mWidth * curInfo.mHeight;
		TRACE_NEW_ARRAY(char, pixelCount * 4, curInfo.mColor);
		for (int j = 0; j < pixelCount; ++j)
		{
			if (startPos + ImageHeaderLength + j >= fileSize)
			{
				std::cout << "error" <<std::endl;
				break;
			}
			unsigned char pixelIndex = fileBuffer[startPos + ImageHeaderLength + j];
			// 0��,1��,2��
			curInfo.mColor[j * 4 + 0] = wilHeader.mColor[pixelIndex][0];// ��
			curInfo.mColor[j * 4 + 1] = wilHeader.mColor[pixelIndex][1];// ��
			curInfo.mColor[j * 4 + 2] = wilHeader.mColor[pixelIndex][2];// ��
			curInfo.mColor[j * 4 + 3] = pixelIndex == 0 ? 0 : (char)255;
		}
		posList[i].x = curInfo.mPosX;
		posList[i].y = curInfo.mPosY;
		// ��ͼƬת��Ϊpng
		encodePNG(outputPath + StringUtility::intToString(i) + ".png", curInfo.mColor, curInfo.mWidth, curInfo.mHeight, FIF_PNG);
		TRACE_DELETE_ARRAY(curInfo.mColor);
	}
	TRACE_DELETE_ARRAY(fileBuffer);

	writePositionFile(outputPath + "position.txt", posList, wixFileHeader.mPositionList.size());
	TRACE_DELETE_ARRAY(posList);
}

void ImageUtility::writePositionFile(const std::string& positionFile, POINT* posList, int posCount)
{
	std::string posStr;
	for (int i = 0; i < posCount; ++i)
	{
		posStr += StringUtility::intToString(posList[i].x);
		posStr += ",";
		posStr += StringUtility::intToString(posList[i].y);
		posStr += "\n";
	}
	FileUtility::writeFile(positionFile, posStr);
}

POINT* ImageUtility::readPositionFile(const std::string& positionFile, int& posCount)
{
	std::string posStr = FileUtility::openTxtFile(positionFile);
	txVector<std::string> posStrList;
	StringUtility::split(posStr, "\n", posStrList);
	posCount = posStrList.size();
	POINT* posList = TRACE_NEW_ARRAY(POINT, posCount, posList);
	for (int i = 0; i < posCount; ++i)
	{
		txVector<std::string> pointList;
		StringUtility::split(posStrList[i], ",", pointList);
		if (pointList.size() != 2)
		{
			continue;
		}
		posList[i].x = StringUtility::stringToInt(pointList[0]);
		posList[i].y = StringUtility::stringToInt(pointList[1]);
	}
	return posList;
}

void ImageUtility::autoGroupHumanImage(const std::string& path)
{
	// �Ȳ��λ���ļ�
	splitPositionFile(path);
	// ����600���ļ�һ��,���뵥�����ļ�����
	autoMoveFile(path, HUMAN_GROUP_SIZE);

	txVector<std::string> folderList;
	FileUtility::findFolders(path, folderList);
	int folderCount = folderList.size();
	for (int i = 0; i < folderCount; ++i)
	{
		// ���ն���������
		txVector<std::string> fileList;
		FileUtility::findFiles(folderList[i], fileList, ".png", false);
		sortByFileNumber(fileList);
		int fileCount = fileList.size();
		for (int j = 0; j < fileCount; ++j)
		{
			std::string actionName;
			int direction;
			int frameIndex;
			bool isValid = getHumanActionInfo(j, actionName, direction, frameIndex);
			// �������ЧͼƬ����Ҫɾ��
			if (!isValid)
			{
				deleteImageWithPosition(fileList[j]);
			}
			std::string actionFolderName = actionName + "_dir" + StringUtility::intToString(direction);
			std::string destPath = StringUtility::getFilePath(fileList[j]) + "/" + actionFolderName + "/";
			moveImageWithPosition(fileList[j], destPath + actionFolderName + "_" + StringUtility::intToString(frameIndex) + StringUtility::getFileSuffix(fileList[j], true));
		}
	}
}

void ImageUtility::autoGroupWeaponImage(const std::string& path)
{
	// �Ȳ��λ���ļ�
	splitPositionFile(path);
	// ����600���ļ�һ��,���뵥�����ļ�����
	autoMoveFile(path, WEAPON_GROUP_SIZE);
	// ��Ϊ�����ļ��ǽ�ɫ�ļ���2��,�����������ֵ�����,������ʱֻ����ǰһ����ļ�,�����ɾ��
	txVector<std::string> folderList;
	FileUtility::findFolders(path, folderList);
	int folderCount = folderList.size();
	for (int i = 0; i < folderCount; ++i)
	{
		// ���ն���������
		txVector<std::string> fileList;
		FileUtility::findFiles(folderList[i], fileList, ".png", false);
		sortByFileNumber(fileList);
		int fileCount = fileList.size();
		for (int j = 0; j < fileCount; ++j)
		{
			if (WEAPON_GROUP_SIZE == 1200 && j >= WEAPON_GROUP_SIZE / 2)
			{
				deleteImageWithPosition(fileList[j]);
			}
			else
			{
				std::string actionName;
				int direction;
				int frameIndex;
				bool isValid = getHumanActionInfo(j, actionName, direction, frameIndex);
				// �������ЧͼƬ����Ҫɾ��
				if (!isValid)
				{
					deleteImageWithPosition(fileList[j]);
				}
				std::string actionFolderName = actionName + "_dir" + StringUtility::intToString(direction);
				std::string destPath = StringUtility::getFilePath(fileList[j]) + "/" + actionFolderName + "/";
				moveImageWithPosition(fileList[j], destPath + actionFolderName + "_" + StringUtility::intToString(frameIndex) + StringUtility::getFileSuffix(fileList[j], true));
			}
		}
	}
}

void ImageUtility::autoGroupMonsterImage0(const std::string& path)
{
	// ���λ���ļ�
	ImageUtility::splitPositionFile(path);
	// Ȼ��360���ļ�һ��,�ƶ����������ļ���
	ImageUtility::autoMoveFile(path, MONSTER_GROUP_SIZE);
	// ����ͼƬ�����ɫ������ͼƬ�������й�������,����ֻ���ڴ��·�����ֶ���ÿ���ļ��н��ж�������
}

void ImageUtility::autoGroupMonsterImage1(const std::string& path)
{
	// �ֶ��Զ������з����,�Ϳ��Զ�ÿ�鶯���ļ����з������
	// �������ļ�,��ÿ���ļ����е�ͼƬ��������Ϊ���ļ����е�λ�����
	ImageUtility::renameImage(path);
	// �Զ����㷽�򲢷���
	ImageUtility::renameByDirection(path);
}

void ImageUtility::autoGroupEffectImage(const std::string& path)
{
	// �Ȳ��λ���ļ�
	splitPositionFile(path);
	// ����10���ļ�һ��,���뵥�����ļ�����
	autoMoveFile(path, EFFECT_GROUP_SIZE);
	// ɾ����ЧͼƬ
	deleteInvalidImage(path);
}

void ImageUtility::saveFrameInfo(const std::string& path, IMAGE_TYPE imageType, SQLite* sqlite)
{
	if (imageType == IT_MONSTER)
	{
		// ��һ���ļ�����ÿ������ķ���,ÿ�������ļ������иù������ж��������з��������֡
		// �����ļ����е�����ͼƬ
		txVector<std::string> folderList;
		FileUtility::findFolders(path, folderList);
		int folderCount = folderList.size();
		for (int i = 0; i < folderCount; ++i)
		{
			MonsterImageGroup imageGroup;
			txVector<std::string> fileList;
			FileUtility::findFiles(folderList[i], fileList, ".png");
			int fileCount = fileList.size();
			for (int j = 0; j < fileCount; ++j)
			{
				POINT pos = getImagePosition(fileList[j]);
				MonsterImage monsterImage;
				monsterImage.mLabel = StringUtility::getFileName(folderList[i]);
				monsterImage.mPosX = pos.x;
				monsterImage.mPosY = pos.y;
				monsterImage.mMonsterID = -1;
				monsterImage.setFileName(StringUtility::getFileNameNoSuffix(fileList[j]));
				imageGroup.addImage(monsterImage);
			}
			// ������֡������,�������ݺ�д�����ݿ�
			writeSQLite(imageGroup.mAllAction, sqlite);
		}
	}
	else if (imageType == IT_HUMAN)
	{
		// ��һ���ļ�����ÿ���·��ķ���,ÿ���·��ļ������и��·����ж��������з��������֡
		// �����ļ����е�����ͼƬ
		txVector<std::string> folderList;
		FileUtility::findFolders(path, folderList);
		int folderCount = folderList.size();
		for (int i = 0; i < folderCount; ++i)
		{
			HumanImageGroup imageGroup;
			txVector<std::string> fileList;
			FileUtility::findFiles(folderList[i], fileList, ".png");
			int fileCount = fileList.size();
			for (int j = 0; j < fileCount; ++j)
			{
				POINT pos = getImagePosition(fileList[j]);
				HumanImage monsterImage;
				monsterImage.mLabel = StringUtility::getFileName(folderList[i]);
				monsterImage.mPosX = pos.x;
				monsterImage.mPosY = pos.y;
				monsterImage.mClothID = -1;
				monsterImage.setFileName(StringUtility::getFileNameNoSuffix(fileList[j]));
				imageGroup.addImage(monsterImage);
			}
			// ������֡������,�������ݺ�д�����ݿ�
			writeSQLite(imageGroup.mAllAction, sqlite);
		}
	}
	else if (imageType == IT_WEAPON)
	{
		// ��һ���ļ�����ÿ�������ķ���,ÿ�������ļ������и��������ж��������з��������֡
		// �����ļ����е�����ͼƬ
		txVector<std::string> folderList;
		FileUtility::findFolders(path, folderList);
		int folderCount = folderList.size();
		for (int i = 0; i < folderCount; ++i)
		{
			WeaponImageGroup imageGroup;
			txVector<std::string> fileList;
			FileUtility::findFiles(folderList[i], fileList, ".png");
			int fileCount = fileList.size();
			for (int j = 0; j < fileCount; ++j)
			{
				POINT pos = getImagePosition(fileList[j]);
				WeaponImage monsterImage;
				monsterImage.mLabel = StringUtility::getFileName(folderList[i]);
				monsterImage.mPosX = pos.x;
				monsterImage.mPosY = pos.y;
				monsterImage.mWeaponID = -1;
				monsterImage.setFileName(StringUtility::getFileNameNoSuffix(fileList[j]));
				imageGroup.addImage(monsterImage);
			}
			// ������֡������,�������ݺ�д�����ݿ�
			writeSQLite(imageGroup.mAllAction, sqlite);
		}
	}
	else if (imageType == IT_EFFECT)
	{
		// ��һ���ļ�����ÿ����Ч�ķ���,ÿ����Ч�ļ������и���Ч���з��������֡,������Чֻ��1������
		// �����ļ����е�����ͼƬ
		txVector<std::string> folderList;
		FileUtility::findFolders(path, folderList);
		int folderCount = folderList.size();
		for (int i = 0; i < folderCount; ++i)
		{
			EffectImageGroup imageGroup;
			txVector<std::string> fileList;
			FileUtility::findFiles(folderList[i], fileList, ".png");
			int fileCount = fileList.size();
			for (int j = 0; j < fileCount; ++j)
			{
				POINT pos = getImagePosition(fileList[j]);
				EffectImage effectImage;
				effectImage.mLabel = StringUtility::getFileName(folderList[i]);
				effectImage.mPosX = pos.x;
				effectImage.mPosY = pos.y;
				effectImage.mID = -1;
				effectImage.setFileName(StringUtility::getFileNameNoSuffix(fileList[j]));
				imageGroup.addImage(effectImage);
			}
			// ������֡������,�������ݺ�д�����ݿ�
			writeSQLite(imageGroup.mAllEffect, sqlite);
		}
	}
}

void ImageUtility::writeSQLite(txMap<std::string, WeaponActionSet>& actionSetList, SQLite* sqlite)
{
	// ������֡������,�������ݺ�д�����ݿ�
	// �������ж���
	auto iter = actionSetList.begin();
	auto iterEnd = actionSetList.end();
	for (; iter != iterEnd; ++iter)
	{
		// �����ö��������з���
		for (int j = 0; j < DIRECTION_COUNT; ++j)
		{
			WeaponActionAnim& actionAnim = iter->second.mDirectionAction[j];
			WeaponFrameData data;
			data.mID = actionAnim.mImageFrame[0].mWeaponID;
			data.mLabel = StringUtility::ANSIToUTF8(actionAnim.mImageFrame[0].mLabel);
			data.mDirection = j;
			data.mAction = iter->first;
			data.mFrameCount = actionAnim.mImageFrame.size();
			// �����ö���������֡��
			for (int kk = 0; kk < data.mFrameCount; ++kk)
			{
				data.mPosX.push_back(actionAnim.mImageFrame[kk].mPosX);
				data.mPosY.push_back(actionAnim.mImageFrame[kk].mPosY);
			}
			bool ret = sqlite->mSQLiteWeaponFrame->insert(data);
			if (!ret)
			{
				break;
			}
		}
	}
}

void ImageUtility::writeSQLite(txMap<std::string, HumanActionSet>& actionSetList, SQLite* sqlite)
{
	auto iter = actionSetList.begin();
	auto iterEnd = actionSetList.end();
	for (; iter != iterEnd; ++iter)
	{
		// �����ö��������з���
		for (int j = 0; j < DIRECTION_COUNT; ++j)
		{
			HumanActionAnim& actionAnim = iter->second.mDirectionAction[j];
			ClothFrameData data;
			data.mID = actionAnim.mImageFrame[0].mClothID;
			data.mLabel = StringUtility::ANSIToUTF8(actionAnim.mImageFrame[0].mLabel);
			data.mDirection = j;
			data.mAction = iter->first;
			data.mFrameCount = actionAnim.mImageFrame.size();
			// �����ö���������֡��
			for (int kk = 0; kk < data.mFrameCount; ++kk)
			{
				data.mPosX.push_back(actionAnim.mImageFrame[kk].mPosX);
				data.mPosY.push_back(actionAnim.mImageFrame[kk].mPosY);
			}
			bool ret = sqlite->mSQLiteClothFrame->insert(data);
			if (!ret)
			{
				break;
			}
		}
	}
}

void ImageUtility::writeSQLite(txMap<std::string, MonsterActionSet>& actionSetList, SQLite* sqlite)
{
	auto iter = actionSetList.begin();
	auto iterEnd = actionSetList.end();
	for (; iter != iterEnd; ++iter)
	{
		// �����ö��������з���
		for (int j = 0; j < DIRECTION_COUNT; ++j)
		{
			MonsterActionAnim& actionAnim = iter->second.mDirectionAction[j];
			MonsterFrameData data;
			data.mID = actionAnim.mImageFrame[0].mMonsterID;
			data.mLabel = StringUtility::ANSIToUTF8(actionAnim.mImageFrame[0].mLabel);
			data.mDirection = j;
			data.mAction = iter->first;
			data.mFrameCount = actionAnim.mImageFrame.size();
			// �����ö���������֡��
			for (int kk = 0; kk < data.mFrameCount; ++kk)
			{
				data.mPosX.push_back(actionAnim.mImageFrame[kk].mPosX);
				data.mPosY.push_back(actionAnim.mImageFrame[kk].mPosY);
			}
			bool ret = sqlite->mSQLiteMonsterFrame->insert(data);
			if (!ret)
			{
				break;
			}
		}
	}
}

void ImageUtility::writeSQLite(txMap<std::string, EffectSet>& actionSetList, SQLite* sqlite)
{
	auto iter = actionSetList.begin();
	auto iterEnd = actionSetList.end();
	for (; iter != iterEnd; ++iter)
	{
		// �����ö��������з���
		auto iterDir = iter->second.mDirectionAction.begin();
		auto iterDirEnd = iter->second.mDirectionAction.end();
		for(; iterDir != iterDirEnd; ++iterDir)
		{
			EffectAnim& effectAnim = iterDir->second;
			EffectFrameData data;
			data.mID = effectAnim.mImageFrame[0].mID;
			data.mLabel = StringUtility::ANSIToUTF8(effectAnim.mImageFrame[0].mLabel);
			data.mDirection = iterDir->first;
			data.mFrameCount = effectAnim.mImageFrame.size();
			data.mAction = iter->first;
			// �����ö���������֡��
			for (int kk = 0; kk < data.mFrameCount; ++kk)
			{
				data.mPosX.push_back(effectAnim.mImageFrame[kk].mPosX);
				data.mPosY.push_back(effectAnim.mImageFrame[kk].mPosY);
			}
			bool ret = sqlite->mSQLiteEffectFrame->insert(data);
			if (!ret)
			{
				break;
			}
		}
	}
}

void ImageUtility::renameImage(const std::string& path)
{
	// ��Ŀ¼�е��ļ����ļ��������,������Ϊ��0��ʼ������
	txVector<std::string> folderList;
	FileUtility::findFolders(path, folderList, true);
	int folderCount = folderList.size();
	for (int i = 0; i < folderCount; ++i)
	{
		txVector<std::string> fileList;
		FileUtility::findFiles(folderList[i], fileList, ".png");
		// �ȸ����ļ�����������
		sortByFileNumber(fileList);
		int count = fileList.size();
		for (int j = 0; j < count; ++j)
		{
			std::string curFilePath = StringUtility::getFilePath(fileList[j]) + "/";
			std::string suffix = StringUtility::getFileSuffix(fileList[j]);
			renameImageWithPosition(fileList[j], curFilePath + StringUtility::intToString(j) + "." + suffix);
		}
	}
}

void ImageUtility::renameImageToAnim(const std::string& path)
{
	// ��Ŀ¼�е��ļ����ļ��������,������Ϊ����֡��ʽ������,���ļ�������ͷ,�Դ�0��ʼ�����ֽ�β
	txVector<std::string> folderList;
	FileUtility::findFolders(path, folderList, true);
	int folderCount = folderList.size();
	for (int i = 0; i < folderCount; ++i)
	{
		txVector<std::string> fileList;
		FileUtility::findFiles(folderList[i], fileList, ".png");
		// �ȸ����ļ�����������
		sortByFileNumber(fileList);
		int count = fileList.size();
		for (int j = 0; j < count; ++j)
		{
			std::string folderName = StringUtility::getFolderName(fileList[j]);
			std::string curFilePath = StringUtility::getFilePath(fileList[j]) + "/";
			std::string suffix = StringUtility::getFileSuffix(fileList[j]);
			renameImageWithPosition(fileList[j], curFilePath + folderName + "_" + StringUtility::intToString(j) + "." + suffix);
		}
	}
}

void ImageUtility::splitPositionFile(const std::string& path)
{
	// ��position.txt�ļ����Ϊ������txt�ļ�,ÿ��txt�ļ���ֻ����һ������
	int posCount = 0;
	POINT* posList = readPositionFile(path + "/position.txt", posCount);
	for (int i = 0; i < posCount; ++i)
	{
		std::string posStr = StringUtility::intToString(posList[i].x) + "," + StringUtility::intToString(posList[i].y);
		FileUtility::writeFile(path + "/" + StringUtility::intToString(i) + ".txt", posStr);
	}
	TRACE_DELETE_ARRAY(posList);
}

void ImageUtility::renameByDirection(const std::string& path)
{
	// ��Ŀ¼�е������ļ��Ȱ����ļ�������,Ȼ����˳�����Ϊ8������,�ٶ�ÿ��������ļ�������
	txVector<std::string> folderList;
	FileUtility::findFolders(path, folderList, true);
	int folderCount = folderList.size();
	for (int i = 0; i < folderCount; ++i)
	{
		txVector<std::string> fileList;
		FileUtility::findFiles(folderList[i], fileList, ".png", false);
		sortByFileNumber(fileList);
		int fileCount = fileList.size();
		int actionFrameCount = fileCount / DIRECTION_COUNT;
		for (int j = 0; j < fileCount; ++j)
		{
			if (fileCount % DIRECTION_COUNT != 0)
			{
				std::cout << "ͼƬ��������,����Ϊ�����������" << std::endl;
				break;
			}
			int imageDir = j / actionFrameCount;
			int index = j % actionFrameCount;
			// ���ļ��ƶ���һ���½��ļ�����
			std::string curPath = StringUtility::getFilePath(fileList[j]) + "/";
			std::string destFolderName = StringUtility::getFolderName(fileList[j]) + "_dir" + StringUtility::intToString(imageDir);
			std::string destPath = StringUtility::getFilePath(curPath) + "/" + destFolderName + "/";
			moveImageWithPosition(fileList[j], destPath + destFolderName + "_" + StringUtility::intToString(index) + "." + StringUtility::getFileSuffix(fileList[j]));
		}
	}
	// ɾ���յ�Ŀ¼
	FileUtility::deleteEmptyFolder(path);
}

void ImageUtility::sortByFileNumber(txVector<std::string>& fileList)
{
	// �����ļ��������ֽ�������
	txMap<int, std::string> sortedList;
	int count = fileList.size();
	for (int i = 0; i < count; ++i)
	{
		sortedList.insert(StringUtility::stringToInt(StringUtility::getFileNameNoSuffix(fileList[i])), fileList[i]);
	}
	if (sortedList.size() != fileList.size())
	{
		return;
	}
	fileList.clear();
	auto iter = sortedList.begin();
	auto iterEnd = sortedList.end();
	for (; iter != iterEnd; ++iter)
	{
		fileList.push_back(iter->second);
	}
}

void ImageUtility::autoMoveFile(const std::string& path, int groupSize)
{
	txVector<std::string> fileList;
	FileUtility::findFiles(path, fileList, ".png");
	sortByFileNumber(fileList);
	int fileCount = fileList.size();
	for (int i = 0; i < fileCount; ++i)
	{
		int groupIndex = i / groupSize;
		std::string destFolderName = StringUtility::intToString(groupIndex, 3);
		std::string destPath = StringUtility::getFilePath(fileList[i]) + "/" + destFolderName + "/";
		moveImageWithPosition(fileList[i], destPath + StringUtility::getFileName(fileList[i]));
	}
}

bool ImageUtility::getHumanActionInfo(int index, std::string& actionName, int& dir, int& frameIndex)
{
	int i = 0;
	while (true)
	{
		if (index - HUMAN_ACTION[i].mMaxFrame * DIRECTION_COUNT < 0)
		{
			break;
		}
		index -= HUMAN_ACTION[i].mMaxFrame * DIRECTION_COUNT;
		++i;
	}
	// ��Ϊһ�鶯����Դ������8�������ϵ����ж���,���Կ��Ը����±��������������֡�±�,ǰ���Ǳ����˿�ͼƬ��Ϊ���λ��
	dir = index / HUMAN_ACTION[i].mMaxFrame;
	frameIndex = index % HUMAN_ACTION[i].mMaxFrame;
	actionName = HUMAN_ACTION[i].mName;
	return frameIndex < HUMAN_ACTION[i].mFrameCount;
}

void ImageUtility::moveImageWithPosition(const std::string& fullFileName, const std::string& destFullFileName)
{
	std::string sourceFileNameNoSuffix = StringUtility::getFileNameNoSuffix(fullFileName);
	std::string destFileNameNoSuffix = StringUtility::getFileNameNoSuffix(destFullFileName);
	std::string sourcePath = StringUtility::getFilePath(fullFileName) + "/";
	std::string destPath = StringUtility::getFilePath(destFullFileName) + "/";
	moveFileWithMeta(fullFileName, destFullFileName);
	// �����ͬ��λ���ļ�,Ҳ��Ҫһ���ƶ�
	std::string positionFileName = sourcePath + sourceFileNameNoSuffix + ".txt";
	if (FileUtility::isFileExist(positionFileName))
	{
		std::string destPosFileName = destPath + destFileNameNoSuffix + ".txt";
		moveFileWithMeta(positionFileName, destPosFileName);
	}
}

void ImageUtility::renameImageWithPosition(const std::string& fullFileName, const std::string& destFullFileName)
{
	std::string sourceFileNameNoSuffix = StringUtility::getFileNameNoSuffix(fullFileName);
	std::string destFileNameNoSuffix = StringUtility::getFileNameNoSuffix(destFullFileName);
	std::string sourcePath = StringUtility::getFilePath(fullFileName) + "/";
	std::string destPath = StringUtility::getFilePath(destFullFileName) + "/";
	renameFileWithMeta(fullFileName, destFullFileName);
	// �����ͬ��λ���ļ�,Ҳ��Ҫһ��������
	std::string positionFileName = sourcePath + sourceFileNameNoSuffix + ".txt";
	if (FileUtility::isFileExist(positionFileName))
	{
		std::string destPosFileName = destPath + destFileNameNoSuffix + ".txt";
		renameFileWithMeta(positionFileName, destPosFileName);
	}
}

void ImageUtility::deleteImageWithPosition(const std::string& fullFileName)
{
	std::string sourceFileNameNoSuffix = StringUtility::getFileNameNoSuffix(fullFileName);
	std::string sourcePath = StringUtility::getFilePath(fullFileName) + "/";
	deleteFileWithMeta(fullFileName);
	// �����ͬ��λ���ļ�,Ҳ��Ҫһ��ɾ��
	std::string positionFileName = sourcePath + sourceFileNameNoSuffix + ".txt";
	if (FileUtility::isFileExist(positionFileName))
	{
		deleteFileWithMeta(positionFileName);
	}
}

void ImageUtility::moveFileWithMeta(const std::string& fullFileName, const std::string& destFullFileName)
{
	// �ƶ��ļ���ͬ��meta�ļ�
	FileUtility::moveFile(fullFileName, destFullFileName);
	std::string metaFile = fullFileName + ".meta";
	if (FileUtility::isFileExist(metaFile))
	{
		FileUtility::moveFile(metaFile, destFullFileName + ".meta");
	}
}

void ImageUtility::renameFileWithMeta(const std::string& fullFileName, const std::string& destFullFileName)
{
	// �������ļ���ͬ��meta�ļ�
	FileUtility::renameFile(fullFileName, destFullFileName);
	std::string metaFile = fullFileName + ".meta";
	if (FileUtility::isFileExist(metaFile))
	{
		FileUtility::renameFile(metaFile, destFullFileName + ".meta");
	}
}

void ImageUtility::deleteFileWithMeta(const std::string& fullFileName)
{
	// ɾ���ļ���ͬ��meta�ļ�
	FileUtility::deleteFile(fullFileName);
	std::string metaFile = fullFileName + ".meta";
	if (FileUtility::isFileExist(metaFile))
	{
		FileUtility::deleteFile(metaFile);
	}
}

void ImageUtility::deleteInvalidImage(const std::string& path)
{
	txVector<std::string> fileList;
	FileUtility::findFiles(path, fileList, ".png");
	sortByFileNumber(fileList);
	int count = fileList.size();
	for (int i = 0; i < count; ++i)
	{
		if (!isInvalidImage(fileList[i]))
		{
			deleteImageWithPosition(fileList[i]);
		}
	}
	FileUtility::deleteEmptyFolder(path);
}

bool ImageUtility::isInvalidImage(const std::string& fileName)
{
	FreeImage_Initialise(1);
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName.c_str());
	FIBITMAP* bitmap = FreeImage_Load(format, fileName.c_str());
	int width = FreeImage_GetWidth(bitmap);
	int height = FreeImage_GetHeight(bitmap);
	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();
	return width * height > 4;
}

POINT ImageUtility::getImagePosition(const std::string& imageFullPath)
{
	POINT pos;
	std::string posFileName = StringUtility::getFilePath(imageFullPath) + "/" + StringUtility::getFileNameNoSuffix(imageFullPath) + ".txt";
	std::string posFile = FileUtility::openTxtFile(posFileName);
	txVector<int> posValue;
	StringUtility::stringToIntArray(posFile, posValue);
	if (posValue.size() == 2)
	{
		pos.x = posValue[0];
		pos.y = posValue[1];
	}
	else
	{
		std::cout << "λ���ļ����ݴ��� : " << imageFullPath << std::endl;
	}
	return pos;
}

void ImageUtility::collectMapTexture(const std::string& fileName)
{
	std::string file = StringUtility::getFileNameNoSuffix(fileName);
	SceneMap* map = TRACE_NEW(SceneMap, map);
	map->readFile(fileName + ".map");
	int tileCount = map->mHeader->mWidth * map->mHeader->mHeight;
	for (int i = 0; i < tileCount; ++i)
	{
		std::string srcFilePath = "../media/Objects" + StringUtility::intToString(map->mTileList[i].mObjFileIdx + 1) + "/";
		std::string srcFileName = StringUtility::intToString(map->mTileList[i].mObjImgIdx) + ".png";
		std::string destFilePath = "../media/MapTexture/" + file + "/";
		if (FileUtility::isFileExist(srcFilePath + srcFileName))
		{
			FileUtility::copyFile(srcFilePath + srcFileName, destFilePath + srcFileName);
			FileUtility::copyFile(srcFilePath + srcFileName + ".meta", destFilePath + srcFileName + ".meta");
		}
	}
	TRACE_DELETE(map);
}

void ImageUtility::groupAtlas(const std::string& filePath)
{
	std::string atlasInfo;
	txSerializer serializer;
	const int TEXTURE_COUNT_PER_ATLAS = 100;
	txVector<std::string> fileList;
	FileUtility::findFiles(filePath, fileList, ".png", false);
	sortByFileNumber(fileList);
	int count = fileList.size();
	for (int i = 0; i < count; ++i)
	{
		int atlasIndex = i / TEXTURE_COUNT_PER_ATLAS;
		std::string curFile = StringUtility::getFileNameNoSuffix(fileList[i]);
		std::string folderName = StringUtility::intToString(atlasIndex);
		std::string newPath = StringUtility::getFilePath(fileList[i]) + "/" + folderName + "/";
		FileUtility::copyFile(fileList[i], newPath + curFile + ".png");
		FileUtility::copyFile(fileList[i] + ".meta", newPath + curFile + ".png.meta");
		// ÿһ����Ҫ��¼ͼƬ��ͼ���е�λ��
		atlasInfo += StringUtility::getFileName(fileList[i]) + ":" + StringUtility::intToString(atlasIndex) + "\n";
		serializer.write<short>(StringUtility::stringToInt(StringUtility::getFileNameNoSuffix(fileList[i])));
		serializer.write<unsigned char>(atlasIndex);
	}
	FileUtility::writeFile(filePath + "/atlas.txt", atlasInfo);
	FileUtility::writeFile(filePath + "/atlas.index", serializer.getBuffer(), serializer.getDataSize());
}

void ImageUtility::texturePacker(const std::string& texturePath)
{
	std::string outputFileName = StringUtility::getFileName(texturePath);
	std::string outputPath = StringUtility::getFilePath(texturePath);
	std::string cmdLine;
	cmdLine += "--data " + outputPath + "/" + outputFileName + ".txt ";
	cmdLine += "--sheet " + outputPath + "/" + outputFileName + ".png ";
	cmdLine += "--format json ";
	cmdLine += "--allow-free-size ";
	cmdLine += "--maxrects-heuristics Best ";
	cmdLine += "--trim-mode None ";
	cmdLine += "--disable-rotation ";
	cmdLine += "--size-constraints AnySize ";
	cmdLine += "--max-size 2048 ";
	cmdLine += "--padding 1 ";
	cmdLine += texturePath;

	SHELLEXECUTEINFOA ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = "C:\\Program Files\\CodeAndWeb\\TexturePacker\\bin\\TexturePacker.exe";
	ShExecInfo.lpParameters = cmdLine.c_str();
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	BOOL ret = ShellExecuteExA(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
}

void ImageUtility::texturePackerAll(const std::string& texturePath)
{
	txVector<std::string> folderList;
	FileUtility::findFolders(texturePath, folderList);
	int count = folderList.size();
	for (int i = 0; i < count; ++i)
	{
		texturePacker(folderList[i]);
	}
}