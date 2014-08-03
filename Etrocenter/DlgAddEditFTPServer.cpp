// DlgAddEditFTPServer.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAddEditFTPServer.h"

// CDlgAddEditFTPServer dialog

IMPLEMENT_DYNAMIC(CDlgAddEditFTPServer, CBaseDialog)

BOOL CDlgAddEditFTPServer::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CBaseDialog::OnEraseBkgnd(pDC);
}

HBRUSH CDlgAddEditFTPServer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


CDlgAddEditFTPServer::CDlgAddEditFTPServer(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgAddEditFTPServer::IDD, pParent)
{
	m_FTPInfo.tcHost[0] = L'\0';
	m_FTPInfo.tcUser[0] = L'\0';
	m_FTPInfo.tcPassword[0] = L'\0';
}

CDlgAddEditFTPServer::~CDlgAddEditFTPServer()
{
}

void CDlgAddEditFTPServer::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAddEditFTPServer, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CDlgAddEditFTPServer::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAddEditFTPServer message handlers

BOOL CDlgAddEditFTPServer::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString csDialogCaption(_T(""));
	LOADSTRING(csDialogCaption, IDS_DLG_ADD_EDIT_FTP) 
	//csDialogCaption.LoadString(CGS::g_hResStr, IDS_DLG_ADD_EDIT_FTP);
	this->SetWindowTextW(csDialogCaption);

	((CEdit*)GetDlgItem(IDC_EDIT_HOST))->SetWindowTextW( (LPCTSTR)m_FTPInfo.tcHost );
	((CEdit*)GetDlgItem(IDC_EDIT_USER))->SetWindowTextW( (LPCTSTR)m_FTPInfo.tcUser );
	((CEdit*)GetDlgItem(IDC_EDIT_PASSWORD))->SetWindowTextW( (LPCTSTR)m_FTPInfo.tcPassword );
	((CEdit*)GetDlgItem(IDC_EDIT_RETYPE_PWD))->SetWindowTextW( (LPCTSTR)m_FTPInfo.tcPassword );

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAddEditFTPServer::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString csFTPHost, csFTPUser, csFTPPassword, csRetypePassword;

	((CEdit*)GetDlgItem(IDC_EDIT_HOST))->GetWindowTextW(csFTPHost);
	((CEdit*)GetDlgItem(IDC_EDIT_USER))->GetWindowTextW(csFTPUser);
	((CEdit*)GetDlgItem(IDC_EDIT_PASSWORD))->GetWindowTextW(csFTPPassword);
	((CEdit*)GetDlgItem(IDC_EDIT_RETYPE_PWD))->GetWindowTextW(csRetypePassword);

	if(csFTPPassword != csRetypePassword)
	{
		CString csMsg(_T(""));
		LOADSTRING(csMsg, IDS_PASSWORD_ERROR);
		AfxMessageBox(csMsg);	
		return;
	}

	memcpy(m_FTPInfo.tcHost, csFTPHost, sizeof(m_FTPInfo.tcHost)/sizeof(TCHAR));
	memcpy(m_FTPInfo.tcUser, csFTPUser, sizeof(m_FTPInfo.tcUser)/sizeof(TCHAR));
	memcpy(m_FTPInfo.tcPassword, csFTPPassword, sizeof(m_FTPInfo.tcPassword)/sizeof(TCHAR));
	
	OnOK();
}

void CDlgAddEditFTPServer::vSetFTPInfo(FTPInfo o_FTPInfo)
{
	memcpy(&m_FTPInfo, &o_FTPInfo, sizeof(m_FTPInfo));
}
