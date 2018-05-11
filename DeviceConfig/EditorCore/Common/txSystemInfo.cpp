#include "txSystemInfo.h"
#include "txUtility.h"

void txSystemInfo::init()
{
	mDone = false;
	HANDLE handle = CreateThread(NULL, 0, deviceInfoThread, this, 0, NULL);
	CloseHandle(handle);
}

DWORD WINAPI txSystemInfo::deviceInfoThread(LPVOID lpParameter)
{
	txSystemInfo* systemInfo = static_cast<txSystemInfo*>(lpParameter);
	DEVICE_PROPERTY propOriMAC;
	if (systemInfo->WMI_DeviceQuery(DT_ORIGINAL_MAC, &propOriMAC, 1) > 0)
	{
		systemInfo->mOriMAC = propOriMAC.szProperty;
	}
	DEVICE_PROPERTY propHDD;
	if (systemInfo->WMI_DeviceQuery(DT_HDD, &propHDD, 1) > 0)
	{
		systemInfo->mHDD = propHDD.szProperty;
	}
	DEVICE_PROPERTY propMainbord;
	if (systemInfo->WMI_DeviceQuery(DT_MAINBORD, &propMainbord, 1) > 0)
	{
		systemInfo->mMainbord = propMainbord.szProperty;
	}
	DEVICE_PROPERTY propCPU;
	if (systemInfo->WMI_DeviceQuery(DT_CPU, &propCPU, 1) > 0)
	{
		systemInfo->mCPU = propCPU.szProperty;
	}
	DEVICE_PROPERTY propBIOS;
	if (systemInfo->WMI_DeviceQuery(DT_BIOS, &propBIOS, 1) > 0)
	{
		systemInfo->mBIOS = propBIOS.szProperty;
	}
	DEVICE_PROPERTY propMainbordType;
	if (systemInfo->WMI_DeviceQuery(DT_MAINBORD_TYPE, &propMainbordType, 1) > 0)
	{
		systemInfo->mMainbordType = propMainbordType.szProperty;
	}
	DEVICE_PROPERTY propCurMAC;
	if (systemInfo->WMI_DeviceQuery(DT_CUR_MAC, &propCurMAC, 1) > 0)
	{
		systemInfo->mCurMAC = propCurMAC.szProperty;
	}
	systemInfo->mDone = true;
	return 0;
}

