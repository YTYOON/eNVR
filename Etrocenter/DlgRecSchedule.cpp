// DlgRecSchedule.cpp : implementation file
//

#include "stdafx.h"
#include "DlgRecSchedule.h"
#include "MainFrm.h"

#define WHITECOLOR RGB(255,255,255)

// CDlgRecSchedule dialog

IMPLEMENT_DYNAMIC(CDlgRecSchedule, CDialog)

BOOL CDlgRecSchedule::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &m_SilverBrush);

	//return CBaseDialog::OnEraseBkgnd(pDC);
	return true;
}

HBRUSH CDlgRecSchedule::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

CDlgRecSchedule::CDlgRecSchedule(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRecSchedule::IDD, pParent), m_iCameraIDX(0), ScheduleDirtyFlag(false)
{
	m_SilverBrush.CreateSolidBrush(GLOBALBKCOLOR);
	m_WhiteBrush.CreateSolidBrush(WHITECOLOR);
}

CDlgRecSchedule::~CDlgRecSchedule()
{
	//DBQuery.Close();
	if(m_ToolTip) delete m_ToolTip;
}

void CDlgRecSchedule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RECORDING_SCHEDULE, m_ScheduleListCtrl);
}


BEGIN_MESSAGE_MAP(CDlgRecSchedule, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_REC_TAB_DELETE, &CDlgRecSchedule::OnBnClickedButtonRecTabDelete)
	ON_BN_CLICKED(IDC_BUTTON_REC_TAB_ADD, &CDlgRecSchedule::OnBnClickedButtonRecTabAdd)
	ON_BN_CLICKED(IDC_BUTTON_REC_TAB_EDIT, &CDlgRecSchedule::OnBnClickedButtonRecTabEdit)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgRecSchedule message handlers

BOOL CDlgRecSchedule::OnInitDialog()
{
	CDialog::OnInitDialog();
	//vDBConnect();

	/*--------------Initial Bitmap Button-----------------*/
	LV_COLUMN ListColumn;
	m_cbRecTabDelete.SubclassDlgItem(IDC_BUTTON_REC_TAB_DELETE, this);
	m_cbRecTabAdd.SubclassDlgItem(IDC_BUTTON_REC_TAB_ADD, this);
	m_cbRecTabEdit.SubclassDlgItem(IDC_BUTTON_REC_TAB_EDIT, this);

	m_cbRecTabDelete.LoadBitmaps(IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL);
	m_cbRecTabAdd.LoadBitmaps(IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD);
	m_cbRecTabEdit.LoadBitmaps(IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT);

	/*-----------------Initial Schedule List Control-------------*/
	WCHAR * ListTitles[5] = {_T("Active"), _T("Schedule Type"), _T("Date"), _T("From Time (Hr)"), _T("Till Time (Hr)")};

	for(int i = 0; i < 5; i++)
	{
		ListColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		ListColumn.fmt = LVCFMT_LEFT;
		ListColumn.iSubItem = i;

		if(i == 0 )
		{ListColumn.cx = 75;}
		else if(i == 1)
		{ListColumn.cx = 140;}
		else if(i == 2)
		{ListColumn.cx = 140;}
		else
		{ListColumn.cx = 160;}

		ListColumn.pszText = ListTitles[i];
		m_ScheduleListCtrl.InsertColumn(i, &ListColumn);
	}

	DWORD dwStyle = m_ScheduleListCtrl.GetStyle();
	dwStyle |= LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT;
	m_ScheduleListCtrl.SetExtendedStyle(dwStyle);

	m_ScheduleListCtrl.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);

	CHeaderCtrl* pHeaderCtrl = (CHeaderCtrl*)m_ScheduleListCtrl.GetHeaderCtrl(); 
	pHeaderCtrl-> EnableWindow( false );

	/*--Microhelp on buttons--*/
	m_ToolTip= new CToolTipCtrl();
	m_ToolTip->Create(this);
	CString ls_st;
	
	LOADSTRING(ls_st,IDS_TOOL_DEL_REC);
	//ls_st.LoadString(CGS::g_hResStr, IDS_TOOL_DEL_REC);
	m_ToolTip->AddTool(&m_cbRecTabDelete,ls_st);
	
	LOADSTRING(ls_st,IDS_TOOL_ADD_REC);
	//ls_st.LoadString(CGS::g_hResStr, IDS_TOOL_ADD_REC);
	m_ToolTip->AddTool(&m_cbRecTabAdd, ls_st);

	LOADSTRING(ls_st,IDS_TOOL_MOD_REC);
	//ls_st.LoadString(CGS::g_hResStr, IDS_TOOL_MOD_REC);
	m_ToolTip->AddTool(&m_cbRecTabEdit,ls_st);

	//m_ToolTip->AddTool(&m_cbRecTabDelete,_T("Delete Recording Schedule"));
	//m_ToolTip->AddTool(&m_cbRecTabAdd,_T("Add New Recording Schedule"));
	//m_ToolTip->AddTool(&m_cbRecTabEdit,_T("Modify Recording Schedule"));
	m_ToolTip->Activate(TRUE);

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRecSchedule::OnBnClickedButtonRecTabDelete()
{
	// TODO: Add your control notification handler code here
	int nItem = m_ScheduleListCtrl.GetSelectionMark();
	if(nItem == -1 )
		return;

	m_ScheduleListCtrl.DeleteItem(nItem);
	ScheduleDirtyFlag = true;
}

