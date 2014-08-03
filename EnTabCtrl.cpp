//_ **********************************************************
//_ 
//_ Name: EnTabCtrl.cpp 
//_ Purpose: 
//_ Created: 15 September 1998 
//_ Author: D.R.Godson
//_ Modified By: 
//_ 
//_ Copyright (c) 1998 Brilliant Digital Entertainment Inc. 
//_ 
//_ **********************************************************

// EnTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "EnTabCtrl.h"
#include "TaskTreeCtrl.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnTabCtrl

DWORD CEnTabCtrl::s_dwCustomLook = 0;

enum { PADDING = 3, EDGE = 20};

//////////////////////////////////////////////////////////////////////////////
// helpers
COLORREF Darker(COLORREF crBase, float fFactor)
{
	ASSERT (fFactor < 1.0f && fFactor > 0.0f);

	fFactor = min(fFactor, 1.0f);
	fFactor = max(fFactor, 0.0f);

	BYTE bRed, bBlue, bGreen;
	BYTE bRedShadow, bBlueShadow, bGreenShadow;

	bRed = GetRValue(crBase);
	bBlue = GetBValue(crBase);
	bGreen = GetGValue(crBase);

	bRedShadow = (BYTE)(bRed * fFactor);
	bBlueShadow = (BYTE)(bBlue * fFactor);
	bGreenShadow = (BYTE)(bGreen * fFactor);

	return RGB(bRedShadow, bGreenShadow, bBlueShadow);
}

COLORREF Lighter(COLORREF crBase, float fFactor) 
{
	ASSERT (fFactor > 1.0f);

	fFactor = max(fFactor, 1.0f);

	BYTE bRed, bBlue, bGreen;
	BYTE bRedHilite, bBlueHilite, bGreenHilite;

	bRed = GetRValue(crBase);
	bBlue = GetBValue(crBase);
	bGreen = GetGValue(crBase);

	bRedHilite = (BYTE)min((int)(bRed * fFactor), 255);
	bBlueHilite = (BYTE)min((int)(bBlue * fFactor), 255);
	bGreenHilite = (BYTE)min((int)(bGreen * fFactor), 255);

	return RGB(bRedHilite, bGreenHilite, bBlueHilite);
}

CSize FormatText(CString& sText, CDC* pDC, int nWidth)
{
	CRect rect(0, 0, nWidth, 20);
	UINT uFlags = DT_CALCRECT | DT_SINGLELINE | DT_MODIFYSTRING | DT_END_ELLIPSIS;

	::DrawText(pDC->GetSafeHdc(), sText.GetBuffer(sText.GetLength() + 4), -1, rect, uFlags);
	sText.ReleaseBuffer();

	return pDC->GetTextExtent(sText);
}

////////////////////////////////////////////////////////////////////////////////////////

CEnTabCtrl::CEnTabCtrl()
:m_clrFont(-1)
{
}

CEnTabCtrl::~CEnTabCtrl()
{
}

/////////////////////////////////////////////////////////////////////////////
// CEnTabCtrl message handlers

void CEnTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	TC_ITEM     tci;
	CDC* pDC = CDC::FromHandle(lpdis->hDC);
	HIMAGELIST hilTabs = (HIMAGELIST)TabCtrl_GetImageList(GetSafeHwnd());

	BOOL bSelected = (lpdis->itemID == (UINT)GetCurSel());
	BOOL bColor = (s_dwCustomLook & ETC_COLOR);

	CRect rItem(lpdis->rcItem);

	if (bSelected)
		rItem.bottom -= 1;
	else
		rItem.bottom += 2;

	// tab
	// blend from back color to COLOR_3DFACE if 16 bit mode or better
	COLORREF crFrom = GetTabColor(bSelected);
	
	if (s_dwCustomLook & ETC_GRADIENT && pDC->GetDeviceCaps(BITSPIXEL) >= 16)
	{
		COLORREF crTo = bSelected ? ::GetSysColor(COLOR_3DFACE) : Darker(!bColor || m_crBack == -1 ? ::GetSysColor(COLOR_3DFACE) : m_crBack, 0.7f);

		int nROrg = GetRValue(crFrom);
		int nGOrg = GetGValue(crFrom);
		int nBOrg = GetBValue(crFrom);
		int nRDiff = GetRValue(crTo) - nROrg;
		int nGDiff = GetGValue(crTo) - nGOrg;
		int nBDiff = GetBValue(crTo) - nBOrg;
		
		int nHeight = rItem.Height();

		for (int nLine = 0; nLine < nHeight; nLine += 2)
		{
			int nRed = nROrg + (nLine * nRDiff) / nHeight;
			int nGreen = nGOrg + (nLine * nGDiff) / nHeight;
			int nBlue = nBOrg + (nLine * nBDiff) / nHeight;
			
			pDC->FillSolidRect(CRect(rItem.left, rItem.top + nLine, rItem.right, rItem.top + nLine + 2), 
								RGB(nRed, nGreen, nBlue));
		}
	}
	else // simple solid fill
		pDC->FillSolidRect(rItem, crFrom);

	// text & icon
	rItem.left += PADDING;
	rItem.top += PADDING + (bSelected ? 1 : 0);

	pDC->SetBkMode(TRANSPARENT);

	CString sTemp;
	tci.mask        = TCIF_TEXT | TCIF_IMAGE;
	tci.pszText     = sTemp.GetBuffer(100);
	tci.cchTextMax  = 99;
	GetItem(lpdis->itemID, &tci);
	sTemp.ReleaseBuffer();

	// icon
	if (hilTabs)
	{
		ImageList_Draw(hilTabs, tci.iImage, *pDC, rItem.left, rItem.top, ILD_TRANSPARENT);
		rItem.left += 16 + PADDING;
	}

	// text
	rItem.right -= PADDING;
	FormatText(sTemp, pDC, rItem.Width());

	pDC->SetTextColor(GetTabTextColor(bSelected));
	pDC->DrawText(sTemp, rItem, DT_NOPREFIX | DT_CENTER);
}

void CEnTabCtrl::DrawItemBorder(LPDRAWITEMSTRUCT lpdis)
{
	ASSERT (s_dwCustomLook & ETC_FLAT);

	BOOL bSelected = (lpdis->itemID == (UINT)GetCurSel());
	BOOL bBackTabs = (s_dwCustomLook & ETC_BACKTABS);

	CRect rItem(lpdis->rcItem);
	CDC* pDC = CDC::FromHandle(lpdis->hDC); 

	COLORREF crTab = GetTabColor(bSelected);
	COLORREF crHighlight = Lighter(crTab, 1.5f);
	COLORREF crShadow = Darker(crTab, 0.75f);

	if (bSelected || bBackTabs)
	{
		rItem.bottom += bSelected ? -1 : 1;

		// edges
		pDC->FillSolidRect(CRect(rItem.left, rItem.top, rItem.left + 1, rItem.bottom), crHighlight);
		pDC->FillSolidRect(CRect(rItem.left, rItem.top, rItem.right, rItem.top + 1), crHighlight);
		pDC->FillSolidRect(CRect(rItem.right - 1, rItem.top, rItem.right, rItem.bottom), crShadow);
	}
	else // draw simple dividers
	{
		pDC->FillSolidRect(CRect(rItem.left - 1, rItem.top, rItem.left, rItem.bottom), crShadow);
		pDC->FillSolidRect(CRect(rItem.right - 1, rItem.top, rItem.right, rItem.bottom), crShadow);
	}
}

void CEnTabCtrl::DrawMainBorder(LPDRAWITEMSTRUCT lpdis)
{
	CRect rBorder(lpdis->rcItem);
	CDC* pDC = CDC::FromHandle(lpdis->hDC); 

	COLORREF crTab = GetTabColor();
	COLORREF crHighlight = Lighter(crTab, 1.5f);
	COLORREF crShadow = Darker(crTab, 0.75f);

	pDC->Draw3dRect(rBorder, crHighlight, crShadow);
}

COLORREF CEnTabCtrl::GetTabColor(BOOL bSelected)
{
	BOOL bColor = (s_dwCustomLook & ETC_COLOR);
	BOOL bHiliteSel = (s_dwCustomLook & ETC_SELECTION);
	BOOL bBackTabs = (s_dwCustomLook & ETC_BACKTABS);
	BOOL bFlat = (s_dwCustomLook & ETC_FLAT);

	if (bSelected && bHiliteSel)
	{
		if (bColor)
			return Lighter((m_crBack == -1) ? ::GetSysColor(COLOR_3DFACE) : m_crBack, 1.4f);
		else
			return Lighter(::GetSysColor(COLOR_3DFACE), 1.4f);
	}
	else if (!bSelected)
	{
		if (bBackTabs || !bFlat)
		{
			if (bColor)
				return Darker((m_crBack == -1) ? ::GetSysColor(COLOR_3DFACE) : m_crBack, 0.9f);
			else
				return Darker(::GetSysColor(COLOR_3DFACE), 0.9f);
		}
		else
			return (m_crBack == -1) ? ::GetSysColor(COLOR_3DFACE) : m_crBack;
	}

	// else
	return ::GetSysColor(COLOR_3DFACE);
}

