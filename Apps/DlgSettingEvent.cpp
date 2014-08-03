// DlgSettingEvent.cpp : implementation file
//

#include "stdafx.h"
//#include "cgi.h"
#include "DlgSettingEvent.h"
#include "DlgCamSelect.h"
#include "INI.h"
#include "MainFrm.h"
#include "DlgSettingRecording.h"
#include "DlgFtp.h"
const int MAXBUF = 4096;


// DlgSettingEvent dialog

IMPLEMENT_DYNAMIC(DlgSettingEvent, CBaseDialog)

HBRUSH DlgSettingEvent::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

BOOL DlgSettingEvent::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CBaseDialog::OnEraseBkgnd(pDC);
}


DlgSettingEvent::DlgSettingEvent(CWnd* pParent /*=NULL*/)
	: CBaseDialog(DlgSettingEvent::IDD, pParent),m_nChildCount(0),m_importEventInfo(NULL),m_bImportEventSave(false),m_memEvent(NULL)
	
{
	nMd=nInput=nOutput=nParam=nPTZParam=nODParam= 1;
	m_treectrl = new CTreeCtrl;
	cEventRuleCounter=0;
	m_currMode.Empty();
	m_DisplayedEvent.clear();
	

}

DlgSettingEvent::~DlgSettingEvent()
{
	//m_DB.Close();
	delete m_treectrl;
	ScanMap.clear();
	EventInfoMap.clear();
	if(m_ToolTip) delete m_ToolTip;
	
	vFreeMemory();
	
}

void DlgSettingEvent::vFreeMemory()
{
	for(int i=0;i<m_EventInfoStruct.size();i++)
	{
			if(m_EventInfoStruct[i])
			delete m_EventInfoStruct[i];
	}
	m_EventInfoStruct.clear();

	for(int i=0;i<m_strEVENTTYPERecycleBin.size();i++)
	{
			if(m_strEVENTTYPERecycleBin[i])
			delete m_strEVENTTYPERecycleBin[i];
	}
	m_strEVENTTYPERecycleBin.clear();

	for(int i=0;i<m_strEVENTACTIONRecycleBin.size();i++)
	{
			if(m_strEVENTACTIONRecycleBin[i])
			delete m_strEVENTACTIONRecycleBin[i];
	}
	m_strEVENTACTIONRecycleBin.clear();

	for(int i=0;i<m_EventInfoStructRecycleBin.size();i++)
	{
			if(m_EventInfoStructRecycleBin[i])
			delete m_EventInfoStructRecycleBin[i];
	}
	m_EventInfoStructRecycleBin.clear();

	for(int i =0;i<m_LVITEMRecycleBin.size();i++)
	{
		if(m_LVITEMRecycleBin[i])
			delete m_LVITEMRecycleBin[i];		
	}
	m_LVITEMRecycleBin.clear();
}

void DlgSettingEvent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DLG_SETTING_EVENT_TREE1, *m_treectrl);
    // IDC_BUTTONADD,IDC_BUTTONDEL id not found
	//DDX_Control(pDX, IDC_BUTTONADD, m_ButAdd);
	//DDX_Control(pDX, IDC_BUTTONDEL, m_ButDel);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_dateTimePickerFrom);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_dateTimePickerTo);
}


BEGIN_MESSAGE_MAP(DlgSettingEvent, CDialog)
	ON_BN_CLICKED(IDC_RADIO_SETTING_EVENT_IPCAM, &DlgSettingEvent::OnBnClickedRadioSettingEventIPCam)
	ON_BN_CLICKED(IDC_CHECK_SETTING_EVENT_ENABLE, &DlgSettingEvent::OnBnClickedCheckSettingEventEnable)
	
	//ON_LBN_SELCHANGE(IDC_LIST_EVENT_TYPE, &DlgSettingEvent::OnLbnSelchangeListEventType)

	//ON_EN_CHANGE(IDC_EDIT2, &DlgSettingEvent::OnEnChangeEdit2)
	ON_BN_CLICKED(IDOK, &DlgSettingEvent::OnBnClickedOk)
//	ON_LBN_SELCHANGE(IDC_EVENT_ACTION, &DlgSettingEvent::OnLbnSelchangeEventAction)
	ON_NOTIFY(NM_CLICK, IDC_DLG_SETTING_EVENT_TREE1, &DlgSettingEvent::OnNMClickDlgSettingEventTree1)
//	ON_NOTIFY(TVN_ITEMCHANGED, IDC_DLG_SETTING_EVENT_TREE1, &DlgSettingEvent::OnTvnSelchangingDlgSettingEventTree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_DLG_SETTING_EVENT_TREE1, &DlgSettingEvent::OnTvnSelchangedDlgSettingEventTree1)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SETEVENT_BUTADD1, &DlgSettingEvent::OnBnClickedSeteventButadd1)
	ON_BN_CLICKED(IDC_SETEVENT_BUTDEL1, &DlgSettingEvent::OnBnClickedSeteventButdel1)
//	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_EVENT_TYPE, &DlgSettingEvent::OnLvnColumnclickListEventType)
//ON_NOTIFY(NM_CLICK, IDC_LIST_EVENT_TYPE, &DlgSettingEvent::OnNMClickListEventType)
//ON_NOTIFY(HDN_ITEMCHANGED, 0, &DlgSettingEvent::OnHdnItemchangedListEventType)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_EVENT_TYPE, &DlgSettingEvent::OnLvnItemchangedListEventType)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_EVENT_ACTION, &DlgSettingEvent::OnLvnItemchangedEventAction)
ON_BN_CLICKED(IDC_BUTSELCAM, &DlgSettingEvent::OnBnClickedButselcam)
ON_BN_CLICKED(IDC_RADIODIGITALIN, &DlgSettingEvent::OnBnClickedRadiodigitalin)
ON_BN_CLICKED(IDC_RADIO_SETTING_EVENT_SOURCE_SYSTEM, &DlgSettingEvent::OnBnClickedRadioSettingEventSourceSystem)
ON_NOTIFY(TVN_SELCHANGING, IDC_DLG_SETTING_EVENT_TREE1, &DlgSettingEvent::OnTvnSelchangingDlgSettingEventTree1)
ON_BN_CLICKED(IDC_RADIO_WARNING, &DlgSettingEvent::OnBnClickedRadioWarning)
ON_BN_CLICKED(IDC_RADIO_CRITICAL, &DlgSettingEvent::OnBnClickedRadioCritical)
ON_BN_CLICKED(IDC_RADIO_INFO, &DlgSettingEvent::OnBnClickedRadioInfo)
//ON_EN_CHANGE(IDC_EDIT_SETTING_EVENT_EVENT_NAME, &DlgSettingEvent::OnEnChangeEditSettingEventEventName)
ON_BN_CLICKED(IDC_BUTSAVE, &DlgSettingEvent::OnBnClickedButsave)
ON_BN_CLICKED(IDC_BUTIMPORT, &DlgSettingEvent::OnBnClickedButimport)
ON_BN_CLICKED(IDC_BUTEXPORT, &DlgSettingEvent::OnBnClickedButexport)
ON_BN_CLICKED(IDC_BUTTON_SETTING_EVENT_IMPORT_ALL, &DlgSettingEvent::OnBnClickedButtonSettingEventImportAll)
ON_WM_CLOSE()
ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, &DlgSettingEvent::OnDtnDatetimechangeDatetimepicker2)
ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER3, &DlgSettingEvent::OnDtnDatetimechangeDatetimepicker3)
ON_BN_CLICKED(IDC_BUTSETTING, &DlgSettingEvent::OnBnClickedButsetting)
ON_EN_CHANGE(IDC_EDIT_SETTING_EVENT_EVENT_NAME, &DlgSettingEvent::OnEnChangeEditSettingEventEventName)
ON_EN_CHANGE(IDC_EDIT_SETTING_EVENT_OD, &DlgSettingEvent::OnEnChangeEditSettingEventOd)
ON_EN_CHANGE(IDC_EDIT_SETTING_EVENT_PTZ, &DlgSettingEvent::OnEnChangeEditSettingEventPtz)
//ON_NOTIFY(NM_CLICK, IDC_LIST_EVENT_TYPE, &DlgSettingEvent::OnNMClickListEventType)
END_MESSAGE_MAP()



BOOL DlgSettingEvent::OnInitDialog()
{
	CString csDialogCaption(_T(""));
	LOADSTRING(csDialogCaption, IDS_DLG_EVENT_SETTING);
	this->SetWindowTextW(csDialogCaption);

	int ll_camidx;

	LOADSTRING(m_MaxMessage, IDS_EVENT_REACH_MAX);
	//m_MaxMessage = _T("You had reached the maximum number of event per IPCAM - 6. ");
	LOADSTRING(m_SaveMessage, IDS_EVENT_SAVE);
	//m_SaveMessage = _T("Event had been changed, save the changes ?");

	CBaseDialog::OnInitDialog();

	m_treectrl =  (CTreeCtrl *)this->GetDlgItem(IDC_DLG_SETTING_EVENT_TREE1);

//	vDBConnect();
	pDB = &(((CMainFrame*)AfxGetMainWnd())->m_DB);

	//ToolTips
	m_ToolTip= new CToolTipCtrl();
	m_ToolTip->Create(this);
	CString ls_st;
	LOADSTRING(ls_st, IDS_TOOL_ADD_EVENT);
	m_ToolTip->AddTool(&m_ButAdd, ls_st);

	LOADSTRING(ls_st, IDS_TOOL_DEL_EVENT);
	m_ToolTip->AddTool(&m_ButDel, ls_st);

	LOADSTRING(ls_st, IDS_TOOL_IMPORT_EVENT);
	m_ToolTip->AddTool(&m_ButImport,ls_st);

	LOADSTRING(ls_st, IDS_TOOL_EXPORT_EVENT);
	m_ToolTip->AddTool(&m_ButExport, ls_st);

	LOADSTRING(ls_st, IDS_TOOL_IMPORT_ALL_EVENT);
	m_ToolTip->AddTool(&m_ButImportAll, ls_st);
	
	//m_ToolTip->AddTool(&m_ButAdd,_T("Add New Event"));
	//m_ToolTip->AddTool(&m_ButDel,_T("Delete Event"));
	//m_ToolTip->AddTool(&m_ButImport,_T("Import Events From IPCAM"));
	//m_ToolTip->AddTool(&m_ButExport,_T("Export Events to IPCAM"));
	//m_ToolTip->AddTool(&m_ButImportAll,_T("Import All Events from IPCAM"));
	m_ToolTip->Activate(TRUE);

	//List Event Type
	m_ListEventType = (CListCtrl *)this->GetDlgItem(IDC_LIST_EVENT_TYPE);
	DWORD dwStyle = m_ListEventType->GetStyle();
	dwStyle |= LVS_EX_CHECKBOXES |LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_ListEventType->SetExtendedStyle(dwStyle);

	//List Event Action
	m_ListAction = (CListCtrl *)this->GetDlgItem(IDC_EVENT_ACTION);
	dwStyle = m_ListAction->GetStyle();
	dwStyle |= LVS_EX_CHECKBOXES |LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_ListAction->SetExtendedStyle(dwStyle);

	m_CamSel = (CBitmapButton*)this->GetDlgItem(IDC_BUTSELCAM);
 
	vRefreshEventTree();

	if (m_entryCamidx>0) vFindCam();

	
	m_dateTimePickerFrom.SetFormat(_T("HH:00"));
	m_dateTimePickerTo.SetFormat(_T("HH:00"));
	//set datetimepicker as 10:00 as default
	CTime time(2011,1,1,10,0,0);
	m_dateTimePickerFrom.SetTime(&time);
	m_dateTimePickerTo.SetTime(&time);

	return TRUE;
}


void DlgSettingEvent::OnBnClickedRadioSettingEventIPCam()
{
		mb_EventDirty  = true;
		vPopulateType(_T("CAMERA"), _T(""));
		((CButton*)GetDlgItem(IDC_RADIO_SETTING_EVENT_SOURCE_SYSTEM))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIODIGITALIN))->SetCheck(0);

}

void DlgSettingEvent::OnBnClickedCheckSettingEventEnable()
{
	mb_EventDirty = true;
}


void DlgSettingEvent::OnBnClickedOk()
{
	//vCGItoCam();  Handle this by Import instead
	HTREEITEM hItem = m_treectrl->GetSelectedItem();
	if(hItem != NULL)
	vSaveDB(hItem);
	
	// TODO: Add your control notification handler code here
	OnOK();
}


void DlgSettingEvent::OnNMClickDlgSettingEventTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	
}

