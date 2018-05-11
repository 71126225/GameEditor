#include "DownloadManager.h"
#include "File.h"
#include "Utility.h"
#include "EditorCore.h"
#include "LibCurlDownloadManager.h"
#include "CommandHeader.h"

#define CLEAR_MAP(className, list)\
{\
	txMap<std::string, className*>::iterator iter = list.begin();\
	txMap<std::string, className*>::iterator iterEnd = list.end();\
	for (; iter != iterEnd; ++iter)\
	{\
		TRACE_DELETE(iter->second); \
	}\
	list.clear();\
}

#define REMOVE_ELEMENT(list, element)\
{\
	txMap<std::string, FileInfo*>::iterator iter = list.find(element); \
	if (iter != list.end())\
	{\
		list.erase(iter);\
	}\
}\

void DownloadManager::destroy()
{
	// һ��Ҫ��ȡ������Ϊtrue,��������޷��رմ���,ȷ�����߳�
	while (mState != US_DONE && mState != US_NONE){}
	CLEAR_MAP(File, mDownloadingFileList);
	CLEAR_MAP(FileInfo, mRemoteFileList);
	CLEAR_MAP(FileInfo, mLocalFileList);
}

void DownloadManager::init()
{
	;
}

void DownloadManager::requestDownloadVersion()
{
	mState = US_DOWNLOADING_VERSION;
	// ��ʼ���ذ汾�ļ�
	repuestDownload(VERSION);
}

int DownloadManager::downloading(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam)
{
	DownloadManager* manager = (DownloadManager*)pParam;
	manager->mCurDownloading->writeFile((char*)pBuffer, nSize * nMemByte);
	return nSize * nMemByte;
}

int DownloadManager::progressing(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	DownloadManager* manager = (DownloadManager*)clientp;
	manager->mCurDownloading->setCurSize((int)dlnow);
	return 0;
}

void DownloadManager::onStart(void* userData, const std::string& fileName, const int& fileLength)
{
	DownloadManager* manager = (DownloadManager*)userData;
	txMap<std::string, File*>::iterator iter = manager->mDownloadingFileList.find(fileName);
	if (iter == manager->mDownloadingFileList.end())
	{
		return;
	}
	manager->mCurDownloading = iter->second;
	manager->mCurDownloading->startWrite(fileName);
	// ֪ͨ��ʼ���ذ汾�ļ�
	if (fileName == VERSION)
	{
		mEditorCore->sendDelayEvent(CET_DOWNLOAD_VERSION, StringUtility::boolToString(false));
	}
	// ֪ͨ��ʼ�����б��ļ�
	else if (fileName == FILELIST)
	{
		mEditorCore->sendDelayEvent(CET_DOWNLOAD_LIST_FILE, StringUtility::boolToString(false));
	}
	// �����ļ���С
	if (fileLength != -1)
	{
		manager->mCurDownloading->setTotalSize(fileLength);
	}
	else
	{
		if (manager->mRemoteFileList.find(fileName) != manager->mRemoteFileList.end())
		{
			manager->mCurDownloading->setTotalSize(manager->mRemoteFileList[fileName]->mFileSize);
		}
	}
}

