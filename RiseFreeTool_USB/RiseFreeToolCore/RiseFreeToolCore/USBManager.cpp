#include "USBManager.h"
#include "EventSystem.h"
#include "RiseFreeToolCore.h"
#include "SpeedDataManager.h"
#include "CommandHeader.h"

USBManager::USBManager()
:
CommandReceiver(TOSTRING(USBManager))
{
	mWriteThread = NULL;
	mReadThread = NULL;
	mParseDataThread = NULL;
	mOpenDeviceThread = NULL;
	mReadBuffer = NULL;
	mDataLength = 0;
	mBufferSize = 0;
#ifdef LIB_USB
	mContext = NULL;
#endif
	mDeviceHandle = NULL;
	VID = 0x0483;
	PID = 0x5750;
	mSearchInterval = 0.5f;
	mCurSearchTime = -1.0f;
}

void USBManager::init()
{
#ifdef LIB_USB
	if (libusb_init(&mContext) < 0)
	{
		EDITOR_CORE_ERROR("init libusb failed!");
		return;
	}
	libusb_set_debug(mContext, 3);
#endif
	// ��USB�豸
	openDevice();
	resizeBuffer(2048);
	mWriteThread = CreateThread(NULL, 0, writeThread, this, 0, NULL);
	mReadThread = CreateThread(NULL, 0, readThread, this, 0, NULL);
	mParseDataThread = CreateThread(NULL, 0, praseDataThread, this, 0, NULL);
	mOpenDeviceThread = CreateThread(NULL, 0, openDeviceThread, this, 0, NULL);
	mCurSearchTime = 0.0f;
}

void USBManager::destroy()
{
	closeDevice();
	CLOSE_THREAD(mWriteThread);
	CLOSE_THREAD(mReadThread);
	CLOSE_THREAD(mParseDataThread);
	TRACE_DELETE_ARRAY(mReadBuffer);
#ifdef LIB_USB
	libusb_exit(NULL);
#endif
}

void USBManager::update(const float& elapsedTime)
{
	;
}

void USBManager::closeDevice()
{
	if (mDeviceHandle != NULL)
	{
#ifdef LIB_USB
		libusb_release_interface(mDeviceHandle, 0);
		libusb_close(mDeviceHandle);
#else
		CloseHandle(mDeviceHandle);
#endif
		mDeviceHandle = NULL;
	}
	UNLOCK(mReadBufferLock);
	UNLOCK(mWriteBufferLock);
	mRiseFreeToolCore->sendEvent(CET_DEVICE_CLOSED);
}

void USBManager::setFriction(const int& friction)
{
	WriteData* writeData = TRACE_NEW(WriteData, writeData);
	writeData->mData = NULL;
	writeData->mDataCount = 0;
	mSpeedDataManager->generateFrictionData(friction, writeData->mData, writeData->mDataCount);
	LOCK(mWriteBufferLock);
	mWriteDataBuffer.push_back(writeData);
	UNLOCK(mWriteBufferLock);
}

void USBManager::notifyDeviceChanged()
{
	// �����ǰ�Ѿ������豸,�����豸�Ѿ�������,��ر��豸
	if (mDeviceHandle != NULL)
	{
		if (!checkDevice())
		{
			closeDevice();
		}
	}
}