void DlgSettingEvent::OnTvnSelchangedDlgSettingEventTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	HTREEITEM  hItem = m_treectrl->GetSelectedItem();
	HTREEITEM hItemChild = m_treectrl->GetChildItem(hItem);


	// if there's no Event in a IPCAM, delete Event Type and Action items and other inputs
	if(hItemChild == NULL)
	{
		m_ListEventType->DeleteAllItems();
		m_ListAction->DeleteAllItems();
		vRadioClear(true,true,true);
		((CEdit*)GetDlgItem(IDC_EDIT_SETTING_EVENT_EVENT_NAME) )->SetWindowTextW(_T(""));
		((CEdit*)GetDlgItem(IDC_EDIT_INTERVAL))->SetWindowTextW(_T(""));
		((CButton*)GetDlgItem(IDC_CHECK_SETTING_EVENT_ENABLE))->SetCheck(0);
		((CEdit*)GetDlgItem(IDC_EDITID) )->SetWindowTextW(_T(""));
			//set datetimepicker as 10:00 as default
		CTime time(2011,1,1,10,0,0);
		m_dateTimePickerFrom.SetTime(&time);
		m_dateTimePickerTo.SetTime(&time);
		((CStatic*)GetDlgItem(IDC_ST_ACTIONINFO))->SetWindowTextW(_T(""));
			
	}

	// Hide the Output duration and PTZ present edit box first
	((CEdit*)GetDlgItem(IDC_EDIT_SETTING_EVENT_OD))->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_OUTPUTDURATION)->ShowWindow(SW_HIDE);
	((CEdit*)GetDlgItem(IDC_EDIT_SETTING_EVENT_PTZ))->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PTZ)->ShowWindow(SW_HIDE);

	// disable select CAM button first
	((CButton*)GetDlgItem(IDC_BUTSELCAM))->EnableWindow(0);

	if (m_treectrl->GetParentItem(hItem) == NULL) //Camera Items
	{
 	}
	else //Event Item
	{
		int	llEventId, llLevel;
		CString ls_name, ls_source, ls_type, ls_action;
		TVITEM TreeItem;
		TreeItem.hItem = hItem;
		TreeItem.mask = TVIF_HANDLE ;
		m_treectrl->GetItem(&TreeItem);
		SettingEventInfo* lpEventInfo = (SettingEventInfo*) TreeItem.lParam;

		if (!lpEventInfo) return;   //Add New Event...stop here...

		//Set the parent camera index
		m_parent_camidx =  lpEventInfo->CameraIdx;

		//Event ID
		llEventId =  lpEventInfo->EventId;
		if (llEventId > 0 )  {
				CString lsID;
				lsID.Format(_T("%d"),llEventId );
				((CEdit*)GetDlgItem(IDC_EDITID))->SetWindowTextW(lsID);
			}

		//Event Name
		ls_name = lpEventInfo->csEventName;
		((CEdit*)GetDlgItem(IDC_EDIT_SETTING_EVENT_EVENT_NAME) )->SetWindowTextW(ls_name.Trim());

		//Source			
		ls_source = (lpEventInfo->csEventSource).Trim();
		vSelectRadioSource(ls_source );
	
		//Populate Types based on the source, and checked on the 
		ls_type = (lpEventInfo->csEventType).Trim();
		ls_action = (lpEventInfo->csEventAction).Trim();
		vPopulateType(ls_source, ls_type, ls_action);

			
		//Alert Level
		llLevel = lpEventInfo->AlertLevel;
		vSelectRadioAlert(llLevel);

		//Active
		if ((lpEventInfo->cActive).Trim()==_T("N"))
			((CButton*)GetDlgItem(IDC_CHECK_SETTING_EVENT_ENABLE))->SetCheck(0);
		else
			((CButton*)GetDlgItem(IDC_CHECK_SETTING_EVENT_ENABLE))->SetCheck(1);

		//Target Camera Info & PTZPoint
		int ll_target = lpEventInfo->TargetCamIdx;
		int ll_pPoint = lpEventInfo->PTZPoint;
		vPopulateTargetCam(ll_target, ll_pPoint);
 	
		//Interval
		int ll_Interval = lpEventInfo->Interval;
		CString ls_Interval;
		ls_Interval.Format(_T("%d"), ll_Interval );
		((CEdit*)GetDlgItem(IDC_EDIT_INTERVAL))->SetWindowTextW(ls_Interval);

		// Duration
		  // eventid = -1 that is event import from IPCAM, if it's duration is always then set radio
		if(llEventId == -1 || lpEventInfo->csDuration == _T("Always"))
		{
			((CButton*)GetDlgItem(IDC_RADIO_ALWAYS_VALID))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_VALID_TIME) )->SetCheck(0);
		}

		  // else populate duration info from DB
		else vPopulateDuration(llEventId,lpEventInfo);

		// Display PTZ Preset number or Digital output duration if exists
		if(lpEventInfo->csParam.Trim() != _T(""))
		{
			if(ls_action == _T("PTZ"))
		
			((CEdit*)GetDlgItem(IDC_EDIT_SETTING_EVENT_PTZ))->SetWindowTextW(lpEventInfo->csParam);

			else if (ls_action == _T("DO"))
						((CEdit*)GetDlgItem(IDC_EDIT_SETTING_EVENT_OD))->SetWindowTextW(lpEventInfo->csParam);
		}
	}
	mb_EventDirty = false;
	m_currMode = "";
	
 	*pResult = 0;
}  //OnTvnSelchangedDlgSettingEventTree1


bool DlgSettingEvent::vAddDBEvent(int o_cameraIdx,  HTREEITEM o_parentTreeItem)
{
	CString ls_desc, ls_sql, ls_EventName, ls_EventSource, ls_EventType, ls_Active, ls_Action, ls_IP, ls_Pass, ls_Username, csDuration,
	csDurationFrom, csDurationTo; 
	CDBVariant varEventId, varStreamId, varLevel, varTargetCam, varPoint, varInterval;
	TVINSERTSTRUCTW itemStru={0};


	ls_sql.Format(_T("SELECT camera_eventid,streamid,alert_level,event_name,event_source_code, event_type_code,device_action_code,target_camera_idx,ptz_point,interval_,cycle_type, CONVERT(VARCHAR(8), cycle_from, 8) AS cycle_from, CONVERT(VARCHAR(8), cycle_till, 8) AS cycle_till FROM ec_camera_event WHERE camera_idx= %d  ORDER BY  sort_order "), o_cameraIdx );

	CRecordset m_rsEvent(pDB);		
	
	m_rsEvent.Open(CRecordset::forwardOnly, ls_sql);

	while (!m_rsEvent.IsEOF())
	{
			SettingEventInfo* structEventInfo = new SettingEventInfo;
			
			//Camera Index
			structEventInfo->CameraIdx = o_cameraIdx;

			m_rsEvent.GetFieldValue(_T("camera_eventid"), varEventId, SQL_C_SSHORT);
			structEventInfo->EventId = varEventId.m_iVal;

			m_rsEvent.GetFieldValue(_T("streamid"), varStreamId, SQL_C_SSHORT);	
			structEventInfo->StreamId = varStreamId.m_iVal;
			
			//Alert Level
			m_rsEvent.GetFieldValue(_T("alert_level"), varLevel, SQL_C_SSHORT);	
			structEventInfo->AlertLevel = varLevel.m_iVal;

			m_rsEvent.GetFieldValue(_T("event_name"), ls_EventName);
			structEventInfo->csEventName = ls_EventName.Trim();

			m_rsEvent.GetFieldValue(_T("event_source_code"), ls_EventSource);
			structEventInfo->csEventSource = ls_EventSource.Trim();

			m_rsEvent.GetFieldValue(_T("event_type_code"), ls_EventType);
			structEventInfo->csEventType = ls_EventType.Trim();

			m_rsEvent.GetFieldValue(_T("device_action_code"), ls_Action);
			structEventInfo->csEventAction = ls_Action.Trim();

			//Target Camera Index
			m_rsEvent.GetFieldValue(_T("target_camera_idx"), varTargetCam, SQL_C_SSHORT);	
			structEventInfo->TargetCamIdx = varTargetCam.m_iVal;

				m_rsEvent.GetFieldValue(_T("ptz_point"), varPoint, SQL_C_SSHORT);	
				structEventInfo->PTZPoint = varPoint.m_iVal;

				m_rsEvent.GetFieldValue(_T("interval_"), varInterval, SQL_C_SSHORT);	
				structEventInfo->Interval = varInterval.m_iVal;

			
			// Duration	
			m_rsEvent.GetFieldValue(_T("cycle_type"), csDuration);
			structEventInfo->csDuration = csDuration.Trim();
			
			m_rsEvent.GetFieldValue(_T("cycle_from"), csDurationFrom);
			structEventInfo->csDurationFrom = csDurationFrom.Trim();

			m_rsEvent.GetFieldValue(_T("cycle_till"), csDurationTo);
			structEventInfo->csDurationTo = csDurationTo.Trim();

			itemStru.item.mask = TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM ;
			itemStru.item.pszText = (LPWSTR)(LPCTSTR)ls_desc;
			itemStru.item.cchTextMax =ls_desc.GetLength();

			itemStru.item.lParam = (LPARAM)structEventInfo ;

			ls_desc.Format( _T("%s - %s/%s"),ls_EventName.Trim(), ls_EventType.Trim(), ls_Action.Trim());

			HTREEITEM lItem = m_treectrl->InsertItem(itemStru.item.mask, (LPCTSTR)ls_desc, 0,0,0,0, (LPARAM)structEventInfo , o_parentTreeItem, TVI_LAST);
			m_rsEvent.MoveNext();
			m_EventInfoStructRecycleBin.push_back(structEventInfo);
	}
	return false;
}

int DlgSettingEvent::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	/*DWORD dwStyle = WS_CHILD | WS_VISIBLE |  WS_THICKFRAME | BS_OWNERDRAW | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | WS_BORDER | TVS_SHOWSELALWAYS;; 
	m_treectrl->Create(  dwStyle, CRect(20, 40,280,560), this, IDC_DLG_SETTING_EVENT_TREE1);*/
	// TODO:  Add your specialized creation code here
	int  ll_x = 135,  ll_y=10, lsize=24;
	
	DWORD dwStyle = WS_CHILD|WS_VISIBLE|BS_OWNERDRAW ;

	//Export All button
	m_ButImportAll.Create(NULL, dwStyle,CRect(ll_x, ll_y, ll_x + lsize, ll_y+lsize),this, IDC_BUTTON_SETTING_EVENT_IMPORT_ALL);
	m_ButImportAll.LoadBitmaps( IDB_REFRESH,IDB_REFRESH,IDB_REFRESH);
	ll_x = ll_x + 30;
	//m_ButImportAll.ShowWindow(SW_HIDE);

	//Import button
	m_ButImport.Create(NULL, dwStyle,CRect(ll_x, ll_y, ll_x + lsize, ll_y+lsize),this, IDC_BUTIMPORT);
	m_ButImport.LoadBitmaps( IDB_IMPORT,IDB_IMPORT,IDB_IMPORT);
	ll_x = ll_x + 30;

	//Export Button
	m_ButExport.Create(NULL, dwStyle,CRect(ll_x, ll_y, ll_x + lsize, ll_y+lsize),this, IDC_BUTEXPORT);
	m_ButExport.LoadBitmaps( IDB_EXPORT, IDB_EXPORT,IDB_EXPORT);
			ll_x = ll_x + 30;

	m_ButAdd.Create(NULL, dwStyle,CRect(ll_x, ll_y, ll_x + lsize, ll_y+lsize),this, IDC_SETEVENT_BUTADD1);
	m_ButAdd.LoadBitmaps( IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD);

	ll_x = ll_x + 30;
	m_ButDel.Create(NULL, dwStyle,CRect(ll_x, ll_y, ll_x + lsize, ll_y+lsize),this, IDC_SETEVENT_BUTDEL1);
	m_ButDel.LoadBitmaps( IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL);

	/*m_CamSel->Create(NULL, dwStyle, CRect(300,400,324,424), this, IDC_BUTSELCAM);
	m_CamSel->LoadBitmaps(IDB_CAM24,IDB_CAM24,IDB_CAM24, IDB_CAM24);*/

	return 0;
}


void DlgSettingEvent::OnBnClickedSeteventButadd1()
{
	// TODO: Add your control notification handler code here
	fnCheckToSave(false);
	vAddNewEvent();
}


void DlgSettingEvent::OnBnClickedSeteventButdel1()
{
	// TODO: Add your control notification handler code here
	vDeleteEvent();
}

void DlgSettingEvent::vPopulateType(CString o_Source, CString o_Type, CString o_Action)
{
		//Populate Event type Based on the source... Mostly triggered by 
	m_ListEventType->DeleteAllItems(); //.ResetContent();
	CString csDBIpCamEventTypeDisplay, csDesc, csCode;
	CString ls_sql, csArea;
 
	try
	{
		ls_sql.Format(  _T("SELECT * FROM ec_hevent_type WHERE event_source_code ='%s'  ORDER BY sort_order"), o_Source);
		CRecordset m_rs(pDB);

		m_rs.Open(CRecordset::dynaset, ls_sql);

			int nDBIpCamEventType = m_rs.GetRecordCount();

			int ci=0;
			while (!m_rs.IsEOF())
			{
				strEVENTTYPE* structEventType = new strEVENTTYPE;
				m_rs.GetFieldValue(_T("event_type_code"), csCode);
				m_rs.GetFieldValue(_T("event_type_desc"), csDesc);
				m_rs.GetFieldValue(_T("define_area"), csArea);				

				LVITEMW* pLitem = new LVITEM;
				pLitem->mask = LVIF_PARAM|LVIF_TEXT;
				pLitem->iItem = ci;
				pLitem->pszText = (LPWSTR)(LPCTSTR)csDesc;
				structEventType->csTypeCode = csCode;
				structEventType->csDefineArea = csArea.Trim();

				pLitem->lParam = (LPARAM) structEventType;
 
				m_ListEventType->InsertItem(pLitem->mask , ci, csDesc, 0,0,0, 	pLitem->lParam);
				o_Type = o_Type.Trim();

				//Make the item checked if  is the passed Type
				if (o_Type!=_T("") && (o_Type==csCode.Trim() || o_Type==csDesc.Trim()))
				{
					m_ListEventType->SetCheck(ci, TRUE);
					vPopulateAction(o_Type,  o_Action);
				}

				ci++;
				m_rs.MoveNext();
				m_strEVENTTYPERecycleBin.push_back(structEventType);
				m_LVITEMRecycleBin.push_back(pLitem);
			}
	
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	} 

}


