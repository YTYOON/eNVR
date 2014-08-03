
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "SRC\scbarg.h"
#include "stdafx.h"
#include "NVSNetLib.h"
#include "ColorBox.h"
#include "EcBaseTab.h"
#include "DlgPTZSetting.h"
#include "mycurl.h"


#define  IDT_TIMER  511 

enum  PTZAction {P_L=0, P_R, T_U, T_D, Z_I, Z_O, PL_TU, PL_TD, PR_TU, PR_TD,STOP,F_I,F_O} ;

class CEcPtz : public CDockablePane  
{
public:

	CEcBaseTab* GetEcBaseTab(){return &m_PtzTab;};
	

	CEcPtz();
	virtual ~CEcPtz();
	void vAttach(NODEITEM *pNode);
	void vPTZMoveCGI(PTZAction ptzaction, int nPTZSpeed);
	void vPTZMoveVSMP(PTZAction ptzaction, int nPTZSpeed);
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarPtz)
	//}}AFX_VIRTUAL
protected:
	//char m_x, m_y;
	int m_x, m_y, m_z;
	LRESULT OnJoyStickZMove(WPARAM wParam, LPARAM lParam);
	LRESULT OnJoyStickMove(WPARAM wParam, LPARAM lParam);
	//LRESULT OnJoyStickMove(WPARAM wParam, LPARAM lParam);
	BOOL bGetJoyStickInfo();

	CNetSession* m_pNetsession;
	NODEITEM *m_pNode;
	BYTE	 m_mac[6];

	/*CString  m_IP;
	CString  m_Port;
	CString  m_CameraName;
	CString  m_User;
	CString  m_Pass;*/
	
	PTZAuthInfo m_AuthInfo;

	CFrameWnd frame;

//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg UINT  StartTimer (UINT TimerDuration);  // Start the Timer
		
	void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CBitmap		m_Logo;
	CDC			m_CDCLogo;
	CStatic		m_curTime;
	CString		m_sCurrtime;
	CColorBox	m_butPTZ, m_butEvent;
	CEcBaseTab	m_PtzTab;	
	CBrush		m_brush;
	

	UINT Timeval;
	Curlplus mycurl;
	int m_nPTZSpeed; 
public:
	CString csDSN, csDBUser, csDBPass, csODBC;	//For Database
	//CDatabase DBQuery;
	CDatabase* pDB;

public:
	void vSetNetSession(CNetSession* pNetsession);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTabPTZButtonClicked(UINT nCmdID);
	afx_msg void OnTabPTZButtonClickedStop();
	afx_msg void OnTabZoomButtonClicked(UINT nCmdId);
	void OnCbnSelchangeComboPTZPreset(int nPreset);
	void OnBnClickedPatrolBut();
	void OnBnClickedPTZSettingBut();
	void vOnZoomScroll(PTZAction ptzaction,int nPTZSpeed);
	//void vDBConnection();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	bool bCheckPTZEnable();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void vGetMACAddress(NODEITEM *p_Node);
	bool m_bCGISent;
	void vJoyStickInputConvert(int nX);
	int m_nPreInput;
	bool m_bJoystickZMove;
	void vSetPresetPoint(int nPreset);
	vector <int> m_vctnPatrolPoints;
	bool m_bPatrolEnable;
	static UINT __stdcall thdPatrol(LPVOID lpParam);
	HANDLE m_hPatrolThread;
	int m_nPatrolSeqDurIdx;
	vector< map <int,pair_INT> > m_vctmapPatrolSeqDurSets;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};