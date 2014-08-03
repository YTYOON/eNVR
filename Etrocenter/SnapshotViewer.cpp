// SnapshotViewer.cpp : implementation file
//

#include "stdafx.h"
#include "SnapshotViewer.h"


// CSnapshotViewer

IMPLEMENT_DYNAMIC(CSnapshotViewer, CStatic)

CSnapshotViewer::CSnapshotViewer():m_picture_buf(NULL),m_rectDraw(0,0,0,0),m_rcImg(0,0,0,0),m_zDelta(0)
{
	m_PickPoint.x = 0;
	m_PickPoint.y = 0;
}

CSnapshotViewer::~CSnapshotViewer()
{
	if(m_picture_buf)
	{
		delete[] m_picture_buf;
	}
	m_picture_buf = NULL;
}


BEGIN_MESSAGE_MAP(CSnapshotViewer, CStatic)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()
// CSnapshotViewer message handlers
int CSnapshotViewer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}
void CSnapshotViewer::vGetData(uint8_t* picture_buf, BITMAPINFOHEADER bih, DWORD biSizeImage, CRect rectDraw)
{
	m_picture_buf =  new BYTE [biSizeImage];
	memcpy(m_picture_buf,picture_buf,biSizeImage);
	memcpy(&m_bih,&bih,sizeof(bih));
	m_rcImg.left = m_rcImg.top = 0;
	m_rcImg.right = m_bih.biWidth;
	m_rcImg.bottom = m_bih.biHeight*(-1);
	m_rectDraw = rectDraw;
}
BOOL CSnapshotViewer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	::SetCursor(LoadCursor(NULL,IDC_SIZEALL));
	m_zDelta += zDelta;
	float fScale = (float)m_bih.biWidth / (float)abs(m_bih.biHeight);
	int nX = (int)((float)(zDelta)*fScale);
	int nY = (zDelta);
	
	CRect rectTmp;
	rectTmp = m_rectDraw;
	rectTmp.left -= nX/4;
	rectTmp.top -= nY/4;	
	rectTmp.right += nX;
	rectTmp.bottom += nY;

	if(rectTmp.left == 0 || rectTmp.top == 0 || rectTmp.Width() <= 0 || rectTmp.Height() <= 0) return 0;

	m_rectDraw = rectTmp;
	this->MoveWindow(&m_rectDraw, true);
	
	Invalidate(true);
	GetParent()->Invalidate(true);

	return true;	
}

void CSnapshotViewer::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags && MK_LBUTTON )
	{		
		m_rectDraw.left -= (m_PickPoint.x - point.x);
		m_rectDraw.top -= (m_PickPoint.y - point.y) ;
		this->MoveWindow(&m_rectDraw, true);
		GetParent()->Invalidate(true);
		Invalidate(true);
		m_PickPoint = point;
	}
	//::PostMessage(m_hWnd, WM_SETCURSOR, (WPARAM)m_hWnd, MAKELONG(512, 1));
	
	CStatic::OnMouseMove(nFlags, point);
}

void CSnapshotViewer::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_PickPoint = point;
	
	CStatic::OnLButtonDown(nFlags, point);
}


void CSnapshotViewer::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (m_picture_buf)
    {
		SetStretchBltMode(dc.m_hDC, COLORONCOLOR);
		if (GDI_ERROR == StretchDIBits(dc.m_hDC, 
		    m_rectDraw.left, m_rectDraw.top, m_rectDraw.right, m_rectDraw.bottom, 
		    m_rcImg.left, m_rcImg.top, m_rcImg.Width(), m_rcImg.Height(),
		    (LPVOID)m_picture_buf, (BITMAPINFO*)&m_bih,
		    DIB_RGB_COLORS, SRCCOPY ))
	    {
		    SDKERRMSG(TRUE);
		    TRACE(_T("ERROR"));
	    }  		
    }	
}

void CSnapshotViewer::OnLButtonUp(UINT nFlags, CPoint point)
{

	CStatic::OnLButtonUp(nFlags, point);
}

