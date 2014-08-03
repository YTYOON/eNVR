// DlgMatrixTab.cpp : implementation file
//

#include "stdafx.h"
#include "DlgMatrixTab.h"
#include "MainFrm.h"
#include "ini.h"
// DlgMatrixTab dialog
//================================
IMPLEMENT_DYNAMIC(CCustomTabCtrl, CTabCtrl)
BEGIN_MESSAGE_MAP(CCustomTabCtrl, CTabCtrl)
//{{AFX_MSG_MAP(CCustomTabCtrl)
    ON_WM_ERASEBKGND()	
    //ON_WM_PAINT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

HBRUSH CCustomTabCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkColor(BKCOLOR);	
	return  m_Brushbk;

}

//================================

IMPLEMENT_DYNAMIC(DlgMatrixTab, CDialog)

DlgMatrixTab::DlgMatrixTab(CWnd* pParent, DWORD nID /*=NULL*/)
: CDialog(nID, pParent),m_nCurTabIdx(0),m_dlgMatrixTag(NULL),btnclose(NULL),m_dlgMatrixEvent(NULL)
,m_bPopupDialog(false)
{

}

DlgMatrixTab::~DlgMatrixTab()
{
	DELETEOBJ(m_dlgMatrixTag);
	DELETEOBJ(btnclose);
	DELETEOBJ(m_dlgMatrixEvent);
}

void DlgMatrixTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
}


BEGIN_MESSAGE_MAP(DlgMatrixTab, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &DlgMatrixTab::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDB_CLOSE, &DlgMatrixTab::OnBnClickedClose)	
	ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// DlgMatrixTab message handlers
// enum
//{
//	ETC_FLAT = 1, 
//	ETC_COLOR = 2, // draw tabs with color
//	ETC_SELECTION = 4, // highlight the selected tab
//	ETC_GRADIENT = 8, // draw colors with a gradient
//	ETC_BACKTABS = 16,
//};

