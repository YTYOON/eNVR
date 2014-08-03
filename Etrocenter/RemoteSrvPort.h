#pragma once
#include "resource.h"


// CRemoteSrvPort dialog

class CRemoteSrvPort : public CDialog
{
	DECLARE_DYNAMIC(CRemoteSrvPort)

public:
	CRemoteSrvPort(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRemoteSrvPort();

// Dialog Data
	enum { IDD = IDD_SETTING_REMOTE_SRV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioCustom();
	afx_msg void OnBnClickedRadio80();
	afx_msg void OnBnClickedRadio8080();
	afx_msg void OnBnClickedRadio8081();
	afx_msg void OnBnClickedRadio8082();
	afx_msg void OnBnClickedRadio8083();
	void vSetPort2Reg();
	static UINT iRestartRemoteSrv (LPVOID lpParam);
	static DWORD GetRemoteSrvID();
	void vSetRTSPPort2Reg();

	int m_nHttpPort;
	int m_nRTSPPort;
};
