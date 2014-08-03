// DlgAutoScanPause.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAutoScanPause.h"
#include "MainFrm.h"
#include "LiveView.h"
// CDlgAutoScanPause dialog

IMPLEMENT_DYNAMIC(CDlgAutoScanPause, CDialog)

CDlgAutoScanPause::CDlgAutoScanPause(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAutoScanPause::IDD, pParent),m_bPause(false)
{

}

CDlgAutoScanPause::~CDlgAutoScanPause()
{
	DELETEOBJ(pBtnPause);
}

void CDlgAutoScanPause::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GROUP_NAME, m_static_GroupName);
}


BEGIN_MESSAGE_MAP(CDlgAutoScanPause, CDialog)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDB_BITMAP_PAUSE50, &CDlgAutoScanPause::OnBnClickedBtnPauseAutoscan)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CDlgAutoScanPause message handlers

void CDlgAutoScanPause::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	DestroyWindow();
	CDialog::OnClose();
}

void CDlgAutoScanPause::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}

BOOL CDlgAutoScanPause::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	
	//vSetDisplayGroupName();

	//SetWindowTextW(_T("Group AutoScan"));
	
	
	DWORD dwStyle = BS_PUSHBUTTON |BS_NOTIFY |WS_VISIBLE;
    COLORREF clBG = MASKCOLOR;
    int nHoveOver = (int)BTNST_AUTO_GRAY;

	pBtnPause = new CButtonST;
	pBtnPause->Create(NULL, dwStyle, CRect(5,5,55,55), this, IDB_BITMAP_PAUSE50);
	if(((CMainFrame*)AfxGetMainWnd())->m_bGroupAutoScanPause)
	{
		pBtnPause->SetBitmaps(IDB_BITMAP_PLAY50, clBG ,nHoveOver);
		//((CLiveView*)m_pParentHwnd)->vPauseAutoScan();
	}
	else
	{		
		pBtnPause->SetBitmaps(IDB_BITMAP_PAUSE50, clBG ,nHoveOver);
	}	
	 CGS::SetTransparent(this->m_hWnd,200);
     CGS::SetTransparent(m_static_GroupName.m_hWnd, 200);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDlgAutoScanPause::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  Change any attributes of the DC here
	if( pWnd->GetDlgCtrlID() == IDC_STATIC_GROUP_NAME )
    {        
        pDC->SetTextColor( RGB( 255, 255, 255 ));
        pDC->SetBkMode( TRANSPARENT );
		//pDC->SetBkColor(BLACK_PEN);
		
    }

       return (HBRUSH)GetStockObject(GRAY_BRUSH);
    

	//TODO:  Return a different brush if the default is not desired
	//return hbr;
}

void CDlgAutoScanPause::vSetDisplayGroupName(void)
{
	if(!this) return;
	((CMainFrame*)AfxGetMainWnd())->vGetGroupName(); 
	CString csGroupName;
	csGroupName = ((CMainFrame*)AfxGetMainWnd())->m_vctcsGroupNames[((CMainFrame*)AfxGetMainWnd())->m_nCurrentPlayGroupId-1];
	m_static_GroupName.SetWindowTextW(csGroupName);
}

BOOL CDlgAutoScanPause::PreTranslateMessage(MSG* pMsg)
{
	//static bool bDrag(false),bMove(false);
	//if ( pMsg->message == WM_LBUTTONDOWN)// && pMsg->message == WM_MOUSEMOVE) 
	//{
	//	bDrag = true;
	//}
	//if (  pMsg->message == WM_MOUSEMOVE) 
	//{
	//	if(bDrag)
	//	bMove = true;
	//}
	//if ( pMsg->message == WM_LBUTTONUP)// && pMsg->message == WM_MOUSEMOVE) 
	//{
	//	if(bDrag && bMove)
	//	{
	//		CPoint pt;
	//		pt.x = GET_X_LPARAM(pMsg->lParam); 
	//		pt.y = GET_Y_LPARAM(pMsg->lParam);
	//		SetWindowPos(NULL,pt.x,pt.y,200,100,SWP_SHOWWINDOW);
	//		bDrag = bMove = false;
	//		
	//	}
	//}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgAutoScanPause::OnBnClickedBtnPauseAutoscan()
{
	// TODO: Add your control notification handler code here
	DWORD dwStyle = BS_PUSHBUTTON |BS_NOTIFY |WS_VISIBLE;
    COLORREF clBG = MASKCOLOR;
    int nHoveOver = (int)BTNST_AUTO_GRAY;
	((CMainFrame*)AfxGetMainWnd())->m_bGroupAutoScanPause ^= 1;
	m_bPause = ((CMainFrame*)AfxGetMainWnd())->m_bGroupAutoScanPause;
	//((CMainFrame*)AfxGetMainWnd())->m_AutoScanCallBack(((CMainFrame*)AfxGetMainWnd())->m_plParam);
	((CLiveView*)m_pParentHwnd)->m_bAutoScanPause = m_bPause;
	if(m_bPause)
	{
		pBtnPause->SetBitmaps(IDB_BITMAP_PLAY50, clBG ,nHoveOver);
		((CLiveView*)m_pParentHwnd)->vPauseAutoScan();
	}
	else
	{		
		pBtnPause->SetBitmaps(IDB_BITMAP_PAUSE50, clBG ,nHoveOver);
		((CLiveView*)m_pParentHwnd)->vResumeLiveView();
	}

}

void CDlgAutoScanPause::OnLButtonDown(UINT nFlags, CPoint point)
{
	SendMessage(WM_NCLBUTTONDOWN,HTCAPTION,0);
	CDialog::OnLButtonDown(nFlags, point);
}
