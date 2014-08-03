// EMapView.cpp : implementation file
//

#include "stdafx.h"
//#include "EtroLiveview.h"
#include "EMapView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEMapView

IMPLEMENT_DYNCREATE(CEMapView, CScrollView)

CEMapView::CEMapView():hMetaHandle(NULL),pData(NULL)
{
}

CEMapView::~CEMapView()
{
}


BEGIN_MESSAGE_MAP(CEMapView, CScrollView)
	//{{AFX_MSG_MAP(CEMapView)
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEMapView drawing

void CEMapView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CEMapView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	//InvalidateRect(&m_rect, true);
	if (hMetaHandle)
	{	
		PlayEnhMetaFile(pDC->m_hDC,hMetaHandle,&m_rect);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CEMapView diagnostics

#ifdef _DEBUG
void CEMapView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CEMapView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEMapView message handlers

void CEMapView::vSetEMap(CString csFile)
{
	CFileFind finder;
	if (FALSE == (finder.FindFile(csFile)))
	{
		ETROMSG(_T("Can't find Map file"));
		return;
	}
	finder.FindNextFile();
	DWORD nSize = finder.GetLength();
	CFile file;
	if (file.Open(csFile.LockBuffer(), CFile::modeRead ))
	{
		//DWORD nSize = 2800*1024;//csFile.GetLength();
		pData = new BYTE[nSize];
#if _MSC_VER >= 1200
		nSize = file.Read(pData, nSize);
#else
		nSize = file.ReadHuge(pData, nSize);
#endif
		hMetaHandle = SetEnhMetaFileBits(nSize, (BYTE*)pData);
		if (hMetaHandle == NULL) 
		{
			SDKERRMSG(1);
		}else
		{
			GetClientRect(&m_rect);
			Invalidate(true);
		}
	}
	file.Close();
	file.m_hFile = CFile::hFileNull;

}

void CEMapView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if (hMetaHandle)
	{	
		PlayEnhMetaFile(dc.m_hDC,hMetaHandle,&m_rect);
	}
}
BOOL CEMapView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	int nFactor = zDelta/6 ;
//	CRect rect(m_rect);
	m_rect.bottom = m_rect.bottom -  zDelta;
	m_rect.right = m_rect.right -  zDelta;
	(m_rect.bottom < 100) ? m_rect.bottom = 100 : NULL;
	(m_rect.right < 100) ? m_rect.right = 100 : NULL;
	CSize sizeTotal;
	sizeTotal.cx =m_rect.right - m_rect.left;
	sizeTotal.cy =m_rect.bottom - m_rect.top;
	Invalidate(true);
	SetScrollSizes( MM_LOENGLISH, sizeTotal );
	return true;

//	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CEMapView::OnDestroy() 
{
	CScrollView::OnDestroy();	
	if (hMetaHandle)
	{
		DeleteEnhMetaFile(hMetaHandle);
		hMetaHandle = 0;
	}
	if (pData)
		delete pData;
}

BOOL CEMapView::OnEraseBkgnd(CDC* pDC) 
{
	pDC->SetBkColor(BKCOLOR);
	CRect rect;
	GetClientRect(rect);
	CGS::GradientFillRect(pDC, rect, BKCOLOR, RGB(0, 0, 0), GRADIENT_FILL_RECT_V);
	return true;	
//	return CScrollView::OnEraseBkgnd(pDC);
}
