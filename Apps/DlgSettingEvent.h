#pragma once

#include "CGIVar.h"
#include "afxwin.h"
#include "mycurl.h"
#include "Resource.h"
#include "BaseDialog.h"
#include "afxcmn.h"
#include "afxext.h"
#include "afxdtctl.h"
//#define CAMERA _T("CAMERA")
#define DIGITALIN _T("DI")
#define SYSTEM _T("SYSTEM")
//#define MAXMESSAGE "The Maximum number of Event per IPCAM is 6, please delete the previous event before adding a new one"




struct SettingEventInfo
{
	int		EventId;
	int		CameraIdx;	
	int		StreamId;
	int		AlertLevel;
	int		TargetCamIdx;
	int		PTZPoint;
	int	    Interval;
	int     nWebPort;
	CString csEventName;
	CString csEventSource;
	CString csEventType;
	CString csEventAction;
	CString csAlarmLevel;
	CString csLifeCycle;
	CString csDuration;
	CString csDurationFrom;
	CString csDurationTo;
	CString	cActive;
	CString csIP;
	CString csUsername;
	CString csPass;
	CString csModelName;
	CString csParam;
} ;

struct strEVENTTYPE
{
	CString csTypeCode;
	CString csDefineArea;
};

struct strEVENTACTION
{
	CString csActionCode;
	CString csCamSelect;
};

// DlgSettingEvent dialog

class DlgSettingEvent : public CBaseDialog 
{
	DECLARE_DYNAMIC(DlgSettingEvent)

public:
	DlgSettingEvent(CWnd* pParent = NULL);   // standard constructor
	
	virtual ~DlgSettingEvent();

// Dialog Data
	enum { IDD = IDD_DLG_SETTING_EVENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl* m_ListEventType;
	CListCtrl*  m_ListAction;
	CBitmapButton*		m_CamSel;

	int	m_curEventID;
	int nMd, nInput, nOutput,nPTZParam,nODParam,nParam;
	CString csEventType, csEventAction, csCameraId;
		
	afx_msg void OnBnClickedRadioSettingEventIPCam();
	
private:
	int cEventRuleCounter;
	int m_parent_camidx;
	
public:
	afx_msg void OnBnClickedCheckSettingEventEnable();
	//afx_msg void OnLbnSelchangeListEventType();
	afx_msg void OnBnClickedOk();
	//afx_msg void OnLbnSelchangeEventAction();

protected:
	HTREEITEM  hItemNew;
	
public:
	CTreeCtrl *m_treectrl;
	CToolTipCtrl* m_ToolTip;

	afx_msg void OnNMClickDlgSettingEventTree1(NMHDR *pNMHDR, LRESULT *pResult);
	
	//bool vDBConnect(void);
	//CDatabase m_DB;
	CDatabase* pDB;

	afx_msg void OnTvnSelchangedDlgSettingEventTree1(NMHDR *pNMHDR, LRESULT *pResult);
	bool vAddDBEvent(int o_cameraIdx, HTREEITEM o_parentTreeItem);
 
	afx_msg void OnBnClickedButftpemail();

//	afx_msg void OnTvnSelchangingDlgSettingEventTree1(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	bool mb_EventDirty;
public:
	/*afx_msg void OnBnClickedRadioWarning();
	afx_msg void OnBnClickedRadioCritical();
	afx_msg void OnBnClickedRadioInfo();*/

	//If Entrty Mode is "NEW" then don't save the event to DB, use structure to pass back,
	//Let the trigger screen handle the saving
	CString m_EntryMode, m_camName, camIP;
	
	//Current mode is to control the current status, IPCAM is after Import, ADD/UPDATE is waiting for save
	CString m_currMode;	  

	bool vClearEventType(void);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CBitmapButton m_ButAdd;
	CBitmapButton m_ButDel;
	CBitmapButton m_ButImport;
	CBitmapButton m_ButExport;
	CBitmapButton m_ButImportAll;