BOOL DlgMatrixTab::OnInitDialog()
{
    //if (m_bPopupDialog)
    //{
    //    SetWindowLong(this->m_hWnd, GWL_STYLE,
    //        ((GetWindowLong(this->m_hWnd, GWL_STYLE) & ~(WS_CHILD)) | WS_POPUP));
    //   //| DS_FIXEDSYS | WS_CHILD | WS_VISIBLE
    //    //STYLE DS_SETFONT | DS_MODALFRAME | DS_FIXEDSYS | WS_POPUP | WS_CAPTION | WS_SYSMENU
    //     //DS_SETFONT | DS_FIXEDSYS | WS_CHILD | WS_SYSMENU

    //    //ModifyStyle(WS_CHILD,  DS_MODALFRAME  | WS_POPUP | WS_CAPTION );
    //}else
    //{                 //GWL_EXSTYLE
    //    SetWindowLong(this->m_hWnd, GWL_STYLE,
    //        ((GetWindowLong(this->m_hWnd, GWL_STYLE) & ~(WS_POPUP)) | WS_CHILD));

    //    //ModifyStyle( WS_POPUP, WS_CHILD);
    //}

	//POINT pos;
	//GetCursorPos(&pos);
	// here is your coordinates
	//int i_x=pos.x;
	//int i_y=pos.y;
	//int i_h=70, i_w=48, i_gap=30, i_sw=10, i_dw=410, i_y1=50 ;

	//SetWindowPos(NULL, i_x-i_dw, i_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
 	CDialog::OnInitDialog();
    if (m_bPopupDialog)
    {
        m_Brushbk.CreateSolidBrush(GLOBALBKCOLOR);
        SetWindowLong(this->m_hWnd, GWL_STYLE, (GetStyle() | DS_SETFONT | DS_FIXEDSYS | WS_POPUP | WS_SYSMENU |WS_BORDER)); 
    }
    else 
        m_Brushbk.CreateSolidBrush(BKCOLOR);


	DWORD dwStyle = BS_PUSHBUTTON |BS_NOTIFY |WS_VISIBLE;
    COLORREF clBG = BKCOLOR;
    int nHoveOver = (int)BTNST_AUTO_GRAY;
	CRect rc;
	GetClientRect(rc);
	//ClientToScreen(rc);
	//MoveWindow(rc.left, rc.top, rc.right, rc.bottom, SWP_NOSIZE | SWP_NOZORDER);
	
	CString csMsg;
	LOADSTRING(csMsg,IDS_EMAPLIVEVIEW);
	AfxExtractSubString(csMsg, csMsg.GetBuffer(), 0, ':');

	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = (LPWSTR)(LPCTSTR)csMsg;
	m_TabCtrl.InsertItem(LV, &tcItem);

	m_dlgMatrixTag = new CDlgMatrixTag;
	m_dlgMatrixTag->Create(IDD_DLG_MATRIX,&m_TabCtrl);
	
	LOADSTRING(csMsg,IDS_ADD_CAM_EVENT_GROUP)
	tcItem.pszText = (LPWSTR)(LPCTSTR)csMsg;
	m_TabCtrl.InsertItem(EVENTLV, &tcItem);
	
	m_dlgMatrixEvent = new CDlgMatrixEvent;
	m_dlgMatrixEvent->Create(IDD_DLG_MATRIX_EVENT,&m_TabCtrl);

	LOADSTRING(csMsg,IDS_FLYAWAY);
	tcItem.pszText = (LPWSTR)(LPCTSTR)csMsg;
	m_TabCtrl.InsertItem(IPWALL, &tcItem);
    vSetTabCtrlStyle();

	CRect rect;
    //rect.OffsetRect(-10, -10);
	m_TabCtrl.GetClientRect(&rect);
	//ClientToScreen(rect);
	rect.top+=20;
	//rect.bottom-=4;
	//rect.left+=4;
	//rect.right-=4;

	m_dlgMatrixTag->MoveWindow(&rect);
	m_dlgMatrixEvent->MoveWindow(&rect);
	
	m_dlgMatrixTag->ShowWindow(SW_SHOW);
	m_dlgMatrixEvent->ShowWindow(SW_HIDE);
	// this runs first than CEtroExplorerView::OnInitialUpdate(), so i have to get this value from ini
	CIniReader ini;
	CString csEventLV = ini.getKeyValue(_T("EventLV"),_T("video"));
	if( _ttoi(csEventLV) == 1) 
	{
		m_TabCtrl.SetCurSel(1);// select event tab
		m_dlgMatrixEvent->ShowWindow(SW_SHOW);
		m_dlgMatrixTag->ShowWindow(SW_HIDE);
		((CMainFrame*)AfxGetMainWnd())->m_bEventLV = true;
	}
	else
		((CMainFrame*)AfxGetMainWnd())->m_bEventLV = false;
	

	((CMainFrame*)AfxGetMainWnd())->m_bIPWallSelected = false;
	
    //if (m_bPopupDialog)
    //{
    //    SetWindowLong(this->m_hWnd, GWL_STYLE,
    //        ((GetWindowLong(this->m_hWnd, GWL_STYLE) & ~(WS_CHILD)) | WS_POPUP | DS_MODALFRAME));
    //   //| DS_FIXEDSYS | WS_CHILD | WS_VISIBLE
    //    //STYLE DS_SETFONT | DS_MODALFRAME | DS_FIXEDSYS | WS_POPUP | WS_CAPTION | WS_SYSMENU
    //     //DS_SETFONT | DS_FIXEDSYS | WS_CHILD | WS_SYSMENU

    //    //ModifyStyle(WS_CHILD,  DS_MODALFRAME  | WS_POPUP | WS_CAPTION );

    //    CRect rc;
    //    GetWindowRect(rc);
    //    SetWindowPos(this, rc.left, rc.top,rc.Width(),rc.Height(),WS_EX_TOPMOST);
    //    CenterWindow();
    //}else
    //{                 
    //    SetWindowLong(this->m_hWnd, GWL_STYLE,
    //        ((GetWindowLong(this->m_hWnd, GWL_STYLE) & ~(WS_POPUP)) | WS_CHILD));
    //}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DlgMatrixTab::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_nCurTabIdx = m_TabCtrl.GetCurSel();
		
	switch(m_nCurTabIdx)
	{
	case LV:
		((CMainFrame*)AfxGetMainWnd())->m_bIPWallSelected = false;
		((CMainFrame*)AfxGetMainWnd())->m_bEventLV = false;
		m_dlgMatrixTag->ShowWindow(SW_SHOW);		
		m_dlgMatrixEvent->ShowWindow(SW_HIDE);
		m_dlgMatrixTag->vSetMyCBoxID(MyCBox_LiveView);
		break;
	case EVENTLV:
		((CMainFrame*)AfxGetMainWnd())->m_bIPWallSelected = false;
		((CMainFrame*)AfxGetMainWnd())->m_bEventLV = true;
		m_dlgMatrixTag->ShowWindow(SW_HIDE);
		m_dlgMatrixEvent->ShowWindow(SW_SHOW);
		break;
	case IPWALL:
		((CMainFrame*)AfxGetMainWnd())->m_bIPWallSelected = true;
		((CMainFrame*)AfxGetMainWnd())->m_bEventLV = false;
		m_dlgMatrixTag->ShowWindow(SW_SHOW);
		m_dlgMatrixEvent->ShowWindow(SW_HIDE);
		m_dlgMatrixTag->vSetMyCBoxID(MyCBox_IPWall);
		break;
	default:break;
	}	
	this->Invalidate(true);
	*pResult = 0;
}

