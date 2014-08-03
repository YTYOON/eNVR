#pragma once

#include "resource.h"
#include "afxwin.h"
#include "BaseDialog.h"
#include "NVSNetLib.h"
// CDlgPTZSetting dialog

struct PTZSettingInfo
{
	CString csEnable;
	CString csProtocol;
	CString csDeviceID;
	CString csBraudRate;
	CString	csDataBits;
	CString csParity;
	CString csStopBits;
	CString csPTZSpeed;
	CString csComm;
	vector< map <int,pair_INT> > *pvctmapPatrolSeqDurSet;
};

struct PTZAuthInfo
{
	TCHAR  csIP[32];
	int  nPort;
	TCHAR  csCameraName[256];
	TCHAR  csUser[32];
	TCHAR  csPass[32];

};
class CDlgPTZSetting : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgPTZSetting)

public:
	CDlgPTZSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPTZSetting();

// Dialog Data
	enum { IDD = IDD_DLG_PTZ_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	PTZSettingInfo* m_pInfo;
	DECLARE_MESSAGE_MAP()

public:
	void SetPtzInfo(PTZSettingInfo* ptzSettingInfo) {m_pInfo = ptzSettingInfo;};
	void GetPtzSpeed(int nPTZSpeed){m_nPTZSpeed = nPTZSpeed;};
	CComboBox m_combobox_protocol;
	CEdit m_edit_deviceid;
	CComboBox m_combobox_baudRate;
	CComboBox m_combobox_data_bits;
	CComboBox m_combobox_parity;
	CComboBox m_combobox_stop_bits;
	CComboBox m_combobox_ptzspeed;

	NODEITEM * m_pNode;
	///////Larry add 102-11-11///////
	map <int,pair_INT> m_mapPatrolSeqDuration;

	//int		m_nPatrolSeqDurIdx;
	int		FindFreePatrolSeqDurIdx();

	int		m_nPatrolSeqCount;
	///////////////////////////////////

	CString m_IP;
	CString m_Port;
	int m_nPTZSpeed;
	PTZAuthInfo m_ptzAuthInfo;

	afx_msg void OnCbnSelchangeComboPtzSettingProtocol();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	
	void vGetPTZSettingInfo();
	void vGetRS485Info();
	void vDisplayRS485Info();
	void vParseSegment(TCHAR* tszBuffer);
	void vParseSegmentNew(TCHAR* tszBuffer);
	void vGetNODEITEM(NODEITEM *pNode);
	//void vGetNODEITEM(CString csIP,CString csPort,CString csCameraName);
	void vGetNODEITEM(PTZAuthInfo authinfo);
	CEdit m_csCameraName;
	CString m_csCameraNameString;	
	afx_msg void OnBnClickedBtnSetPatrol();
	CString m_csPatrolPointSet;

	CEdit m_editPatrolDuration;
	CStatic m_groupbox_patrol_seq;
	void vSetPatrolSeqText(void);
	afx_msg void OnBnClickedBtnResetPatrol();
	afx_msg void OnBnClickedCancel();
};