void DlgSettingEvent::vPopulateType(CString o_Source, CString o_Type)
{
	//Populate Event type Based on the source... Mostly triggered by 
	m_ListEventType->DeleteAllItems(); //.ResetContent();
	CString csDBIpCamEventTypeDisplay, csDesc, csCode;
	CString ls_sql, csArea;
 
	try
	{
		ls_sql.Format(  _T("SELECT * FROM ec_hevent_type WHERE event_source_code ='%s'  ORDER BY sort_order"), o_Source);
		CRecordset m_rs(pDB);

		m_rs.Open(CRecordset::dynaset, ls_sql);

			int nDBIpCamEventType = m_rs.GetRecordCount();

			int ci=0;
			while (!m_rs.IsEOF())
			{
				strEVENTTYPE* structEventType = new strEVENTTYPE;
				m_rs.GetFieldValue(_T("event_type_code"), csCode);
				m_rs.GetFieldValue(_T("event_type_desc"), csDesc);
				m_rs.GetFieldValue(_T("define_area"), csArea);				

				LVITEMW* pLitem = new LVITEM;
				pLitem->mask = LVIF_PARAM|LVIF_TEXT;
				pLitem->iItem = ci;
				pLitem->pszText = (LPWSTR)(LPCTSTR)csDesc;
				structEventType->csTypeCode = csCode;
				structEventType->csDefineArea = csArea.Trim();

				pLitem->lParam = (LPARAM) structEventType;
 
				m_ListEventType->InsertItem(pLitem->mask , ci, csDesc, 0,0,0, 	pLitem->lParam);
				o_Type = o_Type.Trim();

				//Make the item checked if  is the passed Type
				if (o_Type!=_T("") && (o_Type==csCode.Trim() || o_Type==csDesc.Trim()))
				{
					m_ListEventType->SetCheck(ci, TRUE);
					vPopulateAction(o_Type, _T(""));
				}

				ci++;
				m_rs.MoveNext();
				m_LVITEMRecycleBin.push_back(pLitem);
			}
	
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	} 

}



void DlgSettingEvent::OnLvnItemchangedListEventType(NMHDR *pNMHDR, LRESULT *pResult)
{
		CString	ls_TypeCode, ls_Area;

			LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
			// TODO: Add your control notification handler code here
			*pResult = 0;
		 
		 NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

		 /* If the event is not fired because of any change in individual items then the oldstate and 
		 newstate members of pNMListView are not used.  So check for that.*/

		 if (!pNMListView->uOldState && !pNMListView->uNewState)
		  return;

		 BOOL bChecked = ListView_GetCheckState(m_ListEventType->m_hWnd, pNMListView->iItem);
		if (bChecked)
		{
		  for(int nCount=0; nCount<m_ListEventType->GetItemCount(); nCount++)
			if(nCount != pNMListView->iItem) //Unchecked Item
			{
				m_ListEventType->SetCheck(nCount, FALSE);
			}
			else //Checked Item
			{
				strEVENTTYPE* lp = (strEVENTTYPE*)m_ListEventType->GetItemData(nCount);				
				ls_TypeCode = lp->csTypeCode;
				ls_Area = lp->csDefineArea;
				if (ls_Area.Trim() ==_T("Y"))
					((CButton*)GetDlgItem(IDC_BUTDEFINEAREA))->EnableWindow(1);
				else
					((CButton*)GetDlgItem(IDC_BUTDEFINEAREA))->EnableWindow(0);
					
				vPopulateAction(ls_TypeCode.Trim(), _T(""));

			}
		mb_EventDirty = true;
		} 		
		if (m_currMode != "ADD") 
			m_currMode = "UPDATE";
}

//Populate Action into Action List Control When check on Event Type
void DlgSettingEvent::vPopulateAction(CString o_Type, CString o_Action)
{
	 m_ListAction->DeleteAllItems(); //.ResetContent();

	try
	{
		CString ls_sql, csCam, csCode;
		ls_sql.Format(  _T("SELECT * FROM ec_hevent_action WHERE event_type_code ='%s'  ORDER BY sort_order"), o_Type);
		CRecordset m_rs(pDB);

		m_rs.Open(CRecordset::dynaset, ls_sql);

			int nDBIpCamEventType = m_rs.GetRecordCount();

			CString csDBIpCamEventTypeDisplay, csDesc;
			int ci=0;
			while (!m_rs.IsEOF())
			{
				strEVENTACTION*   structEventAction = new strEVENTACTION;
				m_rs.GetFieldValue(_T("event_action_code"), csCode);			
				m_rs.GetFieldValue(_T("event_action_desc"), csDesc);			
				m_rs.GetFieldValue(_T("camera_select"), csCam);			

				LVITEMW* pLitem = new LVITEM;
				pLitem->mask = LVIF_PARAM|LVIF_TEXT;
				pLitem->iItem = ci;
				pLitem->pszText = (LPWSTR)(LPCTSTR)csDesc;
				structEventAction->csActionCode = csCode.Trim();
				structEventAction->csCamSelect = csCam.Trim();

				pLitem->lParam = (LPARAM) structEventAction;
 
				m_ListAction->InsertItem(pLitem->mask , ci, csDesc, 0,0,0, 	pLitem->lParam);
	
				o_Action = o_Action.Trim();
				//Enable or disable Setting button
				vSettingButton(o_Action);

				if (o_Action!=_T("") && (o_Action == csCode.Trim() || o_Action==csDesc.Trim()))
				{
					m_ListAction->SetCheck(ci, TRUE);

					if(csCam == _T("Y"))
						((CButton*)GetDlgItem(IDC_BUTSELCAM))->EnableWindow(1);
					else 
						((CButton*)GetDlgItem(IDC_BUTSELCAM))->EnableWindow(0);
				}
				else
					m_ListAction->SetCheck(ci, false);

				ci++;
				m_rs.MoveNext();
				m_strEVENTACTIONRecycleBin.push_back(structEventAction);
			}
	
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	} 
}

bool DlgSettingEvent::vCGItoCam(void)
{
	//UpdateData(TRUE);
    CString csIP , csPostCgiUrl,csUser, csPass, csEventType, csEventAction;

		if (BST_UNCHECKED==((CButton*)GetDlgItem(IDC_RADIO_SETTING_EVENT_IPCAM))->GetCheck())
			return false;
		//Tree Stuff...
		if(m_treectrl->GetSelectedItem()==NULL  )
		return false;
		
		
		HTREEITEM  hItem = m_treectrl->GetSelectedItem();
		HTREEITEM  hItemParent;
		if (m_treectrl->GetParentItem(hItem) == NULL) //Camera Item
			hItemParent = hItem;
		else 
			//Event Item
			hItemParent =  m_treectrl->GetParentItem(hItem) ;

		//Get parent item's INFO: IP, username, pass
		TVITEM TreeItem;
		TreeItem.hItem = hItemParent;
		TreeItem.mask = TVIF_HANDLE ;
		m_treectrl->GetItem(&TreeItem);
		SettingEventInfo* lpEventInfo = (SettingEventInfo*) TreeItem.lParam;

		if (!lpEventInfo) return false;   //Add New Event...stop here...
		
		csIP = lpEventInfo->csIP;
		csUser = lpEventInfo->csUsername ;
		csPass = lpEventInfo->csPass ;

		 if(m_ListEventType->GetItemCount() ==0)
			 return false; // no Type selected, return

		for(int i=0; i<m_ListEventType->GetItemCount(); i++)
		{
				if (m_ListEventType->GetCheck(i))
				{
					strEVENTTYPE* lp = (strEVENTTYPE*)m_ListEventType->GetItemData(i);
					 csEventType = lp->csTypeCode.Trim();
					break;
				}
		}
 
		//Action Code
		if(m_ListAction->GetItemCount() ==0)
			 return false; // no Action selected, return

		for(int i=0; i<m_ListAction->GetItemCount(); i++)
		{
				if (m_ListAction->GetCheck(i))
				{
					strEVENTACTION* lp = (strEVENTACTION*)m_ListAction->GetItemData(i);
					csEventAction = lp->csActionCode.Trim();
					break;
				}
		}

	/*---Set EventType--*/
	if(csEventType == _T("DI"))
		csEventType.Format(_T("input%d"),nInput);
	else if(csEventType == _T("MD"))
		csEventType.Format(_T("md%d"),nMd);
	else if(csEventType == _T("VL"))
		csEventType.Format(_T("vloss"));

	/*---Set Event Action---*/
	if(csEventAction == _T("PTZ"))
	{	
		CString csPtzPreset;
		csEventAction.Format(_T("ptz")); 
		((CEdit*)GetDlgItem(IDC_EDIT_SETTING_EVENT_PTZ))->GetWindowTextW(csPtzPreset);
		if(csPtzPreset == _T(""))
		{
			AfxMessageBox(AFX_IDS_DLG_SETEVENT1);
			//AfxMessageBox(_T("Please Select PTZ Preset Number (1~3), Default: 1 "));						
		}
		else nPTZParam = _ttoi(csPtzPreset);
		
		nParam = nPTZParam;
	}
	else if (csEventAction == _T("DO"))
	{
		CString csOD;
		csEventAction.Format(_T("output1"));
		((CEdit*)GetDlgItem(IDC_EDIT_SETTING_EVENT_OD))->GetWindowTextW(csOD);
		if(csOD == _T("")) // empty 
			AfxMessageBox(AFX_IDS_DLG_SETEVENT2);
		else 
			nODParam = _ttoi(csOD);
			 
		nParam = nODParam;
	}
	else if (csEventAction == _T("EMAIL"))
			csEventAction.Format(_T("mail")); 			 
	else if (csEventAction == _T("REC"))
			csEventAction.Format(_T("record")); 		 

	// Check available event number from IPCAM; if the event was originally get from ipcam then return;
	/*if(m_currMode == _T("IPCAM") || m_currMode.IsEmpty())
		return false;
	else 
	{		*/
		bCheckEvent(hItemParent,false,true);
		if(iEventNumber.empty())
			return false;

		else cEventRuleCounter = iEventNumber[cEventRuleCounter];
//	}
		
	csPostCgiUrl.Format( _T("http://%s:%d/config/event.cgi?name=%d&event=%s&action=%s&param=%d&schedule=always"), 				
						 csIP, lpEventInfo->nWebPort, cEventRuleCounter,csEventType,csEventAction, nParam);
	
	Curlplus mycurl;
	mycurl.get(csPostCgiUrl,csUser, csPass);
	cEventRuleCounter++;

	// There are 6 event numbers available per IPCAM
	if(cEventRuleCounter>5)
		cEventRuleCounter=0;
		
	   // m_treectrl->DeleteAllItems();
	//	vRefreshEventTree();
	return false;
}

//List Action Select Changed
void DlgSettingEvent::OnLvnItemchangedEventAction(NMHDR *pNMHDR, LRESULT *pResult)
{
		CString ls_ActionCode, ls_Cam;
		LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
			// TODO: Add your control notification handler code here
			*pResult = 0;
		 
		 NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

		 // Make it check only one row
		 BOOL bChecked = ListView_GetCheckState(m_ListAction->m_hWnd, pNMListView->iItem);
		if (bChecked)
		{
			for(int nCount=0; nCount<m_ListAction->GetItemCount(); nCount++)
				if (nCount != pNMListView->iItem) //Unchecked Item
				{
						m_ListAction->SetCheck(nCount, FALSE);
				}
				else  //Checked Item
				{
					CString ls_text = m_ListAction->GetItemText(nCount, 0);
					strEVENTACTION* lp = (strEVENTACTION*)m_ListAction->GetItemData(nCount);		
					ls_ActionCode = lp->csActionCode;
					ls_Cam = lp->csCamSelect;

						//Enable or disable Setting button
					vSettingButton(ls_ActionCode);

					//temp disable it
					if (ls_Cam.Trim() ==_T("Y") && ls_ActionCode == _T("PTZ")) //Camera Related Action
					{
						((CButton*)GetDlgItem(IDC_BUTSELCAM))->EnableWindow(1);
						((CEdit*)GetDlgItem(IDC_EDIT_SETTING_EVENT_PTZ))->ShowWindow(SW_SHOW);
						GetDlgItem(IDC_STATIC_PTZ)->ShowWindow(SW_SHOW);
						((CEdit*)GetDlgItem(IDC_EDIT_SETTING_EVENT_PTZ))->SetWindowTextW(_T("1"));
					}
					else
					{
						((CButton*)GetDlgItem(IDC_BUTSELCAM))->EnableWindow(0);
						((CEdit*)GetDlgItem(IDC_EDIT_SETTING_EVENT_PTZ))->ShowWindow(SW_HIDE);
						GetDlgItem(IDC_STATIC_PTZ)->ShowWindow(SW_HIDE);
						//((CStatic*)GetDlgItem(IDC_ST_CAMINFO))->SetWindowTextW(_T(""));
					}
					
					if(ls_ActionCode == _T("DO"))
					{
						((CEdit*)GetDlgItem(IDC_EDIT_SETTING_EVENT_OD))->ShowWindow(SW_SHOW);
						GetDlgItem(IDC_STATIC_OUTPUTDURATION)->ShowWindow(SW_SHOW);
						((CEdit*)GetDlgItem(IDC_EDIT_SETTING_EVENT_OD))->SetWindowTextW(_T("1"));
					}
					else
					{
						((CEdit*)GetDlgItem(IDC_EDIT_SETTING_EVENT_OD))->ShowWindow(SW_HIDE);
						GetDlgItem(IDC_STATIC_OUTPUTDURATION)->ShowWindow(SW_HIDE);
					}
				}
		mb_EventDirty = true;
		}	//if (bChecked)
		//
		if (m_currMode != "ADD")  m_currMode = "UPDATE";
		*pResult = 0;
}

