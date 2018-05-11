#include "ComManager.h"
#include "EventSystem.h"
#include "RiseFreeToolCore.h"
#include "SpeedDataManager.h"

#define CLOSE_THREAD(thread)	\
if (thread != NULL)				\
{								\
	TerminateThread(thread, 0);	\
	thread = NULL;				\
}

#define CLOSE_HANDLE(handle)		\
if (handle != INVALID_HANDLE_VALUE)	\
{									\
	CloseHandle(handle);			\
	handle = INVALID_HANDLE_VALUE;	\
}

ComManager::ComManager()
{
	mCurDeviceHandle = INVALID_HANDLE_VALUE;
	mWriteThread = NULL;
	mReadThread = NULL;
	mParseDataThread = NULL;
	mReadBuffer = NULL;
	mDataLength = 0;
	mBufferSize = 0;
}

void ComManager::init()
{
	resizeBuffer(2048);
	mParseDataThread = CreateThread(NULL, 0, praseDataThread, this, 0, NULL);
}

void ComManager::destroy()
{
	closeCurDeviceHandle();
	CLOSE_THREAD(mWriteThread);
	CLOSE_THREAD(mReadThread);
	CLOSE_THREAD(mParseDataThread);
	TRACE_DELETE_ARRAY(mReadBuffer);
}

void ComManager::update(float elapsedTime)
{
	;
}

void ComManager::checkDevice()
{
	mDeviceList.clear();
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Hardware\\DeviceMap\\SerialComm", NULL, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		int nCount = 0;
		while (true)
		{
			char szPortName[256];
			char szComName[256];
			DWORD dwLong = 256;
			DWORD dwSize = 256;
			if (RegEnumValue(hKey, nCount, szPortName, &dwLong, NULL, NULL, (PUCHAR)szComName, &dwSize) == ERROR_NO_MORE_ITEMS)
			{
				break;
			}
			if (std::string(szComName) != "COM1")
			{
				mDeviceList.push_back(szComName);
			}
			++nCount;
		}
		RegCloseKey(hKey);
	}
	mRiseFreeToolCore->sendEvent(CET_DEVICE_LIST_CHANGED);
	// ֻҪ�豸�б仯�͹رյ�ǰ����
	setCurDevice("");
}

void ComManager::closeCurDeviceHandle()
{
	CLOSE_THREAD(mWriteThread);
	CLOSE_THREAD(mReadThread);
	CLOSE_HANDLE(mCurDeviceHandle);
	UNLOCK(mReadBufferLock);
	UNLOCK(mWriteBufferLock);
	mRiseFreeToolCore->sendEvent(CET_DEVICE_CLOSED);
}

void ComManager::setCurDevice(const std::string& device)
{
	mCurDevice = device;
	mRiseFreeToolCore->sendEvent(CET_CUR_DEVICE_CHANGED, mCurDevice);
	// ��ǰ����Ϊ��,�رմ���
	if (mCurDevice == "")
	{
		closeCurDeviceHandle();
		return;
	}
	// �����Ѿ���ʱ�����ٴδ�,�����ȹر�
	if (mCurDeviceHandle != INVALID_HANDLE_VALUE)
	{
		return;
	}
	// �򿪴��ڿ�ʼ��ȡ����
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = &sd;
	mCurDeviceHandle = CreateFile(mCurDevice.c_str(), GENERIC_READ | GENERIC_WRITE, 0, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_DEVICE, NULL);
	if (mCurDeviceHandle == INVALID_HANDLE_VALUE)
	{
		DWORD err = GetLastError();
		EDITOR_CORE_ERROR("error : device open failed, device name : %s, error code : %d", mCurDevice.c_str(), err);
	}
	SetupComm(mCurDeviceHandle, 1024, 1024);//���뻺����������������Ĵ�С����1024
	DCB dcb;
	GetCommState(mCurDeviceHandle, &dcb);
	dcb.BaudRate = 115200;		// ������Ϊ115200
	dcb.ByteSize = 8;			// ÿ���ֽ���8λ
	dcb.Parity = NOPARITY;		// ����żУ��λ
	dcb.StopBits = ONESTOPBIT;	// һ��ֹͣλ
	SetCommState(mCurDeviceHandle, &dcb);
	PurgeComm(mCurDeviceHandle, PURGE_TXCLEAR | PURGE_RXCLEAR);
	mRiseFreeToolCore->sendEvent(CET_DEVICE_OPENED);
	mWriteThread = CreateThread(NULL, 0, writeThread, this, 0, NULL);
	mReadThread = CreateThread(NULL, 0, readThread, this, 0, NULL);
}

void ComManager::setFriction(const int& friction)
{
	WriteData* writeData = TRACE_NEW(WriteData, writeData);
	writeData->mData = NULL;
	writeData->mDataCount = 0;
	mSpeedDataManager->generateFrictionData(friction, writeData->mData, writeData->mDataCount);
	LOCK(mWriteBufferLock);
	mWriteDataBuffer.push_back(writeData);
	UNLOCK(mWriteBufferLock);
}

