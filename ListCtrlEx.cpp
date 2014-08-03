// ListCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx

CListCtrlEx::CListCtrlEx(COLORREF bgColor, COLORREF fgColor, COLORREF heightLightbgColor, COLORREF heightLightfgColor)
{
	nCheckedItem=-1;
    //SetBkColor(CR_BASE_BG);
}

CListCtrlEx::~CListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT ( NM_CUSTOMDRAW, OnCustomDrawList )

    ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx message handlers
void CListCtrlEx::SetColor(COLORREF bgColor, COLORREF fgColor, COLORREF heightLightbgColor, COLORREF heightLightfgColor)
{
    m_crBg = bgColor;    
    m_crFg = fgColor;
    m_crHlBg = heightLightbgColor;
    m_crHlFg = heightLightfgColor;
}
void CListCtrlEx::OnCustomDrawList(NMHDR *pNMHDR, LRESULT *pResult)
{	
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	static bool bHighlighted = false;
	
    *pResult = CDRF_DODEFAULT;

    if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        *pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        int iRow = (int)pLVCD->nmcd.dwItemSpec;
		
		bHighlighted = (ListView_GetItemState(m_hWnd, iRow, LVIS_SELECTED) != 0);
		if (bHighlighted)
		{
			pLVCD->clrText   = CR_BASE_HIGHLIGHT_FG; // Use my foreground hilite color
			pLVCD->clrTextBk = CR_BASE_HIGHLIGHT_BG; // Use my background hilite color
			ListView_SetItemState(m_hWnd, iRow, false? 0xff: 0, LVIS_SELECTED);
		}else
		{
			pLVCD->clrText   = CR_BASE_FG;
			pLVCD->clrTextBk = CR_BASE_BG;
		}
		
		*pResult = CDRF_DODEFAULT | CDRF_NOTIFYPOSTPAINT;
		
	}
	else if(CDDS_ITEMPOSTPAINT == pLVCD->nmcd.dwDrawStage)
	{
	if (bHighlighted)
      {
        int  iRow = (int)pLVCD->nmcd.dwItemSpec;

  //      EnableHighlighting(m_hWnd, iRow, true);
		ListView_SetItemState(m_hWnd, iRow, true? 0xff: 0, LVIS_SELECTED);

      }

      *pResult = CDRF_DODEFAULT;

	}

}

void CListCtrlEx::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    *pResult = 0;

    if (pNMListView->uOldState == 0 && pNMListView->uNewState == 0)
        return;    // No change

	TRACE("ITEM:%d\r\n",pNMListView->iItem);

    // Old check box state
    BOOL bPrevState = (BOOL)(((pNMListView->uOldState & 
                LVIS_STATEIMAGEMASK)>>12)-1);  
    if (bPrevState < 0)    // On startup there's no previous state 
        bPrevState = 0; // so assign as false (unchecked)

    // New check box state
    BOOL bChecked =             (BOOL)(((pNMListView->uNewState & LVIS_STATEIMAGEMASK)>>12)-1);   
    if (bChecked < 0) // On non-checkbox notifications assume false

        bChecked = 0; 
	else
	{ 
//		LVITEM* pItem ;
		if ((nCheckedItem!=-1) && (nCheckedItem != pNMListView->iItem))
			SetItemState(nCheckedItem, INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK);	
		nCheckedItem=pNMListView->iItem;
	}
    if (bPrevState == bChecked) // No change in check box
        return;
}

int CListCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CListCtrl::OnCreate(lpCreateStruct) == -1)
        return -1;

    SetBkColor(CR_BASE_BG);

    return 0;
}
