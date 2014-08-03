// DlgAddEditMailServer.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAddEditMailServer.h"


// CDlgAddEditMailServer dialog

IMPLEMENT_DYNAMIC(CDlgAddEditMailServer, CBaseDialog)


//CDlgAddEditMailServer::CDlgAddEditMailServer(CWnd* pParent /*=NULL*/)
//	: CBaseDialog(CDlgAddEditMailServer::IDD, pParent)
//{
//
//}

CDlgAddEditMailServer::CDlgAddEditMailServer(int ClassType, CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgAddEditMailServer::IDD, pParent)
{
	iClassType = ClassType;
}

CDlgAddEditMailServer::~CDlgAddEditMailServer()
{
}

void CDlgAddEditMailServer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAddEditMailServer, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDOK, &CDlgAddEditMailServer::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAddEditMailServer message handlers

HBRUSH CDlgAddEditMailServer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

BOOL CDlgAddEditMailServer::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CBaseDialog::OnEraseBkgnd(pDC);
}

BOOL CDlgAddEditMailServer::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString csDialogCaption(_T(""));
	//csDialogCaption.LoadString(CGS::g_hResStr, IDS_DLG_ADD_EDIT_EMAIL);
	LOADSTRING(csDialogCaption, IDS_DLG_ADD_EDIT_EMAIL);
	this->SetWindowTextW(csDialogCaption);
	
	CString csSMTPServer, csUserName, csPassword;
	CByteArray arBytes2;
	CString csDecrypt;

	switch(iClassType)
	{
	case AddServer:

		((CEdit*)GetDlgItem(IDC_STATIC_ADDRESS))->ShowWindow(SW_HIDE);

		break;

	case EditServer:

		((CEdit*)GetDlgItem(IDC_STATIC_ADDRESS))->ShowWindow(SW_HIDE);

		csSMTPServer = m_MailInfo.cSMTPServer;
		csUserName = m_MailInfo.cUserName;
		csPassword = m_MailInfo.cPassword;
		((CEdit*)GetDlgItem(IDC_EDIT_SMTP_SERVER))->SetWindowTextW( csSMTPServer );
		((CEdit*)GetDlgItem(IDC_EDIT_USER_NAME))->SetWindowTextW( csUserName );
		((CEdit*)GetDlgItem(IDC_EDIT_PASSWORD))->SetWindowTextW( csPassword );
		((CEdit*)GetDlgItem(IDC_EDIT_RETYPE_PASSWORD))->SetWindowTextW( csPassword );

		//arBytes2.SetSize(csPassword.GetLength());

		//for(int iTemp = 0; iTemp <= csPassword.GetLength(); iTemp++)
		//{
		//	arBytes2[iTemp] = 
		//}


		//memcpy(arBytes2.GetData(), csPassword, csPassword.GetLength());
		//m_Crypto.Decrypt(arBytes2, csDecrypt);
		//arBytes2.SetSize(csPassword.GetLength());
		//memcpy(arBytes2.GetData(), csPassword, csPassword.GetLength());
		//m_Crypto.Decrypt(arBytes2, csDecrypt);
		//AfxMessageBox(csDecrypt);

		break;

	case AddMailAddress:

		((CEdit*)GetDlgItem(IDC_STATIC_SMTP_SERVER))->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_EDIT_USER_NAME))->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_EDIT_PASSWORD))->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_EDIT_RETYPE_PASSWORD))->ShowWindow(SW_HIDE);
		//((CEdit*)GetDlgItem(IDC_EDIT_USER_NAME))->EnableWindow( 0 );
		//((CEdit*)GetDlgItem(IDC_EDIT_PASSWORD))->EnableWindow( 0 );
		//((CEdit*)GetDlgItem(IDC_EDIT_RETYPE_PASSWORD))->EnableWindow( 0 );
		((CEdit*)GetDlgItem(IDC_STATIC_PASSWORD))->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_STATIC_RETYPE_PASSWORD))->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_STATIC_USER_NAME))->ShowWindow(SW_HIDE);

		break;
	case EditMailAddress:

		((CEdit*)GetDlgItem(IDC_STATIC_SMTP_SERVER))->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_EDIT_USER_NAME))->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_EDIT_PASSWORD))->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_EDIT_RETYPE_PASSWORD))->ShowWindow(SW_HIDE);

		((CEdit*)GetDlgItem(IDC_STATIC_PASSWORD))->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_STATIC_RETYPE_PASSWORD))->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_STATIC_USER_NAME))->ShowWindow(SW_HIDE);
		
		csSMTPServer = m_MailInfo.cSMTPServer;
		((CEdit*)GetDlgItem(IDC_EDIT_SMTP_SERVER))->SetWindowTextW( csSMTPServer );
		break;
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAddEditMailServer::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString csSMTPServer, csUserName, csPassword, csRetypePassword;

	((CEdit*)GetDlgItem(IDC_EDIT_SMTP_SERVER))->GetWindowTextW(csSMTPServer);
	((CEdit*)GetDlgItem(IDC_EDIT_USER_NAME))->GetWindowTextW(csUserName);
	((CEdit*)GetDlgItem(IDC_EDIT_PASSWORD))->GetWindowTextW(csPassword);
	((CEdit*)GetDlgItem(IDC_EDIT_RETYPE_PASSWORD))->GetWindowTextW(csRetypePassword);

	if(csPassword != csRetypePassword)
	{
		CString csMsg(_T(""));
		LOADSTRING(csMsg, IDS_PASSWORD_ERROR);
		AfxMessageBox(csMsg);	
		return;
	}

	memcpy(m_MailInfo.cSMTPServer, csSMTPServer, sizeof(m_MailInfo.cSMTPServer)/sizeof(TCHAR));
	memcpy(m_MailInfo.cUserName, csUserName, sizeof(m_MailInfo.cUserName)/sizeof(TCHAR));
	memcpy(m_MailInfo.cPassword, csPassword, sizeof(m_MailInfo.cPassword)/sizeof(TCHAR));

	OnOK();
}

void CDlgAddEditMailServer::vSetServerInfo(MailInfo o_MailInfo)
{
	memcpy(&m_MailInfo, &o_MailInfo, sizeof(m_MailInfo));
}