// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "ProgressButton.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CProgressButton, CMFCToolBarButton, 1)

static const int nDefaultWidth = 240;
static const int nDefaultHeight = 20;
static const int nProgressStart = 10;
static const int nProgressWidth = 220;
static const int nProgressHeight = 15;
static const int nProgressLeftMergin = 110;
static const int nProgressTopMergin = 80;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#pragma warning (disable : 4355)
 

CProgressButton::CProgressButton():m_hTimer(NULL),m_hStop(NULL)
//:/m_wndSlider (*this),
	//	m_cpuProgress(this)
{
	m_dwStyle = WS_CHILD | SS_SIMPLE;
	m_iWidth = nDefaultWidth;
	m_nMin = 0;
	m_nMax = 0;
	m_nValue = 0;
    m_bText = true;
	CSA sa;
	m_hStop = CreateEvent(&sa.m_sa, false, false, STOP_EVENT_NAME);
}
//**************************************************************************************
CProgressButton::CProgressButton (UINT uiId,
			int iImage,
			DWORD dwStyle,
			int iWidth) :
	CMFCToolBarButton (uiId, iImage)
		//,m_cpuProgress (this)
{
	m_dwStyle = WS_CHILD | SS_SIMPLE;
	m_iWidth = (iWidth == 0) ? nDefaultWidth : iWidth;
	m_nMin = 0;
	m_nMax = 0;
	m_nValue = 0;
}

#pragma warning (default : 4355)

