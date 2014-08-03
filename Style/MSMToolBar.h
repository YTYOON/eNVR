// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#pragma once
#include "ProgressButton.h"
#include "ClockButton.h"
// MSMToolBar.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CMSMToolBar window

class CMSMToolBar : public CMFCToolBar
{
	DECLARE_DYNCREATE(CMSMToolBar)

// Construction
public:
	CMSMToolBar();

	virtual ~CMSMToolBar ();

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual BOOL LoadToolBar (UINT uiResID, UINT uiColdResID = 0, 
					UINT uiMenuResID = 0, BOOL bLocked = FALSE,
					UINT uiDisabledResID = 0, UINT uiMenuDisabledResID = 0,
  				    UINT uiHotResID = 0, UINT uiPressedResID = 0);

	virtual BOOL SetButtons(const UINT* lpIDArray, int nIDCount, BOOL bRemapImages = TRUE);

protected:
    virtual void RemoveAllButtons();
    void vGetLocalTime(CString& str);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSysColorChange();

	DECLARE_MESSAGE_MAP()

	virtual BOOL DrawButton (CDC* pDC, CMFCToolBarButton* pButton, 
		CMFCToolBarImages* pImages, BOOL bHighlighted,
		BOOL bDrawDisabledImages);
    void vGetLogoWidth();

protected:
    DWORD m_logoMeanWidth;
    HCURSOR m_hCursor;
    CRect m_rect;
    CRect m_rectLogo;
    bool  m_bLogoClick;
	static CMFCToolBarImages m_PressedImages; // Shared pressed images
    CProgressButton m_btnSlider;
    CClockButton m_btnClock;

private:
	int		m_nAuth;

	bool isNeedInsertBtn(int iCmd);
public:
	void SetAuthorize(int nAuth){ m_nAuth = nAuth; };
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnDestroy();
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