void DlgSettingEvent::OnBnClickedButselcam()
{
	// TODO: Add your control notification handler code here
	CDlgCamSelect lDlg;
	if (lDlg.DoModal() == IDOK)
	{
		//re-assign camera index
		m_targetCamidx =  lDlg.m_camidx;
		vPopulateTargetCam(m_targetCamidx, -1);
	}
}

void DlgSettingEvent::vPopulateTargetCam(int o_CamIdx, int o_PPoint)
{
	bool lbEnable = true, lbPEnable=true;
	CString ls_camidx, ls_Point, ls_sql, ls_CamName, lsName;

	if (o_CamIdx<0) 	lbEnable = false;
	if (o_PPoint<0) lbPEnable = false;
	 
	//((CButton*)GetDlgItem(IDC_BUTSELCAM))->EnableWindow(lbEnable);
	//Temp unable
	//((CButton*)GetDlgItem(IDC_BUTSELCAM))->EnableWindow(false);
	((CStatic*) GetDlgItem(IDC_STATIC_PPN))->ShowWindow(lbPEnable);

	//Populate Camidx & ptz point...
	ls_camidx.Format(_T("%d"), o_CamIdx);
	((CEdit*) GetDlgItem(IDC_EDIT_TARGET_CAMIDX))->SetWindowTextW(ls_camidx);

	ls_Point.Format(_T("%d"), o_PPoint);
	//((CEdit*) GetDlgItem(IDC_EDIT_PPOINT))->SetWindowTextW(ls_Point);
	((CEdit*) GetDlgItem(IDC_EDIT_SETTING_EVENT_PTZ))->SetWindowTextW(ls_Point);
	

	//if (lbPEnable)
	//{
	//	ls_Point.Format(_T("PTZ Preset Point: %d"), o_PPoint);
	//	((CStatic*) GetDlgItem(IDC_STATIC_PPN))->SetWindowTextW(ls_Point);
	//}


	if (lbEnable)
	{
		//Retrieve Camera Name and Display	
		ls_sql.Format( _T("SELECT  camera_idx, cameraname FROM ec_camera WHERE camera_idx=%d"),o_CamIdx);			
		CRecordset m_rs(pDB);
		m_rs.Open(CRecordset::dynaset, ls_sql);
		while (!m_rs.IsEOF())
		{
				m_rs.GetFieldValue(_T("cameraname"), ls_CamName);		
				break;
		}
		lsName.Format(_T("Target Camera: (%d) - %s"),o_CamIdx,  ls_CamName.Trim());
		if(o_CamIdx<=0)
		{
			lsName=_T("");
		    ((CStatic*)GetDlgItem(IDC_ST_ACTIONINFO))->SetWindowTextW(lsName);
		}

		else ((CStatic*)GetDlgItem(IDC_ST_ACTIONINFO))->SetWindowTextW(lsName);
		m_rs.Close();
	}

}

void DlgSettingEvent::OnBnClickedRadiodigitalin()
{
	mb_EventDirty = true;
	vPopulateType(DIGITALIN, _T(""));
	((CButton*)GetDlgItem(IDC_RADIO_SETTING_EVENT_SOURCE_SYSTEM))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_SETTING_EVENT_IPCAM))->SetCheck(0);
	// TODO: Add your control notification handler code here
}

void DlgSettingEvent::OnBnClickedRadioSettingEventSourceSystem()
{
	mb_EventDirty = true;
	vPopulateType(SYSTEM, _T(""));
	((CButton*)GetDlgItem(IDC_RADIODIGITALIN))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_SETTING_EVENT_IPCAM))->SetCheck(0);
	// TODO: Add your control notification handler code here
}

