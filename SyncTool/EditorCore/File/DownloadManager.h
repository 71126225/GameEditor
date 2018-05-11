#ifndef _DOWNLOAD_MANAGER_H_
#define _DOWNLOAD_MANAGER_H_

#include "EditorCoreBase.h"
#include "CommonDefine.h"
#include "CommandReceiver.h"

const std::string VERSION = "Version";
const std::string FILELIST = "FileList";
const std::string TEMP_PATH = "Temp_Data/";

class FileInfo
{
public:
	std::string mFileName;
	std::string mMD5;
	int mFileSize;
	bool mBigFile;
};

enum UPGRADE_STATE
{
	US_NONE,
	US_DOWNLOADING_VERSION,
	US_WAIT_FOR_UPGRADE,
	US_DOWNLOADING_FILE_LIST,
	US_PARSING_REMOTE_FILE_LIST,
	US_GENERATE_LOCAL_FILE,
	US_GENERATE_MODIFIED_FILE,
	US_DOWNLOADING_REMOTE_FILE,
	US_UPDATE_LOCAL_FILE,
	US_DONE,
};

class File;
class DownloadManager : public EditorCoreBase, public CommandReceiver
{
public :
	DownloadManager()
		:
	CommandReceiver("DownloadManager")
	{
		mCurDownloading = NULL;
		mCancel = false;
		mState = US_NONE;
		mIgnorePathList.insert("dndl_Data/StreamingAssets/CustomSound/RaceSound");
		mCurTimeCount = -1.0f;
		mCurSpeed = 0.0f;
		mSpeedInSecond = 0.0f;
		mTotalSize = 0.0f;
		mDownloadedSize = 0.0f;
		mRemainTime = 0.0f;
		mDownloadTimeOut = 120.0f;
		mLastDownloadingTime = -1.0f;
	}
	virtual ~DownloadManager()
	{
		destroy();
	}
	void init();
	void destroy();
	void update(float elapsedTime);
	void requestDownloadVersion();
	File* getCurDownloadingFile() { return mCurDownloading; }
	int getTotalCount() { return mModifiedList.size(); }
	int getDownloadedCount() { return mDownloadedList.size(); }
	float getCurSpeed() { return mCurSpeed; }
	float getRemainTime() { return mRemainTime; }
	float getTotalSize(){ return mTotalSize; }
	bool getDownloading() { return mState == US_DOWNLOADING_REMOTE_FILE; }
	void setCancel(const bool& cancel);
	void startUpdateGame(const bool& start);
	static int downloading(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam);
	static int progressing(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
	static void onStart(void* userData, const std::string& fileName, const int& fileLength);
	static void onFinish(void* userData, const std::string& fileName, const bool& downloadSuccess);
protected:
	void notifyVersionDownloaded(File* versionFile);
	void notifyFileListDownloaded(File* listFile);
	void notifyAllDownloaded();	// �����ļ����Ѿ��������
	void parseRemoteFileList(const std::string& listContent);
	void getLocalFileList();
	void repuestDownload(const std::string& fileName);
	void done();
	static txMap<std::string, FileInfo*> generateUselessFile(txMap<std::string, FileInfo*>& remoteList, txMap<std::string, FileInfo*>& localList, txSet<std::string>& ignorePathList);
	static txMap<std::string, FileInfo*> generateModifiedFile(txMap<std::string, FileInfo*>& remoteList, txMap<std::string, FileInfo*>& localList, txSet<std::string>& ignorePathList);
	static bool isIgnoreFile(const std::string& file, txSet<std::string>& ignorePathList);
protected:
	txMap<std::string, File*> mDownloadingFileList;		// �������ػ��Ѿ�������ϵĸ����ļ�
	txMap<std::string, File*> mDownloadedList;			// �Ѿ�������ϵĸ����ļ�
	txMap<std::string, FileInfo*> mRemoteFileList;		// Զ���ļ��б�
	txMap<std::string, FileInfo*> mLocalFileList;		// �����ļ��б�
	txMap<std::string, FileInfo*> mModifiedList;		// ��Ҫ���µ��ļ��б�
	txSet<std::string> mIgnorePathList;
	std::string mRemoteVersion;
	std::string mLocalVersion;
	std::string mDataPath;
	File* mCurDownloading;
	volatile std::atomic<float> mCurTimeCount;		// ��ǰ��ʱ,С��0��ʾ��û�п�ʼ��ʱ,���ڵ���0��ʾ���ڼ�ʱ
	float mCurSpeed;
	float mRemainTime;								// ʣ������ʱ��
	float mDownloadTimeOut;
	volatile std::atomic<float> mTotalSize;			// ��ǰ��Ҫ���ص��ļ����ܴ�С
	volatile std::atomic<float> mDownloadedSize;	// �Ѿ����صĴ�С
	volatile std::atomic<float> mSpeedInSecond;
	volatile std::atomic<float> mLastDownloadingTime;// �ϴν��յ��������ݵ���ǰ��ʱ��,С��0��ʾδ��ʱ,���ڵ���0��ʾ���ڼ�ʱ,�ڵ�һ�ν��յ���������ʱ��ʼ��ʱ
	volatile std::atomic<UPGRADE_STATE> mState;
	volatile std::atomic<bool> mCancel;
};

#endif