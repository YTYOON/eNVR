// SettingPageAccount.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"

#include "NVSENCPLib.h"
#include "SettingPageAccount.h"
#include "ENCPSettingPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingPageAccount dialog


CSettingPageAccount::CSettingPageAccount(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingPageAccount::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingPageAccount)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSettingPageAccount::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingPageAccount)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingPageAccount, CDialog)
	//{{AFX_MSG_MAP(CSettingPageAccount)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_ALLOWGUEST, OnAllowguest)
	ON_EN_CHANGE(IDC_NEWPASS, OnChangeNewpass)
	ON_BN_CLICKED(IDC_RESTORE, OnRestore)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingPageAccount message handlers

void CSettingPageAccount::OnApply() 
{
	CEncpSettingPage * pParent = (CEncpSettingPage*)GetParent();

	if( pParent->ShowAuthDialog() == IDOK )
	{
		GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
		pParent->OnApply(IDD_SETTING_PAGE_ACCOUNT);
	}
}

BOOL CSettingPageAccount::OnInitDialog() 
{
	HENCPITEM hItem = NULL;

	CDialog::OnInitDialog();

	LoadAccountData();

	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
	GetDlgItem(IDC_RESTORE)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingPageAccount::OnAllowguest() 
{
	GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
	GetDlgItem(IDC_RESTORE)->EnableWindow(TRUE);
}

void CSettingPageAccount::OnChangeNewpass() 
{
	GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
	GetDlgItem(IDC_RESTORE)->EnableWindow(TRUE);
}

void CSettingPageAccount::OnRestore() 
{
	LoadAccountData();

	((CEncpSettingPage*)GetParent())->OnRestore(IDD_SETTING_PAGE_ACCOUNT);
	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
	GetDlgItem(IDC_RESTORE)->EnableWindow(FALSE);
}

void CSettingPageAccount::GetSettingValue(CString &pass, BOOL &bAllowGuest)
{
	GetDlgItemText(IDC_NEWPASS, pass);
	bAllowGuest = (((CButton*)GetDlgItem(IDC_ALLOWGUEST))->GetCheck()==BST_CHECKED);
}

void CSettingPageAccount::LoadAccountData()
{
	BOOL bGuestAllow = ((CEncpSettingPage*)GetParent())->m_bGuestAllow;
	CheckDlgButton(IDC_ALLOWGUEST, bGuestAllow?BST_CHECKED:BST_UNCHECKED);
	SetDlgItemText(IDC_NEWPASS, _T(""));
}

void CSettingPageAccount::OnClose() 
{
	((CEncpSettingPage*)GetParent())->Close(IDCANCEL);
}
