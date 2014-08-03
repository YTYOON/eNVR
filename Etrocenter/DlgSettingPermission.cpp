// DlgSettingPermission.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSettingPermission.h"


// CDlgSettingPermission dialog

IMPLEMENT_DYNAMIC(CDlgSettingPermission, CDialog)

CDlgSettingPermission::CDlgSettingPermission(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSettingPermission::IDD, pParent), m_bORG(false)
{

}

CDlgSettingPermission::~CDlgSettingPermission()
{
}

void CDlgSettingPermission::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSettingPermission, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSettingPermission::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSettingPermission message handlers

void CDlgSettingPermission::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	this->GetDlgItemText(IDC_EDIT_PERM_USER, m_csUser);
	this->GetDlgItemText(IDC_EDIT_PERM_PWD, m_csPass);

	m_bLV = ((CButton*)GetDlgItem(IDC_CHECK_PERM_LV))->GetCheck();
	m_bSetting = ((CButton*)GetDlgItem(IDC_CHECK_PERM_SETTING))->GetCheck();
	m_bApp = ((CButton*)GetDlgItem(IDC_CHECK_PERM_APP))->GetCheck();

	if(m_csPass.Compare( m_csOrgPass.GetBuffer() ) == 0)
	{
		m_bORG = true;
	}

	OnOK();
}

void CDlgSettingPermission::vAttach(CString csUser, CString csPass, bool bLv, bool bSetting, bool bApp)
{
	m_csUser = csUser;
	m_csPass = csPass;
	m_csOrgPass = csPass;

	m_bLV = bLv;
	m_bSetting = bSetting;
	m_bApp = bApp;

	
}
BOOL CDlgSettingPermission::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	this->SetDlgItemText(IDC_EDIT_PERM_USER, m_csUser);
	this->SetDlgItemText(IDC_EDIT_PERM_PWD, m_csPass);

	//((CButton*)GetDlgItem(IDC_CHECK_PERM_LV))->SetCheck(m_bLV);
	((CButton*)GetDlgItem(IDC_CHECK_PERM_LV))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK_PERM_LV))->EnableWindow(0);
	((CButton*)GetDlgItem(IDC_CHECK_PERM_SETTING))->SetCheck(m_bSetting);
	((CButton*)GetDlgItem(IDC_CHECK_PERM_APP))->SetCheck(m_bApp);

	if( m_csUser.Compare(_T("root")) == 0)
	{
		((CButton*)GetDlgItem(IDC_CHECK_PERM_LV))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_CHECK_PERM_SETTING))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_CHECK_PERM_APP))->SetCheck(1);

		((CButton*)GetDlgItem(IDC_CHECK_PERM_LV))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_PERM_SETTING))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_CHECK_PERM_APP))->EnableWindow(0);

		GetDlgItem(IDC_EDIT_PERM_USER)->EnableWindow(0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
