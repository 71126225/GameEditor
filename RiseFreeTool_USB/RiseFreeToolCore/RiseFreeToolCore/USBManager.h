#ifndef _USB_MANAGER_H_
#define _USB_MANAGER_H_

#define LIB_USB

#include "EditorCoreBase.h"
#include "CommonDefine.h"
#include "txSerializer.h"
#include "txThreadLock.h"
#ifdef LIB_USB
#include "libusb.h"
#else
extern "C" 
{
#include "hidsdi.h"
#include <setupapi.h>
}
#endif
#include "CommandReceiver.h"

class USBManager : public CommandReceiver, public EditorCoreBase
{
public:
	USBManager();
	virtual ~USBManager(){ destroy(); }
	void init();
	void destroy();
	void update(const float& elapsedTime);
	void setFriction(const int& friction);
	void notifyDeviceChanged();
	void openDevice();
	void closeDevice();
protected:
	static DWORD WINAPI writeThread(LPVOID lpParameter);
	static DWORD WINAPI readThread(LPVOID lpParameter);
	static DWORD WINAPI praseDataThread(LPVOID lpParameter);
	static DWORD WINAPI openDeviceThread(LPVOID lpParameter);
	void resizeBuffer(const int& size);
	void addDataToBuffer(unsigned char* data, const int& dataCount);
	void removeDataFromBuffer(const int& start, const int& count);
	void clearBuffer();
	// ���ָ���豸�Ƿ����
	bool checkDevice();
protected:
	HANDLE mWriteThread;
	HANDLE mReadThread;
	HANDLE mParseDataThread;
	HANDLE mOpenDeviceThread;
	txThreadLock mReadBufferLock;
	txThreadLock mWriteBufferLock;
	unsigned char* mReadBuffer;			// ���յ������ݵĻ�����
	std::atomic<int> mDataLength;		// ��������ʣ����յ������ݳ���
	std::atomic<int> mBufferSize;		// �������ܴ�С
	txVector<WriteData*> mWriteDataBuffer;	// д�����ݵĻ����б�,�����������
	txVector<WriteData*> mWriteDataList;	// ��Ҫд��������б�,����д������
#ifdef LIB_USB
	libusb_context* mContext;
	libusb_device_handle* mDeviceHandle;
	int mEndPointIN;
	int mEndPointOUT;
#else
	HANDLE mDeviceHandle;
#endif
	unsigned short VID;
	unsigned short PID;
	float mSearchInterval;	// �豸δ��ʱ,�����豸��ʱ����
	float mCurSearchTime;
};

#endif