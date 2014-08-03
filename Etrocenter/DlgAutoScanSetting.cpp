// DlgAutoScanSetting.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAutoScanSetting.h"
#include "MainFrm.h"
#include "DlgAutoScanGroupSetting.h"

// CDlgAutoScanSetting dialog

IMPLEMENT_DYNAMIC(CDlgAutoScanSetting, CDialog)

CDlgAutoScanSetting::CDlgAutoScanSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAutoScanSetting::IDD, pParent)
{
		btnAutoScanSettingGroup = new CButtonST;
}

CDlgAutoScanSetting::~CDlgAutoScanSetting()
{
		delete btnAutoScanSettingGroup;
}

void CDlgAutoScanSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAutoScanSetting, CDialog)
	ON_BN_CLICKED(IDB_BITMAP_GROUP_24, &CDlgAutoScanSetting::OnBnAutoScanSettingGroup)
END_MESSAGE_MAP()


// CDlgAutoScanSetting message handlers

void CDlgAutoScanSetting::OnOK()
{
	CString csTime;
	((CEdit*)GetDlgItem(IDC_EDIT_AUTOSCAN_SETTING))->GetWindowTextW( csTime );
	int nTime = 0;
	nTime = _ttoi(csTime);

	if(nTime < 10)
	{
		CString csWarning;
		//csWarning.LoadStringW(CGS::g_hResStr, IDS_AUTOSCAN_WARNING);
		LOADSTRING(csWarning,IDS_AUTOSCAN_WARNING);
		AfxMessageBox(csWarning);
		nTime = 10;
		return;
	}

	((CMainFrame*)AfxGetMainWnd())->m_nAutoScanTime = nTime;

	CDialog::OnOK();
}

BOOL CDlgAutoScanSetting::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString csTime;
	csTime.Format(_T("%d"),((CMainFrame*)AfxGetMainWnd())->m_nAutoScanTime);
	GetDlgItem(IDC_EDIT_AUTOSCAN_SETTING)->SetWindowTextW( csTime );
	((CEdit*)GetDlgItem(IDC_EDIT_AUTOSCAN_SETTING))->SetLimitText(3);

	CString csMsg;
	LOADSTRING(csMsg,IDS_AUTOSCAN_INTERVAL);
	SetDlgItemText(IDC_STATIC_INTERVAL, csMsg);

	LOADSTRING(csMsg,IDS_SECOND);
	SetDlgItemText(IDC_STATIC_SECOND, csMsg);
	
	LOADSTRING(csMsg,IDS_AUTOSCAN_SELECT_GROUP);
	SetDlgItemText(IDC_STATIC_SELECT_GROUP, csMsg);
	//GetDlgItem(IDC_STATIC_SELECT_GROUP)->ShowWindow(SW_HIDE);
	
	DWORD dwStyle = BS_PUSHBUTTON |BS_NOTIFY |WS_VISIBLE;
    COLORREF clBG = MASKCOLOR;
    int nHoveOver = (int)BTNST_AUTO_GRAY;
	btnAutoScanSettingGroup->Create(NULL, dwStyle, CRect(20,38,44,62), this, IDB_BITMAP_GROUP_24);
	btnAutoScanSettingGroup->SetBitmaps(IDB_BITMAP_GROUP_24, clBG ,nHoveOver);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAutoScanSetting::OnBnAutoScanSettingGroup()
{
  //  CGS::SetTransparent(this->GetParent()->m_hWnd,120);
	CDlgAutoScanGroupSetting dlg;
	dlg.DoModal();
  //  CGS::SetTransparent(this->GetParent()->m_hWnd,255);
}