bool USBManager::checkDevice()
{
#ifdef LIB_USB
	libusb_device** devList = NULL;
	int count = libusb_get_device_list(NULL, &devList);
	if (count < 0)
	{
		EDITOR_CORE_ERROR("no usb dev on bus");
		return false;
	}
	bool exist = false;
	libusb_device_descriptor desc;
	for (int i = 0; i < count; ++i)
	{
		libusb_device* temp = devList[i];
		if (temp == NULL)
		{
			break;
		}
		int ret = libusb_get_device_descriptor(temp, &desc);
		if (ret < 0)
		{
			EDITOR_CORE_ERROR("failed to get device descriptor");
			break;
		}
		if (desc.idVendor == VID && desc.idProduct == PID)
		{
			exist = true;
			break;
		}
	}
	libusb_free_device_list(devList, 1);
	return exist;
#else
	bool exist = false;
	ULONG requiredLength = 0;
	SP_INTERFACE_DEVICE_DATA deviceInfoData;
	deviceInfoData.cbSize = sizeof(SP_INTERFACE_DEVICE_DATA);
	GUID hidGuid;
	HidD_GetHidGuid(&hidGuid);
	HDEVINFO hardwareDeviceInfo = SetupDiGetClassDevs(&hidGuid, NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
	for (int i = 0; i < 128; ++i)
	{
		if (!SetupDiEnumDeviceInterfaces(hardwareDeviceInfo, 0, &hidGuid, i, &deviceInfoData))
		{
			continue;
		}
		SetupDiGetDeviceInterfaceDetail(hardwareDeviceInfo, &deviceInfoData, NULL, 0, &requiredLength, NULL);
		ULONG predictedLength = requiredLength;
		PSP_INTERFACE_DEVICE_DETAIL_DATA functionClassDeviceData = (PSP_INTERFACE_DEVICE_DETAIL_DATA)malloc(predictedLength);
		if (!functionClassDeviceData)
		{
			continue;
		}
		functionClassDeviceData->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
		if (!SetupDiGetDeviceInterfaceDetail(hardwareDeviceInfo, &deviceInfoData, functionClassDeviceData, predictedLength, &requiredLength, NULL))
		{
			free(functionClassDeviceData);
			break;
		}
		HANDLE handle = CreateFile(functionClassDeviceData->DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (handle != INVALID_HANDLE_VALUE)
		{
			HIDD_ATTRIBUTES attri;
			HidD_GetAttributes(handle, &attri);
			exist = (attri.VendorID == VID && attri.ProductID == PID);
			CloseHandle(handle);
			handle = NULL;
		}
		free(functionClassDeviceData);
		if (exist)
		{
			break;
		}
	}
	SetupDiDestroyDeviceInfoList(hardwareDeviceInfo);
	if (!exist && mDeviceHandle != NULL)
	{
		CloseHandle(mDeviceHandle);
		mDeviceHandle = NULL;
	}
	return exist;
#endif
}

void USBManager::openDevice()
{
	if (mDeviceHandle != NULL)
	{
		return;
	}
#ifdef LIB_USB
	libusb_device** devList = NULL;
	int count = libusb_get_device_list(NULL, &devList);
	if (count < 0)
	{
		EDITOR_CORE_ERROR("no usb dev on bus");
		return;
	}
	libusb_device* dev = NULL;
	libusb_device_descriptor desc;
	for (int i = 0; i < count; ++i)
	{
		libusb_device* temp = devList[i];
		if (temp == NULL)
		{
			break;
		}
		int ret = libusb_get_device_descriptor(temp, &desc);
		if (ret < 0)
		{
			EDITOR_CORE_ERROR("failed to get device descriptor");
			break;
		}
		if (desc.idVendor == VID && desc.idProduct == PID)
		{
			dev = temp;
			break;
		}
	}
	do
	{
		if (dev == NULL)
		{
			break;
		}
		mDeviceHandle = libusb_open_device_with_vid_pid(mContext, VID, PID);
		if (mDeviceHandle == NULL)
		{
			//EDITOR_CORE_ERROR("can not open device!");
			break;
		}
		int ret = libusb_claim_interface(mDeviceHandle, 0);
		if (ret < 0)
		{
			EDITOR_CORE_ERROR("can't claim interface");
			break;
		}
		libusb_config_descriptor* configDescriptorIN = NULL;
		libusb_config_descriptor* configDescriptorOUT = NULL;
		for (int i = 0; i < desc.bNumConfigurations; ++i)
		{
			int r = 0;
			if (i == 0)
			{
				r = libusb_get_config_descriptor(dev, 0, &configDescriptorIN);
			}
			else if (i == 1)
			{
				r = libusb_get_config_descriptor(dev, 1, &configDescriptorOUT);
			}
			if (r != 0)
			{
				const char* str = libusb_strerror((libusb_error)r);
				EDITOR_CORE_ERROR("libusb error : %s", str);
			}
		}
		if (configDescriptorIN != NULL)
		{
			mEndPointIN = configDescriptorIN->interface->altsetting->endpoint->bEndpointAddress;
			libusb_free_config_descriptor(configDescriptorIN);
		}
		if (configDescriptorOUT != NULL)
		{
			mEndPointOUT = configDescriptorOUT->interface->altsetting->endpoint->bEndpointAddress;
			libusb_free_config_descriptor(configDescriptorOUT);
		}
	} while (false);
	libusb_free_device_list(devList, 1);
#else
	ULONG requiredLength = 0;
	SP_INTERFACE_DEVICE_DATA deviceInfoData;
	deviceInfoData.cbSize = sizeof(SP_INTERFACE_DEVICE_DATA);
	GUID hidGuid;
	HidD_GetHidGuid(&hidGuid);
	HDEVINFO hardwareDeviceInfo = SetupDiGetClassDevs(&hidGuid, NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
	for (int i = 0; i < 128; ++i)
	{
		if (!SetupDiEnumDeviceInterfaces(hardwareDeviceInfo, 0, &hidGuid, i, &deviceInfoData))
		{
			continue;
		}
		SetupDiGetDeviceInterfaceDetail(hardwareDeviceInfo, &deviceInfoData, NULL, 0, &requiredLength, NULL);
		ULONG predictedLength = requiredLength;
		PSP_INTERFACE_DEVICE_DETAIL_DATA functionClassDeviceData = (PSP_INTERFACE_DEVICE_DETAIL_DATA)malloc(predictedLength);
		if (!functionClassDeviceData)
		{
			continue;
		}
		functionClassDeviceData->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
		if (!SetupDiGetDeviceInterfaceDetail(hardwareDeviceInfo, &deviceInfoData, functionClassDeviceData, predictedLength, &requiredLength, NULL))
		{
			free(functionClassDeviceData);
			break;
		}
		mDeviceHandle = CreateFile(functionClassDeviceData->DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (mDeviceHandle != INVALID_HANDLE_VALUE)
		{
			HIDD_ATTRIBUTES attri;
			HidD_GetAttributes(mDeviceHandle, &attri);
			if (attri.VendorID == VID && attri.ProductID == PID)
			{
				break;
			}
			CloseHandle(mDeviceHandle);
			mDeviceHandle = NULL;
		}
		free(functionClassDeviceData);
	}
	SetupDiDestroyDeviceInfoList(hardwareDeviceInfo);
#endif
	mRiseFreeToolCore->sendEvent(CET_DEVICE_OPENED);
}

DWORD WINAPI USBManager::writeThread(LPVOID lpParameter)
{
	USBManager* usbManager = static_cast<USBManager*>(lpParameter);
	while (true)
	{
		Sleep(30);
		if (usbManager->mDeviceHandle == NULL)
		{
			continue;
		}
		// ͬ��д�������б�
		LOCK(usbManager->mWriteBufferLock);
		int count = usbManager->mWriteDataBuffer.size();
		for (int i = 0; i < count; ++i)
		{
			usbManager->mWriteDataList.push_back(usbManager->mWriteDataBuffer[i]);
		}
		usbManager->mWriteDataBuffer.clear();
		UNLOCK(usbManager->mWriteBufferLock);
		if (usbManager->mWriteDataList.size() == 0)
		{
			continue;
		}
		// д������
		auto iterData = usbManager->mWriteDataList.begin();
		for (; iterData != usbManager->mWriteDataList.end();)
		{
#ifdef LIB_USB
			int writtenLen = 0;
			if (usbManager->mEndPointOUT > 0)
			{
				libusb_interrupt_transfer(usbManager->mDeviceHandle, usbManager->mEndPointOUT, (*iterData)->mData, (*iterData)->mDataCount, &writtenLen, 0);
			}
#else
			DWORD writtenLen = 0;
			WriteFile(usbManager->mDeviceHandle, (*iterData)->mData, (*iterData)->mDataCount, &writtenLen, NULL);
#endif
			if (writtenLen > 0)
			{
				OutputDebugStringA((StringUtility::charArrayToHexString((unsigned char*)(*iterData)->mData, (*iterData)->mDataCount) + "\n").c_str());
			}
			TRACE_DELETE_ARRAY((*iterData)->mData);
			TRACE_DELETE(*iterData);
			iterData = usbManager->mWriteDataList.erase(iterData);
		}
		// ��ʱ����ջ�����,��Ϊ��տ��ܵ����޷�д������,��д����������
		PurgeComm(usbManager->mDeviceHandle, PURGE_TXABORT | PURGE_TXCLEAR);
	}
}

DWORD USBManager::readThread(LPVOID lpParameter)
{
	USBManager* usbManager = static_cast<USBManager*>(lpParameter);
	const int bufferLength = 32;
	unsigned char* dataBuffer = TRACE_NEW_ARRAY(unsigned char, bufferLength, dataBuffer);
	while (true)
	{
		Sleep(30);
		if (usbManager->mDeviceHandle == NULL)
		{
			continue;
		}
		memset(dataBuffer, 0, bufferLength);
		ClearCommError(usbManager->mDeviceHandle, NULL, NULL);
#ifdef LIB_USB
		int readCount = 0;
		libusb_interrupt_transfer(usbManager->mDeviceHandle, usbManager->mEndPointIN, dataBuffer, bufferLength, &readCount, 0);
#else
		DWORD readCount = 0;
		ReadFile(usbManager->mDeviceHandle, dataBuffer, bufferLength, &readCount, NULL);
#endif
		// �����յ������ݷ��뻺����
		if (readCount > 0)
		{
			// ������3���ֽ�,������Ҫ�Ƴ�
			// �յ��Ĳ���22���ֽ�,��������
#ifdef LIB_USB
			if (readCount == 22)
			{
#else
			if (readCount == 23)
			{
				// DDK�����������ݻ��һ���ֽ�,��Ҫ�Ƴ���һ���ֽ�
				memmove(dataBuffer, dataBuffer + 1, bufferLength - 1);
#endif
				readCount -= 3;
				LOCK(usbManager->mReadBufferLock);
				usbManager->addDataToBuffer(dataBuffer, readCount);
				UNLOCK(usbManager->mReadBufferLock);
				//OutputDebugStringA((StringUtility::charArrayToHexString(dataBuffer, readedCount) + "\n").c_str());
			}
			PurgeComm(usbManager->mDeviceHandle, PURGE_RXABORT | PURGE_RXCLEAR);
		}
	}
	TRACE_DELETE_ARRAY(dataBuffer);
}

DWORD USBManager::praseDataThread(LPVOID lpParameter)
{
	USBManager* usbManager = static_cast<USBManager*>(lpParameter);
	while (true)
	{
		Sleep(30);
		if (usbManager->mDataLength > 0)
		{
			LOCK(usbManager->mReadBufferLock);
			int parsedCount = 0;
			PARSE_RESULT ret = mSpeedDataManager->setData(usbManager->mReadBuffer, usbManager->mDataLength, parsedCount);
			// �����ɹ�,���ѽ����������Ƴ�
			if (ret == PR_SUCCESS)
			{
				usbManager->removeDataFromBuffer(0, parsedCount + 3);
			}
			// ����ʧ��,�򽫻��������
			else if (ret == PR_ERROR)
			{
				usbManager->clearBuffer();
			}
			// ���ݲ���,�����ȴ���������
			else if (ret == PR_NOT_ENOUGH)
			{
				;
			}
			UNLOCK(usbManager->mReadBufferLock);
		}
	}
	return 0;
}

DWORD USBManager::openDeviceThread(LPVOID lpParameter)
{
	USBManager* usbManager = static_cast<USBManager*>(lpParameter);
	while (true)
	{
		Sleep(30);
		static DWORD beginTime = timeGetTime();
		DWORD curTime = timeGetTime();
		float elapsedTime = (curTime - beginTime) / 1000.0f;
		beginTime = curTime;
		if (usbManager->mCurSearchTime >= 0.0f)
		{
			usbManager->mCurSearchTime += elapsedTime;
			if (usbManager->mCurSearchTime >= usbManager->mSearchInterval)
			{
				usbManager->mCurSearchTime = 0.0f;
				// �����ǰ�豸û�д�,����豸����,�ʹ��豸
				if (usbManager->mDeviceHandle == NULL)
				{
					if (usbManager->checkDevice())
					{
						usbManager->openDevice();
					}
				}
			}
		}
	}
}

void USBManager::resizeBuffer(const int& size)
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

void USBManager::addDataToBuffer(unsigned char* data, const int& dataCount)
{
	// �����ǰ�Ѿ���Ų����µ�������,���ٴ���������
	if (mBufferSize - mDataLength < dataCount)
	{
		return;
	}
	memcpy(mReadBuffer + mDataLength, data, dataCount);
	mDataLength += dataCount;
}

void USBManager::removeDataFromBuffer(const int& start, const int& count)
{
	if (mDataLength < start + count)
	{
		return;
	}
	memmove(mReadBuffer + start, mReadBuffer + start + count, mDataLength - start - count);
	mDataLength -= count;
}

void USBManager::clearBuffer()
{
	mDataLength = 0;
}