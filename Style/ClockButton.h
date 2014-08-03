#pragma once

//#include "StaticClockHint.h"
// CClockButton
class CStaticClock : public CStatic
{
	DECLARE_DYNAMIC(CStaticClock)
public:
	//DECLARE_DYNCREATE(CStaticClock) 
   // DECLARE_SERIAL(CSzjsbList)	
	CStaticClock();
	virtual ~CStaticClock();
protected:
    CBrush m_BrushBg;
		
public:
    CString m_csTime;

	//{{AFX_MSG(CStaticClock)
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CClockButton : public CMFCToolBarButton
{
	//DECLARE_DYNAMIC(CClockButton)
	DECLARE_SERIAL(CClockButton)

//	DECLARE_SERIAL(CMFCToolBarButton)


public:
	CClockButton(UINT uiID, int iImage, LPCTSTR lpszText=NULL, BOOL bUserButton=FALSE, BOOL bLocked=FALSE);
    CClockButton();
	virtual ~CClockButton();
	virtual void OnMove ();
	virtual void OnSize (int iSize);
	virtual SIZE CClockButton::OnCalculateSize (CDC* /*pDC*/, const CSize& /*sizeDefault*/, BOOL bHorz);
	virtual void OnChangeParentWnd (CWnd* pWndParent);
	void SetWindowText(CString& cs){m_wndTxt.SetWindowTextW(cs);};//m_wndTxt.m_csTime = cs;m_wndTxt.UpdateWindow();};
	void vSetClockIcon();

	CStatic m_wndImage;
	CStaticClock m_wndTxt;

protected:
    int m_iWidth;
    DWORD m_dwStyle;
    CBitmap m_bmpClock;
	//DECLARE_MESSAGE_MAP()
};


