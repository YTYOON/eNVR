#pragma once

#include "BaseDialog.h"
#include "BitmapSlider.h"
#include "DlgPtzSetting.h"
#include "mycurl.h"
#include "afxwin.h"
#include "ButtonDisk.h"
#include "PTZGetInfo.h"
#include "VideoThin.h"
#include "VirtualKeyboard.h"
#include "DlgJoyStick.h"

// CDlgPTZ dialog

class CDlgPTZ : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgPTZ)

public:
	enum  PTZAction {PTZINIT=-1, P_L=0, P_R, T_U, T_D, Z_I, Z_O, PL_TU, PL_TD, PR_TU, PR_TD,STOP,F_I,F_O} ;
	enum  PTZCGITYPE {PTZ=0,ZOOM,FOCUS};
	CDlgPTZ(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPTZ();

public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnBitmapSliderMoved(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBitmapSliderMoving(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBitmapPtzPress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBitmapPtzRelease(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedBtnPtzPrsetSet();
	afx_msg void OnBnClickedBtnPtzPrsetGo();
	afx_msg void OnBnClickedPTZSettingBut();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	// Dialog Data
	enum { IDD = IDD_DLG_PTZ };

protected:
	VK* m_pvk;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	CButtonDisk   m_btnDisk;
	CBitmapSlider m_sliderFocus;
	CBitmapSlider m_sliderZoom;
	CBitmapSlider m_togglePartol;
	CBitmapSlider m_toggleAutoScan;
	CBitmapSlider m_toggleAutoFocus;
	CEdit		  m_editPTZPreset;
	CButton		  m_btnPTZPresetSet;
	CButton       m_btnPTZPresetGo;

	CPTZGetInfo*	m_pPTZGetInfo;		//Larry add 102-11-22//
	//BYTE* m_pszGreyDib;
	WORD*		m_pMsgIdxStatus;			//enum {MSG_LOGO, MSG_DISCONNECTION, MSG_RECONNECTION, MSG_NORMAL};//
	BOOL			m_bCanRunCGI;
	int				m_nCheckConnect;

	BOOL			m_bInitvAttach;
	BOOL			m_bJoystickMove;

protected:
	BOOL bCreateControler();


protected:
#define PTZ_THRESHOLD  2
#define NUMOFPRESET 6 // present+1
	CNetSession* m_pNetsession;
	NODEITEM *m_pNode;
	BYTE	 m_mac[6];
	PTZAuthInfo m_AuthInfo;    
	bool m_bCGISent;
	int m_nPreInput;
	bool m_bJoystickZMove;
	vector <int> m_vctnPatrolPoints;
	//bool m_bPatrolEnable;
	bool m_bPTZAutoScan;
	//HANDLE m_hPatrolThread;
	HANDLE m_hPTZCGIThread;
	HANDLE m_hPTZGetInfoThread;
	int			m_nCheckAutoType;

	//int m_nPatrolSeqDurIdx;
	vector< map <int,pair_INT>> m_vctmapPatrolSeqDurSets;
	map <int,pair_INT> m_mapPatrolSeqDurSets;
	UINT Timeval;
	Curlplus mycurl;
	int m_nPTZSpeed; 
	int m_nJoyStickSpeed;
	int m_x, m_y, m_z;

	CDatabase* pDB;
	PTZCGITYPE m_ptzCGIType;
	int        m_nSliderPos;
	PTZAction  m_ptzAction;

	BOOL m_bGetPatrolNun;

	PTZAction m_OldPTZaction;
	int	m_nOldJoyStickSpeed;

	int	m_nOldZoomPos;
	int	m_nOldFocusPos;
	BOOL m_bWaitCGI_FOCUSCONT;		//When AutoFocus is set 1 waiting for the web status respone back//
	DWORD m_dwtmJoyStickFOCUSC;
	BOOL		m_bJoystickFocus;
	BOOL		m_bJoystickZoom;
	

	BOOL		m_bLiveViewAutoScan;

	BOOL CDlgPTZ::bGetJoyStickInfo();
	LRESULT CDlgPTZ::OnJoyStickMove(WPARAM wParam, LPARAM lParam);
	LRESULT CDlgPTZ::OnJoyStickZMove(WPARAM wParam, LPARAM lParam);

	void CDlgPTZ::vPTZMoveCGI(PTZAction ptzaction, int nPTZSpeed);
	void CDlgPTZ::vPTZMoveVSMP(PTZAction ptzaction, int nPTZSpeed);
	void CDlgPTZ::OnLButtonUp(UINT nFlags, CPoint point);
	bool CDlgPTZ::bCheckPTZEnable();
	BOOL CDlgPTZ::PreTranslateMessage(MSG* pMsg);


	void CDlgPTZ::vGoToPresetPoint(int nPreset);
	void CDlgPTZ::OnBnClickedPatrolBut(bool bIsOn);

	/////////////////Larry add 102-11-07///////////////////////
	void CDlgPTZ::OnBnClickedAutoScanBut(bool bIsOn);
	void CDlgPTZ::OnBnClickedAutoFocusBut(bool bIsOn);
	void CDlgPTZ::DisablePTZAutoSetting();

	void CDlgPTZ::ResetPatrolStatus();
	CVideoThin*		m_pVideoThin;
	/////////////////////////////////////////////////////////////

	void vReSetPTZPatrolSetting(CString csPatrolSeq);

	//static UINT  __stdcall CDlgPTZ::thdPatrol(LPVOID lpParam);
	static UINT  __stdcall CDlgPTZ::thdPTZCGI(LPVOID lpParam);

	static UINT  __stdcall CDlgPTZ::thdPTZGetInfo(LPVOID lpParam);


	void CDlgPTZ::vJoyStickInputConvert(int nX);
	void CDlgPTZ::vSetPresetPoint(int nPreset);

	void CDlgPTZ::vSetLiveViewAuto(BOOL bAuto);

	HRESULT CDlgPTZ::UpdateInputState();
	void CDlgPTZ::vJoyStickInputConvert2(int nX, int nY=0);

public:
	//void CDlgPTZ::vAttach(NODEITEM *pNode, BYTE* pszGreyDib=0);
	void CDlgPTZ::vAttach(NODEITEM *pNode, WORD* pMsgIdx=0, CVideoThin* pVideoThin=0);  
	void CDlgPTZ::vSetNetSession(CNetSession* pNetsession);	     
	void CDlgPTZ::vGetMACAddress(NODEITEM *p_Node);
	void CDlgPTZ::vCheckCGIStatus(int nCheckType);
	void CDlgPTZ::vReCheckConnect(BOOL bIsConnect);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL DestroyWindow();

	LRESULT OnUpdateView(WPARAM,LPARAM);
	afx_msg void OnEnSetfocusEditPtzPreset();
	afx_msg void OnEnKillfocusEditPtzPreset();

	void InitJoyStick(HWND hDlg);
	HWND m_hJoyDlg;
	CDlgJoyStick* m_pDlgJoyDebug;
};
