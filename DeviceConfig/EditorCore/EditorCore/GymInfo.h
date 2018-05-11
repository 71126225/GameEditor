#ifndef _GYM_INFO_H_
#define _GYM_INFO_H_

#include "EditorCoreBase.h"
#include "txUtility.h"
#include "txCommandReceiver.h"

typedef void onRegisteCallback(int ret, void* userData);

class GymInfo : public EditorCoreBase, public txCommandReceiver
{
public:
	GymInfo();
	void init();
	// 0��ʾ�ɹ�,1��ʾ�޷����ӷ�����,2��ʾ���ִ���
	void queryRegiste();
	void registeDevice();
public:
	std::string mGymName;		// ��������
	std::string mDeviceCode;	// �豸��
	std::string mRegisteCode;	// ע����
	std::string mRegisteDate;	// ע������
	bool mRegisted;				// ��ǰ�Ƿ���ע��
};

#endif