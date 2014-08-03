#include "stdafx.h"

#include "EventList.h"
#include "IntraEvent.h"

#define STARTID 3000


CEventList::CEventList(void):m_EventRec(NULL),hThreadEventRec(NULL)
{
	m_brush.CreateSolidBrush(BKCOLOR);
	m_nPageCount = 2;
	EventIDCount = 1;	//Event ID Count

}


CEventList::~CEventList(void)
{
	vector<CEventReceive*>::iterator pos;
	
	
	CSA csa;
	HANDLE hEndEvent = ::CreateEvent(&csa.m_sa, false, false, ETROCENTER_EVENT_END);
	::SetEvent(hEndEvent);
	CloseHandle(hThreadEventRec);

	::Sleep(1000);

}

BEGIN_MESSAGE_MAP(CEventList, CListCtrlEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENU_CLEAREVENTLOG, OnEventClearLog)
    ON_WM_DESTROY()
    ON_WM_SIZE()
END_MESSAGE_MAP()


int CEventList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	 m_MaxWidth = 200;
	 m_MaxHeight = 200;
	//DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP;
	if (CListCtrlEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	ofCreateEventList();

	CRect DisplayArea;
	int i, n;

	CListCtrlEx::PreSubclassWindow();
	//m_Parent = GetParent();
	//CString csTitle;
	////csTitle.LoadString(CGS::g_hResStr, IDS_BASE_TAB_EVENT);
	//LOADSTRING(csTitle, IDS_BASE_TAB_EVENT);
	////if (InsertItem(0, _T("Events"))==-1)
	///*if(InsertItem(0, csTitle)==-1)
	//{
	//	CString csMsg(_T(""));
	//	LOADSTRING(csMsg, IDS_INSERT_FAIL_0);
	//	AfxMessageBox(csMsg);
	//}*/

	////csTitle.LoadString(CGS::g_hResStr, IDS_BASE_TAB_PTZ);
	//LOADSTRING(csTitle, IDS_BASE_TAB_PTZ);
	///*if (InsertItem(1, csTitle)==-1)
	//{
	//	CString csMsg(_T(""));
	//	LOADSTRING(csMsg, IDS_INSERT_FAIL_1);
	//	AfxMessageBox(csMsg);
	//}*/
	
	m_CMainFrame =(CMainFrame*)AfxGetMainWnd(); 

	EventReceiveFlag = true;

	//hTabCursor = ::GetCursor();

	m_EventRec = new CEventReceive();
	
	UINT nRetEvent(0);
	//hThreadEventRec = (HANDLE)_beginthreadex(NULL, 0, GetEventFromService, (LPVOID)this, 0, &nRetEvent);

	m_EventRec->vSetCallBack(vCallBack, this);
	m_EventRec->vStartReceive();

	// TODO:  Add your specialized creation code here	
	return 0; 
}


BOOL CEventList::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CBrush brush( BKCOLOR );
	//CBrush brush(LISTFRONTCOLOR);
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &brush);
	return true;
}

HBRUSH CEventList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CListCtrlEx::OnCtlColor(pDC, pWnd, nCtlColor);

   //  We can use CWnd::GetDlgCtrlID() to perform the most efficient test.
	int ll_id = pWnd->GetDlgCtrlID();
	if (ll_id==STARTID+11) return hbr;
 
    // Set the text color to red
    pDC->SetTextColor(GLOBALFONTCOLOR);

    // Set the background mode for text to transparent so background will show thru.
    pDC->SetBkMode(TRANSPARENT);

    // Return handle to our CBrush object
    hbr = m_brush;
   
	return hbr;
}