void DlgSettingEvent::OnTvnSelchangingDlgSettingEventTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	//// TODO: Add your control notification handler code here

	TVITEM hItemNewing = pNMTreeView->itemNew;
	TVITEM hItemOlding = pNMTreeView->itemOld;
	m_memEvent = &(pNMTreeView->itemOld);
	HTREEITEM hItem = m_treectrl->GetSelectedItem();

	if(hItemOlding.hItem == NULL) 	return;

	fnCheckToSave(false); //,hItemOlding );
	return;

}

	bool DlgSettingEvent::vSaveDB(HTREEITEM hItem)
	{
		// if no treeitem has been selected, return
		CString	ls_sql;
		//HTREEITEM  hItem = m_treectrl->GetSelectedItem();
		if(hItem == NULL)
		return false;
		if(m_currMode.IsEmpty()) return false;

		m_treectrl->SelectItem(hItem);

		SettingEventInfo RetrieveDlgEventInfo={0};

		// if events were import from IPCAM, save them all
		if(m_currMode == _T("IPCAM"))
		{
				if (mi_ImportCamIdx<1) return false;
					vDeleteEvent(mi_ImportCamIdx);		
					
				if(m_treectrl->GetParentItem(hItem) == NULL &&  m_treectrl->GetChildItem(hItem) == NULL)
				return false;// there's no event to save, return
			
			// if it is a parent with child, get its child treeitem
			if(m_treectrl->ItemHasChildren(hItem))
				hItem = m_treectrl->GetChildItem(hItem);

	    	int nChildCount = 0;	
			while(hItem != NULL)
			{  				
				//m_treectrl->SelectItem(hItem);
				TVITEM Item={0};
				Item.hItem = hItem;
				Item.mask = TVIF_HANDLE;
				m_treectrl->GetItem(&Item);
				SettingEventInfo* (IPcamEventInfo) = ((SettingEventInfo*)Item.lParam);
				
				if(nChildCount>6)
				{
					AfxMessageBox(AFX_IDS_DLG_SETEVENT3);				
					return false;
				}
				else
				{
					vSaveDBStruct(IPcamEventInfo);
					nChildCount++;
					m_bImportEventSave = true;
				}

				hItem = m_treectrl->GetNextSiblingItem(hItem);
			}
		}	
		
		// Retrieve Event Setting infomation from Dialog
		if(m_currMode ==_T("ADD")||m_currMode ==_T("UPDATE")||m_currMode ==_T("DELETE"))
		{
			int nChildCount = 0;
			HTREEITEM hItemParent = m_treectrl->GetParentItem(hItem);
		
			if(hItemParent) // Node Item
			{
				HTREEITEM hItemChild = m_treectrl->GetChildItem(hItemParent);
				nChildCount++;
				while(hItemChild = m_treectrl->GetNextSiblingItem(hItemChild))
				nChildCount++;
			}
			else // Parent Item
			{
				HTREEITEM hItemChild = m_treectrl->GetChildItem(hItem);
				nChildCount++;
				while(hItemChild = m_treectrl->GetNextSiblingItem(hItemChild))
				nChildCount++;
			}
			if(nChildCount>6)
			{
				AfxMessageBox(m_MaxMessage);
				//AfxMessageBox(_T("The Maximum number of Event per IPCAM is 6, please delete the previous event before adding a new one"));
				return false;
			}
			RetrieveDlgEventInfo = GetDialogInfo();
		}
	
		if (m_currMode == _T("ADD"))
		{
			ls_sql.Format(_T( "INSERT INTO ec_camera_event (camera_idx, alert_level, event_name , event_source_code , event_type_code, device_action_code,  target_camera_idx,  ptz_point , interval_, active_ ,cycle_type,cycle_from,cycle_till) VALUES ( %d, %d,  '%s',  '%s', '%s', '%s', %d, %d, %d,'%s','%s',CONVERT(smalldatetime, '%s'),CONVERT(smalldatetime, '%s') )"), 
				m_parent_camidx, 
				RetrieveDlgEventInfo.AlertLevel, 
				RetrieveDlgEventInfo.csEventName.Trim(), 
				RetrieveDlgEventInfo.csEventSource,
				RetrieveDlgEventInfo.csEventType, 
				RetrieveDlgEventInfo.csEventAction, 
				RetrieveDlgEventInfo.TargetCamIdx, 
				RetrieveDlgEventInfo.PTZPoint, 
				RetrieveDlgEventInfo.Interval, 
				RetrieveDlgEventInfo.cActive, 
				RetrieveDlgEventInfo.csDuration,
				RetrieveDlgEventInfo.csDurationFrom, 
				RetrieveDlgEventInfo.csDurationTo);
		}
		else if (m_currMode == _T("UPDATE"))
		{
 			ls_sql.Format(_T( "UPDATE ec_camera_event SET alert_level=%d, event_name='%s', event_source_code ='%s', event_type_code='%s', device_action_code='%s',  target_camera_idx=%d,   ptz_point=%d , interval_ =%d , active_='%s', cycle_type = '%s', cycle_from =  CONVERT(smalldatetime, '%s') , cycle_till =  CONVERT(smalldatetime, '%s') WHERE camera_eventid=%d"), 
				RetrieveDlgEventInfo.AlertLevel, 
				RetrieveDlgEventInfo.csEventName, 
				RetrieveDlgEventInfo.csEventSource,
				RetrieveDlgEventInfo.csEventType, 
				RetrieveDlgEventInfo.csEventAction, 
				RetrieveDlgEventInfo.TargetCamIdx, 
				RetrieveDlgEventInfo.PTZPoint, 
				RetrieveDlgEventInfo.Interval, 
				RetrieveDlgEventInfo.cActive,
				RetrieveDlgEventInfo.csDuration, 
				RetrieveDlgEventInfo.csDurationFrom, 
				RetrieveDlgEventInfo.csDurationTo,  
				RetrieveDlgEventInfo.EventId);
		}
			else if (m_currMode == _T("DELETE"))
		{
			if (mi_delEventId<=0) return false;
 			ls_sql.Format(_T( "DELETE FROM ec_camera_event WHERE camera_eventid=%d"), 	
				mi_delEventId);
		}
		TRACE(ls_sql);

		try 
		{
			pDB->ExecuteSQL(ls_sql);
			//m_DB.ExecuteSQL(ls_sql_cycle);
		}
		catch(CDBException * p)
		{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}

		//if (m_currMode == _T("UPDATE"))  vUpdateTree(RetrieveDlgEventInfo.EventId, m_parent_camidx  ,RetrieveDlgEventInfo.EventId, RetrieveDlgEventInfo);
		mb_EventDirty = false;
		m_currMode = _T("");
	 
		return false;
	}

	void DlgSettingEvent::vSaveDBStruct(SettingEventInfo* EventInfo)
	{
		// Insert  Database
		CString ls_sql;
		ls_sql.Format(_T( "INSERT INTO ec_camera_event (camera_idx, alert_level, event_name , event_source_code , event_type_code, device_action_code,  target_camera_idx,  ptz_point , interval_, active_ ,cycle_type,cycle_from,cycle_till) VALUES ( %d, %d,  '%s',  '%s', '%s', '%s', %d, %d, %d,'%s','%s',CONVERT(smalldatetime, '%s'),CONVERT(smalldatetime, '%s') )"), 
			m_parent_camidx, 
			EventInfo->AlertLevel, 
			EventInfo->csEventName.Trim(), 
			EventInfo->csEventSource,
			EventInfo->csEventType, 
			EventInfo->csEventAction, 
			EventInfo->TargetCamIdx, 
			EventInfo->PTZPoint, 
			EventInfo->Interval, 
			EventInfo->cActive, 
			EventInfo->csDuration,
			EventInfo->csDurationFrom, 
			EventInfo->csDurationTo);

		try 
		{
			pDB->ExecuteSQL(ls_sql);
		}
		catch(CDBException * p)
		{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}
		mb_EventDirty = false;
	}

	void DlgSettingEvent::OnBnClickedRadioWarning()
	{
		mb_EventDirty=true;
		if (m_currMode != _T("ADD"))  m_currMode = _T("UPDATE");
		// TODO: Add your control notification handler code here
	}

	void DlgSettingEvent::OnBnClickedRadioCritical()
	{
		mb_EventDirty=true;
		if (m_currMode != _T("ADD"))  m_currMode = _T("UPDATE");
		// TODO: Add your control notification handler code here
	}

	void DlgSettingEvent::OnBnClickedRadioInfo()
	{
		mb_EventDirty=true;
		if (m_currMode != _T("ADD"))  m_currMode = _T("UPDATE");
		// TODO: Add your control notification handler code here
	}

	//Select Radio Button for Event Source
	bool DlgSettingEvent::vSelectRadioSource(CString o_Source)
	{
		
		if (o_Source==SYSTEM) //System
		{
			((CButton*)GetDlgItem(IDC_RADIO_SETTING_EVENT_SOURCE_SYSTEM) )->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_SETTING_EVENT_IPCAM) )->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIODIGITALIN) )->SetCheck(0);			
		}
		else if   (o_Source==DIGITALIN) //IPCAM
		{
			((CButton*)GetDlgItem(IDC_RADIODIGITALIN) )->SetCheck(1);		
			((CButton*)GetDlgItem(IDC_RADIO_SETTING_EVENT_IPCAM) )->SetCheck(0);			
			((CButton*)GetDlgItem(IDC_RADIO_SETTING_EVENT_SOURCE_SYSTEM) )->SetCheck(0);
		}
		else //Camera Default
		{
			((CButton*)GetDlgItem(IDC_RADIODIGITALIN) )->SetCheck(0);		
			((CButton*)GetDlgItem(IDC_RADIO_SETTING_EVENT_IPCAM) )->SetCheck(1);			
			((CButton*)GetDlgItem(IDC_RADIO_SETTING_EVENT_SOURCE_SYSTEM) )->SetCheck(0);	
		}
		return true;
	}

	bool DlgSettingEvent::vSelectRadioAlert(int o_Level)
	{
			if (o_Level==1) //Critical
		{
			((CButton*)GetDlgItem(IDC_RADIO_CRITICAL) )->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_WARNING) )->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_INFO) )->SetCheck(0);
		}
		else if (o_Level==2) //Warning
		{
			((CButton*)GetDlgItem(IDC_RADIO_CRITICAL) )->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_WARNING) )->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_INFO) )->SetCheck(0);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_RADIO_CRITICAL) )->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_WARNING) )->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_INFO) )->SetCheck(1);
		}
		return false;
	}

	void DlgSettingEvent::OnBnClickedButsave()
	{
		// TODO: Add your control notification handler code here

		HTREEITEM hItem = m_treectrl->GetSelectedItem();
		if(hItem == NULL)
			return;
		if(m_currMode.IsEmpty()) return;
			
		//vCGItoCam();
		 vSaveDB(hItem);
		 vRefreshEventTree();

	}

	//To make all the radio buttons unchecked
	int DlgSettingEvent::vRadioClear(bool ob_Source, bool ob_Alarm , bool ob_Duration)
	{
		if (ob_Source)
		{
			((CButton*)GetDlgItem(IDC_RADIO_SETTING_EVENT_IPCAM) )->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIODIGITALIN) )->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_SETTING_EVENT_SOURCE_SYSTEM) )->SetCheck(0);
		}

		if (ob_Alarm)
		{
			((CButton*)GetDlgItem(IDC_RADIO_CRITICAL) )->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_WARNING) )->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_INFO) )->SetCheck(0);
		}

		if(ob_Duration)
		{
			//set always as default
			((CButton*)GetDlgItem(IDC_RADIO_ALWAYS_VALID) )->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_VALID_TIME) )->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_MANUAL_CANCEL) )->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_AUTO_CANCEL) )->SetCheck(0);

		}

		return 0;
	}

	void DlgSettingEvent::vAddNewEvent(void)
	{
		TVITEM	ItemP;
		m_currMode = _T("ADD");
 
		//Tree Stuff...
		HTREEITEM  hItem = m_treectrl->GetSelectedItem();	
		
		if(hItem == NULL)
			return;

		HTREEITEM  hItemParent;
		if (m_treectrl->GetParentItem(hItem) == NULL) //Camera Item
			hItemParent = hItem;
		else //Event Item
			hItemParent =  m_treectrl->GetParentItem(hItem) ;

		ItemP.hItem = hItemParent;
		ItemP.mask = TVIF_HANDLE;
		m_treectrl->GetItem(&ItemP);
		

		// check if the number of event is less than 6
		int nChildCount = 0;
		if(m_treectrl->ItemHasChildren(hItemParent))
		{
			HTREEITEM hItemChild = m_treectrl->GetChildItem(hItemParent);
			nChildCount++;
			while(hItemChild = m_treectrl->GetNextSiblingItem(hItemChild))
			nChildCount++;
		}

		if(nChildCount>5)
		{
			AfxMessageBox(m_MaxMessage);
			return;
		}
			
		 hItemNew = m_treectrl->InsertItem(_T("New Event - MD "), hItemParent, hItem);
		m_treectrl->SelectItem(hItemNew);	

		//Get parent item's camera_idx
		TVITEM Item;
		Item.hItem = hItemParent;
		Item.mask = TVIF_HANDLE;
		m_treectrl->GetItem(&Item);
		
		if (ItemP.lParam>0) 
			m_parent_camidx =   ((SettingEventInfo*)ItemP.lParam)->CameraIdx;

		//Get the new eventid
		int  ll_eventid=0;int ll_col=0;
		CString ls_sql = _T("SELECT  MAX(camera_eventid) FROM ec_camera_event WHERE 1=1");	
		CRecordset m_rs(pDB);
		m_rs.Open(CRecordset::dynaset, ls_sql);
		CDBVariant  varMax;

	 	while (!m_rs.IsEOF())
		{
			CString ls_new;

			m_rs.GetFieldValue(ll_col, varMax, SQL_C_SLONG);
			if (varMax.m_iVal<0)
				ll_eventid = 1;
			else
				ll_eventid = varMax.m_iVal + 1;
			ls_new.Format(_T("%d"), ll_eventid);			 
			if (ll_eventid>0) ((CEdit*) GetDlgItem(IDC_EDITID))->SetWindowTextW(ls_new);
			break;
		}

		// Clear and Reset columns
		vRadioClear(true, true, true);
		((CButton*)GetDlgItem(IDC_CHECK_SETTING_EVENT_ENABLE))->SetCheck(1);
		((CEdit*)GetDlgItem(IDC_EDIT_SETTING_EVENT_EVENT_NAME))->SetWindowTextW(_T(""));

		((CButton*)GetDlgItem(IDC_RADIO_SETTING_EVENT_IPCAM))->SetCheck(1);
		vPopulateType(_T("CAMERA"), _T("MD"));
		((CButton*)GetDlgItem(IDC_RADIO_INFO))->SetCheck(1);

		((CEdit*) GetDlgItem(IDC_EDIT_TARGET_CAMIDX))->SetWindowTextW(_T(""));
		((CEdit*) GetDlgItem(IDC_EDIT_PPOINT))->SetWindowTextW(_T(""));

		((CEdit*) GetDlgItem(IDC_EDIT_SETTING_EVENT_EVENT_NAME))->SetWindowTextW(_T("New Event"));
		((CEdit*) GetDlgItem(IDC_EDIT_SETTING_EVENT_EVENT_NAME))->SetFocus();
//javan not found IDC_EDIT_OUTPUT_DURATION id
//		((CEdit*) GetDlgItem(IDC_EDIT_OUTPUT_DURATION))->SetWindowTextW(_T(""));

			mb_EventDirty = true;
		
	}

	void DlgSettingEvent::vRefreshEventTree(void)
	{
		int	ll_camidx=0;
		int nWebPort=0;
		HTREEITEM hIPCam, hEvents;
		TVINSERTSTRUCTW itemStru={0};

		m_treectrl->DeleteAllItems();
	try
	{
			CString csDBEventList = _T("SELECT  camera_idx, cameraname, ipaddress, httpport, username, password, model_code FROM ec_camera WHERE active_='Y' or active_ is null ORDER BY camera_idx");
			CRecordset m_rs(pDB);
			m_rs.Open(CRecordset::dynaset, csDBEventList);
			CString csDBEventListIPCamIdx, csDBEventListIPCamCameraName,csDBEventListIPCamIpaddress, csDBEventListIPCamDisplay,
				csDBEventListIPCamEventName,  csDBUser,  csDBPass ,csDBModelName,csDBWebPort;
		
			//m_rs.Open(CRecordset::forwardOnly,  csDBEventList);

			while (!m_rs.IsEOF())
			{
				//m_rs.GetFieldValue(_T("event_type_code"),  csDBIpCamEventTypeCodeTmp);
				SettingEventInfo* m_EventInfo = new SettingEventInfo;

				m_rs.GetFieldValue(_T("camera_idx"), csDBEventListIPCamIdx);
				ll_camidx = _ttoi(csDBEventListIPCamIdx);
				m_EventInfo->CameraIdx = ll_camidx;

				//m_settingEventInfo->csCameraIdx = csDBEventListIPCamIdx;
				csDBEventListIPCamIdx = _T("(") + csDBEventListIPCamIdx + _T(")");

				m_rs.GetFieldValue(_T("cameraname"), csDBEventListIPCamCameraName);
				csDBEventListIPCamCameraName = csDBEventListIPCamCameraName + _T(" ");

				m_rs.GetFieldValue(_T("ipaddress"), csDBEventListIPCamIpaddress);
				m_EventInfo->csIP =csDBEventListIPCamIpaddress.Trim();

				m_rs.GetFieldValue(_T("httpport"),csDBWebPort);
				nWebPort = _ttoi(csDBWebPort);
				m_EventInfo->nWebPort = nWebPort;

				m_rs.GetFieldValue(_T("username"), csDBUser);
				m_EventInfo->csUsername =csDBUser.Trim();

				m_rs.GetFieldValue(_T("password"), csDBPass);
				m_EventInfo->csPass =csDBPass.Trim();

				
				m_rs.GetFieldValue(_T("model_code"), csDBModelName);
				m_EventInfo->csModelName =csDBModelName.Trim();


				//m_rs.GetFieldValue(_T("event_name"),csDBEventListIPCamEventName);
				//m_settingEventInfo.csEventName = csDBEventListIPCamEventName;
				 csDBEventListIPCamDisplay = csDBEventListIPCamIdx + csDBEventListIPCamCameraName + csDBEventListIPCamIpaddress;
				
				//itemStru.item.mask = TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM ;
				 itemStru.item.lParam = (LPARAM)m_EventInfo ;
				hIPCam = m_treectrl->InsertItem(TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, csDBEventListIPCamDisplay.Trim(), 0,0,0,0, (LPARAM)m_EventInfo  , NULL, TVI_ROOT);
				//hIPCam = m_treectrl->InsertItem(csDBEventListIPCamDisplay.Trim(), TVI_ROOT);

				
				vAddDBEvent(ll_camidx, hIPCam);
				//hEvents = m_treectrl->InsertItem(csDBEventListIPCamEventName, hIPCam);
				//m_treectrl->Expand(hIPCam, TVE_EXPAND);

				//csDBIpCamEventTypeDisplay =  csDBIpCamEventTypeCodeTmp + _T(" - ")+csDBIpCamEventTypeDescTmp;
				//m_ListEventType.AddString(csDBIpCamEventTypeDescTmp);
				m_rs.MoveNext();
				m_treectrl->SetBkColor(GLOBALTREEBKCOLOR);

			}
	}
	
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
		//m_DB.Close();
	} 
	mb_EventDirty=false;
	}

	void DlgSettingEvent::vDeleteEvent(int o_camidx)
	{
			CString ls_sql;
			ls_sql.Format( _T(" DELETE  FROM ec_camera_event WHERE camera_idx=%d"), o_camidx);
			try
			{
				pDB->ExecuteSQL(ls_sql);
			}
			catch(CDBException * p)
			{
				AfxMessageBox(p->m_strError);
				p->Delete();
			} 
	}

	void DlgSettingEvent::vDeleteEvent(void)
	{
		
		// if no selected cam return
		HTREEITEM  hItem = m_treectrl->GetSelectedItem();
		HTREEITEM hItemParent = m_treectrl->GetParentItem(hItem);
		if(hItemParent == NULL)
			hItem = m_treectrl->GetChildItem(hItemParent);
		if(hItem == NULL)
			return;

		int	llEventId, llLevel;
		CString ls_name, ls_desc, ls_type, ls_action;

		//HTREEITEM  hItem = m_treectrl->GetSelectedItem();	
		TVITEM TreeItem;
		TreeItem.hItem = hItem;
		TreeItem.mask = TVIF_HANDLE ;
		m_treectrl->GetItem(&TreeItem);
		SettingEventInfo* lpEventInfo = (SettingEventInfo*) TreeItem.lParam;

		if (!lpEventInfo) return;   //Add New Event...stop here...

		//Event ID
		mi_delEventId =  lpEventInfo->EventId;

		if (mi_delEventId<0) return;
	 
		//Event Name
		ls_name = lpEventInfo->csEventName;
		((CEdit*)GetDlgItem(IDC_EDIT_SETTING_EVENT_EVENT_NAME) )->SetWindowTextW(ls_name.Trim());

	/*	ls_desc.Format(_T("Are you sure to delete this event: ID: %d   -  %s ? "),	llEventId,  ls_name);
		int iRet = AfxMessageBox(ls_desc,MB_YESNO);
		if (iRet == IDYES)
		{*/
			/*try
			{		*/
				/*CString ls_sql;
				ls_sql.Format( _T(" DELETE  FROM ec_camera_event WHERE camera_eventid=%d"), llEventId);
			
				pDB->ExecuteSQL(ls_sql);
				*/
				m_treectrl->DeleteItem(hItem);
				m_treectrl->SelectItem(hItemParent);
				m_currMode = _T("DELETE");
				mb_EventDirty = true;
 
				//vRefreshEventTree();
			//}
		/*	catch(CException * p)
			{
				AfxMessageBox(p->>m_strError);
				p->Delete();
			} */

		//} //Go ahead to delete
	}
	void DlgSettingEvent::vDeleteEvent(HTREEITEM hItem)
	{
		if(hItem ==NULL) return;

		TVITEM TreeItem;
		TreeItem.hItem = hItem;
		TreeItem.mask = TVIF_HANDLE ;
		m_treectrl->GetItem(&TreeItem);
		SettingEventInfo* lpEventInfo = (SettingEventInfo*) TreeItem.lParam;
		
		if (!lpEventInfo) return;
		
		CString ls_sql;
		ls_sql.Format( _T(" DELETE  FROM ec_camera_event WHERE camera_eventid=%d"), lpEventInfo->EventId);
			try
			{
				pDB->ExecuteSQL(ls_sql);
				
			}
			catch(CDBException * p)
			{
				AfxMessageBox(p->m_strError);
				p->Delete();
			} 


	}

	BOOL DlgSettingEvent::PreTranslateMessage(MSG* pMsg)
	{
		// TODO: Add your specialized code here and/or call the base class

	 if(m_ToolTip != NULL)
		m_ToolTip->RelayEvent(pMsg);
		
	switch ( pMsg->message )
	{
		case WM_KEYDOWN:
					
				if(pMsg->wParam == VK_ESCAPE)
				{
					
					HTREEITEM  hItem = m_treectrl->GetSelectedItem();	
				
					if (m_treectrl->GetParentItem(hItem) == NULL) //Camera Items
					{	
						break;
						/*return  CBaseDialog::PreTranslateMessage(pMsg);*/
 					}
					else // node
					{
		
						TVITEM TreeItem={0};
						TreeItem.hItem = hItem;
						TreeItem.mask = TVIF_HANDLE ;
						m_treectrl->GetItem(&TreeItem);
				    	SettingEventInfo* lpEventInfo = (SettingEventInfo*) TreeItem.lParam;
			
					
					if (bCheckEventInputChanged(lpEventInfo))
						{
							//CString ls_confirm = _T("Save the changes before close?");
							//int ll_ret = AfxMessageBox(ls_confirm, MB_YESNO|MB_ICONQUESTION);		
							int ll_ret = AfxMessageBox(IDS_SAVE_DATA, MB_YESNO|MB_ICONQUESTION);		
							if (ll_ret==IDYES)
							{	
								m_currMode == _T("UPDATE");
								vSaveDB(hItem);
								vRefreshEventTree();
																
							}
							::DispatchMessage( pMsg );
							
						}
					}
				}
				break;
					
	  }

			
		 
		 return CBaseDialog::PreTranslateMessage(pMsg);
			
	}

	void DlgSettingEvent::vSaveCycle(int o_Eventid, CString o_mode)
	{
		//Reserved for next release
		/*CREATE TABLE [dbo].[ec_camera_event_cycle](
		[camera_event_cycleid] [int] IDENTITY(1,1) NOT NULL,
		[camera_eventid] [int] NULL,
		[life_cycle] [nchar](10) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL,
		[duration_] [nchar](10) COLLATE Chinese_Taiwan_Stroke_CI_AS NULL,
		[valid_time_from] [smalldatetime] NULL,
		[valid_time_to] [smalldatetime] NULL,

			if (o_mode.Trim() == _T("ADD"))
				ls_sql.Format(_T( "INSERT INTO ec_camera_event_cycle (camera_eventid,  life_cycle, valid_time_from, valid_time_to ) VALUES ( %d, %d,  '%s',  '%s', '%s', '%s', %d, %d, '%s' )"), m_parent_camidx, ll_Alert, ls_name.Trim(), ls_source,ls_Type, ls_Action, ll_camidx, ll_ppoint, ll_interval , ls_enable);
			else
 				ls_sql.Format(_T( "UPDATE ec_camera_event_cycle SET  life_cycle=%s, valid_time_from=%d, valid_time_to =%d, event_name='%s', event_source_code ='%s', event_type_code='%s', device_action_code='%s',  target_camera_idx=%d,   ptz_point=%d , interval_ =%d , active_='%s'  WHERE camera_eventid=%d"), ll_Alert, ls_name.Trim(), ls_source,ls_Type, ls_Action, ll_camidx, ll_ppoint, ll_interval, ls_enable,  ll_eventid);*/

	}

	//This function will select the m_entryCamidx
	void DlgSettingEvent::vFindCam(void)
	{
		TVITEM itemCam;
 
		HTREEITEM itemRoot = m_treectrl->GetRootItem();
		while (itemRoot)
		{				
			TVITEM Item;
			Item.hItem = itemRoot;
			Item.mask = TVIF_HANDLE | TVIF_PARAM ;
			m_treectrl->GetItem(&Item);

		
			
		if (((SettingEventInfo*) Item.lParam)->CameraIdx == m_entryCamidx)
			{
				m_treectrl->Expand(itemRoot, TVE_EXPAND );
				m_treectrl->SelectItem(itemRoot);
				return;
			}

			itemRoot = m_treectrl->GetNextItem(itemRoot, TVGN_NEXT);
			
		}
	}

	void DlgSettingEvent::vEnableCam(CString o_actionCode)
	{
	}


