// DlgModifyPTZ.cpp : implementation file
//

#include "stdafx.h"
#include "DlgModifyPTZ.h"
#include "MainFrm.h"

#define WHITECOLOR RGB(255,255,255)

// CDlgModifyPTZ dialog

IMPLEMENT_DYNAMIC(CDlgModifyPTZ, CDialog)

BOOL CDlgModifyPTZ::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &m_SilverBrush);

	//return CBaseDialog::OnEraseBkgnd(pDC);
	
	return true;
}

HBRUSH CDlgModifyPTZ::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	//return hbr;
	if( nCtlColor == CTLCOLOR_EDIT )//|| nCtlColor == CTLCOLOR_MSGBOX)
	{
		pDC->SetBkColor(WHITECOLOR);
		return m_WhiteBrush;
	}
	
	pDC->SetBkColor(GLOBALBKCOLOR);	
	//RecordDirtyFlag = false;
	return  m_SilverBrush;
}

CDlgModifyPTZ::CDlgModifyPTZ(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModifyPTZ::IDD, pParent)
{
	m_SilverBrush.CreateSolidBrush(GLOBALBKCOLOR);
	m_WhiteBrush.CreateSolidBrush(WHITECOLOR);
}

CDlgModifyPTZ::~CDlgModifyPTZ()
{
}

void CDlgModifyPTZ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgModifyPTZ, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgModifyPTZ message handlers

BOOL CDlgModifyPTZ::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO:  Add extra initialization here
	//vDBConnect();
	vProtocolInit();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgModifyPTZ::vSetMoPTZInfo(MoPTZInfo o_MoPTZInfo)
{
	m_MoPTZInfo = o_MoPTZInfo;
	
	CComboBox* pProtocol = (CComboBox*) GetDlgItem(IDC_COMBO_PTZ_PROTOCOL);

	if( _tcscmp(m_MoPTZInfo.tcEnable, _T("Enable")) == 0)								//PTZ Enable
	{
		((CButton*)(GetDlgItem( IDC_RADIO_PTZ_ENABLE )))->SetCheck( 1 );
		((CButton*)(GetDlgItem( IDC_RADIO_PTZ_DISABLE )))->SetCheck( 0 );
	}
	else
	{
		((CButton*)(GetDlgItem( IDC_RADIO_PTZ_ENABLE )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_PTZ_DISABLE )))->SetCheck( 1 );
	}

	int nItem = pProtocol->FindStringExact(0, (LPCTSTR)m_MoPTZInfo.tcProtocol);		//PTZ Protocol
	if(nItem == -1)
		nItem = pProtocol->FindStringExact(0, _T("Unknown"));
	pProtocol->SetCurSel(nItem);

	if( _tcscmp(m_MoPTZInfo.tcDI1, _T("Normal Open")) == 0)
	{
		((CButton*)(GetDlgItem( IDC_RADIO_DI1_NO )))->SetCheck( 1 );
		((CButton*)(GetDlgItem( IDC_RADIO_DI1_NC )))->SetCheck( 0 );
	}
	else
	{
		((CButton*)(GetDlgItem( IDC_RADIO_DI1_NO )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_DI1_NC )))->SetCheck( 1 );
	}

	if( _tcscmp(m_MoPTZInfo.tcDI2, _T("Normal Open")) == 0)
	{
		((CButton*)(GetDlgItem( IDC_RADIO_DI2_NO )))->SetCheck( 1 );
		((CButton*)(GetDlgItem( IDC_RADIO_DI2_NC )))->SetCheck( 0 );
	}
	else
	{
		((CButton*)(GetDlgItem( IDC_RADIO_DI2_NO )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_DI2_NC )))->SetCheck( 1 );
	}
	
	if( _tcscmp(m_MoPTZInfo.tcDO, _T("Normal Open")) == 0)
	{
		((CButton*)(GetDlgItem( IDC_RADIO_DO_NO )))->SetCheck( 1 );
		((CButton*)(GetDlgItem( IDC_RADIO_DO_NC )))->SetCheck( 0 );
	}
	else
	{
		((CButton*)(GetDlgItem( IDC_RADIO_DO_NO )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_DO_NC )))->SetCheck( 1 );
	}

}