void  CEventList::ofCreateEventList()
{
	CRect lrect;
	GetClientRect(lrect);
	DWORD	dwExtStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT ;
	SetExtendedStyle(GetExtendedStyle()|dwExtStyle);
	
	LVCOLUMN	lvColumn;
    CString csTitle;
    LOADSTRING(csTitle, IDS_EVENTLOG_COLUMN_TITLE);
    TCHAR* tzTitle[4];
    TCHAR tzTok[] =_T(";\r\n");
    tzTitle[0] = _tcstok(csTitle.LockBuffer(), tzTok);
    tzTitle[1] = _tcstok(NULL, tzTok);
    tzTitle[2] = _tcstok(NULL, tzTok);
    tzTitle[3] = _tcstok(NULL, tzTok);

	//Icon
	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = 0;	//20;	 //Hide the small icon //
	lvColumn.pszText = _T("");
	InsertColumn(0, &lvColumn);

	//LogID
	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = 60;
	lvColumn.pszText = tzTitle[0];
	InsertColumn(1, &lvColumn);

	//Camera
	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = 100;
	lvColumn.pszText = tzTitle[1];
	InsertColumn(2, &lvColumn);

	//Event Type
	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = 150;
	lvColumn.pszText =tzTitle[2];
	InsertColumn(3, &lvColumn);

	//Time
	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = 70;
	lvColumn.pszText = tzTitle[3];
	InsertColumn(4, &lvColumn);

	//lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	//lvColumn.fmt = LVCFMT_CENTER;
	//lvColumn.cx = 50;
	//lvColumn.pszText = _T("Alert");
	//pEventList->InsertColumn(5, &lvColumn);

	//m_images.Create(16, 16, ILC_COLOR8, 0, 3);
	m_images.Create(7, 28, ILC_COLOR24, 0, 3);

	CBitmap bIconRed, bIconOrange, bIconYellow;
	bIconRed.LoadBitmapW(IDB_REDCIRCLE);
	bIconOrange.LoadBitmapW(IDB_ORANGECIRCLE);
	bIconYellow.LoadBitmapW(IDB_YELLOWCIRCLE);

	m_images.Add(&bIconRed, RGB(255, 255, 255));
	m_images.Add(&bIconOrange, RGB(255, 255, 255));
	m_images.Add(&bIconYellow, RGB(0, 0, 0));

	SetImageList(&m_images, LVSIL_SMALL);

	//ofManualAddEventList();

}



void CEventList::vCallBack(EventInfo *m_EventInfo, void *lpParam)
{
	CEventList* pThis = (CEventList*)lpParam;
	

	CString csEventIP(_T("")), csEventContext(_T("")), csEventStreamID(_T("")), csEventCamIDX(_T(""));
	csEventIP.Format(_T("%s"), m_EventInfo->tcIP);

	switch(m_EventInfo->nEventType)
	{
	case EVENTTYPE::MOTION_DETECTION:
		//csEventContext.Format(_T("%s"), m_EventInfo->tcEventType);
		if(m_EventInfo->nStatus == EVENT_STATUS::OFF)
		{
			csEventContext = _T("Motion Detection OFF");
		}
		else if(m_EventInfo->nStatus == EVENT_STATUS::ON)
		{
			csEventContext = _T("Motion Detection ON");
		}
		
		break;
	case EVENTTYPE::DIGITAL_INPUT:
		if(m_EventInfo->nStatus == EVENT_STATUS::OFF)
		{
			csEventContext = _T("Digital Input OFF");
		}
		else if(m_EventInfo->nStatus == EVENT_STATUS::ON)
		{
			csEventContext = _T("Digital Input ON");
		}
		break;
	case EVENTTYPE::VIDEO_LOST:
		csEventContext = _T("Video Lost");
		break;
	case EVENTTYPE::VIDEO_OPEN:
		csEventContext = _T("Video Open");
		break;
	case EVENTTYPE::REBOOT:
		csEventContext = _T("Reboot");
		break;
	case EVENTTYPE::DIGITAL_OUTPUT:
		csEventContext = _T("Digital Output");
		break;
	case EVENTTYPE::MISSING_OBJECT:
		csEventContext = _T("Missing Object");
		break;
	case EVENTTYPE::EVENT_TRIGGER:
		csEventContext = _T("Event Trigger");
		break;
    case TRANSACTION:
        csEventContext = _T("Transaction");
        break;
    case RECORDING_FAIL:
        csEventContext = _T("Recording Status Change");
        break;
    case DISK_FULL:
        csEventContext = _T("Overwrite file");
        break;
    case LOG_IN:
        csEventContext = _T("Login");
        break;
    case LOG_OUT:
        csEventContext = _T("Logout");
        break;
    case DISCONNECT:
        csEventContext = _T("Disconnect");
        break;
	default:
        csEventContext = _T("EventId: %d", m_EventInfo->nEventType);
		ASSERT(false);
		break;
	}
	
	//csEventStreamID.Format(_T("%d"), m_EventInfo->nStream);
	csEventCamIDX.Format(_T("%d"), m_EventInfo->nCamIdx);

	if(csEventIP == _T(""))	return;
	
	//CString csDBEventCode = pThis->csGetEventTypeDB(csEventContext);

	//pThis->m_CameraEvent = pThis->GetCameraEventDB(csEventCamIDX, csDBEventCode, csEventStreamID);
	
	int nItem =0;
	CString csTime(_T("")), csEventID(_T("")), csEvent(_T(""));
	CTime timeTime = 0;
	if(pThis->GetItemCount() > pThis->m_nMaxCount)
	{
		pThis->DeleteItem(pThis->GetItemCount() -1);
	}

	if(pThis->EventIDCount > 100000)	pThis->EventIDCount = 1;
		csEventID.Format(_T("%06d"), pThis->EventIDCount);

	timeTime = CTime::GetCurrentTime();
	csTime.Format(_T("%02d:%02d:%02d"), timeTime.GetHour(), timeTime.GetMinute(), timeTime.GetSecond());

	//if( pThis->bCheckCameraExist(csEventIP) )//ignore this ,for performance thinking

	if (pThis->IsWindowVisible())
	{
		nItem = pThis->InsertItem(0, _T(""), (pThis->m_CameraEvent.iAlertLevel+1));
		pThis->SetItemText(nItem, 1, csEventID);
		pThis->SetItemText(nItem, 2, csEventIP);
		pThis->SetItemText(nItem, 3, csEventContext + _T(" ") + csEventStreamID);
		pThis->SetItemText(nItem, 4, csTime);
		pThis->UpdateWindow();

		pThis->EventIDCount++;
	}
	//CString csDBEventType = pThis->csGetEventTypeDB(csEventContext);

	//pThis->vChangeTreeCtrl(csEventIP, csEventContext + _T(" ") + csEventStreamID, csEventCamIDX, m_CameraEvent);
	//CString csSQLInsertEventLog;
	//csSQLInsertEventLog.Format(_T("INSERT INTO ec_event_log (eventid, event_type_code, alert_level, device_id, occur_time) VALUES('%s', '%s %s', '%d', '%s', '%s')"), csEventID, csDBEventType, csEventStreamID, m_CameraEvent.iAlertLevel, csEventCamIDX, csTime);
	//pThis->vInsertEventLogDB(csSQLInsertEventLog);
}

