#include "stdafx.h"
#include "EventManager.h"
#include "MainFrm.h"

CEventManager::CEventManager(void)
{
	fnInit();
	
}

CEventManager::~CEventManager(void)
{
}

void CEventManager::fnInit(void)
{
	pDB = &(((CMainFrame*)AfxGetMainWnd())->m_DB);
	fnLoadEvent();
	int i=0;
}

//Load All the camera events into Structure 
bool CEventManager::fnLoadEvent()
{
		if (!pDB) return false;	//pDB not valid
		CString	ls_sql, ls_EventName, ls_EventSource, ls_EventType, ls_Action, csDuration,csDurationFrom, csDurationTo;
		CString	ls_desc;
		CDBVariant		varEventId, varCamIdx, varStreamId, varLevel, varTargetCam, varPoint, varInterval;

		TVINSERTSTRUCTW itemStru={0};
		ls_sql.Format(_T("SELECT camera_eventid,camera_idx,alert_level,event_name,event_source_code, event_type_code,device_action_code,target_camera_idx,ptz_point,interval_,cycle_type, CONVERT(VARCHAR(8), cycle_from, 8) AS cycle_from, CONVERT(VARCHAR(8), cycle_till, 8) AS cycle_till FROM ec_camera_event   ORDER BY  camera_idx, camera_eventid ") );
		CRecordset m_rsEvent(pDB);		
	
		m_rsEvent.Open(CRecordset::forwardOnly, ls_sql);

		while (!m_rsEvent.IsEOF())
		{
			EVENTINFO* strEventInfo = new EVENTINFO;

			m_rsEvent.GetFieldValue(_T("camera_eventid"), varEventId, SQL_C_SSHORT);
			strEventInfo->EventId = varEventId.m_iVal;

			m_rsEvent.GetFieldValue(_T("camera_idx"), varCamIdx, SQL_C_SSHORT);	
			strEventInfo->camera_idx = varCamIdx.m_iVal;
			
			//Alert Level
			m_rsEvent.GetFieldValue(_T("alert_level"), varLevel, SQL_C_SSHORT);	
			strEventInfo->AlertLevel = varLevel.m_iVal;

			m_rsEvent.GetFieldValue(_T("event_name"), ls_EventName);
			strEventInfo->csEventName = ls_EventName.Trim();

			m_rsEvent.GetFieldValue(_T("event_source_code"), ls_EventSource);
			strEventInfo->csEventSource = ls_EventSource.Trim();

			m_rsEvent.GetFieldValue(_T("event_type_code"), ls_EventType);
			strEventInfo->csEventType = ls_EventType.Trim();

			m_rsEvent.GetFieldValue(_T("device_action_code"), ls_Action);
			strEventInfo->csEventAction = ls_Action.Trim();

			//Target Camera Index
			m_rsEvent.GetFieldValue(_T("target_camera_idx"), varTargetCam, SQL_C_SSHORT);	
			strEventInfo->TargetCamIdx = varTargetCam.m_iVal;

				m_rsEvent.GetFieldValue(_T("ptz_point"), varPoint, SQL_C_SSHORT);	
				strEventInfo->PTZPoint = varPoint.m_iVal;

				m_rsEvent.GetFieldValue(_T("interval_"), varInterval, SQL_C_SSHORT);	
				strEventInfo->Interval = varInterval.m_iVal;
			
			// Duration	
			m_rsEvent.GetFieldValue(_T("cycle_type"), csDuration);
			strEventInfo->csDuration = csDuration.Trim();
			
			m_rsEvent.GetFieldValue(_T("cycle_from"), csDurationFrom);
			strEventInfo->csDurationFrom = csDurationFrom.Trim();

			m_rsEvent.GetFieldValue(_T("cycle_till"), csDurationTo);
			strEventInfo->csDurationTo = csDurationTo.Trim();

			//itemStru.item.mask = TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM ;
			//itemStru.item.pszText = (LPWSTR)(LPCTSTR)ls_desc;
			//itemStru.item.cchTextMax =ls_desc.GetLength();

			//itemStru.item.lParam = (LPARAM)strEventInfo ;

//			ls_desc.Format( _T("%s - %s/%s"),ls_EventName.Trim(), ls_EventType.Trim(), ls_Action.Trim());
//			HTREEITEM lItem = m_treectrl->InsertItem(itemStru.item.mask, (LPCTSTR)ls_desc, 0,0,0,0, (LPARAM)strEventInfo , o_parentTreeItem, TVI_LAST);
 			mvt_eventInfo.push_back(strEventInfo);

			delete strEventInfo;
			m_rsEvent.MoveNext();
	}
}

bool CEventManager::fnLoadEvent(int oCamIdx)
{
	if (!pDB) return false;	//pDB not valid
	return false;
}
