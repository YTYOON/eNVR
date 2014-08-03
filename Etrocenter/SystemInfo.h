
// BarPtz.h: interface for the CBarPtz class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_BARPTZ_H__1865B771_A1C2_4AA9_A161_E3DACCE3449F__INCLUDED_)
//#define AFX_BARPTZ_H__1865B771_A1C2_4AA9_A161_E3DACCE3449F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "SRC\scbarg.h"
#include "stdafx.h"
#include "NVSNetLib.h"
#include "BaseBitmapButton.h"
#include "CpuUsage.h"
#include "NVR.h"

#define  IDT_TIMER  511
#define	 DEFAULT_HEIGHT	300	

class CSystemInfo : public CDockablePane  
{
public:
	void vSetNetSession(CNetSession* m_pNetsession);
	CSystemInfo();
	virtual ~CSystemInfo();
	void vSetHeight();

	void vGetLocal();
	//void CSystemInfo::PrintMemoryInfo( DWORD processID );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarPtz)
	//}}AFX_VIRTUAL
protected:
	char m_x, m_y;
	//LRESULT OnJoyStickMove(WPARAM wParam, LPARAM lParam);
	//BOOL bGetJoyStickInfo();
	CNetSession* m_pNetsession;
	CFrameWnd frame;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg UINT  StartTimer (UINT TimerDuration);  // Start the Timer
		
	void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CBitmap		m_Logo;
	CDC			m_CDCLogo;
	CStatic		m_curTime;
	CStatic		m_cpu;
	CProgressCtrl	m_cpuProgress;
 
	CString		m_sCurrtime;
	CBrush		m_brush;
	UINT Timeval;
	CBaseBitmapButton m_help;
	CStatic m_BitmapLogo;
    CBitmap m_Bmp;
	HBITMAP	m_hBmpCustom;

public:
//	afx_msg void OnPaint();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
//	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void vOpenHelp();
	double CPU_Usage();
	CCpuUsage usageTotal;
	void vOnClickLogo();
	//CAboutDlg* m_aboutDlg;

    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};

//#endif // !defined(AFX_BARPTZ_H__1865B771_A1C2_4AA9_A161_E3DACCE3449F__INCLUDED_)



//#pragma once
//
//
//// CSystemInfo
//
//class CSystemInfo : public CSizingControlBarG
//{
//	DECLARE_DYNAMIC(CSystemInfo)
//
//public:
//	CSystemInfo();
//	virtual ~CSystemInfo();
//	void CBarPtz::vSetNetSession(CNetSession* pNetsession);
//
//protected:
//	CNetSession* m_pNetsession;
//	DECLARE_MESSAGE_MAP()
//	
//public:
////	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
//public:
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//public:
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//};


