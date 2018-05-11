#include "LibCurlDownloadManager.h"
#include "EditorCoreLog.h"
#include "Utility.h"
#include "EditorCore.h"

void LibCurlDownloadManager::init()
{
	char name[64];
	GetModuleFileNameA(NULL, name, 64);
	mLocalPath = StringUtility::getParentDir(name);
	mDownloadFinish = true;
	mUploadFinish = true;
	mRunning = true;
	curl_global_init(CURL_GLOBAL_ALL);
	mDownloadingThread = CreateThread(NULL, 0, LibCurlDownloadManager::downloadingThread, this, 0, NULL);
	mUploadingThread = CreateThread(NULL, 0, LibCurlDownloadManager::uploadingThread, this, 0, NULL);
}

void LibCurlDownloadManager::destroy()
{
	int count = mDownloadList.size();
	for (int i = 0; i < count; ++i)
	{
		TRACE_DELETE(mDownloadList[i]);
	}
	mDownloadList.clear();
	mRunning = false;
	while (!mDownloadFinish){}
	while (!mUploadFinish){}
	curl_global_cleanup();
}

void LibCurlDownloadManager::upload(const std::string& fullURL, const std::string& fileName, const std::string& uploadPath, ProgressCallback progress,
	DownloadingCallback downloading, StartCallback start, FinishCallback finish, void* userData)
{
	LOCK(mUploadListLock);
	DownloadInfo* info = TRACE_NEW(DownloadInfo, info);
	info->mURL = StringUtility::strReplaceAll(fullURL, " ", "%20");
	info->mFileName = fileName;
	info->mUploadPath = uploadPath;
	info->mProgressCallback = progress;
	info->mDownloadingCallback = downloading;
	info->mStartCallback = start;
	info->mFinishCallback = finish;
	info->mUserData = userData;
	mUploadList.push_back(info);
	UNLOCK(mUploadListLock);
}

void LibCurlDownloadManager::download(const std::string& fullURL, const std::string& fileName, ProgressCallback progress,
	DownloadingCallback downloading, StartCallback start, FinishCallback finish, void* userData)
{
	LOCK(mDownloadListLock);
	DownloadInfo* info = TRACE_NEW(DownloadInfo, info);
	// ���ص�ַ����Ҫ���ո��滻Ϊ%20
	info->mURL = StringUtility::strReplaceAll(fullURL, " ", "%20");
	info->mFileName = fileName;
	info->mProgressCallback = progress;
	info->mDownloadingCallback = downloading;
	info->mStartCallback = start;
	info->mFinishCallback = finish;
	info->mUserData = userData;
	mDownloadList.push_back(info);
	UNLOCK(mDownloadListLock);
}

double LibCurlDownloadManager::getDownloadFileLenth(const std::string& url)
{
	double len = 0.0;
	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_NOBODY, 1);			// ������body
	int ret = curl_easy_perform(curl);
	int infoRet = 0;
	if (ret == CURLE_OK)
	{
		infoRet = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &len);
		if (infoRet != CURLE_OK)
		{
			EDITOR_CORE_INFO_DELAY("curl_easy_getinfo failed!\n");
		}
	}
	else
	{
		len = -1;
	}
	return len;
}

DWORD WINAPI LibCurlDownloadManager::downloadingThread(LPVOID lpParameter)
{
	LibCurlDownloadManager* downloadManager = (LibCurlDownloadManager*)(lpParameter);
	downloadManager->mDownloadFinish = false;
	while (downloadManager->mRunning)
	{
		LOCK(downloadManager->mDownloadListLock);
		if (downloadManager->mDownloadList.size() == 0)
		{
			UNLOCK(downloadManager->mDownloadListLock);
			continue;
		}
		DownloadInfo* info = downloadManager->mDownloadList[0];
		downloadManager->mDownloadList.erase(downloadManager->mDownloadList.begin());
		UNLOCK(downloadManager->mDownloadListLock);
		if (info->mStartCallback != NULL)
		{
			// �ļ�̫��ʱ���ȡʧ��,̫��ʱ,����ȡ��������ǰ��ȡ�ļ���С
			//int length = (int)downloadManager->getDownloadFileLenth(info->mURL.c_str());
			int length = -1;
			info->mStartCallback(info->mUserData, info->mFileName, length);
		}
		CURL* curl = curl_easy_init();
		if (curl == NULL)
		{
			continue;
		}
		curl_easy_setopt(curl, CURLOPT_URL, StringUtility::ANSIToUTF8(info->mURL).c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, info->mDownloadingCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, info->mUserData);
		curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, info->mProgressCallback);
		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, info->mUserData);
		// ��ʼִ��perform�����ļ�
		CURLcode retCode = curl_easy_perform(curl);
		std::string strError = curl_easy_strerror(retCode);
		curl_easy_cleanup(curl);
		if (retCode != CURLE_OK)
		{
			if (retCode == CURLE_ABORTED_BY_CALLBACK)
			{
				// ȡ������
				downloadManager->mRunning = false;
			}
			else if (retCode == CURLE_COULDNT_CONNECT)
			{
				EDITOR_CORE_INFO_DELAY("�޷�������������!");
				mEditorCore->sendDelayEvent(CET_STOP_PROGRAM);
				downloadManager->mRunning = false;
			}
			else
			{
				EDITOR_CORE_INFO_DELAY("�����쳣,������:%d", retCode);
				//EDITOR_CORE_INFO_DELAY("����ʧ�� : %d, %s", retCode, strError.c_str());
				mEditorCore->sendDelayEvent(CET_STOP_PROGRAM);
				downloadManager->mRunning = false;
			}
		}
		//����curl����ǰ��ĳ�ʼ��ƥ��  
		if (info->mFinishCallback != NULL)
		{
			info->mFinishCallback(info->mUserData, info->mFileName, retCode == CURLE_OK);
		}
		TRACE_DELETE(info);
	}
	downloadManager->mDownloadFinish = true;
	return 0;
}

