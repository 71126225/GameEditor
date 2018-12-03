#ifndef _LIBCURL_PACKET_H_
#define _LIBCURL_PACKET_H_

#include "CommonDefine.h"
#include "ToolCoreBase.h"

class LibcurlPacket : public ToolCoreBase
{
public:
	LibcurlPacket(){}
	virtual ~LibcurlPacket(){}
	// dataֻ���������Ϣ������Ҫ����Ϣ
	virtual void read(char* data, const int& dataSize){}
	// data�ⲿnew�Ŀյ��ڴ�,��Ϣ����data��д����Ϣ����
	virtual void write(std::string& value) = 0;
	virtual void execute(const std::string& value) = 0;
	const LIBCURL_PACKET& getPacketType() { return mType; }
	const std::string& getURL() { return mURL; }
	const HTTP_METHOD& getMethod(){ return mMethod; }
	void setURL(const std::string& url) { mURL = url; }
	void setPacketType(const LIBCURL_PACKET& type) { mType = type; }
	void setMethod(const HTTP_METHOD& method) { mMethod = method; }
protected:
	LIBCURL_PACKET mType;
	std::string mURL;
	HTTP_METHOD mMethod;
};

#endif