//CString CEventList::csEventCode(CString csCode)
//{
//	if(csCode == _T("m"))
//		return _T("Motion Detect");
//	else if(csCode == _T("i"))
//		return _T("Digital Input");
//	else if(csCode == _T("v"))
//		return _T("Video");
//	else if(csCode == _T("r"))
//		return _T("Reboot");
//	else if(csCode == _T("o"))
//		return _T("Digital Output");
//	else
//		return _T("Missing Object");
//}
//
void CEventList::vDeleteEventRec(CString csIP)
{
	
}

void CEventList::vEventReConnection(CString csIP)
{
	
}

//Popup a menu after mouse right click ...
void CEventList::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	//AfxMessageBox(_T("POPUP"));
	CMenu Menu;
	Menu.LoadMenu(IDR_MENU_EVENT_POPUP);

	CPoint Point = point;
	GetCursorPos(&Point);

	CMenu *pMenu = Menu.GetSubMenu(0);
	if (pMenu)
	pMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, Point.x,Point.y, this);
}

//CameraEvent CEventList::GetCameraEventDB(CString csEventCameraIDX, CString csEventType, CString csStreamID)
//{
//	CString csDSN, csDBUser, csDBPass, csODBC;
//
//	CameraEvent m_CameraEvent;
//
//	CString csAlertLevel;
//	//CDatabase DBQuery;
//
//	if(m_DB.IsOpen() == 0)	//Check DB is Open
//	{this->vDBConnect();}
//
//	try
//	{
//		//if( DBQuery.Open((csDSN), FALSE, FALSE, csODBC) )
//		//{/*AfxMessageBox(_T("Connection OK"));*/}
//		CString SQLAlertLevel;
//	//	SQLAlertLevel.Format(_T("SELECT camera_eventid, alert_level, event_source_code, device_action_code FROM ec_camera_event WHERE camera_idx = '%s' AND event_type_code = '%s' AND streamid = '%s'"), csEventCameraIDX, csEventType, csStreamID);
//		SQLAlertLevel.Format(_T("SELECT camera_eventid, alert_level, event_source_code, device_action_code FROM ec_camera_event WHERE camera_idx = '%s' AND event_type_code = '%s'"), csEventCameraIDX, csEventType);
//		CRecordset	rAlertLevel(&m_DB);
//  		rAlertLevel.Open(CRecordset::forwardOnly, SQLAlertLevel);
//		while (!rAlertLevel.IsEOF()) {
//			rAlertLevel.GetFieldValue(_T("camera_eventid"), m_CameraEvent.csEventID);
//			rAlertLevel.GetFieldValue(_T("alert_level"), csAlertLevel);
//			rAlertLevel.GetFieldValue(_T("event_source_code"), m_CameraEvent.csEventSourceCode);
//			rAlertLevel.GetFieldValue(_T("device_action_code"), m_CameraEvent.csDeviceActionCode);
//			m_CameraEvent.csEventID.TrimRight();
//			csAlertLevel.TrimRight();
//			//AfxMessageBox(Model);
//			rAlertLevel.MoveNext();
//		//}
//		//DBQuery.Close();
//		} //while
//
//	} 
//	catch(CDBException * p)
//	{
//		AfxMessageBox(p->m_strError);
//		p->Delete();
//		//DBQuery.Close();
//	}
//
//	if(csAlertLevel == _T(""))
//	{
//		m_CameraEvent.csEventID = _T("0");
//		m_CameraEvent.iAlertLevel = 3;
//		m_CameraEvent.csEventSourceCode = _T("CAMERA");
//	}
//	else 
//	{
//		int iAlertLevel = _ttoi (csAlertLevel.GetBuffer(0));
//		m_CameraEvent.iAlertLevel = iAlertLevel;
//	}
// 
//	return m_CameraEvent;
//}