COLORREF CEnTabCtrl::GetTabTextColor(BOOL bSelected)
{
	BOOL bColor = (s_dwCustomLook & ETC_COLOR);
	BOOL bFlat = (s_dwCustomLook & ETC_FLAT);
    if (m_clrFont != -1) return m_clrFont;
	if (bSelected)
	{
		return ::GetSysColor(COLOR_WINDOWTEXT);
	}
	else
	{
		if (bColor || bFlat)
			return Darker((m_crBack == -1) ? ::GetSysColor(COLOR_3DFACE) : m_crBack, 0.5f);
		else
			return Darker(::GetSysColor(COLOR_3DFACE), 0.5f);
	}

	// else
	return Darker(::GetSysColor(COLOR_3DFACE), 0.5f);
}

void CEnTabCtrl::EnableCustomLook(BOOL bEnable, DWORD dwStyle)
{
	if (!bEnable)
		dwStyle = 0;

	s_dwCustomLook = dwStyle;
}

void CEnTabCtrl::PreSubclassWindow() 
{
	CBaseTabCtrl::PreSubclassWindow();

	if (s_dwCustomLook)
		ModifyStyle(0, TCS_OWNERDRAWFIXED);
}

////////////////////////////////////////////////////////////////////////// CTreeTabCtrl
CTreeTabCtrl::CTreeTabCtrl():m_pDeviceTree(NULL),m_pLayoutTree(NULL)
{

}

void CTreeTabCtrl::CreateTreeTab(DWORD dwStyle, const RECT& rectParent, CWnd* pParentWnd, UINT nID)
{
	CString csString;
	
	DWORD dwFlags = 0;
	CRect recTab, recTree;
	Create(dwStyle , rectParent, pParentWnd, nID);

	LOADSTRING(csString, IDS_DEVICE_TREE);
	InsertItem(0, csString);
// 	LOADSTRING(csString, IDS_LAYOUT_TREE);
// 	InsertItem(1, csString);
	dwFlags = ETC_FLAT | ETC_COLOR | ETC_SELECTION | ETC_GRADIENT | ETC_BACKTABS;
	SetBkgndColor(BKCOLOR);
	SetFontColor(FONTCOLOR);

	EnableDraw(BTC_ALL);
	CEnTabCtrl::EnableCustomLook(dwFlags, dwFlags);
	
	GetItemRect(0,recTab);
	recTab.DeflateRect(3,3);
	CSize csSize(recTab.Width(), recTab.Height());
	SetItemSize(csSize);
	GetItemRect(0,recTab);

	recTree = rectParent;
	recTree.top = recTab.bottom;
	recTree.bottom = recTree.bottom - (recTab.Height()*3);

	m_pDeviceTree = new CTaskDeviceTree;
	//m_pLayoutTree = new CTaskLayoutTree;

	m_pDeviceTree->Create(WS_CHILD|WS_VISIBLE,recTree, this, nID + 1);
	//m_pLayoutTree->Create(WS_CHILD|WS_VISIBLE,recTree, this, nID + 2);

	m_pDeviceTree->ShowWindow(SW_SHOW);
	//m_pLayoutTree->ShowWindow(SW_HIDE);

	CNVRBk::GetBkObject()->RegistWnd(m_pDeviceTree);
	//CNVRBk::GetBkObject()->RegistWnd(m_pLayoutTree);

	CNVRBk::GetBkObject()->SetTreeType(m_pDeviceTree->GetTreeType());
	Invalidate();
}
void CTreeTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpdis/*lpDrawItemStruct*/)
{
	// TODO:  Add your code to draw the specified item
	TC_ITEM     tci;
	CDC* pDC = CDC::FromHandle(lpdis->hDC);
	HIMAGELIST hilTabs = (HIMAGELIST)TabCtrl_GetImageList(GetSafeHwnd());
	CFont font;
	CFont *pOldFont;
	font.CreatePointFont(76,_T("MS Shell Dlg"), pDC);
	pOldFont = pDC->SelectObject(&font);

	BOOL bSelected = (lpdis->itemID == (UINT)GetCurSel());
	BOOL bColor = (s_dwCustomLook & ETC_COLOR);

	CRect rItem(lpdis->rcItem);
	
	if (lpdis->itemID == 0)
	{
		rItem.right -= 3;
	}
	
	if (bSelected)
	{
		rItem.bottom -= 1;
	}
	else
	{
		rItem.bottom += 2;
	}

	COLORREF crFrom = GetTabColor(bSelected);
	if (s_dwCustomLook & ETC_GRADIENT && pDC->GetDeviceCaps(BITSPIXEL) >= 16)
	{
		COLORREF crTo = bSelected ? ::GetSysColor(COLOR_3DFACE) : Darker(!bColor || m_crBack == -1 ? ::GetSysColor(COLOR_3DFACE) : m_crBack, 0.7f);

		int nROrg = GetRValue(crFrom);
		int nGOrg = GetGValue(crFrom);
		int nBOrg = GetBValue(crFrom);
		int nRDiff = GetRValue(crTo) - nROrg;
		int nGDiff = GetGValue(crTo) - nGOrg;
		int nBDiff = GetBValue(crTo) - nBOrg;

		int nHeight = rItem.Height();

		for (int nLine = 0; nLine < nHeight; nLine += 2)
		{
			int nRed = nROrg + (nLine * nRDiff) / nHeight;
			int nGreen = nGOrg + (nLine * nGDiff) / nHeight;
			int nBlue = nBOrg + (nLine * nBDiff) / nHeight;

			pDC->FillSolidRect(CRect(rItem.left, rItem.top + nLine, rItem.right, rItem.top + nLine + 2), 
				RGB(nRed, nGreen, nBlue));
		}
	}
	else // simple solid fill
		pDC->FillSolidRect(rItem, crFrom);

	pDC->SetBkMode(TRANSPARENT);

	CString sTemp;
	tci.mask        = TCIF_TEXT | TCIF_IMAGE;
	tci.pszText     = sTemp.GetBuffer(100);
	tci.cchTextMax  = 99;
	GetItem(lpdis->itemID, &tci);
	sTemp.ReleaseBuffer();

	pDC->SetTextColor(GetTabTextColor(bSelected));
	pDC->DrawText(sTemp, rItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	pDC->SelectObject(pOldFont);
	font.DeleteObject();
}
BEGIN_MESSAGE_MAP(CTreeTabCtrl, CEnTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CTreeTabCtrl::OnTcnSelchange)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
    ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CTreeTabCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (WM_LBUTTONDBLCLK == pMsg->message)
	{
		GetTree()->OnDblclk(NULL, NULL);
	}

	return 0;
}

void CTreeTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int nPage = GetCurSel();

	if (nPage == 0)
	{
		m_pDeviceTree->ShowWindow(SW_SHOW);
		m_pLayoutTree->ShowWindow(SW_HIDE);

		CNVRBk::GetBkObject()->SetTreeType(m_pDeviceTree->GetTreeType());
		m_pDeviceTree->SetActiveWindow();
	} 
	else
	{
		m_pDeviceTree->ShowWindow(SW_HIDE);
		m_pLayoutTree->ShowWindow(SW_SHOW);

		CNVRBk::GetBkObject()->SetTreeType(m_pLayoutTree->GetTreeType());
		m_pLayoutTree->SetActiveWindow();
	}

	*pResult = 0;
}

void CTreeTabCtrl::OnDestroy()
{
	CEnTabCtrl::OnDestroy();

	// TODO: Add your message handler code here
	if (m_pDeviceTree)
	{
		m_pDeviceTree->DestroyWindow();
		delete m_pDeviceTree;
	}

	if (m_pLayoutTree)
	{
		m_pLayoutTree->DestroyWindow();
		delete m_pLayoutTree;
	}
}

BOOL CTreeTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	CBrush br(RGB(35,35,35));
	CRect rClient,rTab;

	CBaseTabCtrl::OnEraseBkgnd(pDC);
	GetItemRect(0, rTab);
	GetClientRect(rClient);
	rClient.top = rTab.bottom;
	pDC->FillRect(rClient,&br);

	br.DeleteObject();
	return true;
}
void CTreeTabCtrl::OnSize(UINT nType, int cx, int cy)
{
    CEnTabCtrl::OnSize(nType, cx, cy);
    CRect rectItem;
    GetItemRect(0, rectItem);
    if (m_pLayoutTree)
        m_pLayoutTree->MoveWindow(0, rectItem.Height(), cx, cy-rectItem.Height()); 
    if (m_pDeviceTree)
        m_pDeviceTree->MoveWindow(0, rectItem.Height(), cx, cy-rectItem.Height()); 

    // TODO: Add your message handler code here
}
