// DlgActionSetting.cpp : implementation file
//

#include "stdafx.h"
#include "DlgActionSetting.h"


// CDlgActionSetting dialog

IMPLEMENT_DYNAMIC(CDlgActionSetting, CDialog)

CDlgActionSetting::CDlgActionSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgActionSetting::IDD, pParent)
{

}

CDlgActionSetting::~CDlgActionSetting()
{
}

void CDlgActionSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgActionSetting, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgActionSetting::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgActionSetting message handlers

CDlgActionSetting::CDlgActionSetting(int iType, ActionInfo* pActionInfo, CWnd *pParent)
	: CDialog(CDlgActionSetting::IDD, pParent)
{
	m_iType = iType;
	this->pActionInfo = pActionInfo;
}


BOOL CDlgActionSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	switch(this->m_iType)
	{
	case ACTION_SETTING_TYPE::AST_EMAIL:
		((CButton*)GetDlgItem(IDC_EDIT_MAIL_TARGET))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_EDIT_MAIL_TARGET))->SetWindowText(pActionInfo->tcTargetMail);
		break;
	case ACTION_SETTING_TYPE::AST_PTZ:
		((CButton*)GetDlgItem(IDC_COMBO_PTZ_P))->EnableWindow(1);
		m_PTZ_P = (CComboBox*)GetDlgItem(IDC_COMBO_PTZ_P);
		for(int iTemp = 6; iTemp >= -6; iTemp--)
		{
			CString csTemp;
			csTemp.Format(_T("%d"), iTemp);
			m_PTZ_P->AddString( csTemp.GetBuffer() );
		}
		m_PTZ_P->SetCurSel(0);

		((CButton*)GetDlgItem(IDC_COMBO_PTZ_T))->EnableWindow(1);
		m_PTZ_T = (CComboBox*)GetDlgItem(IDC_COMBO_PTZ_T);
		for(int iTemp = 6; iTemp >= -6; iTemp--)
		{
			CString csTemp;
			csTemp.Format(_T("%d"), iTemp);
			m_PTZ_T->AddString( csTemp.GetBuffer() );
		}
		m_PTZ_T->SetCurSel(0);

		((CButton*)GetDlgItem(IDC_COMBO_PTZ_z))->EnableWindow(1);
		m_PTZ_Z = (CComboBox*)GetDlgItem(IDC_COMBO_PTZ_z);
		for(int iTemp = 6; iTemp >= -6; iTemp--)
		{
			CString csTemp;
			csTemp.Format(_T("%d"), iTemp);
			m_PTZ_Z->AddString( csTemp.GetBuffer() );
		}
		m_PTZ_Z->SetCurSel(0);
		break;
	case ACTION_SETTING_TYPE::AST_DO:
		((CButton*)GetDlgItem(IDC_COMBO_AST_DO))->EnableWindow(1);
		m_DO = (CComboBox*)GetDlgItem(IDC_COMBO_AST_DO);
		m_DO->AddString(_T("ON"));
		m_DO->AddString(_T("OFF"));
		m_DO->SetCurSel(0);

		break;
	default:
		ASSERT(true);
		break;
	};


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgActionSetting::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString csValue;
	int nItem;

	switch(this->m_iType)
	{ 
	case ACTION_SETTING_TYPE::AST_EMAIL:
		 ((CButton*)GetDlgItem(IDC_EDIT_MAIL_TARGET))->GetWindowTextW(csValue);

		 if(csValue.GetLength() == 0)
		 {
			AfxMessageBox(_T("Email cannot be blank!"));
			return;
		 }

		 _tcscpy(this->pActionInfo->tcTargetMail, csValue.GetBuffer());

		break;
	case ACTION_SETTING_TYPE::AST_PTZ:
		
		break;
	case ACTION_SETTING_TYPE::AST_DO:
		m_DO = (CComboBox*)GetDlgItem(IDC_COMBO_AST_DO);
		nItem = m_DO->GetCurSel();
		m_DO->GetLBText(nItem, csValue); 

		if(csValue.Find(_T("ON")) != -1)
		{
			this->pActionInfo->bDO_Setting = true;
		}
		else
		{	
			this->pActionInfo->bDO_Setting = false;
		}

		break;
	default:
		ASSERT(true);
		break;
	};

	OnOK();
}