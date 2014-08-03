// DlgJoyStick.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"
#include "DlgJoyStick.h"


// CDlgJoyStick dialog

IMPLEMENT_DYNAMIC(CDlgJoyStick, CDialog)

CDlgJoyStick::CDlgJoyStick(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgJoyStick::IDD, pParent)
{

}

CDlgJoyStick::~CDlgJoyStick()
{
}

void CDlgJoyStick::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgJoyStick, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION, &CDlgJoyStick::OnBnClickedButtonCalibration)
END_MESSAGE_MAP()


// CDlgJoyStick message handlers

void CDlgJoyStick::OnBnClickedButtonCalibration()
{
	// TODO: Add your control notification handler code here
	USES_CONVERSION;
	CString csCmd;
	csCmd=_T("rundll32.exe shell32.dll,Control_RunDLL joy.cpl");
	LPCSTR lpCmd;
	lpCmd=T2A(csCmd.GetBuffer(csCmd.GetLength()));
	WinExec(lpCmd, SW_SHOW);
}
