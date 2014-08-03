// DlgSnapshotDummy.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSnapshotDummy.h"

// DlgSnapshotDummy dialog

IMPLEMENT_DYNAMIC(CDlgSnapshotDummy, CDialog)

CDlgSnapshotDummy::CDlgSnapshotDummy(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSnapshotDummy::IDD, pParent),m_picture_buf(NULL),m_rectDraw(0,0,0,0),m_rcImg(0,0,0,0),m_zDelta(0)
{
	m_PickPoint.x = 0;
	m_PickPoint.y = 0;
}

CDlgSnapshotDummy::~CDlgSnapshotDummy()
{
	if(m_picture_buf)
	{
		delete[] m_picture_buf;
	}
	m_picture_buf = NULL;
}

void CDlgSnapshotDummy::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSnapshotDummy, CDialog)
	//ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	//ON_WM_LBUTTONDOWN()
	//ON_WM_MOUSEMOVE()
	//ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	//ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CDlgSnapshotDummy message handlers

BOOL CDlgSnapshotDummy::OnInitDialog()
{
    CDialog::OnInitDialog();
	CGS::SetTransparent(this->m_hWnd,200);
    CRect m_rectDlg;
	GetDesktopWindow()->GetWindowRect(&m_rectDlg);  

	this->SetWindowPos(NULL,m_rectDlg.left,m_rectDlg.top,m_rectDlg.Width(),m_rectDlg.Height(),SWP_SHOWWINDOW);
	ShowWindow(SW_SHOW);
	CPaintDC dc(this);
	CBrush brush(RGB(100,100,100));
	dc.FillRect(&m_rectDlg,&brush);
   
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSnapshotDummy::vGetBMP(uint8_t* picture_buf, BITMAPINFOHEADER bih, DWORD biSizeImage, CString csFileName)
{ 
   	m_picture_buf =  new BYTE [biSizeImage];
	memcpy(m_picture_buf,picture_buf,biSizeImage);
	memcpy(&m_bih,&bih,sizeof(bih));
	m_rcImg.left = m_rcImg.top = 0;
	m_rcImg.right = m_bih.biWidth;
	m_rcImg.bottom = m_bih.biHeight*(-1);
	//m_rectDraw = rectDraw;
}

//BOOL CDlgSnapshotDummy::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	return CDialog::OnEraseBkgnd(pDC);
//}

void CDlgSnapshotDummy::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	this->DestroyWindow();
	CDialog::OnClose();
}

//void CDlgSnapshotDummy::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	m_PickPoint = point;
//}
//
//void CDlgSnapshotDummy::OnMouseMove(UINT nFlags, CPoint point)
//{
//	if (nFlags & MK_LBUTTON )
//	{	
//		
//		m_rectDraw.left -= (m_PickPoint.x - point.x);
//		m_rectDraw.top -= (m_PickPoint.y - point.y) ;
//		this->MoveWindow(&m_rectDraw, true);
//		GetParent()->Invalidate(true);
//		Invalidate(true);
//		m_PickPoint = point;
//	}
//
//	CDialog::OnMouseMove(nFlags, point);
//}
//
//BOOL CDlgSnapshotDummy::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//	::SetCursor(LoadCursor(NULL,IDC_SIZEALL));
//	m_zDelta += zDelta;
//	float fScale = (float)m_bih.biWidth / (float)abs(m_bih.biHeight);
//	int nX = (int)((float)(zDelta)*fScale);
//	int nY = (zDelta);
//	
//	CRect rectTmp;
//	rectTmp = m_rectDraw;
//	rectTmp.left -= nX/4;
//	rectTmp.top -= nY/4;	
//	rectTmp.right += nX;
//	rectTmp.bottom += nY;
//
//	if(rectTmp.left == 0 || rectTmp.top == 0 || rectTmp.Width() <= 0 || rectTmp.Height() <= 0) return 0;
//
//	m_rectDraw = rectTmp;
//	this->MoveWindow(&m_rectDraw, true);
//	
//	Invalidate(true);
//	GetParent()->Invalidate(true);
//
//	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
//}
//
void CDlgSnapshotDummy::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetDesktopWindow()->GetWindowRect(&rect);  
	CBrush brush(RGB(0,0,0));
	dc.FillRect(rect,&brush);


}

//void CDlgSnapshotDummy::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	CDialog::OnLButtonUp(nFlags, point);
//}