/*--- populate event duration and lifecycle from ec_camera_event_cycle, also store data into struct event info---*/
void DlgSettingEvent::vPopulateDuration(int o_EventId, SettingEventInfo * pEventInfo)
{

	CString csLifeCycle, csDuration, csValidTimeFrom, csValidTimeTo, ls_sql ;
	


	// Restore data
	((CButton*)GetDlgItem(IDC_RADIO_AUTO_CANCEL) )->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_MANUAL_CANCEL) )->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_ALWAYS_VALID) )->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_VALID_TIME) )->SetCheck(0);
	//m_dateTimePickerFrom.GetTime();
	//m_dateTimePickerTo.GetTime();
	
	try
	{
		//ls_sql.Format(  _T("SELECT  cycle_type, CONVERT(VARCHAR(8), cycle_from, 8) AS cycle_from, CONVERT(VARCHAR(8), cycle_till, 8) AS cycle_till FROM ec_camera_event WHERE camera_eventid ='%d'"), o_EventId);

		ls_sql.Format(  _T("SELECT  cycle_type,CONVERT(VARCHAR(8), cycle_from, 8) AS cycle_from, CONVERT(VARCHAR(8), cycle_till, 8) AS cycle_till FROM ec_camera_event WHERE camera_eventid ='%d'"), o_EventId);
		CRecordset m_rs(pDB);

			m_rs.Open(CRecordset::dynaset, ls_sql);

			int nDBIpCamEventType = m_rs.GetRecordCount();

			int ci=0;
			while (!m_rs.IsEOF())
			{
				
				//m_rs.GetFieldValue(_T("life_cycle"), csLifeCycle);
				m_rs.GetFieldValue(_T("cycle_type"), csDuration);
				m_rs.GetFieldValue(_T("cycle_from"), csValidTimeFrom);
				m_rs.GetFieldValue(_T("cycle_till"), csValidTimeTo);

				/*---Set LifeCycle radio button---*/

			/*	if(csLifeCycle ==_T("Automatic"))
					((CButton*)GetDlgItem(IDC_RADIO_AUTO_CANCEL) )->SetCheck(1);
				else ((CButton*)GetDlgItem(IDC_RADIO_MANUAL_CANCEL) )->SetCheck(1);*/

				pEventInfo->csDuration = csDuration.Trim();
				pEventInfo->csDurationFrom = csValidTimeFrom.Trim();
				pEventInfo->csDurationTo = csValidTimeTo.Trim();
	

				/*---Set Duration radio button and DateTimePicker---*/
				if(csDuration == _T("Always"))
					((CButton*)GetDlgItem(IDC_RADIO_ALWAYS_VALID) )->SetCheck(1);
				else if (csDuration == _T("Within"))
				{
					((CButton*)GetDlgItem(IDC_RADIO_VALID_TIME) )->SetCheck(1);

					TCHAR *tcValidTimeFrom, *tcValidTimeTo;
					int iHour, iMinute,iSecond;

					 tcValidTimeFrom = (TCHAR*)(LPCTSTR) csValidTimeFrom;									//Time From
					_stscanf(tcValidTimeFrom, _T("%d:%d:%d"), &iHour, &iMinute, &iSecond);
					CTime ctimeValidFrom(1970,1,1,iHour,iMinute,iSecond);

					tcValidTimeTo = (TCHAR*)(LPCTSTR) csValidTimeTo;									//Time To
					_stscanf(tcValidTimeTo, _T("%d:%d:%d"), &iHour, &iMinute, &iSecond);
					CTime ctimeValidTo(1970,1,1,iHour,iMinute,iSecond);
						
					 m_dateTimePickerFrom.SetTime(&ctimeValidFrom);
					 m_dateTimePickerTo.SetTime(&ctimeValidTo);
				 

				}
			
				
			    ci++;
				m_rs.MoveNext();
			}
	
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	} 	
}

void DlgSettingEvent::OnBnClickedButimport()
{
	// TODO: Add your control notification handler code here
	//start Importing here....

	int iYesNo = AfxMessageBox(IDS_IMPORT_EVENT, MB_YESNO);
	//int iYesNo = AfxMessageBox(_T("Import will override the existing data in the database, ok to process?"), MB_YESNO);
	if(iYesNo == IDNO) return;				

	//Tree Stuff...
		HTREEITEM  hItem = m_treectrl->GetSelectedItem();			
		if(hItem == NULL) 	return;

		m_currMode == _T("IPCAM");

		// find the root treeitem
		HTREEITEM  hItemParent;
		if (m_treectrl->GetParentItem(hItem) == NULL) //Camera Item
			hItemParent = hItem;
		else//Event Item		
			hItemParent =  m_treectrl->GetParentItem(hItem) ;
		
		//
		TVITEM ItemP;
		ItemP.hItem = hItemParent;
		ItemP.mask = TVIF_HANDLE;
		m_treectrl->GetItem(&ItemP);
		mi_ImportCamIdx =   ((SettingEventInfo*)ItemP.lParam)->CameraIdx;		 
		
		int nChildCount = 0;
										
		TVITEM Item={0};
		Item.hItem = hItemParent;
		Item.mask = TVIF_HANDLE;
		m_treectrl->GetItem(&Item);

		// delete TREEITEM that is not from IPCAM when there's child in the tree,  count the number of children 
		    HTREEITEM hItemChild =  m_treectrl->GetChildItem(hItemParent);
			while (hItemChild)
			{
				HTREEITEM hItemChildNext =  m_treectrl->GetNextSiblingItem(hItemChild);
				//vDeleteEvent(hItemChild);
			    m_treectrl->DeleteItem(hItemChild);
				nChildCount++;
				hItemChild = hItemChildNext;			
			}
			
			m_EventInfoStruct.clear();
			m_DisplayedEvent.clear();
			m_nChildCount = nChildCount;
			// GET IPCAM EVENT AND DISPLAY THEM
			BeginWaitCursor();
		
			if(!bCheckEvent(hItemParent, false, true))
			{
					vRefreshEventTree();
					return;
			}
			vDisplayEventInfo(hItemParent);
     		EndWaitCursor();

			mb_EventDirty = true;
}
		
		//if(m_DisplayedEvent.empty()) 
		//{				
		//		// GET IPCAM EVENT AND DISPLAY THEM
		//			BeginWaitCursor();
		//			if(!bCheckEvent(hItemParent, false,true))
		//			{
		//				vRefreshEventTree();
		//				return;
		//			}
		//			vDisplayEventInfo(hItemParent);
	 //        		EndWaitCursor();
		//			m_nChildCount = nChildCount;				
		//}
		//else  //m_DisplayEvent NOT empty
		//{
		//	switch (m_bImportEventSave)
		//	{
		//		case true:
		//			m_EventInfoStruct.clear();
		//			m_DisplayedEvent.clear();
		//			m_nChildCount = nChildCount;
		//			// GET IPCAM EVENT AND DISPLAY THEM
		//			BeginWaitCursor();
		//		
		//			if(!bCheckEvent(hItemParent, false, true))
		//			{
		//					vRefreshEventTree();
		//					return;
		//			}
		//			vDisplayEventInfo(hItemParent);
	 //        		EndWaitCursor();
		//			break;

			//case false: 
			//			
			//		int nSaveYesNo = AfxMessageBox(_T("Do you want to SAVE the Previous IPCAM Events?"), MB_YESNO);

			//		if(nSaveYesNo == IDNO)
			//		{
			//			
			//			// clear Display Event vector
			//			for(int i=0;i<m_DisplayedEvent.size();i++)
			//			{
			//
			//				m_treectrl->DeleteItem(m_DisplayedEvent[i]);
			//
			//			}
			//			
			//				m_EventInfoStruct.clear();
			//				m_DisplayedEvent.clear();
			//				m_nChildCount = nChildCount;
			//				// GET IPCAM EVENT AND DISPLAY THEM
			//				BeginWaitCursor();
			//			
			//				if(!bCheckEvent(hItemParent, false, true))
			//				{
			//					vRefreshEventTree();
			//					return;
			//				}
			//				vDisplayEventInfo(hItemParent);
	  //       				EndWaitCursor();
			//		}
			//		else if (nSaveYesNo == IDYES) 
			//		 {
			//		
			//			// save previous imported ipcam events, given one node will save all
			//			
			//				m_currMode == _T("IPCAM");
			//				vSaveDB(m_DisplayedEvent[0]);	
			//				vRefreshEventTree();					
			//						
			//			
			//		 }
			//	break;
			//}
	//	}
	
//}

void DlgSettingEvent::OnBnClickedButexport()
{
	// TODO: Add your control notification handler code here
	vCGItoCam();
}

void DlgSettingEvent::OnBnClickedButtonSettingEventImportAll()
{
	// TODO: Add your control notification handler code here
	//Tree Stuff...
		HTREEITEM  hItem = m_treectrl->GetSelectedItem();	
		
		if(hItem == NULL)
			return;
		
		hItem = m_treectrl->GetFirstVisibleItem();

		HTREEITEM  hItemParent;
		if (m_treectrl->GetParentItem(hItem) == NULL) //Camera Item
			hItemParent = hItem;
		else //Event Item
			hItemParent =  m_treectrl->GetParentItem(hItem) ;

	// clear Display Event vector
	for(int i=0;i<m_DisplayedEvent.size();i++)
	{
	
		m_treectrl->DeleteItem(m_DisplayedEvent[i]);

	}

	if ( IDNO == AfxMessageBox(IDS_LONG_ACTION,MB_YESNO))
	//if ( IDNO == AfxMessageBox(_T("This action may take a few minutes, Would you like to proceed?"),MB_YESNO))
		return;


BeginWaitCursor();
while (hItemParent != NULL)
{
	// Check if it is root
	if(m_treectrl->GetParentItem(hItemParent) != NULL)
		hItemParent =  m_treectrl->GetParentItem(hItemParent);
	
	bCheckEvent(hItemParent, false, false);
	vDisplayEventInfo(hItemParent);
	//m_treectrl->Expand(hItemParent,TVE_EXPAND);
	hItemParent= m_treectrl->GetNextItem(hItemParent, TVGN_NEXT);
	
	
} 


EndWaitCursor();

}