DWORD WINAPI ComManager::writeThread(LPVOID lpParameter)
{
	ComManager* comManager = static_cast<ComManager*>(lpParameter);
	while (true)
	{
		Sleep(30);
		if (comManager->mCurDeviceHandle == INVALID_HANDLE_VALUE)
		{
			continue;
		}
		// ͬ��д�������б�
		LOCK(comManager->mWriteBufferLock);
		int count = comManager->mWriteDataBuffer.size();
		for (int i = 0; i < count; ++i)
		{
			comManager->mWriteDataList.push_back(comManager->mWriteDataBuffer[i]);
		}
		comManager->mWriteDataBuffer.clear();
		UNLOCK(comManager->mWriteBufferLock);
		if (comManager->mWriteDataList.size() == 0)
		{
			continue;
		}
		// д������
		txVector<WriteData*>::iterator iterData = comManager->mWriteDataList.begin();
		for (; iterData != comManager->mWriteDataList.end();)
		{
			DWORD writtenLen = 0;
			if (!WriteFile(comManager->mCurDeviceHandle, (*iterData)->mData, (*iterData)->mDataCount, &writtenLen, NULL))
			{
				DWORD err = GetLastError();
				EDITOR_CORE_ERROR_DELAY("error : write device failed! error : %d", err);
			}
			else
			{
				OutputDebugStringA((StringUtility::charArrayToHexString((unsigned char*)(*iterData)->mData, (*iterData)->mDataCount) + "\n").c_str());
			}
			TRACE_DELETE_ARRAY((*iterData)->mData);
			TRACE_DELETE(*iterData);
			iterData = comManager->mWriteDataList.erase(iterData);
		}
		// ��ʱ����ջ�����,��Ϊ��տ��ܵ����޷�д������,��д����������
		//PurgeComm(comManager->mCurDeviceHandle, PURGE_TXABORT | PURGE_TXCLEAR);
	}
}

DWORD ComManager::readThread(LPVOID lpParameter)
{
	ComManager* comManager = static_cast<ComManager*>(lpParameter);
	const DWORD bufferSize = 32;
	unsigned char dataBuffer[bufferSize];
	while (true)
	{
		Sleep(30);
		if (comManager->mCurDeviceHandle == INVALID_HANDLE_VALUE)
		{
			continue;
		}
		memset(dataBuffer, 0, bufferSize);
		ClearCommError(comManager->mCurDeviceHandle, NULL, NULL);
		DWORD readedCount = bufferSize;
		if (ReadFile(comManager->mCurDeviceHandle, (void*)dataBuffer, bufferSize, &readedCount, NULL))
		{
			// �����յ������ݷ��뻺����
			if (readedCount > 0)
			{
				LOCK(comManager->mReadBufferLock);
				comManager->addDataToBuffer(dataBuffer, readedCount);
				UNLOCK(comManager->mReadBufferLock);
				//OutputDebugStringA((StringUtility::charArrayToHexString(dataBuffer, readedCount) + "\n").c_str());
			}
		}
		PurgeComm(comManager->mCurDeviceHandle, PURGE_RXABORT | PURGE_RXCLEAR);
	}
}

DWORD ComManager::praseDataThread(LPVOID lpParameter)
{
	ComManager* comManager = static_cast<ComManager*>(lpParameter);
	while (true)
	{
		Sleep(30);
		if (comManager->mDataLength > 0)
		{
			LOCK(comManager->mReadBufferLock);
			int parsedCount = 0;
			PARSE_RESULT ret = mSpeedDataManager->setData(comManager->mCurDevice, comManager->mReadBuffer, comManager->mDataLength, parsedCount);
			// �����ɹ�,���ѽ����������Ƴ�
			if (ret == PR_SUCCESS)
			{
				comManager->removeDataFromBuffer(0, parsedCount);
			}
			// ����ʧ��,�򽫻��������
			else if (ret == PR_ERROR)
			{
				comManager->clearBuffer();
			}
			// ���ݲ���,�����ȴ���������
			else if (ret == PR_NOT_ENOUGH)
			{
				;
			}
			UNLOCK(comManager->mReadBufferLock);
		}
	}
	return 0;
}

void ComManager::resizeBuffer(int size)
{
	if (mBufferSize >= size)
	{
		return;
	}
	LOCK(mReadBufferLock);
	if (mReadBuffer != NULL)
	{
		// �����µĻ�����,��ԭ�������ݿ������»�������,����ԭ������,ָ���»�����
		unsigned char* newBuffer = TRACE_NEW_ARRAY(unsigned char, size, newBuffer);
		if (mDataLength > 0)
		{
			memcpy(newBuffer, mReadBuffer, mDataLength);
		}
		TRACE_DELETE_ARRAY(mReadBuffer);
		mReadBuffer = newBuffer;
		mBufferSize = size;
	}
	else
	{
		mReadBuffer = TRACE_NEW_ARRAY(unsigned char, size, mReadBuffer);
		mBufferSize = size;
	}
	UNLOCK(mReadBufferLock);
}

void ComManager::addDataToBuffer(unsigned char* data, int dataCount)
{
	// �����ǰ�Ѿ���Ų����µ�������,���ٴ���������
	if (mBufferSize - mDataLength < dataCount)
	{
		return;
	}
	memcpy(mReadBuffer + mDataLength, data, dataCount);
	mDataLength += dataCount;
}

void ComManager::removeDataFromBuffer(int start, int count)
{
	if (mDataLength < start + count)
	{
		return;
	}
	memmove(mReadBuffer + start, mReadBuffer + start + count, mDataLength - start - count);
	mDataLength -= count;
}

void ComManager::clearBuffer()
{
	mDataLength = 0;
}