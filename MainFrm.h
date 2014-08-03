// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#pragma once

#include "NVR.h"
#include "MSMCaptionBar.h"
#include "MSMToolBar.h"
#include "WorkSpaceToolBar.h"
#include "EcBaseTab.h"
#include "monitors.h"
#include "multimonitor.h"
#include "DlgFlyAway.h"
#include "EventList.h"
#include "DlgPTZ.h"
#include "TaskTreeCtrl.h"
#include "MyTasksPane.h"



//#ifdef PLUGIN 
#include "DlgEmap.h"
#include "DlgAc.h"
#include "DlgGate.h"
#include "DlgLPR.h"
//#endif



#define CMDIFrameWnd CMDIFrameWndEx
class CTreeTabCtrl;
class CNVRBk;

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	virtual ~CMainFrame();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  
	CMSMCaptionBar  m_wndCaptionBar;
	CMFCMenuBar		m_wndMenuBar;
	CMSMToolBar		m_wndToolBar;	
	CMyTasksPane	m_wndTaskPane;
	DlgMatrixTab	m_wndMatrix;
    CDlgPTZ        m_PTZ;
	CEventList		m_wndEventList;
    BOOL		    m_bIsHighColor;
	CTreeTabCtrl*	m_pTreeCtrlTab;

public:
    //HWND GetMatrixWindow(){return m_pTreeCtrlTab->m_hWnd;};
	struct ServiceInfo
	{
		CString csEventName;
		bool bRestart;
		CMainFrame* pThis;
	};

protected:
	LRESULT OnDeleteFlyAway(WPARAM,LPARAM);
	LRESULT OnDockingIcon(WPARAM wParam, LPARAM lParam);
	
	BOOL CreateTaskPane();
    void vSetWindowStyle();
    BOOL bCustomizeModeCheck();
    void SetupMemoryBitmapSize (const CSize& sz); 
	BOOL GetAeroType(){return m_wndCaptionBar.m_bCustomizeMode;}
	
	int CMainFrame::OnPreCreate();
	virtual BOOL OnShowPopupMenu (CMFCPopupMenu* pMenuPopup);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	CTaskTreeCtrl*	vGetIpcamTreeCtrl() { return GetTreeCtrlType();}
	CTaskTreeCtrl* GetTreeView()
	{
		return GetTreeCtrlType();
	};
/////////////////////////////////////////////////////
/////// begin merge from etrocenter /////////////////
/////////////////////////////////////////////////////
public:
	vector<CRect>	m_rectMainFrm;
	int				m_LayoutIdx;
	int				m_OrgLayoutIdx;
	int				m_LiveViewMax;
	bool			m_bOnFullScreen;
	CDatabase		m_DB;
	bool			m_bDBOpen;
	long			m_nOriginalStyle;
	CIniReader		m_ini;
	bool			m_bDisplayOSD;
	CDlgEmap*		m_emap;
	CDlgAc*			m_ac;
	CDlgGate*		m_gate;
	CDlgLPR*		m_lpr;
	CMonitor		m_monitor;
	CMonitors		m_monitors;
	vector<CMonitor>	m_vctMonitor;
	vector<STRMONITOR>	m_vctMonitorConfig;
	CDlgFlyAway*		m_pDlgFlyAway;
	vector<CDlgFlyAway*>	m_vctDlgFlyAwayRecycleBin;
	int						m_nMainStartIdx;
	vector <bool>			m_vctbServiceStatus;
	CString					m_csAppPath;
	vector<int> m_vctnLiveViewIdx;
	vector<int> m_vctnPlayBackIdx;
	vector<int> m_vctnEventIdx;
	vector<int> m_vctnEmapIdx;
	vector<int> m_vctnAccessControlIdx;
	vector<int> m_vctnGateControlIdx;
	bool m_bAutoScan;
	bool m_bAutoScanIPWall;
	UINT m_nAutoScanTime;
	bool m_bEventLV;
	bool m_bRestartByHealth;
	bool m_bIPWallSelected;
	bool m_bWizardEnable;
	bool m_bStartupFullScreen;
	bool m_bGroupAutoPlay;
	int	 m_iAuth;
	TCHAR szCurrentUser[1024];
	bool m_bLoginInfoReady;
	CCrypto m_Crypto;
	vector <int> m_vctnGroupAutoPlayIdx;
	vector<CString> m_vctcsGroupNames;
	bool m_bGroupAutoScanPause;
	bool m_bGroupAutoScanResume;
	int m_nCurrentPlayGroupId;
	void *m_plParam;
	CDlgSettingTag		m_SettingDlg;
	CNVRBk *m_pbk;