void CDlgModifyPTZ::vProtocolInit()
{
	CComboBox* pProtocol = (CComboBox*) GetDlgItem(IDC_COMBO_PTZ_PROTOCOL);

	try
	{
		CString csProtocol;
		CRecordset rProtocol(&(((CMainFrame*)AfxGetMainWnd())->m_DB));
		rProtocol.Open(CRecordset::forwardOnly, _T("SELECT ptz_protocol_desc FROM ec_hptz_protocol"));
		while (!rProtocol.IsEOF()) { 
			rProtocol.GetFieldValue(_T("ptz_protocol_desc"), csProtocol);
			
			pProtocol->AddString(csProtocol);
			pProtocol->SetCurSel(0);

			rProtocol.MoveNext();
		}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

//bool CDlgModifyPTZ::vDBConnect(void)
//{
//	CIniReader m_ini(ECINI);
//	CString cpDns = m_ini.getKeyValue(_T("DSN"),_T("database"));
//	CString cpUid = m_ini.getKeyValue(_T("UID"),_T("database"));
//	CString cpPwd = m_ini.getKeyValue(_T("PWD"),_T("database"));
// 
//	CString ls_connect;
//	ls_connect.Format(_T("ODBC;UID=%s;PWD=%s"), cpUid, cpPwd);
//	return (((CMainFrame*)AfxGetMainWnd())->m_DB).Open((LPCTSTR)cpDns, FALSE, FALSE, ls_connect );
//}

MoPTZInfo CDlgModifyPTZ::mGetMoPTZInfo()
{
	_tcscpy(m_MoPTZInfo.tcEnable ,_T(""));
	_tcscpy(m_MoPTZInfo.tcProtocol ,_T(""));
	_tcscpy(m_MoPTZInfo.tcDI1 ,_T(""));
	_tcscpy(m_MoPTZInfo.tcDI2 ,_T(""));
	_tcscpy(m_MoPTZInfo.tcDO ,_T(""));

	
	if(((CButton*)(GetDlgItem( IDC_RADIO_PTZ_ENABLE )))->GetCheck() == 1)
		_tcscpy(m_MoPTZInfo.tcEnable ,_T("Enable"));
	else
		_tcscpy(m_MoPTZInfo.tcEnable ,_T("Disable"));

	CComboBox* pProtocol = (CComboBox*) GetDlgItem(IDC_COMBO_PTZ_PROTOCOL);
	int nIndex = pProtocol->GetCurSel();
	pProtocol->GetLBText(nIndex, (LPTSTR)(LPCTSTR)m_MoPTZInfo.tcProtocol);

	if(((CButton*)(GetDlgItem( IDC_RADIO_DI1_NO )))->GetCheck() == 1)
		_tcscpy(m_MoPTZInfo.tcDI1 ,_T("Normal Open"));
	else
		_tcscpy(m_MoPTZInfo.tcDI1 ,_T("Normal Close"));

	if(((CButton*)(GetDlgItem( IDC_RADIO_DI2_NO )))->GetCheck() == 1)
		_tcscpy(m_MoPTZInfo.tcDI2 ,_T("Normal Open"));
	else
		_tcscpy(m_MoPTZInfo.tcDI2 ,_T("Normal Close"));

	if(((CButton*)(GetDlgItem( IDC_RADIO_DO_NO )))->GetCheck() == 1)
		_tcscpy(m_MoPTZInfo.tcDO ,_T("Normal Open"));
	else
		_tcscpy(m_MoPTZInfo.tcDO ,_T("Normal Close"));

	return m_MoPTZInfo;
}