void CDlgRecSchedule::OnBnClickedButtonRecTabAdd()
{
	// TODO: Add your control notification handler code here
	if(m_iCameraIDX == 0)
	{		
		AfxMessageBox(IDS_CAM_SELECT);
		//	AfxMessageBox(_T("Please Select a Camera First."));
		return;
	}

	if(m_CDlgAddModifyRecTime.DoModal() == IDOK)
	{
		memcpy(&m_RecTimeInfo, &m_CDlgAddModifyRecTime.m_RecTimeInfo, sizeof(m_RecTimeInfo));
		ScheduleDirtyFlag = true;
	}
	else
		return;

	CString csTimeFrom, csTimeTo;
	csTimeFrom.Format(_T("%c%c"), m_RecTimeInfo.cTimeFrom[0], m_RecTimeInfo.cTimeFrom[1]);
	csTimeTo.Format(_T("%c%c"), m_RecTimeInfo.cTimeTo[0], m_RecTimeInfo.cTimeTo[1]);
	//_stscanf(m_RecTimeInfo.cTimeFrom, _T("%02d:%02d:%02d"), &csTimeFrom);
	//_stscanf(m_RecTimeInfo.cTimeTo, _T("%02d:%02d:%02d"), &csTimeTo);

	int nItem = m_ScheduleListCtrl.InsertItem(0, _T(""));
	m_ScheduleListCtrl.SetCheck(nItem, 1);
	m_ScheduleListCtrl.SetItemText(nItem, 1, (LPCTSTR)m_RecTimeInfo.cDayType);
	m_ScheduleListCtrl.SetItemText(nItem, 2, (LPCTSTR)m_RecTimeInfo.cDate);
	m_ScheduleListCtrl.SetItemText(nItem, 3, csTimeFrom);
	//m_ScheduleListCtrl.SetItemText(nItem, 4, _T("Till"));
	m_ScheduleListCtrl.SetItemText(nItem, 4, csTimeTo);
}

void CDlgRecSchedule::OnBnClickedButtonRecTabEdit()
{
	// TODO: Add your control notification handler code here
	int nItem = m_ScheduleListCtrl.GetSelectionMark();
	if(nItem == -1 )
		return;

	CString csDayType = m_ScheduleListCtrl.GetItemText(nItem, 1);
	CString csDate = m_ScheduleListCtrl.GetItemText(nItem, 2);
	CString csTimeFrom = m_ScheduleListCtrl.GetItemText(nItem, 3);
	CString csTimeTo = m_ScheduleListCtrl.GetItemText(nItem, 4);

	m_CDlgAddModifyRecTime.vSetScheduleTime(csDayType, csDate, csTimeFrom, csTimeTo);

	if(m_CDlgAddModifyRecTime.DoModal() == IDOK)
	{
		memcpy(&m_RecTimeInfo, &m_CDlgAddModifyRecTime.m_RecTimeInfo, sizeof(m_RecTimeInfo));
		ScheduleDirtyFlag = true;
	}
	else
		return;

	//CString csTimeFrom, csTimeTo;
	csTimeFrom.Format(_T("%s"), m_RecTimeInfo.cTimeFrom);
	csTimeTo.Format(_T("%s"), m_RecTimeInfo.cTimeTo);
	//csTimeFrom.Format(_T("%s%s"), m_RecTimeInfo.cTimeFrom[0], m_RecTimeInfo.cTimeFrom[1]);
	//csTimeTo.Format(_T("%s%s"), m_RecTimeInfo.cTimeTo[0], m_RecTimeInfo.cTimeTo[1]);
	
	m_ScheduleListCtrl.SetItemText(nItem, 1, (LPCTSTR)m_RecTimeInfo.cDayType);
	m_ScheduleListCtrl.SetItemText(nItem, 2, (LPCTSTR)m_RecTimeInfo.cDate);
	m_ScheduleListCtrl.SetItemText(nItem, 3, csTimeFrom);
	//m_ScheduleListCtrl.SetItemText(nItem, 4, _T("Till"));
	m_ScheduleListCtrl.SetItemText(nItem, 4, csTimeTo);
}

