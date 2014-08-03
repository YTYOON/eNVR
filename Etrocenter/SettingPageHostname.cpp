// SettingPageHostname.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"
#include "NVSENCPLib.h"
#include "SettingPageHostname.h"
#include "ENCPSettingPage.h"
//#include "Authentication.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingPageHostname dialog


CSettingPageHostname::CSettingPageHostname(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingPageHostname::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingPageHostname)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSettingPageHostname::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingPageHostname)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingPageHostname, CDialog)
	//{{AFX_MSG_MAP(CSettingPageHostname)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_EN_CHANGE(IDC_DOMAINNAME, OnChangeDomainname)
	ON_EN_CHANGE(IDC_HOSTNAME, OnChangeHostname)
	ON_BN_CLICKED(IDC_RESTORE, OnRestore)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingPageHostname message handlers

void CSettingPageHostname::OnApply() 
{
	CEncpSettingPage * pParent = (CEncpSettingPage*)GetParent();

	if( pParent->ShowAuthDialog() == IDOK )
	{
		GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
		pParent->OnApply(IDD_SETTING_PAGE_HOSTNAME);
	}
}

void CSettingPageHostname::OnChangeDomainname() 
{
	GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
	GetDlgItem(IDC_RESTORE)->EnableWindow(TRUE);
}

void CSettingPageHostname::OnChangeHostname() 
{
	GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
	GetDlgItem(IDC_RESTORE)->EnableWindow(TRUE);
}

BOOL CSettingPageHostname::OnInitDialog() 
{
	HENCPITEM hItem = NULL;
	CDialog::OnInitDialog();

	LoadHostnameData();

	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
	GetDlgItem(IDC_RESTORE)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingPageHostname::OnRestore() 
{
	LoadHostnameData();

	((CEncpSettingPage*)GetParent())->OnRestore(IDD_SETTING_PAGE_HOSTNAME);
	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
	GetDlgItem(IDC_RESTORE)->EnableWindow(FALSE);
}

void CSettingPageHostname::GetSettingValue(CString &strHostname, CString &strDomain)
{
	GetDlgItemText(IDC_HOSTNAME, strHostname);
	GetDlgItemText(IDC_DOMAINNAME, strDomain);
}

void CSettingPageHostname::LoadHostnameData()
{
	USES_CONVERSION;;
	PENCPNODEINFO pNodeInfo = &((CEncpSettingPage*)GetParent())->m_nodeInfo;

	SetDlgItemText(IDC_HOSTNAME, A2W(pNodeInfo->hostName));
	SetDlgItemText(IDC_DOMAINNAME, A2W(pNodeInfo->domainName));
	SetDlgItemText(IDC_MODELNAME, A2W(pNodeInfo->modelName));
}

void CSettingPageHostname::OnClose() 
{
	((CEncpSettingPage*)GetParent())->Close(IDCANCEL);
}
