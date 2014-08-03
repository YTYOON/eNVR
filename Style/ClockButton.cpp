// ClockButton.cpp : implementation file
//

#include "stdafx.h"
//#include "NVR.h"
#include "ClockButton.h"
#include "Resource.h"



static const int nIconStart = 30;
static const int nIconWidth = 18;
const int nSparateWidth = 5;

static const int nDefaultWidth = 250;
static const int nDefaultHeight = 20;
static const int nClockStart = 0;
static const int nClockWidth = 200;
static const int nClockHeight = 15;
static const int nClockLeftMergin = 110;
static const int nClockTopMergin = 80;


//IMPLEMENT_DYNAMIC(CClockButton, CMFCToolBarButton)
IMPLEMENT_SERIAL(CClockButton, CObject, VERSIONABLE_SCHEMA | 1)
CClockButton::CClockButton(UINT uiID, int iImage, LPCTSTR lpszTextL, BOOL bUserButton, BOOL bLocked)
:CMFCToolBarButton(uiID, iImage, lpszTextL, bUserButton, bLocked)//   IDB_BITMAP_CLOC
{
	m_dwStyle = WS_CHILD | SS_SIMPLE;
	m_iWidth = nDefaultWidth;
    m_bmpClock.LoadBitmap(IDB_BITMAP_CLOCK);
    
}
CClockButton::CClockButton()
{  
	m_dwStyle = WS_CHILD | SS_SIMPLE;
	m_iWidth = nDefaultWidth;
    m_bmpClock.LoadBitmap(IDB_BITMAP_CLOCK);

}

CClockButton::~CClockButton()
{
}

//
//BEGIN_MESSAGE_MAP(CClockButton, CMFCToolBarButton)
//END_MESSAGE_MAP()