void DownloadManager::onFinish(void* userData, const std::string& fileName, const bool& downloadSuccess)
{
	DownloadManager* manager = (DownloadManager*)userData;
	txMap<std::string, File*>::iterator iter = manager->mDownloadingFileList.find(fileName);
	if (iter == manager->mDownloadingFileList.end())
	{
		return;
	}
	// ���û�����سɹ�,���ټ���ִ�к��������߼�
	if (!downloadSuccess)
	{
		manager->mState = US_NONE;
		return;
	}
	File* file = iter->second;
	if (fileName == VERSION)
	{
		mEditorCore->sendDelayEvent(CET_DOWNLOAD_VERSION, StringUtility::boolToString(true));
		manager->notifyVersionDownloaded(file);
	}
	else if (fileName == FILELIST)
	{
		mEditorCore->sendDelayEvent(CET_DOWNLOAD_LIST_FILE, StringUtility::boolToString(true));
		manager->notifyFileListDownloaded(file);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------
void DownloadManager::notifyVersionDownloaded(File* versionFile)
{
	// �汾�ļ�������Ϻ�,��ʾ�Ƿ����°汾���Ը���
	int length = 0;
	char* buffer = NULL;
	bool hasNewVersion = false;
	if (!FileUtility::openFile(VERSION, length, buffer, true))
	{
		hasNewVersion = true;
	}
	else
	{
		std::string remoteVersion = StringUtility::bufferToString(versionFile->getFileData(), versionFile->getCurSize());
		hasNewVersion = remoteVersion != std::string(buffer);
	}
	if (hasNewVersion)
	{
		mState = US_DOWNLOADING_FILE_LIST;
		// ���ȴ�Զ�������б��ļ�
		repuestDownload(FILELIST);
	}
	else
	{
		mState = US_DONE;
		EDITOR_CORE_INFO_DELAY("�Ѿ������°汾!");
	}
}

void DownloadManager::notifyFileListDownloaded(File* listFile)
{
	// �б��ļ��������,�����б�
	mState = US_PARSING_REMOTE_FILE_LIST;
	std::string listContent = StringUtility::bufferToString(listFile->getFileData(), listFile->getCurSize());
	parseRemoteFileList(listContent);
	mState = US_GENERATE_LOCAL_FILE;
	// ��鱾���ļ�
	getLocalFileList();
	mState = US_GENERATE_MODIFIED_FILE;
	// Ȼ�������޸Ĺ����������ļ�
	mModifiedList = generateModifiedFile(mRemoteFileList, mLocalFileList, mIgnorePathList);
	mState = US_DONE;
	// ֪ͨ�����Ƿ����ļ��������δͬ��
	txVector<std::string> params;
	std::string allFile;
	txMap<std::string, FileInfo*>::iterator iter = mModifiedList.begin();
	txMap<std::string, FileInfo*>::iterator iterEnd = mModifiedList.end();
	for (; iter != iterEnd; ++iter)
	{
		allFile += iter->first;
		allFile += "%";
	}
	StringUtility::removeLast(allFile, '%');
	params.push_back(allFile);
	mEditorCore->sendDelayEvent(CET_FINISH_GENERATE_MODIFY_FILE, params);
}

void DownloadManager::parseRemoteFileList(const std::string& listContent)
{
	txVector<std::string> fileList;
	StringUtility::split(listContent, "\r\n", fileList);
	if (fileList.size() == 0)
	{
		EDITOR_CORE_ERROR_DELAY("�б��ļ�����!");
		return;
	}
	int readFileCount = StringUtility::stringToInt(fileList[0]);
	if (readFileCount == 0)
	{
		EDITOR_CORE_ERROR_DELAY("�ļ�������ȡ����!");
		return;
	}
	fileList.erase(fileList.begin());
	int fileCount = fileList.size();
	if (fileCount != readFileCount)
	{
		EDITOR_CORE_ERROR_DELAY("�ļ�������ƥ��, �ļ�ͷ��¼���� : %d, ʵ������ : %d", readFileCount, fileCount);
		return;
	}
	// ����Զ���ļ��б�
	for (int i = 0; i < fileCount; ++i)
	{
		txVector<std::string> contentList;
		StringUtility::split(fileList[i], "\t", contentList);
		if (contentList.size() != 3)
		{
			EDITOR_CORE_ERROR_DELAY("Զ���ļ��б�����!�к� : ", i + 1);
			return;
		}
		FileInfo* info = TRACE_NEW(FileInfo, info);
		info->mFileName = contentList[0];
		info->mFileSize = StringUtility::stringToInt(contentList[1]);
		info->mMD5 = contentList[2];
		mRemoteFileList.insert(info->mFileName, info);
	}
}

void DownloadManager::getLocalFileList()
{
	if (mLocalFileList.size() > 0)
	{
		EDITOR_CORE_ERROR_DELAY("local list is not empty!");
		return;
	}
	txVector<std::string> fileList;
	FileUtility::findFiles("./", fileList);
	int fileCount = fileList.size();
	for (int i = 0; i < fileCount; ++i)
	{
		FileInfo* info = TRACE_NEW(FileInfo, info);
		std::string newPath = fileList[i].substr(strlen("./"), fileList[i].length() - strlen("./"));
		mLocalFileList.insert(newPath, info);
	}
	// �ų��汾�ļ�,�б��ļ�,��������
	char thisFileName[256];
	GetModuleFileNameA(NULL, thisFileName, sizeof(thisFileName));
	std::string strThisFile = StringUtility::getFileName(thisFileName);
	REMOVE_ELEMENT(mLocalFileList, strThisFile);
	REMOVE_ELEMENT(mLocalFileList, VERSION);
	REMOVE_ELEMENT(mLocalFileList, FILELIST);

	mEditorCore->sendDelayEvent(CET_START_GENERATE_LOCAL_FILE, StringUtility::intToString(mLocalFileList.size()));

	// �����ļ���Ϣ
	const int MD5_BUFFER_SIZE = 1024 * 1024 * 4;
	char* md5Buffer = TRACE_NEW_ARRAY(char, MD5_BUFFER_SIZE, md5Buffer);
	txMap<std::string, FileInfo*>::iterator iter = mLocalFileList.begin();
	txMap<std::string, FileInfo*>::iterator iterEnd = mLocalFileList.end();
	for (int i = 0; iter != iterEnd; ++iter, ++i)
	{
		iter->second->mFileName = iter->first;
		iter->second->mFileSize = FileUtility::getFileSize(iter->first);
		iter->second->mMD5 = FileUtility::generateFileMD5(iter->first, md5Buffer, MD5_BUFFER_SIZE);
		txVector<std::string> params;
		params.push_back(StringUtility::intToString(mLocalFileList.size()));
		params.push_back(StringUtility::intToString(i + 1));
		mEditorCore->sendDelayEvent(CET_GENERATING_LOCAL_FILE, params);
	}
	TRACE_DELETE_ARRAY(md5Buffer);
}

void DownloadManager::repuestDownload(const std::string& fileName)
{
	File* file = TRACE_NEW(File, file);
	mDownloadingFileList.insert(fileName, file);
	mLibCurlDownloadManager->download(REMOTE_URL + fileName, fileName, progressing, downloading, onStart, onFinish, this);
}

txMap<std::string, FileInfo*> DownloadManager::generateModifiedFile(txMap<std::string, FileInfo*>& remoteList, txMap<std::string, FileInfo*>& localList, txSet<std::string>& ignorePathList)
{
	txMap<std::string, FileInfo*> modifiedList;
	txMap<std::string, FileInfo*>::iterator iterRemote = remoteList.begin();
	txMap<std::string, FileInfo*>::iterator iterRemoteEnd = remoteList.end();
	for (; iterRemote != iterRemoteEnd; ++iterRemote)
	{
		if (isIgnoreFile(iterRemote->first, ignorePathList))
		{
			continue;
		}
		bool isModify = false;
		txMap<std::string, FileInfo*>::iterator iterLocal = localList.find(iterRemote->first);
		// Զ���б���Ҳ�е�
		if (iterLocal != localList.end())
		{
			// �ļ���С��һ��,�����ļ���Сһ��,����md5�벻һ��,����Ǵ��ļ�,��Сһ��ʱ�Ϳ�����Ϊ���ļ�û���޸�
			if (iterRemote->second->mFileSize != iterLocal->second->mFileSize || iterRemote->second->mMD5 != iterLocal->second->mMD5)
			{
				isModify = true;
			}
		}
		// ����û�е�
		else
		{
			isModify = true;	
		}
		if (isModify)
		{
			modifiedList.insert(iterRemote->first, iterRemote->second);
		}
	}
	return modifiedList;
}

bool DownloadManager::isIgnoreFile(const std::string& file, txSet<std::string>& ignorePathList)
{
	txSet<std::string>::iterator iter = ignorePathList.begin();
	txSet<std::string>::iterator iterEnd = ignorePathList.end();
	for (; iter != iterEnd; ++iter)
	{
		if (StringUtility::startWith(file, *iter))
		{
			return true;
		}
	}
	return false;
}