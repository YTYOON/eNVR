// DlgAddRemoteNVR.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAddRemoteNVR.h"

// CDlgAddRemoteNVR dialog

IMPLEMENT_DYNAMIC(CDlgAddRemoteNVR, CDialog)

CDlgAddRemoteNVR::CDlgAddRemoteNVR(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddRemoteNVR::IDD, pParent)
{

}

CDlgAddRemoteNVR::~CDlgAddRemoteNVR()
{
}

void CDlgAddRemoteNVR::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAddRemoteNVR, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgAddRemoteNVR::OnBnClickedOk)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CDlgAddRemoteNVR message handlers

void CDlgAddRemoteNVR::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_REMOTE_IP, m_csHost);
	GetDlgItemText(IDC_EDIT_REMOTE_PORT, m_csPort);
	GetDlgItemText(IDC_EDIT_REMOTE_ACCOUNT, m_csUser);
	GetDlgItemText(IDC_EDIT_REMOTE_PASSWORD, m_csPassword);

	CDlgScanRemoteCam* pCDlgScanRemoteCam = new CDlgScanRemoteCam();

	//m_RemoteNVRInfo.iHttpPort = ::_ttoi(m_csPort);
	pCDlgScanRemoteCam->m_RemoteNVRInfo.iHttpPort = ::_ttoi(m_csPort);
	
	::_tcscpy(pCDlgScanRemoteCam->m_RemoteNVRInfo.tcIPAddress, m_csHost.GetBuffer());

	::_tcscpy(pCDlgScanRemoteCam->m_RemoteNVRInfo.tcUser, m_csUser.GetBuffer());

	::_tcscpy(pCDlgScanRemoteCam->m_RemoteNVRInfo.tcPass, m_csPassword.GetBuffer());

	if( pCDlgScanRemoteCam->DoModal() == IDOK)
	{
		CNVRBk::GetBkObject()->SendMessage(MsgUpdateView, UM_REFRESH_DEVICETREE, 0);
	}

	OnOK();
}

int CDlgAddRemoteNVR::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	
	return 0;
}
