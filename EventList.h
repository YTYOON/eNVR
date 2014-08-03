//#include "afxcmn.h"
#pragma once
#include "stdafx.h"
#include "ListCtrlEx.h"
#include "TabpageEventDialog.h"
#include "EventReceive.h"
#include "IPCamTreeView.h"



class CMainFrame;



class CEventList : public CListCtrlEx
{
	public:
		CEventList(void);
		~CEventList(void);
		DECLARE_MESSAGE_MAP()
		
		//TCITEM tcPtz, tcEvent;
		CWnd*	m_Parent;
		CDialog *m_pDialogs[2];
		//CDialog	m_test;

		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

		void ofCreateEventList();
		void ofManualAddEventList();

		int m_MaxWidth, m_MaxHeight;
	
		//afx_msg void OnTabPTZButtonClicked(UINT nCmdID);
		////afx_msg void OnTabPTZButtonClickedStop(UINT nCmdID);
		///*---Zoom In Out button---*/
		//afx_msg void OnTabZoomButtonClicked(UINT nCmdID);
		////Preset combobox
		//afx_msg void OnCbnSelchangeComboPTZPreset();
		//// Patrol button
		//afx_msg void OnBnClickedPatrolBut();
		////PTZ Setting button
		//afx_msg void OnBnClickedPTZSettingBut();
		//afx_msg void OnBnClickedPresetSet();
		//afx_msg void OnBnClickedPresetGo();

		//CStatic m_PresetP;
		//CComboBox* pCBPreset;	
		//CDatabase m_DB;
		CMainFrame* m_CMainFrame;
		HCURSOR hTabCursor;

			CBrush					m_brush;
			//CListCtrl*				pEventList;

			int m_nPageCount;
			//LVITEM lvItem;
public:
//	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);


protected:
    DWORD m_nMaxCount;
	CameraEvent m_CameraEvent;
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		//Array to hold the list of dialog boxes/tab pages for CListCtrlEx

		//CDialog Array Variable to hold the dialogs 
		//Function to Create the dialog boxes during startup
		void InitDialogs();

		//Function to activate the tab dialog boxes 
		void ActivateTabDialogs();


public:
		void ClickBitmapButton();
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		//afx_msg void OnEventPopSetting();
		afx_msg void OnEventClearLog();

public:
public:
	EventInfo m_EventInfo;

	static void vCallBack(EventInfo *m_EventInfo, void *lpParam);
	//bool bCameraEnable(CString csCamIDX);
	//CString csEventCode(CString csCode);
	LVITEM lvItem;
	int EventIDCount;
	CCriticalSection m_CCriticalSection;
	CIPCamTreeCtrl* m_CIPCamTreeCtrl;
	CImageList m_images;

	//vector <CEventReceive*> vEventRec;
	void vDeleteEventRec(CString csIP);
	void vEventReConnection(CString csIP);
	CIPCamTreeCtrl* GetCIPCamTreeCtrl(){return m_CIPCamTreeCtrl;};
	//CString csGetEventAlarm(int iAlertLevel);
	//int iGetAlertLevelDB(CString csCameraID, CString csEventType, CString csStreamID);
	//void vInsertEventLogDB(CString SQLInsertEventLog);
	//CameraEvent GetCameraEventDB(CString csCameraIDX, CString csEventType, CString csStreamID);
	//CString csGetEventTypeDB(CString csCGIEventType);
 	bool vDBConnect(void);

	//CameraEvent m_CameraEvent;
	//static UINT _stdcall PushToListCtrl(LPVOID lpParam);
	CEventReceive* m_EventRec;

	HANDLE hThreadEventRec;

	bool bCheckCameraExist(CString csIP);
    afx_msg void OnDestroy();

	bool EventReceiveFlag;
	CDatabase m_DB;
    afx_msg void OnSize(UINT nType, int cx, int cy);
};
