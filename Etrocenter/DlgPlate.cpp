// DlgPlate.cpp : implementation file
//

#include "stdafx.h"
#include "DlgPlate.h"


// CDlgPlate dialog

IMPLEMENT_DYNAMIC(CDlgPlate, CBaseDialog)

CDlgPlate::CDlgPlate(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgPlate::IDD, pParent)
{

}

CDlgPlate::~CDlgPlate()
{
}

void CDlgPlate::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITPLATE1, m_lplate1);
}


BEGIN_MESSAGE_MAP(CDlgPlate, CBaseDialog)
END_MESSAGE_MAP()


// CDlgPlate message handlers

BOOL CDlgPlate::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	((CEdit*)GetDlgItem(IDC_EDITCARD))->SetWindowTextW(m_card);
	((CEdit*)GetDlgItem(IDC_EDITNAME))->SetWindowTextW(m_name);

	((CEdit*)GetDlgItem(IDC_EDITPLATE1))->SetWindowTextW(m_plate1);
	((CEdit*)GetDlgItem(IDC_EDITPLATE2))->SetWindowTextW(m_plate2);
	((CEdit*)GetDlgItem(IDC_EDITPLATE3))->SetWindowTextW(m_plate3);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPlate::OnOK()
{
	((CEdit*)GetDlgItem(IDC_EDITCARD))->GetWindowTextW(m_card);
	((CEdit*)GetDlgItem(IDC_EDITNAME))->GetWindowTextW(m_name);

	((CEdit*)GetDlgItem(IDC_EDITPLATE1))->GetWindowTextW(m_plate1);
	((CEdit*)GetDlgItem(IDC_EDITPLATE2))->GetWindowTextW(m_plate2);
	((CEdit*)GetDlgItem(IDC_EDITPLATE3))->GetWindowTextW(m_plate3);

	CBaseDialog::OnOK();
}