BOOL txSystemInfo::WMI_DoWithPNPDeviceID(const char* PNPDeviceID, char* MacAddress, int uSize)
{
	char   DevicePath[MAX_PATH];
	BOOL    isOK = FALSE;

	// �����豸·����  
	StringCchCopyA(DevicePath, MAX_PATH, "\\\\.\\");
	StringCchCatA(DevicePath, MAX_PATH, PNPDeviceID);
	StringCchCatA(DevicePath, MAX_PATH, "#{ad498944-762f-11d0-8dcb-00c04fc3358c}");

	// ����PNPDeviceID���еġ�/���滻�ɡ�#�����Ի���������豸·����  
	std::replace(DevicePath + 4, DevicePath + 4 + strlen(PNPDeviceID), '\\', '#');

	// ��ȡ�豸���  
	HANDLE  hDeviceFile = CreateFileA(DevicePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if (hDeviceFile != INVALID_HANDLE_VALUE)
	{
		ULONG   dwID;
		BYTE    ucData[8];
		DWORD   dwByteRet;

		// ��ȡ����ԭ��MAC��ַ  
		dwID = OID_802_3_PERMANENT_ADDRESS;
		isOK = DeviceIoControl(hDeviceFile, IOCTL_NDIS_QUERY_GLOBAL_STATS, &dwID, sizeof(dwID), ucData, sizeof(ucData), &dwByteRet, NULL);
		if (isOK)
		{
			// ���ֽ�����ת����16�����ַ���  
			for (int i = 0; i < (int)dwByteRet; ++i)
			{
				StringCchPrintfA(MacAddress + (i << 1), uSize - (i << 1), "%02X", ucData[i]);
			}
			MacAddress[dwByteRet << 1] = '\0';  // д���ַ����������  
		}
		CloseHandle(hDeviceFile);
	}

	return isOK;
}

BOOL txSystemInfo::WMI_DoWithHarddiskSerialNumber(char* serialNumber, int uSize)
{
	int iLen = (int)strlen(serialNumber);
	if (iLen == 40)    // InterfaceType = "IDE"  
	{
		// ��Ҫ��16���Ʊ��봮ת��Ϊ�ַ���  
		char szBuf[32];
		bool ret = true;
		for (int i = 0; i < 20; ++i)
		{   // ��16�����ַ�ת��Ϊ��4λ  
			char ch = serialNumber[i * 2];
			unsigned char b;
			if ((ch >= '0') && (ch <= '9'))
			{
				b = ch - '0';
			}
			else if ((ch >= 'A') && (ch <= 'F'))
			{
				b = ch - 'A' + 10;
			}
			else if ((ch >= 'a') && (ch <= 'f'))
			{
				b = ch - 'a' + 10;
			}
			else
			{   // �Ƿ��ַ�
				ret = false;
				break;
			}

			b <<= 4;

			// ��16�����ַ�ת��Ϊ��4λ  
			ch = serialNumber[i * 2 + 1];
			if ((ch >= '0') && (ch <= '9'))
			{
				b += ch - '0';
			}
			else if ((ch >= 'A') && (ch <= 'F'))
			{
				b += ch - 'A' + 10;
			}
			else if ((ch >= 'a') && (ch <= 'f'))
			{
				b += ch - 'a' + 10;
			}
			else
			{   // �Ƿ��ַ�
				ret = false;
				break;
			}
			szBuf[i] = b;
		}

		if (ret)
		{
			// ת���ɹ�  
			szBuf[20] = '\0';
			StringCchCopyA(serialNumber, uSize, szBuf);
			iLen = strlen(serialNumber);
		}
	}

	// ÿ2���ַ�����λ��  
	for (int i = 0; i < iLen; i += 2)
	{
		if (i + 1 >= iLen)
		{
			break;
		}
		std::swap(serialNumber[i], serialNumber[i + 1]);
	}

	// ȥ���ո�  
	std::remove(serialNumber, serialNumber + strlen(serialNumber) + 1, ' ');
	return TRUE;
}

BOOL txSystemInfo::WMI_DoWithProperty(DEVIDE_TYPE iQueryType, char* szProperty, int uSize)
{
	BOOL isOK = TRUE;
	if (iQueryType == DT_ORIGINAL_MAC)
	{
		isOK = WMI_DoWithPNPDeviceID(szProperty, szProperty, uSize);
	}
	else if (iQueryType == DT_HDD)
	{
		isOK = WMI_DoWithHarddiskSerialNumber(szProperty, uSize);
	}
	else if (iQueryType == DT_CUR_MAC)
	{
		std::remove(szProperty, szProperty + strlen(szProperty) + 1, ':');
	}
	else
	{
		std::remove(szProperty, szProperty + strlen(szProperty) + 1, ' ');
	}
	return isOK;
}

// ����Windows Management Instrumentation��Windows����淶��  
int txSystemInfo::WMI_DeviceQuery(DEVIDE_TYPE iQueryType, DEVICE_PROPERTY* properties, int iSize)
{
	// �жϲ�ѯ�����Ƿ�֧��  
	if ((iQueryType < 0) || (iQueryType >= sizeof(szWQLQuery) / sizeof(WQL_QUERY)))
	{
		return -1;  // ��ѯ���Ͳ�֧��  
	}

	// ��ʼ��COM  
	HRESULT hres = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
		return -2;
	}

	// ����COM�İ�ȫ��֤����  
	hres = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	if (FAILED(hres) && hres != RPC_E_TOO_LATE)
	{
		CoUninitialize();
		return -2;
	}

	// ���WMI����COM�ӿ�  
	IWbemLocator* pLoc = NULL;
	hres = CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER, IID_IWbemLocator, reinterpret_cast<LPVOID*>(&pLoc));
	if (FAILED(hres))
	{
		CoUninitialize();
		return -2;
	}

	// ͨ�����ӽӿ�����WMI���ں˶�����"ROOT\\CIMV2"  
	IWbemServices* pSvc = NULL;
	hres = pLoc->ConnectServer(_bstr_t("ROOT\\CIMV2"), NULL, NULL, NULL, 0, NULL, NULL, &pSvc);
	if (FAILED(hres))
	{
		pLoc->Release();
		CoUninitialize();
		return -2;
	}

	// �����������İ�ȫ����  
	hres = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	if (FAILED(hres))
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return -2;
	}

	// ͨ�������������WMI��������  
	IEnumWbemClassObject *pEnumerator = NULL;
	hres = pSvc->ExecQuery(bstr_t("WQL"), bstr_t(szWQLQuery[iQueryType].szSelect), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);
	if (FAILED(hres))
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return -3;
	}
	INT iTotal = 0;
	// ѭ��ö�����еĽ������    
	while (pEnumerator)
	{
		if ((properties != NULL) && (iTotal >= iSize))
		{
			break;
		}
		IWbemClassObject *pclsObj = NULL;
		ULONG uReturn = 0;
		pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

		if (uReturn == 0)
		{
			break;
		}

		if (properties != NULL)
		{   // ��ȡ����ֵ  
			VARIANT vtProperty;

			VariantInit(&vtProperty);
			pclsObj->Get(szWQLQuery[iQueryType].szProperty, 0, &vtProperty, NULL, NULL);
			size_t len = wcslen(vtProperty.bstrVal) + 1;
			size_t converted = 0;
			char* cStr = TRACE_NEW_ARRAY(char, len, cStr);
			wcstombs_s(&converted, cStr, len, vtProperty.bstrVal, _TRUNCATE);
			StringCchCopyA(properties[iTotal].szProperty, DEV_PROP_LEN, cStr);
			TRACE_DELETE_ARRAY(cStr);
			VariantClear(&vtProperty);

			// ������ֵ����һ���Ĵ���  
			if (WMI_DoWithProperty(iQueryType, properties[iTotal].szProperty, DEV_PROP_LEN))
			{
				++iTotal;
			}
		}
		else
		{
			++iTotal;
		}

		pclsObj->Release();
	}

	// �ͷ���Դ  
	pEnumerator->Release();
	pSvc->Release();
	pLoc->Release();
	CoUninitialize();
	return iTotal;
}