int LibCurlDownloadManager::debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr)
{
	std::string str = data;
	str += "\n";
	OutputDebugStringA(str.c_str());
	return 0;
}

DWORD WINAPI LibCurlDownloadManager::uploadingThread(LPVOID lpParameter)
{
	LibCurlDownloadManager* downloadManager = (LibCurlDownloadManager*)(lpParameter);
	downloadManager->mUploadFinish = false;
	while (downloadManager->mRunning)
	{
		LOCK(downloadManager->mUploadListLock);
		if (downloadManager->mUploadList.size() == 0)
		{
			UNLOCK(downloadManager->mUploadListLock);
			continue;
		}

		DownloadInfo* info = downloadManager->mUploadList[0];
		downloadManager->mUploadList.erase(downloadManager->mUploadList.begin());
		UNLOCK(downloadManager->mUploadListLock);
		if (info->mStartCallback != NULL)
		{
			info->mStartCallback(info->mUserData, info->mFileName, 0);
		}
		curl_httppost* formpost = NULL;
		curl_httppost* lastptr = NULL;
		// �ϴ��ļ���·���ֶ�
		// curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "path", CURLFORM_FILE, info->mUploadPath.c_str(), CURLFORM_END);
		// ��д�ļ��ϴ��ֶΡ���ʹ��libcurl���м������ݵ�����curl_easy_perform����ʱ�������ļ�����
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "sendfile", CURLFORM_FILE, info->mFileName.c_str(), CURLFORM_END);
		// ͬ�����ļ��ϴ��ֶ�
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "upfile", CURLFORM_FILE, info->mFileName.c_str(), CURLFORM_END);
		//
		// �ļ����ֶ�
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "filename", CURLFORM_END);
		// �ύ�ֶ� (��ʹ�������Ҫ)
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "submit", CURLFORM_COPYCONTENTS, "send", CURLFORM_END);

		//------------------------------------------------------------------------------------------------------------
		CURL* curl = curl_easy_init();
		if (curl == NULL)
		{
			continue;
		}
		// ƴ�Ӵ�·�����ϴ���ַ
		std::string mNewUrl = info->mURL +"?path=["+ info->mUploadPath + "]";
		curl_easy_setopt(curl, CURLOPT_HEADER, 0);
		curl_easy_setopt(curl, CURLOPT_URL, mNewUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
		//curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, debug_callback);	// ��ʱ��Ҫdebug ��Ϊ���ϴ��ϴ��ļ���ʱ�� ��Ӱ���ٶ�
		// ��ʼִ��perform�����ļ�
		CURLcode retCode = curl_easy_perform(curl);
		std::string strError = curl_easy_strerror(retCode);
		curl_easy_cleanup(curl);
		if (retCode != CURLE_OK)
		{
			if (retCode == CURLE_ABORTED_BY_CALLBACK)
			{
				// ȡ���ϴ�
				downloadManager->mRunning = false;
			}
			else if (retCode == CURLE_COULDNT_CONNECT)
			{
				EDITOR_CORE_INFO_DELAY("�޷�������������!");
				mEditorCore->sendDelayEvent(CET_STOP_PROGRAM);
				downloadManager->mRunning = false;
			}
			else
			{
				if (retCode == 23)
				{
					// ����23 �س� Failed writing received data to disk/application ��ʱ���� ��Ӱ���ļ����������ϴ�
				}
				else
				{
					EDITOR_CORE_INFO_DELAY("�����쳣,������:%d", retCode);
					//EDITOR_CORE_INFO_DELAY("�ϴ�ʧ�� : %d, %s", retCode, strError.c_str());
					mEditorCore->sendDelayEvent(CET_STOP_PROGRAM);
					downloadManager->mRunning = false;
				}
			}
		}
		//����curl����ǰ��ĳ�ʼ��ƥ��  
		if (info->mFinishCallback != NULL)
		{
			info->mFinishCallback(info->mUserData, info->mFileName, retCode == CURLE_OK);
		}
		curl_formfree(formpost);
		TRACE_DELETE(info);
	}
	downloadManager->mUploadFinish = true;
	return 0;
}