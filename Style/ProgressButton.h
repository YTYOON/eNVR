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
#include "CpuUsage.h"

class  CProgressButton;
class CMyProgressCtrl : public CStatic//CProgressCtrl
{
public:
    //DECLARE_DYNCREATE(CMyProgressCtrl) 
    //DECLARE_SERIAL(CMyProgressCtrl)   
    CMyProgressCtrl();
    virtual ~CMyProgressCtrl();
protected:
    CProgressCtrl progressCtrl;
 	CCpuUsage m_usageTotal;	
 	CBrush m_BrushBg;
public:
	//{{AFX_MSG(CMyProgressCtrl)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public: 	
   void UpdateCpuUsage();
    void SetRange (int iMin, int iMax) {progressCtrl.SetRange(iMin, iMax);};
    void SetPos (int iValue) { progressCtrl.SetPos(iValue);}; 
    int  GetPos (){return progressCtrl.GetPos();};
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
};
class CProgressButton : public CMFCToolBarButton  
{
	DECLARE_SERIAL(CProgressButton)

public:
	CProgressButton();
	CProgressButton (UINT uiId,
			int iImage = -1,
			DWORD dwStyle = 0,
			int iWidth = 0);

	virtual ~CProgressButton();

	void SetRange (int iMin, int iMax);
	void SetValue (int iValue, BOOL bNotify = TRUE);

	static int GetPos (UINT uiCmd);

// Overrides:
    virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz=TRUE, BOOL bCustomizeMode=FALSE, BOOL bHighlight=FALSE, BOOL bDrawBorder=TRUE, BOOL bGrayDisabledButtons=TRUE);

	virtual void Serialize(CArchive& ar);
	virtual void CopyFrom (const CMFCToolBarButton& src);
	virtual SIZE OnCalculateSize (CDC* pDC, const CSize& sizeDefault, BOOL bHorz);
	virtual void OnShow (BOOL bShow);
	virtual void OnChangeParentWnd (CWnd* pWndParent);
	virtual void OnMove ();
	virtual void OnSize (int iSize);
	virtual HWND GetHwnd ()
	{	
		return m_cpuProgress.GetSafeHwnd ();
	}

	virtual BOOL CanBeStretched () const
	{	
		return TRUE;	
	}

	virtual BOOL HaveHotBorder () const
	{
		return FALSE;
	}

	//virtual void OnDraw (CDC* /*pDC*/, const CRect& /*rect*/, CMFCToolBarImages* /*pImages*/,
	//					BOOL /*bHorz*/ = TRUE, BOOL /*bCustomizeMode*/ = FALSE,
	//					BOOL /*bHighlight*/ = FALSE,
	//					BOOL /*bDrawBorder*/ = TRUE,
	//					BOOL /*bGrayDisabledButtons*/ = TRUE) {}
    
   void UpdateCpuUsage()
    {
		m_cpuProgress.UpdateCpuUsage();
    }
// Attributes:
public:
	//CCustomSliderCtrl	m_wndSlider;
	int				m_iWidth;
	DWORD			m_dwStyle;
	int				m_nMin;
	int				m_nMax;
	int				m_nValue;
private:

	CMyProgressCtrl	m_cpuProgress;
    //CStatic         m_cpuText;
	HANDLE         m_hTimer;
	HANDLE         m_hStop;

};
