#include "File.h"
#include "FileUtility.h"

void File::destroy()
{
	TRACE_DELETE(mFileData);
}

void File::startWrite(const std::string& fileName)
{
	mFileName = fileName;
	mFileData = TRACE_NEW(txSerializer, mFileData);
}

bool File::finishWrite()
{
	if (mIsBigFile)
	{
		FileUtility::writeFile(mFileName + mBigFileTempSuffix, mFileData->getDataSize(), mFileData->getBuffer(), true);
		TRACE_DELETE(mFileData);
		// ɾ��ԭ�ļ�,Ȼ����ʱ�ļ�����Ϊ�����ļ���
		FileUtility::deleteFile(mFileName);
		bool ret = FileUtility::renameFile(mFileName + mBigFileTempSuffix, mFileName);
		if (!ret)
		{
			return false;
		}
	}
	else
	{
		FileUtility::writeFile(mFileName, mFileData->getDataSize(), mFileData->getBuffer());
		TRACE_DELETE(mFileData);
	}
	return true;
}

void File::writeFile(char* data, int length)
{
	mFileData->writeBuffer(data, length);
	// ����Ǵ��ļ�,���������Ժ���Ҫд����ʱ�ļ�
	if (mIsBigFile && mFileData->getDataSize() >= BIG_FILE_SIZE)
	{
		FileUtility::writeFile(mFileName + mBigFileTempSuffix, mFileData->getDataSize(), mFileData->getBuffer(), true);
		TRACE_DELETE(mFileData);
		mFileData = TRACE_NEW(txSerializer, mFileData);
	}
}

char* File::getFileData()
{
	if (mFileData == NULL)
	{
		return NULL;
	}
	return mFileData->getBuffer();
}