void DlgSettingEvent::vParseSegment(TCHAR* tszBuffer, map <CString, VERSETS>::iterator pos)
{
	TCHAR* tszLine;
	tszLine = wcstok(tszBuffer, _T("\r\n"));
	while (tszLine)
	{
		TCHAR* tszName = tszLine;
		TCHAR* tsz = wcschr(tszLine, '=');		
		if (tsz)
		{
			*tsz = '\0';
			tsz++;
			pos->second.aSet[tszName] = SETS(tsz, tszName);
		}
		tszLine = wcstok(NULL, _T("\r\n"));
	}	

//	delete tszBuffer;
	return;
}
void DlgSettingEvent::vParseSegmentNew(TCHAR *tszBuffer, map<CString,VERSETS>::iterator pos)
{
	TCHAR* tszLine;
	tszLine = wcstok(tszBuffer, _T(", "));

	while (tszLine)
	{
		TCHAR* tszName = tszLine;
		TCHAR* tsz = wcschr(tszLine, ':');	
		if (tsz)
		{
			*tsz = '\0';
			tsz++;
			pos->second.aSet[tszName] = SETS(tsz, tszName);
			
		}

		tszLine = wcstok(NULL, _T(", "));
	}	
	
	return;
}

bool DlgSettingEvent::bCheckEvent(HTREEITEM hItemParent, bool bDisplayErrorMsg, bool bDisplayErrorMsgDenied)
{
	    map<CString, VERSETS>::iterator pos;

		ScanMap.clear();
		EventInfoMap.clear();
		 
		ScanMap[_T("config/event_info.cgi")];
		pos = ScanMap.begin();

		TVITEM Item;
		Item.hItem = hItemParent;
		Item.mask = TVIF_HANDLE;
		m_treectrl->GetItem(&Item);
	
		CString csIP  =  ((SettingEventInfo*)Item.lParam)->csIP;
		CString csUsername = ((SettingEventInfo*)Item.lParam)->csUsername;
		CString csPass = ((SettingEventInfo*)Item.lParam)->csPass;
		int nWebPort =  ((SettingEventInfo*)Item.lParam)->nWebPort;

		// Get IPCAM Event Info via CGI
	    CString csUrl, csBuffer;
		csUrl.Format(_T("http://%s:%d/config/event_info.cgi"), csIP,nWebPort);
		Curlplus CGIScanData;
	
		csBuffer = CGIScanData.get(csUrl,csUsername,csPass);

		// check if error
		if((csBuffer.Find(_T("Denied")) != -1) || (csBuffer.Find(_T("Error")) != -1) || csBuffer ==_T(""))
		{
			if(bDisplayErrorMsgDenied)
			{
				CString csErrorMsg;
				csErrorMsg.Format(_T("Access Denied or Error @ %s "),csIP);
			    AfxMessageBox(csErrorMsg);
				return false;
			}
			else return false;	
		}

		vParseSegment(csBuffer.GetBuffer(), pos);
	
		//Check IPCAM Event
		map<CString, VERSETS>::iterator pos2;
	
		CString nNumOfevent;


	if(ScanMap[_T("config/event_info.cgi")].aSet[_T("num")].csKey == _T("0"))
		return false;
	else
	{
		CString csEventNames;
		TCHAR * tszEventNames;
		TCHAR * tszLine;
		CString csUrl;
		Curlplus CGIScanData;
		CString csEventInfo;
	

		csEventNames = ScanMap[_T("config/event_info.cgi")].aSet[_T("name")].csKey;
		
		tszEventNames = csEventNames.GetBuffer();
		
		

		tszLine = wcstok(tszEventNames,_T(","));

		if(tszLine)
		iEventNumber.push_back(_wtoi(tszLine));
		else
			iEventNumber.push_back(1);
		
		while (tszLine)
		{
			
			tszLine = wcstok(NULL, _T(","));
			if(tszLine)
			iEventNumber.push_back(_wtoi(tszLine));
		}	


		for(int i = 0 ; i<iEventNumber.size(); i++)
		{
			int nEventNumber;

			nEventNumber = iEventNumber[i];

			CString csSaveEventInfo;

			csSaveEventInfo.Format(_T("%s/event/name=%d"),csIP,nEventNumber);

			EventInfoMap[csSaveEventInfo];

			csUrl.Format(_T("http://%s:%d/config/event.cgi?name=%d"),csIP,nWebPort,nEventNumber);
		
			csEventInfo = CGIScanData.get(csUrl, csUsername,csPass);

			
			pos2 = EventInfoMap.find(csSaveEventInfo);
			vParseSegment(csEventInfo.GetBuffer(), pos2);
			

		}

		return true;
	}


}
void DlgSettingEvent::vDisplayEventInfo(HTREEITEM hItemParent)
{
	map<CString, VERSETS>::iterator posEvent1;
	map<CString, SETS>::iterator posEvent2;
	CString csDisplayEventInfoType , csDisplayEventInfoAction,csDisplayEventInfo, csDisplayEventInfoDuration, csDisplayParam , csInterval;
	int nEventInfoIndex=1;
	csDisplayEventInfo=_T("");
	CString ls_desc;
	
	

	// get HTREEITEM hItemParent's info
	TVITEM Item;
	Item.hItem = hItemParent;
	Item.mask = TVIF_HANDLE;
	m_treectrl->GetItem(&Item);
	m_parent_camidx =   ((SettingEventInfo*)Item.lParam)->CameraIdx;
	

	// Set mode to IPCAM which means event from IPCAM;
	m_currMode = _T("IPCAM");

	for(posEvent1 = EventInfoMap.begin(); posEvent1 != EventInfoMap.end(); posEvent1++)
	{
		for(posEvent2 = posEvent1->second.aSet.begin(); posEvent2 != posEvent1->second.aSet.end(); posEvent2++)
			{
				if(posEvent2->first == _T("event"))
				{
					if(posEvent2->second.csKey == _T("none"))
						csDisplayEventInfoType =  _T("");
					else 
						 csDisplayEventInfoType = posEvent2->second.csKey ;

				}

			    if(posEvent2->first == _T("action"))
				{
					if(posEvent2->second.csKey == _T("none"))
						csDisplayEventInfoAction = _T("");
					else
						 csDisplayEventInfoAction =  posEvent2->second.csKey;

				}
				 
				if(posEvent2->first == _T("schedule"))
				{
					if(posEvent2->second.csKey == _T("always"))
						csDisplayEventInfoDuration = _T("Always");
				

				}

				if(posEvent2->first == _T("param"))
				{
					if(posEvent2->second.csKey != _T(""))
						 csDisplayParam = posEvent2->second.csKey;

				}

				if(posEvent2->first == _T("interval"))
				{
					if(posEvent2->second.csKey != _T(""))
						 csInterval = posEvent2->second.csKey;

				}


				
			}
				
					if( csDisplayEventInfoAction !=_T("") || csDisplayEventInfoType!=_T(""))
					{					
						//change Event Type to DB code
						if(csDisplayEventInfoType == _T("md1"))
							csDisplayEventInfoType = _T("MD");
						else if(csDisplayEventInfoType == _T("md2"))
							csDisplayEventInfoType = _T("MD");
						else if(csDisplayEventInfoType == _T("md3"))
							csDisplayEventInfoType = _T("MD");
						else if(csDisplayEventInfoType == _T("input1"))
							csDisplayEventInfoType = _T("DI");
						else if(csDisplayEventInfoType == _T("input2"))
							csDisplayEventInfoType = _T("DI");
						else if(csDisplayEventInfoType == _T("vloss"))
							csDisplayEventInfoType = _T("VL");

						//change Event Action to DB code
						if(csDisplayEventInfoAction == _T("mail"))
							csDisplayEventInfoAction = _T("EMAIL");
						else if(csDisplayEventInfoAction == _T("ptz"))
							csDisplayEventInfoAction = _T("PTZ");
						else if(csDisplayEventInfoAction == _T("output1"))
							csDisplayEventInfoAction = _T("DO");
						else if(csDisplayEventInfoAction == _T("record"))
							csDisplayEventInfoAction = _T("REC");
									
			
					
					TVINSERTSTRUCTW itemStru={0};
					 m_importEventInfo  = new SettingEventInfo ;
					 SettingEventInfo EventInfo;

					itemStru.item.mask = TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM ;
					itemStru.item.pszText = (LPWSTR)(LPCTSTR)ls_desc;
					itemStru.item.cchTextMax =ls_desc.GetLength();
					itemStru.item.lParam = (LPARAM) &EventInfo; 
					// Set display name on the tree
					csDisplayEventInfo.Format(_T("IPCAM EVENT%d"),nEventInfoIndex);

					// Set the struct EventInfo data
					m_importEventInfo->csEventName = csDisplayEventInfo;
					m_importEventInfo->csDuration = csDisplayEventInfoDuration;
					m_importEventInfo->csEventAction = csDisplayEventInfoAction;
					m_importEventInfo->csEventType = csDisplayEventInfoType;
					m_importEventInfo->csEventSource = _T("CAMERA");
					m_importEventInfo->Interval = _ttoi(csInterval);
					m_importEventInfo->CameraIdx = ((SettingEventInfo*)Item.lParam)->CameraIdx;

					//Set TargetCamIdx and PTZPoint to 0 and Initial some values
					m_importEventInfo->TargetCamIdx = 0;
					m_importEventInfo->PTZPoint = 0;
					m_importEventInfo->AlertLevel = 1; //IPCam
					m_importEventInfo->cActive = _T("Y"); // on
					m_importEventInfo->StreamId = 1; // default :1
					m_importEventInfo->csDurationFrom.Empty();
					m_importEventInfo->csDurationTo.Empty();
					

					// events import from IPCAM, given its eventid = -1
					m_importEventInfo->EventId = -1;

				
					// Set the Param when event action is PTZ and DO
					if(csDisplayEventInfoAction == _T("PTZ") || csDisplayEventInfoAction == _T("DO"))
						m_importEventInfo->csParam = csDisplayParam;
				
					// Set display name  
					ls_desc.Format( _T("IPCAM EVENT%d - %s/%s"),nEventInfoIndex, csDisplayEventInfoType.Trim(),  csDisplayEventInfoAction.Trim());
					
					HTREEITEM lItem = m_treectrl->InsertItem(itemStru.item.mask, (LPCTSTR)ls_desc, 0,0,0,0, (LPARAM)m_importEventInfo , hItemParent, TVI_LAST);
					//m_treectrl->SetItemState(hItemParent,TVIS_EXPANDED ,TVIS_EXPANDED);
					m_DisplayedEvent.push_back(lItem);
					m_EventInfoStruct.push_back(m_importEventInfo);
					nEventInfoIndex++;
									
					}
				

	}// end of for loop

	m_bImportEventSave = false;
	//if there's event in IPCAM, expand the tree

	if(nEventInfoIndex !=1)
		m_treectrl->Expand(hItemParent,TVE_EXPAND);



}

// check if Event Setting Inputs are changed by user or not

bool DlgSettingEvent::bCheckEventInputChanged(SettingEventInfo *pPreviousEventInfo)
{
		if(pPreviousEventInfo == NULL)
			return false;

		SettingEventInfo GetNowEventInfo;
		//Name
		((CEdit*)GetDlgItem(IDC_EDIT_SETTING_EVENT_EVENT_NAME))->GetWindowTextW(GetNowEventInfo.csEventName);

		//Type Code	
		for(int i=0; i<m_ListEventType->GetItemCount(); i++)
		{
				if (m_ListEventType->GetCheck(i))
				{
					strEVENTTYPE* lp = (strEVENTTYPE*)m_ListEventType->GetItemData(i);
					GetNowEventInfo.csEventType = lp->csTypeCode.Trim();
					break;
				}
		}
	
 
		//Action Code
		for(int i=0; i<m_ListAction->GetItemCount(); i++)
		{
				if (m_ListAction->GetCheck(i))
				{
					strEVENTACTION* lp = (strEVENTACTION*)m_ListAction->GetItemData(i);
					GetNowEventInfo.csEventAction = lp->csActionCode.Trim();
					break;
				}
		}


		
		//Interval
		CString	ls_interval;
	
			((CEdit*)GetDlgItem(IDC_EDIT_INTERVAL))->GetWindowTextW(ls_interval);
		if (ls_interval !=_T(""))
			GetNowEventInfo.Interval = _ttoi( ls_interval );
		else
			GetNowEventInfo.Interval = 0;


		//Duration
		if(BST_CHECKED==((CButton*)GetDlgItem(IDC_RADIO_ALWAYS_VALID))->GetCheck())
			GetNowEventInfo.csDuration = _T("Always");
		else if(BST_CHECKED==((CButton*)GetDlgItem(IDC_RADIO_VALID_TIME))->GetCheck())
		{
			GetNowEventInfo.csDuration = _T("Within");
			UpdateData(FALSE);
			
			CTime ls_dateTimePickerFrom;
	
			m_dateTimePickerFrom.GetTime(ls_dateTimePickerFrom);
			GetNowEventInfo.csDurationFrom = ls_dateTimePickerFrom.Format(_T("%H:00:00"));
			
			CTime ls_dateTimePickerTo;
			
			m_dateTimePickerTo.GetTime(ls_dateTimePickerTo);
			GetNowEventInfo.csDurationTo = ls_dateTimePickerTo.Format(_T("%H:00:00"));
	
		}


		// check if any one of the entries was changed, return true
		// event name
		if (pPreviousEventInfo->csEventName != GetNowEventInfo.csEventName)
			return true;
		// event type
		if (pPreviousEventInfo->csEventType != GetNowEventInfo.csEventType)
			return true;
		//event action
		if (pPreviousEventInfo->csEventAction != GetNowEventInfo.csEventAction)
			return true;
		//interval
		if (pPreviousEventInfo->Interval != GetNowEventInfo.Interval)
			return true;
		//duration
		if (pPreviousEventInfo->csDuration!= GetNowEventInfo.csDuration)
			return true;
		// duration timefrom and timeto
		if( pPreviousEventInfo->csDuration == _T("Within") && GetNowEventInfo.csDuration == _T("Within"))
		{
			if (pPreviousEventInfo->csDurationFrom != GetNowEventInfo.csDurationFrom || pPreviousEventInfo->csDurationTo != GetNowEventInfo.csDurationTo)
			{
				
				return true;
			}
		}
	return false;
}

