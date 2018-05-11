#include "txUtility.h"

void txUtility::getAllProcess(txSet<std::string>& processList)
{
	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//���ϵͳ���վ�� 
	PROCESSENTRY32* info = TRACE_NEW(PROCESSENTRY32, info);
	info->dwSize = sizeof(PROCESSENTRY32);
	//����һ�� Process32First �������ӿ����л�ȡ�����б� 
	Process32First(handle, info);
	//�ظ����� Process32Next��ֱ���������� FALSE Ϊֹ 
	while (Process32Next(handle, info))
	{
		processList.insert(info->szExeFile);
	}
	TRACE_DELETE(info);
}

bool txUtility::launchExecutable(const std::string& path, const std::string& fullName)
{
	// ������Ϸ����
	SHELLEXECUTEINFO sei;
	memset(&sei, 0, sizeof(sei));
	sei.cbSize = sizeof(sei);
	sei.lpFile = fullName.c_str();
	sei.lpDirectory = path.c_str();
	sei.lpVerb = "open";
#ifndef __WXWINCE__
	sei.nShow = SW_SHOWNORMAL; // SW_SHOWDEFAULT not defined under CE (#10216)
#else
	sei.nShow = SW_SHOWDEFAULT;
#endif
	sei.fMask = SEE_MASK_FLAG_NO_UI;
	return ShellExecuteEx(&sei) == TRUE;
}

void txUtility::setRegistryValue(const std::string& path, const std::string& key, const std::string& value)
{
	HKEY hKey;
	LONG nError = RegOpenKeyExA(HKEY_CURRENT_USER, path.c_str(), NULL, KEY_ALL_ACCESS, &hKey);
	if (nError == ERROR_FILE_NOT_FOUND)
	{
		nError = RegCreateKeyExA(HKEY_CURRENT_USER, path.c_str(), NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	}
	if (nError != ERROR_SUCCESS)
	{
		return;
	}
	nError = RegSetValueExA(hKey, key.c_str(), NULL, REG_DWORD, (LPBYTE)(value.c_str()), value.length());
	if (nError != ERROR_SUCCESS)
	{
		return;
	}
}

std::string txUtility::getRegistryValue(const std::string& path, const std::string& key)
{
	HKEY hKey;
	LONG nError = RegOpenKeyExA(HKEY_CURRENT_USER, path.c_str(), NULL, KEY_ALL_ACCESS, &hKey);
	if (nError != ERROR_SUCCESS)
	{
		return "";
	}
	DWORD type = REG_SZ;
	int size = 1024;
	char value[1024];
	memset(value, 0, size);
	nError = RegQueryValueEx(hKey, key.c_str(), NULL, &type, (LPBYTE)(value), (LPDWORD)&(size));
	if (nError != ERROR_SUCCESS)
	{
		return "";
	}
	return std::string(value);
}

std::string txUtility::getDateTimeString()
{
	char date[256];
	SYSTEMTIME time;
	GetSystemTime(&time);
	SPRINTF(date, 256, "%d-%d-%d_%d-%d-%d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return date;
}