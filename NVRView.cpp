// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "WorkSpaceToolBar.h"

#include "NVRDoc.h"
#include "NVRView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNVRView

IMPLEMENT_DYNCREATE(CNVRView, CView)

BEGIN_MESSAGE_MAP(CNVRView, CView)
	ON_WM_CONTEXTMENU()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNVRView construction/destruction

CNVRView::CNVRView()
{
	// TODO: add construction code here

}

CNVRView::~CNVRView()
{
}

BOOL CNVRView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CNVRView drawing

void CNVRView::OnDraw(CDC* pDCView)
{
	CMemDC memDC (*pDCView, this);
	CDC* pDC = &memDC.GetDC ();

	CRect rectClient;
	GetClientRect (rectClient);

	pDC->FillSolidRect (rectClient, RGB (0, 0, 0));

}

/////////////////////////////////////////////////////////////////////////////
// CNVRView printing

void CNVRView::OnFilePrintPreview() 
{
	AFXPrintPreview (this);
}

BOOL CNVRView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CNVRView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CNVRView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CNVRView diagnostics

#ifdef _DEBUG
void CNVRView::AssertValid() const
{
	CView::AssertValid();
}

void CNVRView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNVRDoc* CNVRView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNVRDoc)));
	return (CNVRDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNVRView message handlers

void CNVRView::OnContextMenu(CWnd*, CPoint point)
{
	//theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}