//void CDlgRecSchedule::vDBConnect()
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

void CDlgRecSchedule::vSetCameraIDX(int iCameraIDX)
{
	/*--Clean Dialog--*/
	m_ScheduleListCtrl.DeleteAllItems();

	/*-----Set Camera IDX------*/
	m_iCameraIDX = iCameraIDX;

	/*--Set Schedule List Control--*/
	vSetScheduleListCtrl(m_iCameraIDX);
}

void CDlgRecSchedule::vSetScheduleListCtrl(int m_iCameraIDX)
{
	m_ScheduleListCtrl.DeleteAllItems();

	try
	{
		CString csSQL_RecordingID, csRecordingID;
		csSQL_RecordingID.Format(_T("SELECT camera_recordingid FROM ec_camera_recording WHERE camera_idx = %d"), m_iCameraIDX);

		CRecordset rRecordingID(&((CMainFrame*)AfxGetMainWnd())->m_DB);
		rRecordingID.Open(CRecordset::forwardOnly, csSQL_RecordingID);
		while(!rRecordingID.IsEOF())
		{
			rRecordingID.GetFieldValue(_T("camera_recordingid"), csRecordingID);
			csRecordingID.TrimRight();
			rRecordingID.MoveNext();
		}
	
		CString SQL_RecSchedule;
		SQL_RecSchedule.Format(_T("SELECT day_type, CONVERT(VARCHAR(10), recording_date, 110) AS recording_date, CONVERT(VARCHAR(8), time_from, 108) AS time_from, CONVERT(VARCHAR(8), time_to, 108) AS time_to, active_ FROM ec_camera_recording_time WHERE camera_recordingid = '%s';"), csRecordingID);

		CString csDayType, csRecordingDate, csTimeFrom, csTimeTo, csActive;
		CRecordset rSchedule(&((CMainFrame*)AfxGetMainWnd())->m_DB);
		rSchedule.Open(CRecordset::forwardOnly, SQL_RecSchedule);
		while (!rSchedule.IsEOF()) { 
			rSchedule.GetFieldValue(_T("day_type"), csDayType);
			rSchedule.GetFieldValue(_T("recording_date"), csRecordingDate);
			rSchedule.GetFieldValue(_T("time_from"), csTimeFrom);
			rSchedule.GetFieldValue(_T("time_to"), csTimeTo);
			rSchedule.GetFieldValue(_T("active_"), csActive);

			csDayType.TrimRight();
			csRecordingDate.TrimRight();
			csTimeFrom.TrimRight();
			csTimeTo.TrimRight();
			csActive.TrimRight();
			
			csTimeFrom = csTimeFrom.Left(2);
			csTimeTo = csTimeTo.Left(2);

			int nItem = m_ScheduleListCtrl.InsertItem(0, _T(""));
			m_ScheduleListCtrl.SetItemText(nItem, 1, csINTtoDAY(csDayType));
			m_ScheduleListCtrl.SetItemText(nItem, 2, csRecordingDate);
			m_ScheduleListCtrl.SetItemText(nItem, 3, csTimeFrom);
			//m_ScheduleListCtrl.SetItemText(nItem, 4, _T("Till"));
			m_ScheduleListCtrl.SetItemText(nItem, 4, csTimeTo);

			if(csActive == _T("Y"))
				m_ScheduleListCtrl.SetCheck(nItem, 1);
			else
				m_ScheduleListCtrl.SetCheck(nItem, 0);

			rSchedule.MoveNext();
		}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

CString CDlgRecSchedule::csINTtoDAY(CString csDayType)
{
	if(csDayType == _T("1"))
		return _T("Everyday");
	else if(csDayType == _T("2"))
		return _T("Sunday");
	else if(csDayType == _T("3"))
		return _T("Monday");
	else if(csDayType == _T("4"))
		return _T("Tuesday");
	else if(csDayType == _T("5"))
		return _T("Wednesday");
	else if(csDayType == _T("6"))
		return _T("Thursday");
	else if(csDayType == _T("7"))
		return _T("Friday");
	else if(csDayType == _T("8"))
		return _T("Saturday");
	else if(csDayType == _T("9"))
		return _T("Specific Day");
	else
		return _T("");
}

void CDlgRecSchedule::vSaveRecTime(CString csRecordingID)
{
	try
	{
		for(int iTemp = 0; iTemp < m_ScheduleListCtrl.GetItemCount(); iTemp++)
		{
			CString csActive, csDayType, csDate, csTimeFrom, csTimeTo;

			/*--Get Recording Time Date from UI--*/
			if(m_ScheduleListCtrl.GetCheck(iTemp) == 1)
				csActive = _T("Y");
			else
				csActive = _T("N");
			
			csDayType = m_ScheduleListCtrl.GetItemText(iTemp, 1);
			csDate = m_ScheduleListCtrl.GetItemText(iTemp, 2);
			csTimeFrom.Format(_T("%s:00:00"), m_ScheduleListCtrl.GetItemText(iTemp, 3) );
			csTimeTo.Format(_T("%s:00:00"), m_ScheduleListCtrl.GetItemText(iTemp, 4) );;

			CString SQLInsertRecTime;
			if(csDate == _T(""))
				SQLInsertRecTime.Format(_T("DECLARE @iDayType smallint;  SELECT @iDayType = sort_order FROM ec_hdaytype WHERE daytype_desc = '%s';  INSERT INTO ec_camera_recording_time(camera_recordingid, active_, day_type, time_from, time_to) VALUES('%s', '%s', @iDayType, CONVERT(smalldatetime, '%s'), CONVERT(smalldatetime, '%s'));"),csDayType, csRecordingID, csActive, csTimeFrom, csTimeTo);
				//SQLInsertRecTime.Format(_T("INSERT INTO ec_camera_recording_time(camera_recordingid, active_, day_type, time_from, time_to) VALUES('%s', '%s', '%s', CONVERT(smalldatetime, '%s'), CONVERT(smalldatetime, '%s'))"), csRecordingID, csActive, csDAYtoINT(csDayType), csTimeFrom, csTimeTo);
			else
				SQLInsertRecTime.Format(_T("DECLARE @iDayType smallint;  SELECT @iDayType = sort_order FROM ec_hdaytype WHERE daytype_desc = '%s';  INSERT INTO ec_camera_recording_time(camera_recordingid, active_, day_type, recording_date, time_from, time_to) VALUES('%s', '%s', @iDayType, CONVERT(smalldatetime, '%s', 101), CONVERT(smalldatetime, '%s'), CONVERT(smalldatetime, '%s'));"), csDayType, csRecordingID, csActive, csDate, csTimeFrom, csTimeTo);
				//SQLInsertRecTime.Format(_T("INSERT INTO ec_camera_recording_time(camera_recordingid, active_, day_type, recording_date, time_from, time_to) VALUES('%s', '%s', '%s', CONVERT(smalldatetime, '%s', 101), CONVERT(smalldatetime, '%s'), CONVERT(smalldatetime, '%s'))"), csRecordingID, csActive, csDAYtoINT(csDayType), csDate, csTimeFrom, csTimeTo);
			(((CMainFrame*)AfxGetMainWnd())->m_DB).ExecuteSQL(SQLInsertRecTime);
		}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

BOOL CDlgRecSchedule::PreTranslateMessage(MSG *pMsg)
{
	if(m_ToolTip != NULL)
       m_ToolTip->RelayEvent(pMsg);

	if   (pMsg-> message   ==   WM_KEYDOWN) 
    { 
        if   (pMsg-> wParam   ==   VK_RETURN   ||   pMsg-> wParam   ==   VK_ESCAPE) 
            return   TRUE; 
    } 

    return   CDialog::PreTranslateMessage(pMsg); 
}