public:
	void vGetPathInfo();
    void StartWizard();
	void OnToolbarDropDown(NMHDR *pnmtb, LRESULT *plr);
	void vSetFocusVideoSet(CNetSession* pNetSession, NODEITEM* pNode);
	void vSetFocusVideoSet(NODEITEM* pNode, WORD* nMsgIdx=0, CVideoThin* pVideoThin=0);
	void vItemChanged(NODEITEM* pNode);
	vector<CRect>		GetRectVct();
	void	vAlocateViewCell(UINT nSize);
	void	vVideoPresent(vector <NODEITEM *> pNodeItem);
	CRect	bGetRect();
	bool	vIsFullScreen();	
	void	RunLiveView();
	void	OnFullScreen(bool o_flag);
	int		vDockingBar(bool o_createFlag);
	void	vFreeVideoSet(void);
	void	vFreeVideoSet(int CamId);
	void	vIsDragging(bool m_bLDragging);
	bool	bOutOfRange(int nNodeCount);
	void	vCheckDockWnd(int nGroupCaptionHeight);
	void	vReCheckConnect(BOOL bIsConnect);		//Larry add 102-12-10//
	void	vSetMatrixBtnHot(UINT uID);			//Larry add 102-12-10//
	void	CheckRecordPathAvailable();
	void	OnMenuExitfullscreen();
	void	vSetMonitorStartupEvent();
	void	vSetMonitorStopEvent();
	bool	vOpenEmapView();
	void	vDocking();
	void	vSetStartMonitor();
	void	vGetMultiMonitorConfig();
	void	vStartProgramToMonitor(int nType,int nMonitorIdx);
	void	vGetAppPath();
	void	vCloseService(int nService);
	void	vRestartService(int nService);
	bool	bQueryServiceStatus(CString pName);
	void	vCheckAllServiceStatus();
	void	fnOpenAc();
	void	fnOpenGate();
	void	fnOpenAc(CRect rect, CRect rect2);
	void	fnOpenLPR();
	void	fnOpenGate(CRect rect,int nChainId, CRect rect2);
	CString csGetServiceName(int nService);
	void	vSetMainFrmToMonitor(); 
	void	vCreateDlgFlyAway(int nMatrixIdx,vector <NODEITEM*> vctpGroupNode, CRect rect,CRect rect2, bool bAutoPlay);
	void	vSetAutoScan(bool bEnable , UINT nMyCBoxID);
	void	vSetEventLV();
	void	vScreenToViewClientRect(POINT * pt);
	void	vGetCurrentAllNodeItems(vector<NODEITEM*> &vctpNode);
	void	vDatabaseCheck(void);
	CString csDecryptAuthPassword( CString &csEncrypt );
	void	strtohex(const char* str, unsigned char* hex);
	void	vGetGroupCam(vector<NODEITEM*>& vctNode);
	void	vGetGroupCam(int nGroupId, vector<NODEITEM*>& vctNode);
	bool	bCheckGroupCamNum(int nGroupId);
	void	vGetGroupName();
	int		nGetCurrentSelectedGroupId();
	void	vSetGroupAutoScan(bool bEnable);
	void	vSetAutoScanCallBack(void* fnParam, void* plParam){m_AutoScanCallBack = (AutoScanCallBack)fnParam; m_plParam = (void*)plParam;};
	void	vPauseLiveView(void);
	void	vSaveAndResetOSD(void);
	bool	bGetCameraCount();
	bool	bCheckCameraCountWithMassAssignIP(int iCheckNumber);
	void	OpenDialogTag(int i_Mode);
	int		vSettingManager(int o_SettingID);
	void	vStartDisplayAutoPlay();
	
	typedef void(*AutoScanCallBack)(void* lpParam);
	AutoScanCallBack m_AutoScanCallBack;
	static UINT  ServiceThread(LPVOID lpParam);

protected:
	BOOL	VerifyBarState(LPCTSTR lpszProfileName);
	bool	bDBConnect(void);
	int		nAuthorize();

private:
	void GetToolbarID(UINT& uiToolbarHotID, UINT& uiToolbarColdID, UINT& uiToolbarDisID, UINT& uiToolbarPresID, UINT& uiMenuID);
	CTaskTreeCtrl* GetTreeCtrlType();
	void CreateTreePane(const CRect& rect);
	void CreateMatrixPane();
	void CreatePTZPane();
	void CreateEventListPane(const CRect& rect);
	void OpenAddDeviceDlg();
	void CheckCmdline(CString& csAuto, const CString& csUser, const CString& csEnPass);

public:
	//{{AFX_MSG(CMainFrame)
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	afx_msg LRESULT OnToolbarContextMenu(WPARAM,LPARAM);
	afx_msg void OnUpdateViewWorkspace(CCmdUI* pCmdUI);
	afx_msg void OnWindowManager();
	afx_msg void OnMax();
	afx_msg void OnMin();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEmapView(CRect rect);
	afx_msg void OnEmapView(CRect rect, int nGroupId);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLiveView();
	afx_msg void OnRecodringSet();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnFirmwareUpgrade();
	afx_msg void OnSetNetwork();
	afx_msg void OnRemoteRestart();
	afx_msg void OnRemoteClose();	
	afx_msg void OnMenuReLogin();
	afx_msg void OnMenuNotifyExit();
	afx_msg void OnRecordClose();
	afx_msg void OnRecordRestart();
	afx_msg void OnEventRestart();
	afx_msg void OnEventClose();
	afx_msg void OnStreamRestart();
	afx_msg void OnStreamClose();
	afx_msg void OnHealthRestart();
	afx_msg void OnHealthClose();
	afx_msg void OnRangeCmds(UINT nID);
	afx_msg void RunFullScreen();
	afx_msg void OnSystemSetting();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnJoystick();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