//CString CEventList::csGetEventAlarm(int iAlertLevel)
//{
//	CString csAlarmLevel;
//	CString csDSN, csDBUser, csDBPass, csODBC;
//
//	if(m_DB.IsOpen() == 0)	//Check DB is Open 
//	{this->vDBConnect();}
//
//	try
//	{
//		CString SQLAlertLevel;
//		SQLAlertLevel.Format(_T("SELECT alarm_level_type FROM ec_hevent_alarm WHERE sort_order = '%d'"), iAlertLevel);
//		CRecordset rAlertLevel(&m_DB);//&DBQuery);
//		rAlertLevel.Open(CRecordset::forwardOnly, SQLAlertLevel);
//		while (!rAlertLevel.IsEOF()) {
//			rAlertLevel.GetFieldValue(_T("alarm_level_type"), csAlarmLevel);
//			csAlarmLevel.TrimRight();
//			//AfxMessageBox(Model);
//			rAlertLevel.MoveNext();
//		}
//	}
//	catch(CDBException * p)
//	{
//		AfxMessageBox(p->m_strError);
//		p->Delete();
//	}
//
//	if(csAlarmLevel == _T(""))
//		return _T("No Match Alarm Level");
//
//	return csAlarmLevel;
//}
//
//void CEventList::vInsertEventLogDB(CString SQLInsert)
//{
//	CString csDSN, csDBUser, csDBPass, csODBC;
//
//	if(m_DB.IsOpen() == 0)	//Check DB is Open
//	{this->vDBConnect();}
//
//	try
//	{
//		TRACE(SQLInsert);
//		m_DB.ExecuteSQL(SQLInsert);
//	}
//	catch(CDBException * p)
//	{
//		AfxMessageBox(p->m_strError);
//		p->Delete();
//	}
//}
//
//CString CEventList::csGetEventTypeDB(CString csCGIEventType)
//{
//	CString csDBEventType;
//	CString csDSN, csDBUser, csDBPass, csODBC;
//
//	if(m_DB.IsOpen() == 0)	//Check DB is Open
//	{this->vDBConnect();}
//
//	try
//	{
//		CString SQLEventTypeCode;
//		SQLEventTypeCode.Format(_T("SELECT event_type_code FROM ec_hevent_type WHERE event_type_desc = '%s'"), csCGIEventType);
//		//CRecordset rEventTypeCode(&DBQuery);
//		CRecordset rEventTypeCode(&m_DB);
//		rEventTypeCode.Open(CRecordset::forwardOnly, SQLEventTypeCode);
//		while (!rEventTypeCode.IsEOF()) {
//			rEventTypeCode.GetFieldValue(_T("event_type_code"), csDBEventType);
//			csDBEventType.TrimRight();
//			//AfxMessageBox(Model);
//			rEventTypeCode.MoveNext();
//		} //Try
//	}
//	catch(CDBException * p)
//	{
//		AfxMessageBox(p->m_strError);
//		p->Delete();
//	}
//
//	return csDBEventType;
//}
//
//void CEventList::OnEventPopSetting()
//{
//	DlgSettingEvent lDlg;
//	lDlg.DoModal();
//}

