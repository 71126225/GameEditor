#ifndef _WIRELESS_DEVICE_PANEL_H_
#define _WIRELESS_DEVICE_PANEL_H_

#include "EditorPanel.h"
#include "CommonDefine.h"

class HeartRateData;
class WirelessDevicePanel : public EditorPanel
{
public:
	WirelessDevicePanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name);
	~WirelessDevicePanel(){}
	void init();
	void update(float elapsedTime);
	DECLARE_EVENT_TABLE()
	void OnListCkeck0(wxCommandEvent& event);
	void OnListCkeck1(wxCommandEvent& event);
	void OnListCkeck2(wxCommandEvent& event);
	void OnListCkeck3(wxCommandEvent& event);
protected:
	void setup();
protected:
	wxCheckBox* mListCheck0;
	wxCheckBox* mListCheck1;
	wxCheckBox* mListCheck2;
	wxCheckBox* mListCheck3;
	wxStaticText* mHeartRateLabel;
	wxStaticText* mRidingFrequencySpeedLabel;
	wxStaticText* mRidingFrequencyLabel;
	wxStaticText* mSpeedLabel;
	wxListCtrl* mHeartRateDataList;
	wxListCtrl* mRidingFrequencySpeedDataList;
	wxListCtrl* mRidingFrequencyDataList;
	wxListCtrl* mSpeedDataList;
	wxStaticText* mLastHeartBeat;	// �ϴν��յ���������ǰ��ʱ��
	wxStaticText* mHeartBeatBytes;	// ���յ�������������
	wxStaticText* mBattery;			// �ź�ǿ��
};

#endif