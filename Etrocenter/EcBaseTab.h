#include "afxcmn.h"
#include "BaseBitmapButton.h"
#include "EcBaseSlider.h"
#include "TabpageEventDialog.h"
#include "ColorBox.h"
#include "EventReceive.h"
#include "IPCamTreeView.h"
#pragma once


class CMainFrame;

class CEcBaseTab : public CTabCtrl
{
	public:
		
		CEcBaseTab(void);
		~CEcBaseTab(void);
		friend class CEcPtz;
		DECLARE_MESSAGE_MAP()
		
		TCITEM tcPtz, tcEvent;
		CWnd*	m_Parent;
		CDialog *m_pDialogs[2];
		CDialog	m_test;

		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

		BOOL CreateTab(CDialog *, UINT, int);		
		BOOL InitCreate();

		//void ShowHideImage(bool o_show);
		static UINT ShowHideImage(LPVOID lpParam);
		bool m_lb_flag;

//		void ofCreateEventList();
		void ofManualAddEventList();

		int m_MaxWidth, m_MaxHeight;
	
		afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnTabPTZButtonClicked(UINT nCmdID);
		//afx_msg void OnTabPTZButtonClickedStop(UINT nCmdID);
		/*---Zoom In Out button---*/
		afx_msg void OnTabZoomButtonClicked(UINT nCmdID);
		//Preset combobox
		afx_msg void OnCbnSelchangeComboPTZPreset();
		// Patrol button
		afx_msg void OnBnClickedPatrolBut();
		//PTZ Setting button
		afx_msg void OnBnClickedPTZSettingBut();
		afx_msg void OnBnClickedPresetSet();
		afx_msg void OnBnClickedPresetGo();

		CStatic m_PresetP;
		CComboBox* pCBPreset;	
		//CDatabase m_DB;
		CMainFrame* m_CMainFrame;
		HCURSOR hTabCursor;

protected:
			CBitmapButton* pHome;
			CBitmapButton* pTop;
			CBitmapButton* pBot;
			CBitmapButton* pLft;
			CBitmapButton* pRgt;
			CBitmapButton* pTRgt;
			CBitmapButton* pBRgt;
			CBitmapButton* pTLft;
			CBitmapButton* pBLft;
			CBitmapButton* pZoomIn;
			CBitmapButton* pZoomOut;
			CButton* pPTZSetPreset;
			CButton* pPTZGo;

			CColorBox*	pPatrol;
			CColorBox*	pPTZSet;
			CEdit*      pPTZInput;

			CEcBaseSlider*			pZoom;
			CTabpageEventDialog*	pEvent;
			CBrush					m_brush;
			//CListCtrl*				pEventList;

			int m_nPageCount;
			//LVITEM lvItem;
public:
//	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);

	typedef struct _LVITEM { 
	UINT mask; 
	int iItem; 
	int iSubItem; 
	UINT state; 
	UINT stateMask; 
	LPTSTR pszText; 
	int cchTextMax; 
	int iImage; 
	LPARAM lParam;
	#if (_WIN32_IE >= 0x0300)
		int iIndent;
	#endif
} LVITEM, FAR *LPLVITEM;


protected:
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		//Array to hold the list of dialog boxes/tab pages for CTabCtrl
		int m_DialogID[1];

		//CDialog Array Variable to hold the dialogs 
		CDialog *m_Dialog[1];

		//Function to Create the dialog boxes during startup
		void InitDialogs();

		//Function to activate the tab dialog boxes 
		void ActivateTabDialogs();


public:
		void ClickBitmapButton();
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		//afx_msg void OnEventPopSetting();
		afx_msg void OnEventClearLog();

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

public:
	EventInfo m_EventInfo;

	//static void vCallBack(EventInfo *m_EventInfo, void *lpParam);
	//static void vTreeCrlCallBack(CEventReceive *m_CEventReceive, void *lpParam);
	//static UINT _stdcall GetEventFromService(LPVOID lpParam);
	//void ParseEvent(CString lParam);
	//void vInsertEventList(CString csEventIP, CString csCGIEventType, CString csStreamID, CString csCameraIDX, CString csDBEventType, CameraEvent m_CameraEvent);
	bool bCameraEnable(CString csCamIDX);
	void vChangeTreeCtrl(CString csEventIP, CString csEventType, CString csCamIDX, CameraEvent m_CameraEvent);

	//vector <NODEITEM*>* m_pvtNode;
	
	CString csEventCode(CString csCode);
	//CListCtrl*				pEventList;
	//int m_nPageCount;
	LVITEM lvItem;
	int EventIDCount;
	CCriticalSection m_CCriticalSection;
	CIPCamTreeCtrl* m_CIPCamTreeCtrl;
	CImageList m_images;

	//vector <CEventReceive*> vEventRec;
	void vDeleteEventRec(CString csIP);
	void vEventReConnection(CString csIP);
	CIPCamTreeCtrl* GetCIPCamTreeCtrl(){return m_CIPCamTreeCtrl;};
	CString csGetEventAlarm(int iAlertLevel);
	//int iGetAlertLevelDB(CString csCameraID, CString csEventType, CString csStreamID);
	void vInsertEventLogDB(CString SQLInsertEventLog);
	//CameraEvent GetCameraEventDB(CString csCameraIDX, CString csEventType, CString csStreamID);
	CString csGetEventTypeDB(CString csCGIEventType);
 	bool vDBConnect(void);
	int vGoLiveView(CString oIP, CString oType);

	//CameraEvent m_CameraEvent;
	//static UINT _stdcall PushToListCtrl(LPVOID lpParam);
	//CEventReceive* m_EventRec;

	HANDLE hThreadEventRec;

	bool bCheckCameraExist(CString csIP);
    afx_msg void OnDestroy();
};
