// EcBaseSlider.cpp : implementation file
//

#include "stdafx.h"
#include "EcBaseSlider.h"


// CEcBaseSlider

IMPLEMENT_DYNAMIC(CEcBaseSlider, CSliderCtrl)

CEcBaseSlider::CEcBaseSlider()
{

}

CEcBaseSlider::~CEcBaseSlider()
{
}


BEGIN_MESSAGE_MAP(CEcBaseSlider, CSliderCtrl)
	ON_WM_ERASEBKGND()
//	ON_WM_HSCROLL()
END_MESSAGE_MAP()



// CEcBaseSlider message handlers



BOOL CEcBaseSlider::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CBrush brush(GLOBALBKCOLOR);
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &brush);
	return CSliderCtrl::OnEraseBkgnd(pDC);
}

//void CEcBaseSlider::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
//{
//	// TODO: Add your message handler code here and/or call default
//	int CurPos = m_CarSlider.GetPos() - 1;
//	CBitmap Bmp;
//
//	Bmp.LoadBitmap(CarPicture[CurPos]);
//
//	m_Picture.SetBitmap(Bmp);
//	UpdateData(FALSE);
//
//	CSliderCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
//}
