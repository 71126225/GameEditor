#ifndef _TX_REGISTER_TOOL_H_
#define _TX_REGISTER_TOOL_H_

#include "txEngineDefine.h"
#include "txEngineBase.h"

class txRegisterTool : public txEngineBase
{
public:
	txRegisterTool(){}
	virtual ~txRegisterTool(){ destroy(); }
	void init();
	void destroy(){}
	// ����������
	std::string generateRequestCode();
	// ����ע����
	std::string generateRegisteCode(const std::string& requestCode, const std::string& encodeKey);
protected:
	void encode(std::string& str, const std::string& encodeKey);
protected:
};

#endif