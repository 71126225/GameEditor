#include "txRegisterTool.h"
#include "txSystemInfo.h"
#include "md5/md5.h"

// ����������
std::string txRegisterTool::generateRequestCode()
{
	while (!mSystemInfo->isDone()){}
	const std::string& oriMAC = mSystemInfo->getOriMAC();
	const std::string& hdd = mSystemInfo->getHDD();
	const std::string& mainbord = mSystemInfo->getMainbord();
	const std::string& cpu = mSystemInfo->getCPU();
	const std::string& bios = mSystemInfo->getBIOS();
	const std::string& mainbordType = mSystemInfo->getMainbordType();
	const std::string& curMAC = mSystemInfo->getCurMAC();
	std::string systemInfo = oriMAC + hdd + mainbord + cpu + bios + mainbordType + curMAC;
	MD5 md5(systemInfo);
	// ����MD5ֵ
	return md5.md5();
}

// ����ע����
std::string txRegisterTool::generateRegisteCode(const std::string& requestCode, const std::string& encodeKey)
{
	// �ٴμ���MD5
	MD5 md5(requestCode);
	std::string retStr = md5.md5();
	// Ȼ���ټ���
	encode(retStr, encodeKey);
	return retStr;
}

void txRegisterTool::encode(std::string& str, const std::string& encodeKey)
{
	static std::string REGISTER_CODE = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUBWXYZ";
	static int CODE_LEN = REGISTER_CODE.length();
	int strLen = str.length();
	int encodeLen = encodeKey.length();
	for (int i = 0; i < strLen; ++i)
	{
		str[i] ^= encodeKey[i % encodeLen];
		str[i] = REGISTER_CODE[std::abs((str[i] + encodeKey[i % encodeLen]) + 0xff) % CODE_LEN];
	}
}