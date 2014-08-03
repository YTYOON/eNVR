// EmapAccessCam.cpp : implementation file
//

#include "stdafx.h"
#include "EmapAccessCam.h"
#include  "DlgAc.h"

// CEmapAccessCam

IMPLEMENT_DYNAMIC(CEmapAccessCam, CBitmapButton)

CEmapAccessCam::CEmapAccessCam()
{
	mb_butDown = false;
	mb_dragging = false;
	mb_dirty = false;
	m_mapx = 0;
	m_mapy = 0;
	m_camgroupid = 0;
}

CEmapAccessCam::~CEmapAccessCam()
{
}

BEGIN_MESSAGE_MAP(CEmapAccessCam, CBitmapButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_CONTROL_REFLECT(BN_CLICKED, &CEmapAccessCam::OnBnClicked)
	ON_CONTROL_REFLECT(BN_DOUBLECLICKED, &CEmapAccessCam::OnBnDoubleclicked)
	ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()

// CEmapAccessCam message handlers

void CEmapAccessCam::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd* pParent;
	pParent = GetParent();
	CRect lpRect;

	GetWindowRect(&lpRect);
 
	CPoint pt(point);	//get our current mouse coordinates
	ClientToScreen(&pt); 
	m_nX = point.x;
	m_nY = point.y;
	mb_butDown = true;
 
	 CBitmapButton::OnLButtonDown(nFlags, point);
}

void CEmapAccessCam::OnMouseMove(UINT nFlags, CPoint point)
{
 	if (mb_butDown)  //Only allow move for settings
	{
		mb_dragging = true;
		CBitmap bitmap;

		CString ls;
		CPoint pt = point;
		ClientToScreen(&pt);
		GetParent()->ScreenToClient(&pt);

		MoveWindow(pt.x - m_nX,  pt.y - m_nY  , CAMSIZEW, CAMSIZEH,true);
	}
	CBitmapButton::OnMouseMove(nFlags, point);
}

void CEmapAccessCam::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
 	if (nChar ==VK_DELETE)
	{
			//((CDlgAc*)GetParent())->fnRemoveCam(m_idx);
			CWnd *pOwner = GetOwner();
			//int ll_cid = GetDlgCtrlID();
			STR_CAM		nmsp;
			nmsp.camidx = m_idx;
			//nmsp.cskey = _T("delete");
			//pOwner->SendMessage(WM_KEYDOWN,  ll_cid, (LPARAM)&nmsp);
			pOwner->SendMessage(WM_KEYDOWN,  VK_DELETE, (LPARAM)&nmsp);
	}

	CBitmapButton::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEmapAccessCam::OnBnClicked()
{
	//((CDlgAc*)GetParent())->fnResumeCam(m_idx);
		STR_CAM		nmsp;
		CWnd *pOwner = GetOwner();
		int ll_cid = GetDlgCtrlID();
		nmsp.camidx = m_idx;
		nmsp.cskey = _T("click");
		pOwner->SendMessage(WM_KEYDOWN,  ll_cid, (LPARAM)&nmsp);
		mb_butDown = false;
		return;
}

void CEmapAccessCam::OnBnDoubleclicked()
{
	//((CDlgAc*)GetParent())->fnDisplayVideo(m_idx);
			int ll_cid = GetDlgCtrlID();		
			STR_CAM		nmsp;
			CWnd *pOwner = GetOwner();
			nmsp.camidx = m_idx;
			pOwner->SendMessage(WM_NCLBUTTONDBLCLK,  ll_cid, (LPARAM)&nmsp);
}

void CEmapAccessCam::OnMouseHover(UINT nFlags, CPoint point)
{
	CBitmapButton::OnMouseHover(nFlags, point);
}

void CEmapAccessCam::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (mb_dragging)
	{
		CPoint pt1 = point;
		ClientToScreen(&pt1);
		GetParent()->ScreenToClient(&pt1);		

		CWnd *pOwner = GetOwner();
		STR_CAM		nmsp;
		nmsp.camidx = m_idx;
		nmsp.cpoint = &pt1;
		nmsp.xdiff = pt1.x - m_nX;
		nmsp.ydiff = pt1.y-m_nY;
		int ll_cid = GetDlgCtrlID();
		pOwner->SendMessage(WM_NCLBUTTONUP,  ll_cid, (LPARAM)&nmsp);

		MoveWindow(pt1.x - m_nX, pt1.y - m_nY, CAMSIZEW, CAMSIZEH,true);

		mb_dragging = false;
		mc_mode = 'A';
		mb_butDown = false;
	}
	CBitmapButton::OnLButtonUp(nFlags, point);
}
