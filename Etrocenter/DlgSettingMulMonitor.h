#pragma once
#include "resource.h"
#include "BaseDialog.h"
#include "INI.h"
#include "afxcmn.h"
#include "Monitors.h"
#include "MultiMonitor.h"
#include "afxwin.h"
#include "afxext.h"

#define	MAXGROUP 20
#define MAXMONITOR 10
#define MAXMATRIX 10

#define MAINCONSOLE _T("MAIN")
#define LIVEVIEW _T("LIVEVIEW")
#define EMAP _T("EMAP")
#define EVENTM _T("EVENTM")
#define PLAYBACK _T("PLAYBACK")
#define ACCESSCONTROL _T("AC")
#define GATECONTROL   _T("GC")
#define NONE _T("NONE")
#define YES _T("Y")

// CDlgSettingMulMonitor dialog

class CDlgSettingMulMonitor : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgSettingMulMonitor)

public:
	CDlgSettingMulMonitor(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSettingMulMonitor();
	CDatabase*	 pDB;

// Dialog Data
	enum { IDD = IDD_SETTING_MULMONITOR };

public:

 
	struct STRMATRIX
	{
		INT		mcode;
		char	mdesc[10];
	};

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

protected:
	void vInitList(void);
//	int vDBConnect(void);

public:
//	afx_msg void OnBnClickedOk();
protected:
	int m_totMonitor;

public:
	//CListCtrl		m_LCStartList;
	//CBitmapButton	m_Matrix;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedButaddstart();

protected:
	int		m_nMatrixCode;

	//CBitmapButton ma_BitmapMon[];

	void vArrangeMonitor(void);
	CBitmapButton m_Monitor1, m_Monitor2, m_Monitor3, m_Monitor4, m_Monitor5, m_Monitor6;
public:
//	afx_msg void OnBnClickedButmon1();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedRadioliveview();
protected:
	void vBuildComboValue(void);
	void HideEmapOption();
public:
	afx_msg void OnBnClickedRadiomain();
	CComboBox* m_ComboGroup;
	int m_curMonitor; //Keep track of seleted monitor

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	//STRMONITOR ma_Monitor[MAXMONITOR];
	//STRMATRIX	ma_Matrix[MAXMATRIX];

	
	vector <int> m_vctGroupId;
	vector <int> m_vctChainId;
	vector <STRMONITOR> ma_Monitor;

	CString m_StartApp;
//	afx_msg void OnBnSetfocusButmon1();
	void vSetRadioFocus(int nIndex);
	afx_msg void OnBnClickedButmon1();
	afx_msg void OnBnClickedButmon2();
	afx_msg void OnBnClickedButmon3();
	afx_msg void OnBnClickedButmon4();
	afx_msg void OnBnClickedButmon5();
	afx_msg void OnBnClickedButmon6();
	void vMonitorChanged(int nMonitor);

	CMonitor m_Primonitor; // = CMonitors::GetPrimaryMonitor();

	bool mb_Dirty;
	CIniReader		m_ini;

//	afx_msg void OnBnKillfocusButmon1();
	afx_msg void OnCbnSelchangeCombogroup();
protected:
	virtual void OnOK();
public:
	CBitmapButton m_ButMatrix;
	afx_msg void OnBnClickedButmatrix();
	CStatic m_StMatrix;
	bool vSaveDB(void);
	afx_msg void OnBnClickedRadioplayback();
	afx_msg void OnBnClickedRadioevent();
	afx_msg void OnBnClickedRadioemap();
	void vRadioClick(CString o_Mode);
	void vResetMonitorFocus(void);
	bool vUpdateStruct(CString osMode);
	CString csGetMatrixCode(int lret);
	afx_msg void OnBnClickedRadionone();
	afx_msg void OnCbnSelchangeComboEmapGroup();
	afx_msg void OnBnClickedRadioAccessControl();
	afx_msg void OnBnClickedRadioGateControl();
	CComboBox m_combo_emap_group;
	CComboBox m_combo_GateControl;
	void vBuildGateComboValue(void);
	afx_msg void OnCbnSelchangeComboGateControl();
	int m_nMainFrameStartMonitorIdx;	
	int m_nPlayBackStartMonitorIdx;	
	void vResetMainPlaybackMonitorIdx(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
