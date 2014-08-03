// BaseCheckBox.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"
#include "BaseCheckBox.h"


// CBaseCheckBox

IMPLEMENT_DYNAMIC(CBaseCheckBox, CButton)

CBaseCheckBox::CBaseCheckBox()
:m_bChecked(false),m_BrushBg( BKCOLOR ),m_clrFg(FONTCOLOR)
{
}

CBaseCheckBox::~CBaseCheckBox()
{
}


BEGIN_MESSAGE_MAP(CBaseCheckBox, CButton)
//{{AFX_MSG_MAP(CBaseCheckBox)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()




HBRUSH CBaseCheckBox::CtlColor(CDC* pDC, UINT nCtlColor)
{
	//SetBkColor(pDC->m_hDC, RGB(0,0,0));
	/*pDC->SetTextColor(FONTCOLOR);	
	pDC->SetBkMode(TRANSPARENT);
	return  m_BrushBg;*/
	if (nCtlColor == CTLCOLOR_STATIC)
    {
	   // pDC->SetBkColor(BKCOLOR);
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetTextColor(m_clrFg);
       //return (HBRUSH)GetStockObject( NULL_BRUSH ) ;
         /* SetBkMode(hdc,TRANSPARENT);
          SetTextColor(hdc,RGB(0,255,0));*/
          //return (LRESULT)hBrush;
    }
	return  m_BrushBg;
}

BOOL CBaseCheckBox::OnEraseBkgnd(CDC* pDC)
{
	//SetBkColor(pDC->m_hDC, RGB(0,0,0));
	//CRect rect;
	//GetClientRect(rect);
	//pDC->FillRect(&rect, &m_BrushBg);
	pDC->SetTextColor(m_clrFg);	
	//pDC->SetBkMode(TRANSPARENT);
	return CButton::OnEraseBkgnd(pDC);
}


void CBaseCheckBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CRect rect =  lpDrawItemStruct->rcItem;
    CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
    
    //GetClientRect(rect);
	//CBrush bkColor(BKGNDCOLOR);
	//pDC->FillRect(&rect,&m_BrushBg);

    pDC->SetTextColor(FONTCOLOR);	
    int nSaveDC=pDC->SaveDC();
    UINT state = lpDrawItemStruct->itemState;
    POINT pt ;
    TCHAR strText[MAX_PATH + 1];
    ::GetWindowText(m_hWnd, strText, MAX_PATH);


    if (state & ODS_FOCUS)
    {
    }else
    {
    }
    if (state & ODS_SELECTED || state & ODS_DEFAULT){
    }


    CRect BoxRect;
    BoxRect=rect;
    BoxRect.right=BoxRect.left+13;
    //DWORD n = GetCheck();
    //SetCheck(BST_UNCHECKED);
    //n = GetCheck();
    DWORD nCheckStyle = 0;
    if (m_bChecked)  
        nCheckStyle =  DFCS_CHECKED;

    //pDC->DrawFrameControl(BoxRect, DFC_BUTTON, DFCS_BUTTONCHECK|(GetCheck()? DFCS_CHECKED: 0));  
    pDC->DrawFrameControl(BoxRect, DFC_BUTTON, DFCS_BUTTONCHECK|nCheckStyle);  
    



    if (strText!=NULL)
    {
        CFont* hFont = GetFont();
        CFont* hOldFont = pDC->SelectObject(hFont);
        CSize szExtent = pDC->GetTextExtent(strText, lstrlen(strText));
        CPoint pt( rect.CenterPoint().x - szExtent.cx / 2+4, rect.CenterPoint().y - szExtent.cy / 2);
        //if (state & ODS_SELECTED) 
          //pt.Offset(1, 1);
        int nMode = pDC->SetBkMode(TRANSPARENT);
        //if (state & ODS_DISABLED)
            //pDC->DrawState(pt, szExtent, strText, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
        //else
          pDC->DrawState(pt, szExtent, strText, DSS_NORMAL, TRUE, 0, (HBRUSH)NULL);
        pDC->SelectObject(hOldFont);
        pDC->SetBkMode(nMode);
    }

    pDC->RestoreDC(nSaveDC);
}


void CBaseCheckBox::PreSubclassWindow()
{
    SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW|WS_VISIBLE);
    CButton::PreSubclassWindow();
}

void CBaseCheckBox::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bChecked ^= 1;
    SetCheck(m_bChecked ? BST_CHECKED : BST_UNCHECKED);
    CButton::OnLButtonDown(nFlags, point);
}