//**************************************************************************************
CProgressButton::~CProgressButton()
{
	if (m_cpuProgress.GetSafeHwnd () != NULL)
	{
		SetEvent(m_hStop);
		m_cpuProgress.DestroyWindow ();
	}
}
//**************************************************************************************
SIZE CProgressButton::OnCalculateSize (CDC* /*pDC*/, const CSize& /*sizeDefault*/, BOOL bHorz)
{
	if (!IsVisible ())
	{
		if (m_cpuProgress.GetSafeHwnd () != NULL)
		{
			m_cpuProgress.ShowWindow (SW_HIDE);
		}

		return CSize (0,0);
	}

	if (bHorz)
	{
		if (m_cpuProgress.GetSafeHwnd () != NULL && !IsHidden ())
		{
			m_cpuProgress.ShowWindow (SW_SHOWNOACTIVATE);
			//m_cpuProgress.ModifyStyle (TBS_VERT, TBS_HORZ);
		}

		return CSize (m_iWidth, nDefaultHeight);
	}
	else
	{
		if (m_cpuProgress.GetSafeHwnd () != NULL && !IsHidden ())
		{
			m_cpuProgress.ShowWindow (SW_SHOWNOACTIVATE);
			//m_cpuProgress.ModifyStyle (TBS_HORZ, TBS_VERT);
		}

		return CSize (nDefaultHeight, m_iWidth);
	}
}
//**************************************************************************************
void CProgressButton::Serialize(CArchive& ar)
{
	CMFCToolBarButton::Serialize (ar);

	if (ar.IsLoading ())
	{
		ar >> m_iWidth;
		ar >> m_dwStyle;

		int		nMin;
		int		nMax;
		int		nValue;

		ar >> nMin;
		ar >> nMax;
		ar >> nValue;

		SetRange (nMin, nMax);
		SetValue (nValue, FALSE);
	}
	else
	{
		ar << m_iWidth;
		ar << m_dwStyle;
		ar << m_nMin;
		ar << m_nMax;
		ar << m_nValue;
	}
}
//**************************************************************************************
void CProgressButton::OnMove ()
{
	if (m_cpuProgress.GetSafeHwnd () != NULL &&
		(m_cpuProgress.GetStyle () & WS_VISIBLE))
	{
		m_cpuProgress.SetWindowPos (NULL,
			m_rect.left + 1 +nProgressStart, m_rect.top +  (nProgressTopMergin- nDefaultHeight)/2, m_rect.Width () - 2 - nProgressStart, m_rect.Height () - 2,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
}
//**************************************************************************************
void CProgressButton::OnSize (int iSize)
{
	m_iWidth = iSize;
	m_rect.right = m_rect.left + m_iWidth;

	if (m_cpuProgress.GetSafeHwnd () != NULL &&
		(m_cpuProgress.GetStyle () & WS_VISIBLE))
	{
		m_cpuProgress.SetWindowPos (NULL,
			m_rect.left + 1, m_rect.top + 1, m_rect.Width () - 2, m_rect.Height () - 2,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
}
//**************************************************************************************
void CProgressButton::OnChangeParentWnd (CWnd* pWndParent)
{

	if (m_cpuProgress.GetSafeHwnd () != NULL)
	{
		CWnd* pWndParentCurr = m_cpuProgress.GetParent ();
		ASSERT (pWndParentCurr != NULL);

		if (pWndParent != NULL && pWndParentCurr != NULL &&
			pWndParentCurr->GetSafeHwnd () == pWndParent->GetSafeHwnd ())
		{
			return;
		}
		SetEvent(m_hStop);
		m_cpuProgress.DestroyWindow();
	}

	if (pWndParent == NULL || pWndParent->GetSafeHwnd () == NULL)
	{
		return;
	}

	CString str;
	LOADSTRING(str, IDC_CPU);
	if (!m_cpuProgress.Create (str, m_dwStyle, m_rect, pWndParent, m_nID))
	{
		ASSERT (FALSE);
		return;
	}

}
//**************************************************************************************
void CProgressButton::CopyFrom (const CMFCToolBarButton& s)
{
	CMFCToolBarButton::CopyFrom (s);

	const CProgressButton& src = (const CProgressButton&) s;

	m_dwStyle = src.m_dwStyle;
	m_iWidth = src.m_iWidth;
	m_nMin = src.m_nMin;
	m_nMax = src.m_nMax;
	m_nValue = src.m_nValue;
}
//************************************************************************************
void CProgressButton::SetRange (int iMin, int iMax)
{
	m_nMin = iMin;
	m_nMax = iMax;

	if (m_cpuProgress.GetSafeHwnd () != NULL)
	{
		m_cpuProgress.SetRange (iMin, iMax);
	}

	CObList listButtons;
	if (CMFCToolBar::GetCommandButtons (m_nID, listButtons) > 0)
	{
		for (POSITION pos = listButtons.GetHeadPosition (); pos != NULL;)
		{
			CProgressButton* pSlider = DYNAMIC_DOWNCAST (CProgressButton, 
												listButtons.GetNext (pos));
			if (pSlider != NULL && pSlider != this)
			{
				pSlider->m_nMin = iMin;
				pSlider->m_nMax = iMax;

				if (pSlider->m_cpuProgress.GetSafeHwnd () != NULL)
				{
					pSlider->m_cpuProgress.SetRange (iMin, iMax);
				}
			}
		}
	}
}
//************************************************************************************
void CProgressButton::SetValue (int iValue, BOOL bNotify)
{
	m_nValue = iValue;
	if (GetHwnd () != NULL)
	{
		m_cpuProgress.SetPos (iValue);
	}

	CObList listButtons;
	if (CMFCToolBar::GetCommandButtons (m_nID, listButtons) > 0)
	{
		for (POSITION pos = listButtons.GetHeadPosition (); pos != NULL;)
		{
			CProgressButton* pSlider = DYNAMIC_DOWNCAST (CProgressButton, 
												listButtons.GetNext (pos));
			if (pSlider != NULL && pSlider != this)
			{
				pSlider->m_nValue = iValue;

				if (pSlider->GetHwnd () != NULL)
				{
					pSlider->m_cpuProgress.SetPos (iValue);
				}
			}
		}
	}

	if (bNotify && m_cpuProgress.GetSafeHwnd () != NULL)
	{
		m_cpuProgress.GetOwner ()->SendMessage (WM_COMMAND, m_nID);
	}
}
//************************************************************************************
int CProgressButton::GetPos (UINT uiCmd)
{
	CObList listButtons;
	if (CMFCToolBar::GetCommandButtons (uiCmd, listButtons) > 0)
	{
		for (POSITION pos = listButtons.GetHeadPosition (); pos != NULL;)
		{
			CProgressButton* pSlider = DYNAMIC_DOWNCAST (CProgressButton, 
												listButtons.GetNext (pos));
			if (pSlider != NULL && pSlider->GetHwnd () != NULL)
			{
				return pSlider->m_cpuProgress.GetPos ();
			}
		}
	}

	return -1;
}
//****************************************************************************************
void CProgressButton::OnShow (BOOL bShow)
{
	if (m_cpuProgress.GetSafeHwnd () != NULL)
	{
		if (bShow)
		{
			m_cpuProgress.ShowWindow (SW_SHOWNOACTIVATE);
			OnMove ();
		}
		else
		{
			m_cpuProgress.ShowWindow (SW_HIDE);
            //m_cpuProgress.SetWindowText(_T("CPU"));
		}
	}
}
void CProgressButton::OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz, BOOL bCustomizeMode, BOOL bHighlight, BOOL bDrawBorder, BOOL bGrayDisabledButtons)
{
     //CMFCToolBarButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);
     //pDC->TextOut(rect.left, rect.top,_T("TestSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS"),true);
}
//IMPLEMENT_SERIAL(CMyProgressCtrl, CProgressCtrl)
CMyProgressCtrl::CMyProgressCtrl()
:m_BrushBg(GLOBALBKCOLORLAYER1)
{
	
}
CMyProgressCtrl::~CMyProgressCtrl()
{
}

BEGIN_MESSAGE_MAP(CMyProgressCtrl, CProgressCtrl)
    ON_WM_DESTROY()
    ON_WM_CREATE()
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()  

void CMyProgressCtrl::OnDestroy()
{
    CSA sa;
    HANDLE h = CreateEvent(&sa.m_sa, false, false, STOP_EVENT_NAME );//);
    ::SetEvent(h);
    CStatic::OnDestroy();
} 


int CMyProgressCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CStatic::OnCreate(lpCreateStruct) == -1)
        return -1;
    //CRect rect(nProgressLeftMergin, 0, nProgressLeftMergin+nProgressWidth, nProgressHeight);
    CRect rect(nProgressLeftMergin, 0, nProgressWidth, nProgressHeight);
    int nID(65960);
    DWORD dwStyle = WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_NOTICKS;
    bool bRet = progressCtrl.Create(dwStyle, rect, this, nID);
    progressCtrl.SetBarColor(RGB(35,200,35));
    progressCtrl.SetBkColor (RGB(35,35,35));
    progressCtrl.SetRange(0, 100);
    progressCtrl.ShowWindow(true);    
    SetWindowText(_T("Cpu :"));
//    pDC->TextOut(0,0, _T("CPU Use:"));

    // TODO:  Add your specialized creation code here

    return 0;
}

BOOL CMyProgressCtrl::OnEraseBkgnd(CDC* pDC)
{
	SetBkColor(pDC->m_hDC, GLOBALBKCOLORLAYER1);
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &m_BrushBg);
	pDC->SetTextColor(FONTCOLOR);	
	return CStatic::OnEraseBkgnd(pDC);

}

HBRUSH CMyProgressCtrl::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	SetBkColor(pDC->m_hDC, GLOBALBKCOLORLAYER1);
	pDC->SetTextColor(FONTCOLOR);	
	return  m_BrushBg;
    return NULL;
}

void CMyProgressCtrl::UpdateCpuUsage()
{
	int nCpu = m_usageTotal.GetCpuUsage();	 
	::IsWindow(progressCtrl) ?
		progressCtrl.SetPos(nCpu) : NULL;
	
	static CString csFull;
	CString cs;
	CString csCPU;
	CClientDC dc(this);
	CBrush br(RGB(0,0,0));
	
	LOADSTRING(csCPU, IDC_CPU);
	csFull.Format(_T("%s%d%%"), csCPU, 100);
	cs.Format(_T("%s%d%%"), csCPU, nCpu);
	
	CSize sizeText = dc.GetTextExtent(csFull);
	dc.SetBkColor(RGB(0,0,0));
	dc.SetTextColor(RGB(255,255,255));
	dc.FillRect(CRect(0,0,sizeText.cx,sizeText.cy),&br);
	dc.TextOut(0,0,cs);
	br.DeleteObject();
}	