void DlgSettingEvent::OnClose()
{
	// TODO: Add your message handler code here and/or call default
			HTREEITEM  hItem = m_treectrl->GetSelectedItem();
	 
			TVITEM TreeItem;
			TreeItem.hItem = hItem;
			TreeItem.mask = TVIF_HANDLE ;
			m_treectrl->GetItem(&TreeItem);
			SettingEventInfo* lpEventInfo = (SettingEventInfo*) TreeItem.lParam;
		
			if ( m_currMode!= _T(""))
			//if (bCheckEventInputChanged(lpEventInfo) || m_currMode!= _T(""))
			{
					//CString ls_confirm = _T("Save the changes before close?");
					int ll_ret = AfxMessageBox(IDS_SAVE_DATA, MB_YESNO|MB_ICONQUESTION);		
					//int ll_ret = AfxMessageBox(ls_confirm, MB_YESNO|MB_ICONQUESTION);		
					if (ll_ret==IDYES) 	
					{	
							vSaveDB(hItem);			
							vRefreshEventTree();
					}					
			}
		CBaseDialog::OnClose();
}

void DlgSettingEvent::OnDtnDatetimechangeDatetimepicker2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO_VALID_TIME))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_ALWAYS_VALID))->SetCheck(0);
	*pResult = 0;
}

void DlgSettingEvent::OnDtnDatetimechangeDatetimepicker3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO_VALID_TIME))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_ALWAYS_VALID))->SetCheck(0);
	*pResult = 0;
}

SettingEventInfo DlgSettingEvent::GetDialogInfo()
{
	int  ll_camidx=0, ll_ppoint=0, ll_eventid=0,ll_interval=0;
	CString ls_name, ls_source, ls_Type, ls_Action, ls_enable,csValidTimeFrom,csValidTimeTo, ls_duration, ls_camidx,ls_interval,ls_ppoint;

	SettingEventInfo DlgEventInfo={0};
				

		//Save the current event into
	
		//Event ID
		CString ls_eventid;
		((CEdit*)GetDlgItem(IDC_EDITID))->GetWindowTextW(ls_eventid);
		if (ls_eventid !=_T(""))
			DlgEventInfo.EventId = _ttoi( ls_eventid );
		else
			return DlgEventInfo; //shouldn't happen here...


		//Alert
		 if  (((CButton*)GetDlgItem(IDC_RADIO_CRITICAL))->GetCheck() == BST_CHECKED)
				DlgEventInfo.AlertLevel =1;
		 else if (((CButton*)GetDlgItem(IDC_RADIO_WARNING))->GetCheck() == BST_CHECKED)
				DlgEventInfo.AlertLevel =2;
		else
			DlgEventInfo.AlertLevel =3;
	/*	DlgEventInfo.AlertLevel = ((CButton*)GetDlgItem(IDC_RADIO_CRITICAL))->GetCheck();
		if (ll_Alert ==BST_UNCHECKED )
		{
			if (BST_CHECKED==((CButton*)GetDlgItem(IDC_RADIO_WARNING))->GetCheck())
				DlgEventInfo.AlertLevel = 2;
			else
				DlgEventInfo.AlertLevel =3;
		}*/
		 
		//Name
		((CEdit*)GetDlgItem(IDC_EDIT_SETTING_EVENT_EVENT_NAME))->GetWindowTextW(DlgEventInfo.csEventName);

		//Source
		if (BST_CHECKED==((CButton*)GetDlgItem(IDC_RADIODIGITALIN))->GetCheck())
			DlgEventInfo.csEventSource = DIGITALIN;
		else if (BST_CHECKED==((CButton*)GetDlgItem(IDC_RADIO_SETTING_EVENT_SOURCE_SYSTEM))->GetCheck())
			DlgEventInfo.csEventSource=SYSTEM;
		else
			DlgEventInfo.csEventSource = _T("CAMERA");

		//Type Code	
		for(int i=0; i<m_ListEventType->GetItemCount(); i++)
		{
				if (m_ListEventType->GetCheck(i))
				{
					strEVENTTYPE* lp = (strEVENTTYPE*)m_ListEventType->GetItemData(i);
					DlgEventInfo.csEventType = lp->csTypeCode.Trim();
					break;
				}
		}

		
 
		//Action Code
		m_ListAction->UpdateData();
		for(int i=0; i<m_ListAction->GetItemCount(); i++)
		{
				if (m_ListAction->GetCheck(i))
				{
					strEVENTACTION* lp = (strEVENTACTION*)m_ListAction->GetItemData(i);
					DlgEventInfo.csEventAction = lp->csActionCode.Trim();
					break;
				}
		}

		//Active
		if(((CButton*)GetDlgItem(IDC_CHECK_SETTING_EVENT_ENABLE))->GetCheck())
			DlgEventInfo.cActive=_T("Y");
		else
			DlgEventInfo.cActive=_T("N");

		//Target Camera Index
	
		((CEdit*)GetDlgItem(IDC_EDIT_TARGET_CAMIDX))->GetWindowTextW(ls_camidx);
		if (ls_camidx !=_T(""))
			DlgEventInfo.TargetCamIdx = _ttoi( ls_camidx );

		//PTZ Point
	
		//((CEdit*)GetDlgItem(IDC_EDIT_PPOINT))->GetWindowTextW(ls_ppoint);
		((CEdit*)GetDlgItem(IDC_EDIT_SETTING_EVENT_PTZ))->GetWindowTextW(ls_ppoint);
		if (ls_ppoint !=_T(""))
			DlgEventInfo.PTZPoint = _ttoi( ls_ppoint );

				
		//Interval
		((CEdit*)GetDlgItem(IDC_EDIT_INTERVAL))->GetWindowTextW(ls_interval);
		if (ls_interval !=_T(""))
			DlgEventInfo.Interval = _ttoi( ls_interval );
		else
			DlgEventInfo.Interval = 0;


		//Duration
		if(BST_CHECKED==((CButton*)GetDlgItem(IDC_RADIO_ALWAYS_VALID))->GetCheck())
			DlgEventInfo.csDuration = _T("Always");
		else if(BST_CHECKED==((CButton*)GetDlgItem(IDC_RADIO_VALID_TIME))->GetCheck())
		{
			DlgEventInfo.csDuration = _T("Within");
			UpdateData(FALSE);
			
			CTime ls_dateTimePickerFrom;
			
			//m_dateTimePickerFrom.SetFormat(_T("HH:00:00"));
			//m_dateTimePickerTo.SetFormat(_T("HH:00:00"));
			m_dateTimePickerFrom.GetTime(ls_dateTimePickerFrom);
			DlgEventInfo.csDurationFrom = ls_dateTimePickerFrom.Format(_T("%X"));
			
			CTime ls_dateTimePickerTo;
			
			m_dateTimePickerTo.GetTime(ls_dateTimePickerTo);
			DlgEventInfo.csDurationTo = ls_dateTimePickerTo.Format(_T("%X"));
		}

		//Life Cycle
		CString ls_lifeCycle;
		if(BST_CHECKED==((CButton*)GetDlgItem(IDC_RADIO_AUTO_CANCEL))->GetCheck())
			DlgEventInfo.csLifeCycle = _T("Automatic");
		else if(BST_CHECKED==((CButton*)GetDlgItem(IDC_RADIO_VALID_TIME))->GetCheck())
		{
			DlgEventInfo.csLifeCycle = _T("Manual");
		}

		return DlgEventInfo;
}

//Update Tree item/Event info, usually after saved
bool DlgSettingEvent::vUpdateTree(int o_curSelEventId, int o_updateCamidx, int o_updateEventId, SettingEventInfo o_EventInfo)
{
	//if(m_treectrl->GetSelectedItem()==NULL  )

	TVITEM itemCam;

		HTREEITEM itemRoot = m_treectrl->GetRootItem();
		while (itemRoot)
		{				
			TVITEM Item;
			Item.hItem = itemRoot;
			Item.mask = TVIF_HANDLE | TVIF_PARAM ;
			m_treectrl->GetItem(&Item);
			//SettingEventInfo* lpEventInfo = (SettingEventInfo*) TreeItem.lParam;

			//Search the camera idx root
			if (((SettingEventInfo*) Item.lParam)->CameraIdx == o_updateCamidx)
			{
				HTREEITEM hItemChild = m_treectrl->GetChildItem(itemRoot);
				
				//Find the right event and modify it
				while(hItemChild)
				{
					TVITEM ItemEvt;
					ItemEvt.hItem = hItemChild;
					ItemEvt.mask = TVIF_HANDLE | TVIF_PARAM ;
					m_treectrl->GetItem(&ItemEvt);
					if (o_updateEventId == ((SettingEventInfo*) ItemEvt.lParam)->EventId)
					{
						//SettingEventInfo* lEventInfo = (SettingEventInfo*) ItemEvt.lParam;
						CString ls_new;
						ls_new.Format(_T("%s - %s/%s"),  o_EventInfo.csEventName.Trim(),  o_EventInfo.csEventType.Trim(),   o_EventInfo.csEventAction.Trim());
						m_treectrl->SetItemText(hItemChild  , ls_new);
						return true;
					}
					hItemChild = m_treectrl->GetNextSiblingItem(hItemChild);
				}

				//m_treectrl->Expand(itemRoot, TVE_EXPAND );
				//m_treectrl->SelectItem(itemRoot);
				return true;
			}

			itemRoot = m_treectrl->GetNextItem(itemRoot, TVGN_NEXT);

		}

	return false;
}

//Make Setting button enabled/disabled based on the action code
bool DlgSettingEvent::vSettingButton(CString o_action)
{
	bool lb_enable;
	lb_enable = false;
	if (o_action==_T("PTZ")|| o_action==_T("REC") || (o_action==_T("MSM"))|| (o_action==_T("EMAIL"))|| (o_action==_T("MSM")))
			lb_enable = true;
	else 
			lb_enable = false;

	((CButton*)GetDlgItem(IDC_BUTFTPEMAIL))->EnableWindow(lb_enable);
	return lb_enable;
}

void DlgSettingEvent::OnBnClickedButsetting()
{
	// TODO: Add your control notification handler code here
	 	CString	ls_action;
		for(int i=0; i<m_ListAction->GetItemCount(); i++)
		{
				if (m_ListAction->GetCheck(i))
				{
					strEVENTACTION* lp = (strEVENTACTION*)m_ListAction->GetItemData(i);
					ls_action = lp->csActionCode.Trim();
					break;
				}
		}

		if (ls_action ==_T("REC") )
		{
			CDlgSettingRecording lDlg;
			if (lDlg.DoModal() == IDOK)
			{
			}
		}
		else if (ls_action ==_T("MSM") || ls_action ==_T("EMAIL") )
		{
			CDlgFtp lDlg;
			if (lDlg.DoModal() == IDOK)
			{
			}
		}
}

void DlgSettingEvent::OnEnChangeEditSettingEventEventName()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBaseDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	/*mb_EventDirty = true;
	if (m_currMode != "ADD")  m_currMode = "UPDATE";*/
	vToModify();
}

void DlgSettingEvent::OnEnChangeEditSettingEventOd()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBaseDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	vToModify();
}

void DlgSettingEvent::vToModify(void)
{
	mb_EventDirty = true;
	if (m_currMode != "ADD")  m_currMode = "UPDATE";
}

void DlgSettingEvent::OnEnChangeEditSettingEventPtz()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBaseDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	vToModify();
}

bool DlgSettingEvent::fnCheckToSave(bool obSilent)
{
	return DlgSettingEvent::fnCheckToSave(obSilent, *m_memEvent);
}

// Handle save ask info for tree control 
bool DlgSettingEvent::fnCheckToSave(bool obSilent, TVITEM	oItem)
{
	if (!mb_EventDirty) return true;
				CString ls_sql;

	if (!obSilent) //NOT Slient, ask to save
	{
		INT iRet = AfxMessageBox(AFX_IDS_DLG_SETEVENT4,MB_YESNO);
		if(iRet ==IDNO)
		{	
			if (m_currMode == "ADD")
				m_treectrl->DeleteItem(m_treectrl->GetSelectedItem());
			
		 	//vRefreshEventTree();
		}
		else	//Save the
		{
			if (m_currMode == "ADD")
			{
				//SettingEventInfo* (IPcamEventInfo) = ((SettingEventInfo*)m_memEvent->lParam);
				HTREEITEM hItem = m_treectrl->GetSelectedItem();
				if(hItem != NULL) vSaveDB(hItem);
				int i=0;
			}
			else if (m_currMode == "DELETE")
			{
				ls_sql.Format( _T(" DELETE  FROM ec_camera_event WHERE camera_eventid=%d"), mi_delEventId);
				pDB->ExecuteSQL(ls_sql);
			}
			m_currMode == "IPCAM";
		}
	}

	mb_EventDirty = false;

	return false;
}

//void DlgSettingEvent::OnNMClickListEventType(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}
