// ToolBarEx.cpp : implementation file
//

#include "stdafx.h"
#include "ToolBarEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
const COLORREF TOOLBOXBG        RGB(255,255,255);

/////////////////////////////////////////////////////////////////////////////
// CToolBarEx

CToolBarEx::CToolBarEx()
{
	hCursor = ::LoadCursor(NULL , IDC_HAND);
    if(hCursor)
    {
        ::SetCursor(hCursor);
    }

//	hCursor=AfxGetApp()->LoadCursor(IDC_HAND);
}

CToolBarEx::~CToolBarEx()
{
}


BEGIN_MESSAGE_MAP(CToolBarEx, CToolBar)
	//{{AFX_MSG_MAP(CToolBarEx)
	ON_WM_SETCURSOR()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_ICONERASEBKGND()
	ON_WM_SIZE()
	ON_WM_NCCALCSIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//DEL int CToolBarEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
//DEL {
//DEL 	if (CToolBar::OnCreate(lpCreateStruct) == -1)
//DEL 		return -1;
//DEL 	return 0;
//DEL }



BOOL CToolBarEx::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	 ::SetCursor(hCursor);
	 return true;
	//return CToolBar::OnSetCursor(pWnd, nHitTest, message);
}


void CToolBarEx::OnDestroy() 
{
	CToolBar::OnDestroy();
}

HBRUSH CToolBarEx::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CToolBar::OnCtlColor(pDC, pWnd, nCtlColor);
//	pDC->SetBkColor(BKCOLOR);
//	CRect rect;
//	GetClientRect(rect);
//	pDC->FillRect(&rect, &CBrush(BKCOLOR));
	static CBrush br(BKCOLOR);
	return HBRUSH(br) ;
}

void CToolBarEx::OnIconEraseBkgnd(CDC* pDC) 
{
	pDC->SetBkColor(BKCOLOR);
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &CBrush(BKCOLOR));
	
	CToolBar::OnIconEraseBkgnd(pDC);
}

void CToolBarEx::OnSize(UINT nType, int cx, int cy) 
{
	CToolBar::OnSize(nType, 1024, cy);
	
	// TODO: Add your message handler code here
	
}

void CToolBarEx::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	CToolBar::OnNcCalcSize(bCalcValidRects, lpncsp);
}



BOOL CToolBarEx::OnEraseBkgnd(CDC* pDC) 
{
 	pDC->SetBkColor(BKCOLOR);
 	CRect rect;
 	GetClientRect(rect);
 	pDC->FillRect(&rect, &CBrush(BKCOLOR));
 	return true;	
//	return CToolBar::OnEraseBkgnd(pDC);
}