	afx_msg void OnBnClickedSeteventButadd1();
	afx_msg void OnBnClickedSeteventButdel1();
protected:
	void vPopulateType(CString o_Source, CString o_Type);
	void vPopulateType(CString o_Source, CString o_Type, CString o_Action);

public:
//	afx_msg void OnLvnColumnclickListEventType(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnNMClickListEventType(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnHdnItemchangedListEventType(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListEventType(NMHDR *pNMHDR, LRESULT *pResult);
	//void vPopulateAction(CString o_Type);
	void vPopulateAction(CString o_Type, CString o_Action);
	bool vCGItoCam(void);
	afx_msg void OnLvnItemchangedEventAction(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButselcam();
	void vPopulateTargetCam(int o_CamIdx, int o_PPoint);
	afx_msg void OnBnClickedRadiodigitalin();
	afx_msg void OnBnClickedRadioSettingEventSourceSystem();
	afx_msg void OnTvnSelchangingDlgSettingEventTree1(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	bool vSaveDB(HTREEITEM hItem);
public:
	afx_msg void OnBnClickedRadioWarning();
	afx_msg void OnBnClickedRadioCritical();
	afx_msg void OnBnClickedRadioInfo();
//	afx_msg void OnEnChangeEditSettingEventEventName();
	bool vSelectRadioSource(CString o_Source);
	bool vSelectRadioAlert(int o_Level);
	afx_msg void OnBnClickedButsave();
protected:
	int vRadioClear(bool ob_Source, bool ob_Alarm, bool ob_Duraion);
public:
	void vAddNewEvent(void);
	void vRefreshEventTree(void);
	void vDeleteEvent(void);
	void vDeleteEvent(int o_camidx);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	void vSaveCycle(int o_Eventid, CString o_mode);
public:
	int m_entryCamidx;
	int m_entryEventid;
	void vFindCam(void);
	int	m_targetCamidx;
protected:
	void vEnableCam(CString o_actionCode);
public:
	CDateTimeCtrl m_dateTimePickerFrom;
	CDateTimeCtrl m_dateTimePickerTo;
	void vPopulateDuration(int o_eventId,SettingEventInfo * pEventInfo);
	afx_msg void OnBnClickedButimport();
	afx_msg void OnBnClickedButexport();
	afx_msg void OnBnClickedButtonSettingEventImportAll();
	void vParseSegment(TCHAR* tszBuffer, map <CString, VERSETS>::iterator pos);
	void vParseSegmentNew(TCHAR* tszBuffer, map <CString, VERSETS>::iterator pos);
	bool bCheckEvent(HTREEITEM hItemParent, bool bDisplayErrorMsg,bool bDisplayErrorMsgDenied);
	map<CString, VERSETS> EventInfoMap;
	map <CString, VERSETS> ScanMap;
	void vDisplayEventInfo(HTREEITEM hItemParent);
	SettingEventInfo * m_importEventInfo;
	vector <HTREEITEM> m_DisplayedEvent;
	vector <SettingEventInfo*> m_EventInfoStruct;
	vector <SettingEventInfo*> m_EventInfoStructRecycleBin;
	vector <strEVENTTYPE *> m_strEVENTTYPERecycleBin;
	vector <strEVENTACTION* > m_strEVENTACTIONRecycleBin;
	vector <LVITEMW* > m_LVITEMRecycleBin;
	void  vSaveDBStruct(SettingEventInfo* EventInfo);
	int m_nChildCount;

	// available Event number in ipcam
	vector <int> iEventNumber;

	//check if event input changed
	bool bCheckEventInputChanged(SettingEventInfo *pPreviousEventInfo);
	afx_msg void OnClose();
	afx_msg void OnDtnDatetimechangeDatetimepicker2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeDatetimepicker3(NMHDR *pNMHDR, LRESULT *pResult);
	SettingEventInfo GetDialogInfo();
	void vDeleteEvent(HTREEITEM hItem);
	bool m_bImportEventSave;
	void vFreeMemory();

	//TVITEM m_hEventItem;

	CString m_MaxMessage, m_SaveMessage;
	bool vUpdateTree(int o_curSelEventId, int o_updateCamidx, int o_updateEventId, SettingEventInfo o_EventInfo);
	int	mi_ImportCamIdx;
	int	mi_delEventId;
	bool vSettingButton(CString o_action);
	afx_msg void OnBnClickedButsetting();
	CString	m_curAction;
	afx_msg void OnEnChangeEditSettingEventEventName();
	afx_msg void OnEnChangeEditSettingEventOd();
	void vToModify(void);
	afx_msg void OnEnChangeEditSettingEventPtz();
	// Handle Check To save ask info for tree control 
	bool fnCheckToSave(bool obSilent);
	bool fnCheckToSave(bool obSilent, TVITEM oItem);
	TVITEM* m_memEvent;
//	afx_msg void OnNMClickListEventType(NMHDR *pNMHDR, LRESULT *pResult);
};


