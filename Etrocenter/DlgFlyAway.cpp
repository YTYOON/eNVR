// DlgFlyAway.cpp : implementation file
//

#include "stdafx.h"
#include "DlgFlyAway.h"


// CDlgFlyAway dialog

const UINT  MsgDeleteFlyAway =RegisterWindowMessage(_T("MsgDeleteFlyAway") );

IMPLEMENT_DYNAMIC(CDlgFlyAway, CDialog)

CDlgFlyAway::CDlgFlyAway(CWnd* pParent /*=NULL*/)
: CDialog(CDlgFlyAway::IDD, pParent),m_nLayOutIdx(0),m_rect(0,0,0,0),m_pLiveView(NULL),m_nOriginalStyle(0),m_bFullScreen(false),
m_bAutoPlay(false),m_vctNode()
{

}

CDlgFlyAway::~CDlgFlyAway()
{
	
}

void CDlgFlyAway::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgFlyAway, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_WM_CLOSE()	
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgFlyAway message handlers

BOOL CDlgFlyAway::OnInitDialog()
{
	//CDialog::OnInitDialog();
	CRect rect;
	//GetDesktopWindow()->GetWindowRect(&rect);
	
	
	//this->SetWindowPos(&wndTopMost,rect.left,rect.top,rect.right,rect.bottom,SWP_SHOWWINDOW);
	
	
	//m_pEtroView.Create(NULL,NULL,WS_CHILD,rect,this,NULL);
	//m_pEtroView.ShowWindow(SW_SHOW);
	//m_pEtroView.m_bOnFlyAway = true;
	//m_pEtroView.OnInitialUpdate();
	//m_pEtroView.vAttach(m_vctNode);
	

	this->SetWindowTextW(_T("IP-Video Wall"));
    HMODULE hResOem = ::LoadLibrary(_T("Manufacturer.DLL"));
    if (hResOem)
    {
        SetIcon(::LoadIcon(hResOem, MAKEINTRESOURCE(IDR_MAINFRAME)), false);
        FreeLibrary(hResOem);
    }


	m_pLiveView = new CLiveView ;
	
	m_pLiveView->m_bOnFlyAway = true;
	m_pLiveView->m_bAutoPlay = m_bAutoPlay;
	m_pLiveView->Create(NULL,WS_CHILD,m_rect,this,NULL);
	m_pLiveView->m_rect = m_rect;
	m_pLiveView->SetWindowPos(NULL,m_rect.left,m_rect.top,m_rect.Width(),m_rect.Height(),SWP_SHOWWINDOW);	
	(m_pLiveView)->vSetGrids(m_nLayOutIdx);
	(m_pLiveView)->vAttach(m_vctNode);

	m_nOriginalStyle =  GetWindowLong(m_hWnd, GWL_STYLE);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFlyAway::OnSize(UINT nType, int cx, int cy)
{
	
	m_pLiveView->OnSize(nType,cx,cy);
}

void CDlgFlyAway::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	//m_pEtroView->OnDraw(&dc);
}

void CDlgFlyAway::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CDialog::OnLButtonDown(nFlags, point);
	m_pLiveView->OnLButtonDown(nFlags,point);
}

void CDlgFlyAway::GetNodeItem(vector <NODEITEM*> vctNode)
{
	if(m_vctNode.size()!=0) m_vctNode.clear();

	for(int i=0;i<vctNode.size();i++)
	{
		m_vctNode.push_back(vctNode[i]);

	}


}
void CDlgFlyAway::GetMatrixIdx(int LayOutIdx)
{

	m_nLayOutIdx = LayOutIdx;

}
void CDlgFlyAway::GetRect(CRect rect)
{
  memcpy(&m_rect,&rect,sizeof(rect));	

}

void CDlgFlyAway::GetAutoPlay(bool bAutoPlay)
{
	
	m_bAutoPlay = bAutoPlay;

}

void CDlgFlyAway::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	m_pLiveView->OnContextMenu(pWnd,point);
}


void CDlgFlyAway::OnClose()
{
	CDialog::OnClose();
	ShowWindow(SW_HIDE);
	DELETEOBJ(this->m_pLiveView);
	AfxGetMainWnd()->SendMessage(MsgDeleteFlyAway,(WPARAM)this,NULL);
	
	//this->DestroyWindow();
}

BOOL CDlgFlyAway::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pLiveView) m_pLiveView->PreTranslateMessage(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgFlyAway::OnDestroy()
{
	if (this->m_pLiveView)
	{
		delete this->m_pLiveView;
	}
	

	// TODO: Add your message handler code here
}

void CDlgFlyAway::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_pLiveView)
	m_pLiveView->OnMouseMove(nFlags,point);

}
void CDlgFlyAway::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (m_pLiveView)
	m_pLiveView->OnLButtonUp(nFlags,point);
}

void CDlgFlyAway::OnFullScreen()
{
	m_bFullScreen ^=1;
	long 	nStyle = m_nOriginalStyle;

	if(m_bFullScreen)
	{
		nStyle &= ~WS_BORDER ;
	}
	else
	{
		nStyle = m_nOriginalStyle;
	}

	SetWindowLong(m_hWnd, GWL_STYLE, nStyle);

	if(m_pLiveView)
	m_pLiveView->SetWindowPos(NULL,m_rect.left,m_rect.top,m_rect.Width(),m_rect.Height(),SWP_FRAMECHANGED);	

	::InvalidateRect(m_hWnd, NULL, TRUE);
	
}
BOOL CDlgFlyAway::OnEraseBkgnd(CDC* pDC)
{
	pDC->SetBkColor(BKCOLOR);//BKCOLOR
	CRect rect;
	GetClientRect(rect);
	CBrush bkColor(BKCOLOR);
	pDC->FillRect(&rect,&bkColor);

	return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CDlgFlyAway::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(CTLCOLOR_DLG == nCtlColor)
    {
       
        return (HBRUSH)GetStockObject(BLACK_PEN);
    }	
	
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CDlgFlyAway::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}