void CEventList::OnEventClearLog()
{
    DeleteAllItems();
// mark by Javan, it's too terrible, delete all database event log
/*
    //CString ls_confirm = _T("Are you sure to clear all the events log from database ?");
    int ll_ret = AfxMessageBox(IDS_CLEAR_EVENT_DB, MB_YESNO|MB_ICONQUESTION);
    if (ll_ret==IDYES) 	
    {				
	    try
	    {
		    //Clear DB log
		    CString ls_sql;
		    ls_sql = _T("delete from ec_event_log where 1=1");
		    m_DB.ExecuteSQL(ls_sql);
		    DeleteAllItems();

	    }
	    catch(CDBException * p)
	    {
		    AfxMessageBox(p->m_strError);
		    p->Delete();
	    }
    }
*/	
}





bool CEventList::vDBConnect(void)
{
	try {
		
		CIniReader m_ini;
		m_ini.setINIFullFileName(_T("\\ec.ini"));
		
		CString cpDns = m_ini.getKeyValue(_T("DSN"),_T("database"));
		CString cpUid = m_ini.getKeyValue(_T("UID"),_T("database"));
		CString cpPwd = m_ini.getKeyValue(_T("PWD"),_T("database"));
	 
		CString ls_connect;
		ls_connect.Format(_T("ODBC;UID=%s;PWD=%s"), cpUid, cpPwd);
		return m_DB.Open((LPCTSTR)cpDns, FALSE, FALSE, ls_connect );
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
	return true;
}
//bool CEventList::bCameraEnable(CString csCamIDX)
//{
//	if(m_DB.IsOpen() == 0)	//Check DB is Open
//	{this->vDBConnect();}
//
//	try
//	{
//		CString csActive, csSQL;
//		csSQL.Format(_T("SELECT * FROM ec_camera WHERE camera_idx = '%s'"), csCamIDX);
//		CRecordset rCameraEnable(&m_DB);
//		rCameraEnable.Open(CRecordset::forwardOnly, csSQL);
//		while (!rCameraEnable.IsEOF()) {
//			rCameraEnable.GetFieldValue(_T("active_"), csActive);
//			csActive.TrimRight();
//			rCameraEnable.MoveNext();
//		} 
//
//		if(csActive == _T("Y"))return true;
//		else return false;
//	}
//	catch(CDBException * p)
//	{
//		AfxMessageBox(p->m_strError);
//		p->Delete();
//	}
//
//	return true;
//}
//
bool CEventList::bCheckCameraExist(CString csIP)
{
	try
	{
		CString csSQL, csValue;
		csSQL.Format(_T("SELECT * FROM ec_camera WHERE ipaddress = '%s'"), csIP);
	
		CRecordset rCheckCam(&m_DB);
		rCheckCam.Open(CRecordset::forwardOnly, csSQL);
		while(!rCheckCam.IsEOF()) {

			rCheckCam.GetFieldValue(_T("camera_idx"), csValue);
			csValue.TrimRight();

			rCheckCam.MoveNext();
		}

		if(csValue.GetLength() == 0)
		{
			return false;
		}

	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
		return false;
	}

	return true;
}
void CEventList::OnDestroy()
{
	m_EventRec->vStopEventReceive();
    CListCtrlEx::OnDestroy();
}

void CEventList::OnSize(UINT nType, int cx, int cy)
{
    CListCtrlEx::OnSize(nType, cx, cy);
    m_nMaxCount = GetCountPerPage();

    // TODO: Add your message handler code here
}
