#pragma once
#include "resource.h"
#include "DlgScanRemoteCam.h"
// CDlgAddISAP dialog

//#define ISAP 1
//#define REMOTE_ETROCENTER 2

class CDlgAddISAP : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddISAP)

public:
	CDlgAddISAP(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAddISAP();

// Dialog Data
	enum { IDD = IDD_DLG_ADD_ISAP };

protected:
    bool CheckisOnLocalLan(CString& csIp);
    DWORD dwIpStringToDword(CString& csIp);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	
	void vAddISAPGroup();
	void vAddISAPChannel();
public:
	int m_iResponseLength;
	int m_CurlChannel;
	int m_iISAPGroupID;
	int m_iGroupID;
	int m_iDBExitCams;
	int m_iTopCamera;
	
	BYTE byIPAddr[4];
	int m_iRemote;

	vector<int> veChannelLength;
	virtual BOOL OnInitDialog();
};
