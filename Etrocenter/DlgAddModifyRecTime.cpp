 // DlgAddModifyRecTime.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAddModifyRecTime.h"
#include "MainFrm.h"

#define WHITECOLOR RGB(255,255,255)

// CDlgAddModifyRecTime dialog

IMPLEMENT_DYNAMIC(CDlgAddModifyRecTime, CDialog)


BOOL CDlgAddModifyRecTime::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &m_SilverBrush);

	//return CBaseDialog::OnEraseBkgnd(pDC);
	return true;
}

HBRUSH CDlgAddModifyRecTime::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
	return  m_SilverBrush;
}

CDlgAddModifyRecTime::CDlgAddModifyRecTime(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddModifyRecTime::IDD, pParent), EditFlag(false)
{
	m_SilverBrush.CreateSolidBrush(GLOBALBKCOLOR);
	m_WhiteBrush.CreateSolidBrush(WHITECOLOR);	 
}

CDlgAddModifyRecTime::~CDlgAddModifyRecTime()
{
	//vDBClose();
}

void CDlgAddModifyRecTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DAY_TYPE, pDayTypeCombo);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DATE, pDatePicker);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TIME_FROM, pTimeFromPicker);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TIME_TO, pTimeToPicker);
}


BEGIN_MESSAGE_MAP(CDlgAddModifyRecTime, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_CBN_SELCHANGE(IDC_COMBO_DAY_TYPE, &CDlgAddModifyRecTime::OnCbnSelchangeComboDayType)
	ON_BN_CLICKED(IDOK, &CDlgAddModifyRecTime::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAddModifyRecTime message handlers

BOOL CDlgAddModifyRecTime::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CString csDialogCaption(_T(""));
	//csDialogCaption.LoadString(CGS::g_hResStr, IDS_DLG_ADD_MODIFY_RECORDING);
	LOADSTRING(csDialogCaption, IDS_DLG_ADD_MODIFY_RECORDING);
	this->SetWindowTextW(csDialogCaption);

	pDB = &(((CMainFrame*)AfxGetMainWnd())->m_DB);
	//vDBConnect();
	// TODO:  Add extra initialization here
	/*--Initial Day Type ComboBox--*/

	try
	{
		CString SQL_DayType(_T("SELECT * FROM ec_hdaytype"));

		CString csDayType;
		CRecordset rDayType(pDB);
		rDayType.Open(CRecordset::forwardOnly, SQL_DayType);
		while (!rDayType.IsEOF()) { 
			rDayType.GetFieldValue(_T("daytype_desc"), csDayType);
			csDayType.TrimRight();
			pDayTypeCombo.AddString(csDayType);
			
			rDayType.MoveNext();
		}

		pDayTypeCombo.SetCurSel(0);
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
	
	//vDBClose();

	/*--Set Date Picker Disable--*/
	pDatePicker.EnableWindow(0);

	//pTimeFromPicker.SetFormat(_T("HH:00:00"));
	//pTimeToPicker.SetFormat(_T("HH:00:00"));

	pTimeFromPicker.SetFormat(_T("HH ('Hr')"));
	pTimeToPicker.SetFormat(_T("HH ('Hr')"));

	if(EditFlag == true)
	{
		int nItem = pDayTypeCombo.FindStringExact(0, m_csDayType);				//Day Type
		pDayTypeCombo.SetCurSel(nItem);
		
		int iYear, iMonth, iDay, iHour, iMinute, iSecond;
		TCHAR *tcTemp = NULL;	

		if(m_csDate != _T(""))													//Date
		{
			tcTemp = (TCHAR*)(LPCTSTR)m_csDate;								
			_stscanf(tcTemp, _T("%d-%d-%d"), &iMonth, &iDay, &iYear);				
			CTime timeDate(iYear, iMonth, iDay, 0, 0, 0);
			pDatePicker.SetTime(&timeDate);
		}

		tcTemp = (TCHAR*)(LPCTSTR)m_csTimeFrom;									//Time From
		_stscanf(tcTemp, _T("%02d"), &iHour);

		
	    //memset(&sysTimeFrom, 0, sizeof(sysTimeFrom));
		//sysTimeFrom.wHour = iHour;
		COleDateTime COleDateTimeFrom;
		COleDateTimeFrom.SetTime(iHour, 0, 0);
		pTimeFromPicker.SetTime(COleDateTimeFrom);	

		tcTemp = (TCHAR*)(LPCTSTR)m_csTimeTo;									//Time To
		_stscanf(tcTemp, _T("%02d"), &iMinute);			
		
		//SYSTEMTIME sysTimeTo;
		//memset(&sysTimeTo, 0, sizeof(sysTimeTo));
		//sysTimeTo.wHour = iMinute;
		COleDateTime COleDateTimeTo;
		COleDateTimeTo.SetTime(iMinute, 0, 0);
		pTimeToPicker.SetTime(COleDateTimeTo);

		if(m_csDayType == _T("Specific Day"))
			pDatePicker.EnableWindow(1);
		else
			pDatePicker.EnableWindow(0);
	}

	this->UpdateWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAddModifyRecTime::OnCbnSelchangeComboDayType()
{
	// TODO: Add your control notification handler code here

	int iCurSelect = pDayTypeCombo.GetCurSel();
	CString csDayType;
	pDayTypeCombo.GetLBText(iCurSelect, csDayType);
	if(csDayType == _T("Specific Day"))
		pDatePicker.EnableWindow(1);
	else
		pDatePicker.EnableWindow(0);
}

void CDlgAddModifyRecTime::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	
	CString csDayType, csDate, csTimeFrom, csTimeTo;
	CTime timeTime;
	/*--Get DayType--*/
	int iCurSelect = pDayTypeCombo.GetCurSel();
	pDayTypeCombo.GetLBText(iCurSelect, csDayType);

	/*--Get Date--*/
	if(csDayType == _T("Specific Day"))
	{
		pDatePicker.GetTime(timeTime);
		csDate = timeTime.Format(_T("%m-%d-%Y"));
	}
	else
		csDate = _T("");
	
	/*--Get Time From--*/
	COleDateTime COleDateTimeFrom;
	pTimeFromPicker.GetTime(COleDateTimeFrom);
	csTimeFrom = COleDateTimeFrom.Format(_T("%H"));

	/*--Get Time To--*/
	COleDateTime COleDateTimeTo;
	pTimeToPicker.GetTime(COleDateTimeTo);
	csTimeTo = COleDateTimeTo.Format(_T("%H"));

	//--Check Time 
	int iTimeFrom = 0, iTimeTo = 0;
	iTimeFrom = ::_ttoi(csTimeFrom);
	iTimeTo = ::_ttoi(csTimeTo);

	if(iTimeFrom > iTimeTo)
	{
		AfxMessageBox(IDS_RECORD_TIME_ERROR); 
		//AfxMessageBox(_T("Recording Time Error!")); 
		return;
	}

	memcpy(m_RecTimeInfo.cDayType, csDayType, sizeof(m_RecTimeInfo.cDayType)/sizeof(TCHAR));
	memcpy(m_RecTimeInfo.cDate, csDate, sizeof(m_RecTimeInfo.cDate)/sizeof(TCHAR));
	memcpy(m_RecTimeInfo.cTimeFrom, csTimeFrom, sizeof(m_RecTimeInfo.cTimeFrom)/sizeof(TCHAR));
	memcpy(m_RecTimeInfo.cTimeTo, csTimeTo, sizeof(m_RecTimeInfo.cTimeTo)/sizeof(TCHAR));

	OnOK();
}

void CDlgAddModifyRecTime::vSetScheduleTime(CString csDayType, CString csDate, CString csTimeFrom, CString csTimeTo)
{
	EditFlag = true;
	m_csDayType = csDayType;
	m_csDate = csDate;
	m_csTimeFrom = csTimeFrom;
	m_csTimeTo = csTimeTo;
}

//void CDlgAddModifyRecTime::vDBConnect()
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

//void CDlgAddModifyRecTime::vDBClose()
//{
//	DBQuery.Close();
//}