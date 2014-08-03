#pragma once
#include "resource.h"
#include "DlgScanRemoteCam.h"

// CDlgAddRemoteNVR dialog


class CDlgAddRemoteNVR : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddRemoteNVR)

public:
	CDlgAddRemoteNVR(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAddRemoteNVR();

// Dialog Data
	enum { IDD = IDD_DLG_ADD_REMOTE_NVR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	CString m_csHost, m_csPort, m_csUser, m_csPassword;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
	RemoteNVRInfo	m_RemoteNVRInfo;
};
