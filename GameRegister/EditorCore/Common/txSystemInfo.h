#ifndef _TX_SYSTEM_INFO_H_
#define _TX_SYSTEM_INFO_H_

#include "CommonDefine.h"
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment (lib, "comsuppw.lib")  
#pragma comment (lib, "wbemuuid.lib")  

enum DEVIDE_TYPE
{
	DT_ORIGINAL_MAC,	// ����ԭ��MAC��ַ
	DT_HDD,				// Ӳ�����к�
	DT_MAINBORD,		// �������к�
	DT_CPU,				// CPU ID
	DT_BIOS,			// BIOS���к�
	DT_MAINBORD_TYPE,	// �����ͺ�
	DT_CUR_MAC,			// ������ǰMAC��ַ
};

const int DEV_PROP_LEN = 128;
struct DEVICE_PROPERTY
{
	char szProperty[DEV_PROP_LEN];
};

struct WQL_QUERY
{
	char*   szSelect;       // SELECT���  
	wchar_t*  szProperty;     // �����ֶ�  
};

// WQL��ѯ���  
const WQL_QUERY szWQLQuery[] =
{
	// ����ԭ��MAC��ַ  
	"SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))", L"PNPDeviceID",
	// Ӳ�����к�  
	"SELECT * FROM Win32_DiskDrive WHERE (SerialNumber IS NOT NULL) AND (MediaType LIKE 'Fixed hard disk%')", L"SerialNumber",
	// �������к�  
	"SELECT * FROM Win32_BaseBoard WHERE (SerialNumber IS NOT NULL)", L"SerialNumber",
	// ������ID  
	"SELECT * FROM Win32_Processor WHERE (ProcessorId IS NOT NULL)", L"ProcessorId",
	// BIOS���к�  
	"SELECT * FROM Win32_BIOS WHERE (SerialNumber IS NOT NULL)", L"SerialNumber",
	// �����ͺ�  
	"SELECT * FROM Win32_BaseBoard WHERE (Product IS NOT NULL)", L"Product",
	// ������ǰMAC��ַ  
	"SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))", L"MACAddress",
};

class txSystemInfo
{
public:
	txSystemInfo(){}
	virtual ~txSystemInfo(){ destroy(); }
	void init();
	void destroy(){}
	bool isDone()							{ return mDone; }
	const std::string& getOriMAC()			{ return mOriMAC; }
	const std::string& getHDD()				{ return mHDD; }
	const std::string& getMainbord()		{ return mMainbord; }
	const std::string& getCPU()				{ return mCPU; }
	const std::string& getBIOS()			{ return mBIOS; }
	const std::string& getMainbordType()	{ return mMainbordType; }
	const std::string& getCurMAC()			{ return mCurMAC; }
protected:
	static DWORD WINAPI deviceInfoThread(LPVOID lpParameter);
	// ͨ����PNPDeviceID����ȡ����ԭ��MAC��ַ
	static BOOL WMI_DoWithPNPDeviceID(const char* PNPDeviceID, char* MacAddress, int uSize);
	// ����Ӳ�����к�
	static BOOL WMI_DoWithHarddiskSerialNumber(char* serialNumber, int uSize);
	// ����Ӳ������ֵ
	static BOOL WMI_DoWithProperty(DEVIDE_TYPE iQueryType, char* szProperty, int uSize);
	//-1����֧�ֵ��豸����ֵ
	//-2��WMI����ʧ��
	//-3������ȷ��WQL��ѯ���
	//>= 0����ȡ���豸����
	int WMI_DeviceQuery(DEVIDE_TYPE iQueryType, DEVICE_PROPERTY* properties, int iSize);
protected:
	std::string mOriMAC;
	std::string mHDD;
	std::string mMainbord;
	std::string mCPU;
	std::string mBIOS;
	std::string mMainbordType;
	std::string mCurMAC;
	volatile bool mDone;
};

#endif