void CClockButton::OnMove ()
{
	if (m_wndTxt.GetSafeHwnd () != NULL &&
		(m_wndTxt.GetStyle () & WS_VISIBLE))
	{
		m_wndTxt.SetWindowPos (NULL,
			m_rect.left + 1 +nIconStart +nIconWidth +nSparateWidth, m_rect.top +  (nClockTopMergin -nDefaultHeight)/2, m_rect.Width () - 2 -nIconStart -nIconWidth, m_rect.Height () - 2,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
    if (m_wndImage.GetSafeHwnd () != NULL &&
	    (m_wndImage.GetStyle () & WS_VISIBLE))
    {
	    m_wndImage.SetWindowPos (NULL,
		    m_rect.left + 1+nIconStart, m_rect.top +  (nClockTopMergin- nDefaultHeight)/2, m_rect.left +nIconStart +nIconWidth - 2 - nIconStart, m_rect.Height () - 2,
		    SWP_NOZORDER | SWP_NOACTIVATE);
    }
    
}
//**************************************************************************************

SIZE CClockButton::OnCalculateSize (CDC* /*pDC*/, const CSize& /*sizeDefault*/, BOOL bHorz)
{
	if (!IsVisible ())
	{
		if (m_wndTxt.GetSafeHwnd () != NULL)
		{
			m_wndTxt.ShowWindow (SW_HIDE);
		}

		return CSize (0,0);
	}

	if (bHorz)
	{
		if (m_wndTxt.GetSafeHwnd () != NULL && !IsHidden ())
		{
			m_wndTxt.ShowWindow (SW_SHOWNOACTIVATE);
            //m_wndImage.ShowWindow (SW_SHOWNOACTIVATE);
			//m_cpuProgress.ModifyStyle (TBS_VERT, TBS_HORZ);
		}

		return CSize (m_iWidth, nDefaultHeight);
	}
	else
	{
		if (m_wndTxt.GetSafeHwnd () != NULL && !IsHidden ())
		{
			m_wndTxt.ShowWindow (SW_SHOWNOACTIVATE);
            //m_wndImage.ShowWindow (SW_SHOWNOACTIVATE);
			//m_cpuProgress.ModifyStyle (TBS_HORZ, TBS_VERT);
		}

		return CSize (nDefaultHeight, m_iWidth);
	}
}
void CClockButton::OnSize (int iSize)
{
	m_iWidth = iSize;
	m_iWidth = nDefaultWidth;
	m_rect.right = m_rect.left + m_iWidth;

	if (m_wndTxt.GetSafeHwnd () != NULL &&
		(m_wndTxt.GetStyle () & WS_VISIBLE))
	{
		m_wndTxt.SetWindowPos (NULL,
			m_rect.left + 1, m_rect.top + 1, m_rect.Width () - 2, m_rect.Height () - 2,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void CClockButton::OnChangeParentWnd (CWnd* pWndParent)
{
	//m_wndImage
	if (m_wndTxt.GetSafeHwnd () != NULL)
	{
		CWnd* pWndParentCurr = m_wndTxt.GetParent ();
		ASSERT (pWndParentCurr != NULL);

		if (pWndParent != NULL && pWndParentCurr != NULL &&
			pWndParentCurr->GetSafeHwnd () == pWndParent->GetSafeHwnd ())
		{
			return;
		}
		m_wndTxt.DestroyWindow();
	}
	if (m_wndImage.GetSafeHwnd () != NULL)
	{
		CWnd* pWndParentCurr = m_wndImage.GetParent ();
		ASSERT (pWndParentCurr != NULL);

		if (pWndParent != NULL && pWndParentCurr != NULL &&
			pWndParentCurr->GetSafeHwnd () == pWndParent->GetSafeHwnd ())
		{
			return;
		}
		m_wndImage.DestroyWindow();
	}
    
	if (pWndParent == NULL || pWndParent->GetSafeHwnd () == NULL)
	{
		return;
	}
 	DWORD dwStyle =  WS_CHILD | WS_VISIBLE;//WS_CHILD | SS_SIMPLE | SS_NOTIFY | BS_NOTIFY |;

	if (!m_wndTxt.Create (_T(""), dwStyle, m_rect, pWndParent, 1234))
	{
		ASSERT (FALSE);
		return;
	}
	m_wndTxt.ShowWindow(true);
    dwStyle = WS_CHILD | WS_VISIBLE|SS_BITMAP | SS_CENTERIMAGE;
    CRect rect(nIconStart, nClockTopMergin, nIconWidth, nClockTopMergin +20);
	if (!m_wndImage.Create (_T(""), dwStyle, rect, pWndParent, 1235))
	{
		ASSERT (FALSE);
		return;
	}
    //(::LoadBitmap(NULL, MAKEINTRESOURCE(IDB_BITMAP_CLOCK)));
    m_wndImage.SetBitmap((HBITMAP)m_bmpClock.m_hObject); 
	m_wndImage.ShowWindow(true);

}

IMPLEMENT_DYNAMIC(CStaticClock, CStatic)
CStaticClock::CStaticClock()
:m_BrushBg(GLOBALBKCOLORLAYER1)
{
}
CStaticClock::~CStaticClock()
{
}
BEGIN_MESSAGE_MAP(CStaticClock, CStatic)
//{{AFX_MSG_MAP(CStaticClock)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


HBRUSH CStaticClock::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	SetBkColor(pDC->m_hDC,GLOBALBKCOLORLAYER1);
	pDC->SetTextColor(FONTCOLOR);	
	pDC->SetBkMode(TRANSPARENT);
	return  m_BrushBg;
}

BOOL CStaticClock::OnEraseBkgnd(CDC* pDC)
{
	SetBkColor(pDC->m_hDC, GLOBALBKCOLORLAYER1);
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &m_BrushBg);
	pDC->SetTextColor(FONTCOLOR);	
	pDC->SetBkMode(TRANSPARENT);
	return CStatic::OnEraseBkgnd(pDC);
}

void CStaticClock::OnPaint()
{
    CPaintDC dc(this); 
    CDC* pDc = GetDC();
    pDc->TextOut(0,0, m_csTime);
    ReleaseDC(pDc);
     //m_btnClock.SetWindowText( m_btnClock.m_strText);
}

void CClockButton::vSetClockIcon()
{
	//DWORD dw = m_wndImage.GetStyle();

	//m_wndImage.ModifyStyle(0, SS_BITMAP);

	//m_wndImage.SetBitmap( ::LoadBitmap(NULL, MAKEINTRESOURCE(IDB_BITMAP_CLOCK)) );

	return;
}