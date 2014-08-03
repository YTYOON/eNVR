// DlgRecRecord.cpp : implementation file
//

#include "stdafx.h"
#include "DlgRecRecord.h"
#include "MainFrm.h"

#define WHITECOLOR RGB(255,255,255)

// CDlgRecRecord dialog

IMPLEMENT_DYNAMIC(CDlgRecRecord, CDialog)

BOOL CDlgRecRecord::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &m_SilverBrush);

	//return CBaseDialog::OnEraseBkgnd(pDC);
	return true;
}

HBRUSH CDlgRecRecord::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

CDlgRecRecord::CDlgRecRecord(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRecRecord::IDD, pParent), RecordDirtyFlag(false)
{
	m_SilverBrush.CreateSolidBrush(GLOBALBKCOLOR);
	m_WhiteBrush.CreateSolidBrush(WHITECOLOR);

}

CDlgRecRecord::~CDlgRecRecord()
{
//	DBQuery.Close();
}

void CDlgRecRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgRecRecord, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
//	ON_BN_CLICKED(IDC_RADIO_RECORDING_RECORD_CONTINUOUS, &CDlgRecRecord::OnBnClickedRadioRecordingRecordContinuous)
//	ON_BN_CLICKED(IDC_RADIO_RECORDING_RECORD_EVENT, &CDlgRecRecord::OnBnClickedRadioRecordingRecordEvent)
//	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_RADIO_RECORDING_RECORD_CONTINUOUS, &CDlgRecRecord::OnBnHotItemChangeRadioRecordingRecordContinuous)
//	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_RADIO_RECORDING_RECORD_EVENT, &CDlgRecRecord::OnBnHotItemChangeRadioRecordingRecordEvent)
	ON_BN_CLICKED(IDC_RADIO_RECORDING_RECORD_CONTINUOUS, &CDlgRecRecord::OnBnClickedRadioRecordingRecordContinuous)
	ON_BN_CLICKED(IDC_RADIO_RECORDING_RECORD_EVENT, &CDlgRecRecord::OnBnClickedRadioRecordingRecordEvent)
	ON_BN_CLICKED(IDC_RADIO_RECORDING_RECORD_SCHEDULING, &CDlgRecRecord::OnBnClickedRadioRecordingRecordScheduling)
END_MESSAGE_MAP()


// CDlgRecRecord message handlers

BOOL CDlgRecRecord::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//vDBConnect();
	pDB = &(((CMainFrame*)AfxGetMainWnd())->m_DB);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

CString CDlgRecRecord::vSetCameraIDX(int iCameraIDX)
{
	/*--Clean Dialog--*/
	((CButton*)(GetDlgItem( IDC_RADIO_RECORDING_RECORD_EVENT )))->SetCheck( 0 );
	((CButton*)(GetDlgItem( IDC_RADIO_RECORDING_RECORD_CONTINUOUS )))->SetCheck( 0 );
	((CButton*)(GetDlgItem( IDC_RADIO_RECORDING_RECORD_SCHEDULING )))->SetCheck( 0 );

	/*-----Set Camera IDX------*/
	m_iCameraIDX = iCameraIDX;

	/*------Show Recording Type------*/
	CString csRecordingType = csGetRecordingType(m_iCameraIDX);

	if(csRecordingType == _T("Event"))
	{
		((CButton*)(GetDlgItem( IDC_RADIO_RECORDING_RECORD_EVENT )))->SetCheck( 1 );
		((CButton*)(GetDlgItem( IDC_RADIO_RECORDING_RECORD_CONTINUOUS )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_RECORDING_RECORD_SCHEDULING )))->SetCheck( 0 );
	}
	else if(csRecordingType == _T("Continuous"))
	{
		((CButton*)(GetDlgItem( IDC_RADIO_RECORDING_RECORD_EVENT )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_RECORDING_RECORD_CONTINUOUS )))->SetCheck( 1 );
		((CButton*)(GetDlgItem( IDC_RADIO_RECORDING_RECORD_SCHEDULING )))->SetCheck( 0 );
	}
	else if(csRecordingType == _T("Scheduling"))
	{
		((CButton*)(GetDlgItem( IDC_RADIO_RECORDING_RECORD_EVENT )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_RECORDING_RECORD_CONTINUOUS )))->SetCheck( 0 );
		((CButton*)(GetDlgItem( IDC_RADIO_RECORDING_RECORD_SCHEDULING )))->SetCheck( 1 );
	}

	RecordDirtyFlag = false;

	return csRecordingType;
}
//void CDlgRecRecord::vDBConnect()
//{
//	CIniReader s_ini(_T(".\\ec.ini"));
//	csDSN.Format(_T("%s"), s_ini.getKeyValue(_T("DSN"),_T("database")));
//	csDBUser.Format(_T("%s"), s_ini.getKeyValue(_T("UID"),_T("database")));
//	csDBPass.Format(_T("%s"), s_ini.getKeyValue(_T("PWD"),_T("database")));
//	csODBC = _T("ODBC;DSN=") + csDSN + _T(";UID=") + csDBUser + _T(";PWD=") + csDBPass;
//
//	if( DBQuery.Open((csDSN), FALSE, FALSE, csODBC) )
//	{/*AfxMessageBox(_T("Connection OK"));*/}
//}

