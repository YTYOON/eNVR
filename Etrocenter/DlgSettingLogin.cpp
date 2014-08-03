// DlgSettingLogin.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSettingLogin.h"
#include "MainFrm.h"

#define WHITECOLOR RGB(255,255,255)

// CDlgSettingLogin dialog

IMPLEMENT_DYNAMIC(CDlgSettingLogin, CBaseDialog)

CDlgSettingLogin::CDlgSettingLogin(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgSettingLogin::IDD, pParent)
{
	m_SilverBrush.CreateSolidBrush(GLOBALBKCOLOR);
	m_WhiteBrush.CreateSolidBrush(WHITECOLOR);	
}

CDlgSettingLogin::~CDlgSettingLogin()
{
	
}

void CDlgSettingLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSettingLogin, CBaseDialog)
	ON_BN_CLICKED(IDOK, &CDlgSettingLogin::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgSettingLogin::OnBnClickedCancel)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgSettingLogin message handlers

BOOL CDlgSettingLogin::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSettingLogin::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	GetDlgItemTextW(IDC_EDIT_USER_NAME, m_csUser);
	GetDlgItemTextW(IDC_EDIT_PWD, m_csPass);
	m_bOK = true;

	OnOK();
}

void CDlgSettingLogin::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	m_bOK = false;
	
	OnOK();
}
