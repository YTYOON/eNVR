#include "StdAfx.h"
#include "EmapCam.h"
#include "DlgEmap.h"
//#include "DlgAc.h"
#include "resource.h"

CEmapCam::CEmapCam(void)
{
	mb_butDown = false;
	mb_dragging = false;
	mb_dirty = false;
	m_mapx = 0;
	m_mapy = 0;
	m_camgroupid = 0;
}

CEmapCam::~CEmapCam(void)
{
}

BEGIN_MESSAGE_MAP(CEmapCam, CBitmapButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &CEmapCam::OnBnClicked)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
//	ON_WM_MOUSEHOVER()
//ON_CONTROL_REFLECT(BN_SETFOCUS, &CEmapCam::OnBnSetfocus)
ON_WM_KEYDOWN()
ON_CONTROL_REFLECT(BN_DOUBLECLICKED, &CEmapCam::OnBnDoubleclicked)
END_MESSAGE_MAP()

void CEmapCam::OnBnClicked()
{
	TRACE (_T("button clicked \r\n"));
	((CDlgEmap*)GetParent())->fnResumeCam(m_idx);
	mb_butDown = false;
	return;
}

void CEmapCam::OnLButtonDown(UINT nFlags, CPoint point)
{	
	CWnd* pParent;
	pParent = GetParent();
	CRect lpRect;

	GetWindowRect(&lpRect);
		TRACE (_T("button down \r\n"));
	CPoint pt(point);	//get our current mouse coordinates
	ClientToScreen(&pt); 
	m_nX = point.x;
	m_nY = point.y;
	mb_butDown = true;
 
	CBitmapButton::OnLButtonDown(nFlags, point);
}

void CEmapCam::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACE (_T("mouse move \r\n"));
 	if (mb_butDown)  //Only allow move for settings
	{
		mb_dragging = true;
		CBitmap bitmap;

		CString ls;
		CPoint pt = point;
		ClientToScreen(&pt);
		GetParent()->ScreenToClient(&pt);

		MoveWindow(pt.x - m_nX,  pt.y - m_nY  , CAMSIZEW, CAMSIZEH,true);
		TRACE (_T("mouse move  1\r\n"));
	}

	CBitmapButton::OnMouseMove(nFlags, point);
} //OnMouseMove

 
void CEmapCam::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (mb_dragging)
	{
		CPoint pt1 = point;
		ClientToScreen(&pt1);
		GetParent()->ScreenToClient(&pt1);		
		bool	lb_drop;
	/*	if (mb_ac)
			lb_drop = ((CDlgAc*)GetParent())->fnDropWithin(&pt1);
		else*/
			lb_drop = ((CDlgEmap*)GetParent())->fnDropWithin(&pt1);
		if (!lb_drop) 
		{
			mb_dragging = true;
			return;
		}

		MoveWindow(pt1.x - m_nX, pt1.y - m_nY, CAMSIZEW, CAMSIZEH,true);

		mb_dragging = false;
		//((CDlgEmap*)GetParent())->fnAfterMove(pt1.x - m_nX, pt1.y-m_nY, m_camgroupid);
		
		//if (mb_ac)
		//	((CDlgAc*)GetParent())->fnAfterMove(pt1.x - m_nX, pt1.y-m_nY, m_camgroupid, m_idx);
		//else	
			((CDlgEmap*)GetParent())->fnAfterMove(pt1.x - m_nX, pt1.y-m_nY, m_camgroupid, m_idx);

		mc_mode = 'A';
		mb_butDown = false;
	}
	CBitmapButton::OnLButtonUp(nFlags, point);
}


void CEmapCam::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar ==VK_DELETE)
	{
			//if (mb_ac)
			//	((CDlgAc*)GetParent())->fnRemoveCam(m_idx);
			//else
				((CDlgEmap*)GetParent())->fnRemoveCam(m_idx);
	}
	CBitmapButton::OnKeyDown(nChar, nRepCnt, nFlags);
}

//bool CDlgEmap::fnAfterMove(int o_newx, int o_newy, int o_camgroupid, int o_camidx)
//{
//	/*
//			int	ll_basex=0;
//		CPoint pt(o_newx, o_newy);
//		bool lb_drop = fnDropWithin(&pt);
//
//		CRect		lrect;
//		m_map1.GetWindowRect(&lrect);
//		ll_basex = lrect.left;
//
//	for (int i=0; i<m_vpcam.size(); i++)
//	{
//		CEmapCam* lpCam = m_vpcam[i];
//		if (lpCam->m_camgroupid == o_camgroupid)
//		{
//			//lpCam->m_mapx = o_newx - ll_basex;
//			lpCam->m_mapx = o_newx;
//			lpCam->m_mapy = o_newy;
//			lpCam->mb_dirty = true;
//			ib_dirty = true;
//			break;
//		}
//	}
//
//	//Move the Access icon as well
//	for (int i=0; i<m_vpAccess.size(); i++)
//	{
//		CEmapAccessCam*	lpAccess = m_vpAccess[i];
//		if (lpAccess->m_camidx == o_camidx)
//		{
//			lpAccess->MoveWindow(o_newx , o_newy -ACCESSH , ACCESSW, ACCESSH, true);
//		}
//	}
//
//	//move the static text as well
//	for (int i=0; i<m_vpName.size(); i++)
//	{
//		CBaseStatic*  lpstc = m_vpName[i];
//		if (lpstc->m_id == o_camgroupid)
//		{
//			lpstc->MoveWindow(o_newx + 40, o_newy, STCW, STCH, true);
//			break;
//		}
//	}*/
//	return false;
//}
//
//bool CDlgEmap::fnDropWithin(LPPOINT mpt)
//{
//		//if mpt is within the map1, return true, otherwise return false
//	CRect rect; 	
//	m_map1.GetWindowRect(&rect);
// 
//	ScreenToClient(&rect); //optional step - see below  //position:  rect.left, rect.top //size: rect.Width(), rect.Height() 
//	if (mpt->x < rect.right && mpt->x > rect.left && mpt->y >rect.top && mpt->y<rect.bottom)
//		return true;
//	else
//		return false;
//}


void CEmapCam::OnBnDoubleclicked()
{
	((CDlgEmap*)GetParent())->fnDisplayVideo(m_idx);
		//if (mb_ac)
		//	((CDlgAc*)GetParent())->fnDisplayVideo(m_idx);
}