BOOL DlgMatrixTab::OnEraseBkgnd(CDC* pDC)
{
	m_nCurTabIdx = m_TabCtrl.GetCurSel();

	/*CBrush brush;

	switch(m_nCurTabIdx)
	{
	case LV:
		brush.CreateSolidBrush(RGB(0,200,0));
		break;
	case IPWALL:
		brush.CreateSolidBrush(RGB(0,0,200));
		break;
	case EVENTLV:
		brush.CreateSolidBrush(RGB(200,0,0));
		break;
	default:
		brush.CreateSolidBrush(GLOBALBKCOLOR);
		break;
	}*/
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &m_Brushbk);
	return true;
}

void DlgMatrixTab::OnBnClickedClose()
{
	EndDialog(-1);
}
HBRUSH DlgMatrixTab::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    //HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    pDC->SetTextColor(FONTCOLOR);
	pDC->SetBkColor(BKCOLOR);	
	return  m_Brushbk;
}


void DlgMatrixTab::vSetTabCtrlStyle(void)
{

    m_TabCtrl.SetBkgndColor(BKCOLOR);
    m_TabCtrl.SetFontColor(FONTCOLOR);

    DWORD dwFlags = 0;

		dwFlags |= ETC_FLAT;

		dwFlags |= ETC_COLOR;

		dwFlags |= ETC_SELECTION;

		dwFlags |= ETC_GRADIENT;

		dwFlags |= ETC_BACKTABS;

	//m_TabCtrl.SetBkgndColor(BKCOLOR)/;/? m_color : ::GetSysColor(COLOR_3DFACE));
	m_TabCtrl.SetBkgndColor(BKCOLOR);
	m_TabCtrl.EnableDraw(BTC_ALL);//1 ? BTC_ALL : dwFlags ? BTC_TABS : BTC_NONE); 

	CEnTabCtrl::EnableCustomLook(dwFlags, dwFlags);
	m_TabCtrl.Invalidate();
	Invalidate();
}


void DlgMatrixTab::vRestoreBtnState(int nType)
{
	switch(nType)
	{
	case LIVEVIEWBTN: // liveview btn was clicked, restore event btn state		
		m_dlgMatrixEvent->vSetMatrixBtnHot(0);
		break;
	case EVENTBTN: // event btn was clicked, restore liveview btn state
		m_dlgMatrixTag->vSetMatrixBtnHot(0);
		break;
	}
}