CString CDlgRecRecord::csGetRecordingType(int m_iCameraIDX)
{
	CString csRecordingFlag;
	try
	{
		CString SQL_Recording_Type;
		SQL_Recording_Type.Format(_T("SELECT recording_type FROM ec_camera_recording WHERE camera_idx = '%d'"), m_iCameraIDX);

		CRecordset rRecType(pDB);
		rRecType.Open(CRecordset::forwardOnly, SQL_Recording_Type);
		while (!rRecType.IsEOF()) { 
			rRecType.GetFieldValue(_T("recording_type"), csRecordingFlag);
			csRecordingFlag.TrimRight();
			rRecType.MoveNext();
		}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	RecordDirtyFlag = false;

	return csRecordingFlag;
}
//void CDlgRecRecord::OnBnClickedRadioRecordingRecordContinuous()
//{
//	// TODO: Add your control notification handler code here
//	//if(((CButton*)(GetDlgItem( IDC_RADIO_RECORDING_RECORD_EVENT )))->GetCheck == 1)
//	RecordDirtyFlag = true;
//}

//void CDlgRecRecord::OnBnClickedRadioRecordingRecordEvent()
//{
//	// TODO: Add your control notification handler code here
//	
//}

//void CDlgRecRecord::OnBnHotItemChangeRadioRecordingRecordContinuous(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// This feature requires Internet Explorer 6 or greater.
//	// The symbol _WIN32_IE must be >= 0x0600.
//	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	RecordDirtyFlag = true;
//
//	*pResult = 0;
//}

//void CDlgRecRecord::OnBnHotItemChangeRadioRecordingRecordEvent(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// This feature requires Internet Explorer 6 or greater.
//	// The symbol _WIN32_IE must be >= 0x0600.
//	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	RecordDirtyFlag = true;
//	*pResult = 0;
//}

void CDlgRecRecord::OnBnClickedRadioRecordingRecordContinuous()
{
	// TODO: Add your control notification handler code here
	RecordDirtyFlag = true;
}

void CDlgRecRecord::OnBnClickedRadioRecordingRecordEvent()
{
	// TODO: Add your control notification handler code here
	RecordDirtyFlag = true;
}

BOOL CDlgRecRecord::PreTranslateMessage(MSG *pMsg)
{
	if   (pMsg-> message   ==   WM_KEYDOWN) 
    { 
        if   (pMsg-> wParam   ==   VK_RETURN   ||   pMsg-> wParam   ==   VK_ESCAPE) 
            return   TRUE; 
    } 

    return   CDialog::PreTranslateMessage(pMsg); 
}

void CDlgRecRecord::OnBnClickedRadioRecordingRecordScheduling()
{
	// TODO: Add your control notification handler code here
	RecordDirtyFlag = true;
}
