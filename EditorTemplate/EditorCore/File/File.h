#ifndef _FILE_H_
#define _FILE_H_

#include "EditorCoreBase.h"
#include "CommonDefine.h"
#include "txSerializer.h"

class File : public EditorCoreBase
{
public :
	File()
	{
		mFileData = NULL;
		mTotalSize = 0;
		mCurSize = 0;
		mBigFileTempSuffix = ".temp";
		MAX_FILE_SIZE = 1024 * 1024 * 16;	// ���ڵ���16MB���ļ�Ϊ���ļ�
	}
	virtual ~File()
	{
		destroy();
	}
	void destroy();
	void startWrite(const std::string& fileName = "");
	bool finishWrite();
	void writeFile(char* data, int length);
	void setFileName(const std::string& fileName) { mFileName = fileName; }
	const std::string& getFileName() { return mFileName; }
	const int& getTotalSize() { return mTotalSize; }
	void setTotalSize(const int& size) 
	{
		if (size < 0)
		{
			return;
		}
		mTotalSize = size;
		if (mTotalSize >= MAX_FILE_SIZE)
		{
			mIsBigFile = true;
		}
	}
	const int& getCurSize() { return mCurSize; }
	void setCurSize(const int& size) { mCurSize = size; }
	char* getFileData();
protected:
	std::string mFileName;
	std::string mBigFileTempSuffix;
	txSerializer* mFileData;
	int mTotalSize;	// �ļ��ܴ�С
	int mCurSize;	// ��ǰ�����ش�С
	int MAX_FILE_SIZE;
	bool mIsBigFile;
};

#endif