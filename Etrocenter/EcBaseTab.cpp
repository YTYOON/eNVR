#include "stdafx.h"
#include "EcBaseTab.h"
#include "Resource.h"
#include "EcBaseSlider.h"
#include "BaseBitmapButton.h"
#include "TabpageEventDialog.h"
#include "TabpagePTZDialog.h"
#include "ColorBox.h"
#include "IPCamDiscovery.h"
#include "INI.h"
//#include "MainFrm.h"
//#include "DlgSettingEvent.h"
#include "EcPtz.h"
#include "MainFrm.h"
#include "ListCtrlEx.h"

#define STARTID 3000

//static CListCtrlEx* pEventList;
static bool EventReceiveFlag;
//static CameraEvent m_CameraEvent;
static CDatabase m_DB;

static void AddEventList(void* lParam)
{
	//CEventReceive * pEvent = (CEventReceive *) lParam;
	//CEcBaseTab * pThis = (CEcBaseTab *) lpParam;

	//int nItem;

	//CString csIPAddress;
	//csIPAddress.Format(_T("%s"), pEvent->m_NODEITEM.ip);

	//pThis->lvItem.mask = LVIF_TEXT;

	//nItem = pThis->pEventList->InsertItem(0, _T(""), 0);
	//pThis->pEventList->SetItemText(nItem, 1, csIPAddress);

}

CEcBaseTab::CEcBaseTab(void):hThreadEventRec(NULL)//m_EventRec(NULL),
{
	m_brush.CreateSolidBrush(GLOBALBKCOLOR);
	m_DialogID[0] =IDD_DLG_TABPTZ;
	//m_DialogID[1] =IDD_DLG_TABEVENT;

	m_Dialog[0] = new CTabpagePTZDialog();
	//m_Dialog[1] = new CTabpageEventDialog();

	pHome = new CBitmapButton();	
	pTop = new CBitmapButton();
	pTRgt = new CBitmapButton();
	pRgt = new CBitmapButton();
	pBRgt = new CBitmapButton();
	pBot = new CBitmapButton();
	pBLft = new CBitmapButton();
	pLft = new CBitmapButton();
	pTLft = new CBitmapButton();
	pZoomIn = new CBitmapButton();
	pZoomOut = new CBitmapButton();
	pPTZSetPreset= new CButton();
	pPTZGo= new CButton();
	
	pPTZInput = new CEdit();

	pPatrol = new CColorBox();
	pZoom = new CEcBaseSlider();
	pCBPreset = new CComboBox();
	pPTZSet = new CColorBox();
	//pEventList = new CListCtrlEx();

	m_nPageCount = 2;
	EventIDCount = 1;	//Event ID Count

}


CEcBaseTab::~CEcBaseTab(void)
{
	vector<CEventReceive*>::iterator pos;
	//Delete Tabpage...and bitmaps
	delete m_Dialog[0];
	//delete m_Dialog[1];	

	delete pHome;	
	delete pTop;
	delete pTRgt;
	delete pRgt;
	delete pBRgt;
	delete pBot;
	delete pBLft;
	delete pLft;
	delete pTLft;
	delete pZoomIn;
	delete pZoomOut;

	delete pPatrol;
	delete pZoom;
	delete pCBPreset;
	delete pPTZSet;
//	delete pEventList;
		
//	delete m_EventRec;

	delete pPTZSetPreset;
	delete pPTZGo;
	delete pPTZInput;
	
	CSA csa;
	HANDLE hEndEvent = ::CreateEvent(&csa.m_sa, false, false, _T("Global\\ETROCENTER_EVENT_END"));
	::SetEvent(hEndEvent);
	CloseHandle(hThreadEventRec);

	::Sleep(1000);

}

BEGIN_MESSAGE_MAP(CEcBaseTab, CTabCtrl)
//	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CREATE()
//	ON_WM_PAINT()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CEcBaseTab::OnTcnSelchange)
	//ON_CONTROL_RANGE(BN_CLICKED, IDB_PTZTOP, IDB_PTZHOME , &CEcBaseTab::OnTabPTZButtonClicked)
	//ON_CONTROL_RANGE(WM_LBUTTONUP, IDB_PTZTOP, IDB_PTZHOME , &CEcBaseTab::OnTabPTZButtonClickedStop)
	//ON_CONTROL_RANGE(BN_CLICKED, IDB_MINUS, IDB_PLUS , &CEcBaseTab::OnTabZoomButtonClicked)
	//ON_NOTIFY_REFLECT(NM_CLICK, &CEcBaseTab::OnNMClick)
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_CTLCOLOR()
//	ON_WM_DRAWITEM()
	ON_WM_CONTEXTMENU()
	//ON_COMMAND(ID_MENUPOP_EVENTSETTING, OnEventPopSetting)
	ON_COMMAND(ID_MENU_CLEAREVENTLOG, OnEventClearLog)
	ON_CBN_SELCHANGE(IDC_COMBO_PRESET, &CEcBaseTab::OnCbnSelchangeComboPTZPreset)
	ON_BN_CLICKED(STARTID+12, &CEcBaseTab::OnBnClickedPatrolBut)
	ON_BN_CLICKED(STARTID+13, &CEcBaseTab::OnBnClickedPTZSettingBut)
	ON_BN_CLICKED(STARTID+14, &CEcBaseTab::OnBnClickedPresetSet)
	ON_BN_CLICKED(STARTID+15, &CEcBaseTab::OnBnClickedPresetGo)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


int CEcBaseTab::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	 m_MaxWidth = 200;
	 m_MaxHeight = 200;
	//DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP;
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitCreate();
	//ofCreateEventList();

	CRect DisplayArea;
	int i, n;

	CTabCtrl::PreSubclassWindow();
	m_Parent = GetParent();
	CString csTitle;
	//csTitle.LoadString(CGS::g_hResStr, IDS_BASE_TAB_EVENT);
	//LOADSTRING(csTitle, IDS_BASE_TAB_EVENT);
	////if (InsertItem(0, _T("Events"))==-1)
	//if(InsertItem(0, csTitle)==-1)
	//{
	//	CString csMsg(_T(""));
	//	LOADSTRING(csMsg, IDS_INSERT_FAIL_0);
	//	AfxMessageBox(csMsg);
	//}

	//csTitle.LoadString(CGS::g_hResStr, IDS_BASE_TAB_PTZ);
	LOADSTRING(csTitle, IDS_BASE_TAB_PTZ);
	if (InsertItem(1, csTitle)==-1)
	{
		CString csMsg(_T(""));
		LOADSTRING(csMsg, IDS_INSERT_FAIL_1);
		AfxMessageBox(csMsg);
	}
	 
	//Set Events tabpage as opened
	//SetCurSel(1);
	//ShowHideImage(true);
	//pHome->ShowWindow(false);
	//pTop->ShowWindow(false);
	//pTRgt->ShowWindow(false);
	//pTLft->ShowWindow(false);
	//pRgt->ShowWindow(false);
	//pBRgt->ShowWindow(false);
	//pBot->ShowWindow(false);
	//pBLft->ShowWindow(false);
	//pLft->ShowWindow(false);
	//pZoom->ShowWindow(false);
 //   pZoomIn->ShowWindow(SW_HIDE);
 //   pZoomOut->ShowWindow(SW_HIDE);
	//pPatrol->ShowWindow(false);
	//m_PresetP.ShowWindow(false);
	////pCBPreset->ShowWindow(false);
	//pPTZSet->ShowWindow(false);
//	pEventList->ShowWindow(true);

	/*pPTZSetPreset->ShowWindow(SW_HIDE);
	pPTZGo->ShowWindow(SW_HIDE);
	pPTZInput->ShowWindow(SW_HIDE);*/
	
	m_CMainFrame =(CMainFrame*)AfxGetMainWnd(); 

	EventReceiveFlag = true;

	hTabCursor = ::GetCursor();

//	m_EventRec = new CEventReceive();
	
	UINT nRetEvent(0);
	//hThreadEventRec = (HANDLE)_beginthreadex(NULL, 0, GetEventFromService, (LPVOID)this, 0, &nRetEvent);

	//m_EventRec->vSetCallBack(vCallBack, this);
	//m_EventRec->vStartReceive();

	// TODO:  Add your specialized creation code here	
	return 0; 
}


BOOL CEcBaseTab::InitCreate()
{
	int		i_midx=80, i_w=20,  i_d=60, i_dis=40;
	int		i_top=50, i_h=20, i_h1=25, i_boty=160;

	//vDBConnect();
	
	
	pHome->Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(i_midx-15, i_top + ((i_boty-i_top)/2)-15, i_midx+35,  i_top + ((i_boty-i_top)/2) +35), this, IDB_PTZHOME);
	pHome->LoadBitmaps(IDB_PTZHOME, IDB_PTZHOME, IDB_PTZHOME, IDB_PTZHOME); 
	//pHome->SetIndex(0);

//	enum {BTNLEFT}

// Create the bitmap button (must include the BS_OWNERDRAW style).
	pTop->Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(i_midx, i_top, i_midx+i_w, i_top+i_h), this, IDB_PTZTOP);
	pTop->LoadBitmaps(IDB_PTZTOP, IDB_PTZTOP, IDB_PTZTOP, IDB_PTZTOP);
	//pTop->SetIndex(1);

	//Top-Right
	pTRgt->Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(i_midx + i_dis, i_top+ 20, i_midx+i_dis+i_w, i_top+20+i_h), this, IDB_PTZTRGT);
	pTRgt->LoadBitmaps(IDB_PTZTRGT, IDB_PTZTRGT, IDB_PTZTRGT, IDB_PTZTRGT);
	//pTRgt->SetIndex(2);

	// Load the bitmaps for this button.
	pRgt->Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(i_midx+i_d, i_top + (i_boty-i_top)/2, i_midx+i_d+i_w,  (i_top+(i_boty-i_top)/2)+i_h), this, IDB_PTZRGT);
	pRgt->LoadBitmaps(IDB_PTZRGT, IDB_PTZRGT, IDB_PTZRGT, IDB_PTZRGT); 
	//pRgt->SetIndex(3);

	//Bottom Right
	pBRgt->Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(i_midx + i_dis, i_boty-i_h, i_midx+i_dis+i_w, i_boty-i_h+i_h), this, IDB_PTZBRGT);
	pBRgt->LoadBitmaps(IDB_PTZBRGT, IDB_PTZBRGT, IDB_PTZBRGT, IDB_PTZBRGT); 
	//pBRgt->SetIndex(4);

	// Bottom
	pBot->Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(i_midx, i_boty, i_midx+i_w, i_boty+i_h), this, IDB_PTZBOT);
	pBot->LoadBitmaps(IDB_PTZBOT, IDB_PTZBOT, IDB_PTZBOT, IDB_PTZBOT); 
	//pBot->SetIndex(5);

	//Bottom Left
	pBLft->Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(i_midx-i_dis, i_boty-i_h,  i_midx-i_dis+i_w, i_boty-i_h+i_h), this, IDB_PTZBLFT);
	pBLft->LoadBitmaps(IDB_PTZBLFT, IDB_PTZBLFT, IDB_PTZBLFT, IDB_PTZBLFT); 
	//pBLft->SetIndex(6);

	// Left
	pLft->Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(i_midx-i_d, i_top + (i_boty-i_top)/2, i_midx-i_d+i_w,  (i_top+(i_boty-i_top)/2)+i_h), this, IDB_PTZLFT);
	pLft->LoadBitmaps(IDB_PTZLFT, IDB_PTZLFT, IDB_PTZLFT, IDB_PTZLFT); 
	//pLft->SetIndex(7);

	//Top Left
	pTLft->Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(i_midx-i_dis, i_top+ 20, i_midx-i_dis+i_w,  i_top+ 20+i_h), this, IDB_PTZTLFT);
	pTLft->LoadBitmaps(IDB_PTZTLFT, IDB_PTZTLFT, IDB_PTZTLFT, IDB_PTZTLFT); 
	//pTLft->SetIndex(8);

	//Zoom In buttom
	pZoomIn->Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(165, i_boty-i_h+i_h + 30,195, i_boty-i_h+i_h+60), this, IDB_PLUS);
	pZoomIn->LoadBitmaps(IDB_MINUS, IDB_MINUS, IDB_MINUS, IDB_MINUS); 
	//Zoom Out buttom
	pZoomOut->Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(5, i_boty-i_h+i_h + 30, 30 , i_boty-i_h+i_h+60), this, IDB_MINUS);
	pZoomOut->LoadBitmaps(IDB_MINUS, IDB_MINUS, IDB_MINUS, IDB_MINUS); 

	// Zoom Scroll
	pZoom->Create( WS_CHILD|WS_VISIBLE|TBS_HORZ , CRect(20, i_boty-i_h+i_h + 30,166, i_boty-i_h+i_h+60), this, STARTID+9);
	pZoom->SetRange(-6,6,true);
	pZoom->SetPos(0);

	int ll_xpos=200;
	int ll_py= 50;
	
	CString csStaticString;
	//csStaticString.LoadString(CGS::g_hResStr, IDS_BASE_TAB_PRE_POS);
	LOADSTRING(csStaticString, IDS_BASE_TAB_PRE_POS);

	m_PresetP.Create(csStaticString, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(ll_xpos, ll_py, ll_xpos+100, ll_py+20), this, STARTID+10);
	//Preset Position Editbox
	/* pCBPreset->Create( WS_CHILD|WS_VISIBLE|BS_OWNERDRAW|CBS_DROPDOWNLIST , CRect(ll_xpos, ll_py+20, ll_xpos+100, ll_py+150), this, STARTID+11);
	 pCBPreset->AddString(_T("Pos1"));
	 pCBPreset->AddString(_T("Pos2"));
	 pCBPreset->AddString(_T("Pos3"));
	 pCBPreset->AddString(_T("Pos4"));
	 pCBPreset->AddString(_T("Pos5"));
	 pCBPreset->SetCurSel(0);*/
	pPTZInput->Create(WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,CRect(ll_xpos, ll_py+20, ll_xpos+30, ll_py+40),this,STARTID+11);
	pPTZInput-> ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_DRAWFRAME);
	pPTZInput->SetWindowTextW(_T("1"));

	// preset set button
	pPTZSetPreset->Create(_T("Set"),BS_PUSHBUTTON,CRect(ll_xpos+40, ll_py+20, ll_xpos+70, ll_py+40),this,STARTID+14);
	// preset go button
	pPTZGo->Create(_T("Go"),BS_PUSHBUTTON,CRect(ll_xpos+70, ll_py+20, ll_xpos+95, ll_py+40),this,STARTID+15);
	

	//Patrol Button
	//csStaticString.LoadString(CGS::g_hResStr, IDS_BASE_TAB_PATROL);
	LOADSTRING(csStaticString, IDS_BASE_TAB_PATROL);
	pPatrol->Create(csStaticString, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(ll_xpos+10,ll_py+60,ll_xpos+90,ll_py+95), this, STARTID+12);

	//csStaticString.LoadString(CGS::g_hResStr, IDS_BASE_TAB_SET_PTZ);
	LOADSTRING(csStaticString, IDS_BASE_TAB_SET_PTZ);
	pPTZSet->Create(csStaticString, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(ll_xpos, ll_py+100,ll_xpos+100, ll_py+135), this, STARTID+13);

	return true;
}


// Create the dialogs that are the tab control's contents.
// The parent must be the same parent as the tab control's.
// Therefore they will be repositioned later. Until
// then we gather the maximum sizes so the tab control can
// be adjusted in size so the tab dialogs will fit.
BOOL CEcBaseTab::CreateTab(CDialog *pDlg, UINT nId, int x) {
	CRect WindowRect;
	int tw, th;

	if (!pDlg->Create(nId, GetParent()))
		return FALSE;
	pDlg->GetWindowRect(&WindowRect);
	tw = WindowRect.Width();
	m_MaxWidth = tw < m_MaxWidth ? m_MaxWidth : tw;
	th = WindowRect.Height();
	m_MaxHeight = th < m_MaxHeight ? m_MaxHeight : th;
	return TRUE;

}


void CEcBaseTab::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	::SetCursor(hTabCursor);

	if (GetCurSel()==1)//Event
	{
		m_lb_flag = true;
	}
	else
	{
		m_lb_flag = false;
	}

	//AfxBeginThread(ShowHideImage, (LPVOID)this);
	ShowHideImage(this);
	//ShowHideImage(this);
	//this->UpdateWindow();
	// TODO: Add your control notification handler code here
	*pResult = 0;

}

//void  CEcBaseTab::ShowHideImage(bool o_show)
UINT CEcBaseTab::ShowHideImage(LPVOID lpParam)
{
	CEcBaseTab* m_CEcBaseTab = (CEcBaseTab*)lpParam;
	
	m_CEcBaseTab->pHome->ShowWindow(m_CEcBaseTab->m_lb_flag);
	m_CEcBaseTab->pTop->ShowWindow(m_CEcBaseTab->m_lb_flag);
	m_CEcBaseTab->pTRgt->ShowWindow(m_CEcBaseTab->m_lb_flag);
	m_CEcBaseTab->pTLft->ShowWindow(m_CEcBaseTab->m_lb_flag);
	m_CEcBaseTab->pRgt->ShowWindow(m_CEcBaseTab->m_lb_flag);
	m_CEcBaseTab->pBRgt->ShowWindow(m_CEcBaseTab->m_lb_flag);
	m_CEcBaseTab->pBot->ShowWindow(m_CEcBaseTab->m_lb_flag);
	m_CEcBaseTab->pBLft->ShowWindow(m_CEcBaseTab->m_lb_flag);
	m_CEcBaseTab->pLft->ShowWindow(m_CEcBaseTab->m_lb_flag);
    m_CEcBaseTab->pZoom->ShowWindow(m_CEcBaseTab->m_lb_flag);
    m_CEcBaseTab->pZoomIn->ShowWindow(m_CEcBaseTab->m_lb_flag);
    m_CEcBaseTab->pZoomOut->ShowWindow(m_CEcBaseTab->m_lb_flag);
	m_CEcBaseTab->pPatrol->ShowWindow(m_CEcBaseTab->m_lb_flag);
	//m_CEcBaseTab->pCBPreset->ShowWindow(m_CEcBaseTab->m_lb_flag);
	m_CEcBaseTab->m_PresetP.ShowWindow(m_CEcBaseTab->m_lb_flag);
	m_CEcBaseTab->pPTZSetPreset->ShowWindow(m_CEcBaseTab->m_lb_flag);
	m_CEcBaseTab->pPTZGo->ShowWindow(m_CEcBaseTab->m_lb_flag);
	m_CEcBaseTab->pPTZInput->ShowWindow(m_CEcBaseTab->m_lb_flag);

	m_CEcBaseTab->pPTZSet->ShowWindow(m_CEcBaseTab->m_lb_flag);
//	pEventList->ShowWindow(!m_CEcBaseTab->m_lb_flag);
	m_CEcBaseTab->UpdateWindow();

	return 0;
	/*pHome->ShowWindow(o_show);
	pTop->ShowWindow(o_show);
	pTRgt->ShowWindow(o_show);
	pTLft->ShowWindow(o_show);
	pRgt->ShowWindow(o_show);
	pBRgt->ShowWindow(o_show);
	pBot->ShowWindow(o_show);
	pBLft->ShowWindow(o_show);
	pLft->ShowWindow(o_show);
	pZoom->ShowWindow(o_show);
	pPatrol->ShowWindow(o_show);
	m_PresetP.ShowWindow(o_show);
	pCBPreset->ShowWindow(o_show);

	pPTZSet->ShowWindow(o_show);
	pEventList->ShowWindow(!o_show);*/
}

BOOL CEcBaseTab::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CBrush brush(GLOBALBKCOLOR);
	//CBrush brush(LISTFRONTCOLOR);
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &brush);
	return true;
}

void CEcBaseTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	//This function is use to hookup the SDK for Zooming
	int i_pos;
	CString t;
	PTZAction ptzaction;

	i_pos = pZoom->GetPos();
	//t.Format(_T("%d"), i_pos);
	if( i_pos >=0 )
		ptzaction = Z_I;
	else
	{
		ptzaction = Z_O;
		i_pos = -(i_pos);
	}	
		

	if(nSBCode == SB_ENDSCROLL)
	{
		pZoom->SetPos(0);
		((CEcPtz*)GetParent())->OnTabPTZButtonClickedStop();
		return;
	}

	((CEcPtz*)GetParent())->vOnZoomScroll(ptzaction, i_pos);
	//::Sleep(5);

	 //AfxMessageBox(t);

	//CTabCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CEcBaseTab::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CTabCtrl::OnCtlColor(pDC, pWnd, nCtlColor);

   //  We can use CWnd::GetDlgCtrlID() to perform the most efficient test.
	int ll_id = pWnd->GetDlgCtrlID();
	if (ll_id==STARTID+11) return hbr;
 
    // Set the text color to red
    pDC->SetTextColor(GLOBALFONTCOLOR);

    // Set the background mode for text to transparent so background will show thru.
    pDC->SetBkMode(TRANSPARENT);

    // Return handle to our CBrush object
    hbr = m_brush;
   
	return hbr;
}


//void  CEcBaseTab::ofCreateEventList()
//{
//	CRect lrect;
//	GetClientRect(lrect);
//
//	//DWORD	dwStyle = LVS_ ;
//	DWORD	dwExtStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT ;
//
//
////	pEventList->Create( dwExtStyle, CRect(lrect.left,lrect.top+28,lrect.right,lrect.bottom), this, STARTID + 14);   
//
////	pEventList->SetExtendedStyle(pEventList->GetExtendedStyle()|dwExtStyle);
//	
//	LVCOLUMN	lvColumn;
//    CString csTitle;
//    LOADSTRING(csTitle, IDS_EVENTLOG_COLUMN_TITLE);
//    TCHAR* tzTitle[4];
//    TCHAR tzTok[] =_T(";\r\n");
//    tzTitle[0] = _tcstok(csTitle.LockBuffer(), tzTok);
//    tzTitle[1] = _tcstok(NULL, tzTok);
//    tzTitle[2] = _tcstok(NULL, tzTok);
//    tzTitle[3] = _tcstok(NULL, tzTok);
//	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
//	lvColumn.fmt = LVCFMT_CENTER;
//	lvColumn.cx = 20;
//	lvColumn.pszText = _T("");
////	pEventList->InsertColumn(0, &lvColumn);
//
//	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
//	lvColumn.fmt = LVCFMT_CENTER;
//	lvColumn.cx = 70;
//	lvColumn.pszText = tzTitle[0];
//	pEventList->InsertColumn(1, &lvColumn);
//
//	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
//	lvColumn.fmt = LVCFMT_CENTER;
//	lvColumn.cx = 80;
//	lvColumn.pszText = tzTitle[1];
//	pEventList->InsertColumn(2, &lvColumn);
//
//	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
//	lvColumn.fmt = LVCFMT_CENTER;
//	lvColumn.cx = 100;
//	lvColumn.pszText =tzTitle[2];
//	pEventList->InsertColumn(3, &lvColumn);
//
//	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
//	lvColumn.fmt = LVCFMT_CENTER;
//	lvColumn.cx = 70;
//	lvColumn.pszText = tzTitle[3];
//	pEventList->InsertColumn(4, &lvColumn);
//
//	//lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
//	//lvColumn.fmt = LVCFMT_CENTER;
//	//lvColumn.cx = 50;
//	//lvColumn.pszText = _T("Alert");
//	//pEventList->InsertColumn(5, &lvColumn);
//
//	m_images.Create(16, 16, ILC_COLOR8, 0, 3);
//
//	CBitmap bIconRed, bIconOrange, bIconYellow;
//	bIconRed.LoadBitmapW(IDB_REDCIRCLE);
//	bIconOrange.LoadBitmapW(IDB_ORANGECIRCLE);
//	bIconYellow.LoadBitmapW(IDB_YELLOWCIRCLE);
//
//	m_images.Add(&bIconRed, RGB(255, 255, 255));
//	m_images.Add(&bIconOrange, RGB(0, 0, 0));
//	m_images.Add(&bIconYellow, RGB(0, 0, 0));
//
//	pEventList->SetImageList(&m_images, LVSIL_SMALL);
//
//	//ofManualAddEventList();
//
//}
//

//
//void CEcBaseTab::vCallBack(EventInfo *m_EventInfo, void *lpParam)
//{
//	CEcBaseTab* pThis = (CEcBaseTab*)lpParam;
//
//	CString csEventIP(_T("")), csEventContext(_T("")), csEventStreamID(_T("")), csEventCamIDX(_T(""));
//	csEventIP.Format(_T("%s"), m_EventInfo->tcIP);
//
//	switch(m_EventInfo->nEventType)
//	{
//	case EVENT_TYPE::MOTION_DETECTION:
//		//csEventContext.Format(_T("%s"), m_EventInfo->tcEventType);
//		if(m_EventInfo->nStatus == EVENT_STATUS::OFF)
//		{
//			csEventContext = _T("Motion Detection OFF");
//		}
//		else if(m_EventInfo->nStatus == EVENT_STATUS::ON)
//		{
//			csEventContext = _T("Motion Detection ON");
//		}
//		
//		break;
//	case EVENT_TYPE::DIGITAL_INPUT:
//		if(m_EventInfo->nStatus == EVENT_STATUS::OFF)
//		{
//			csEventContext = _T("Digital Input OFF");
//		}
//		else if(m_EventInfo->nStatus == EVENT_STATUS::ON)
//		{
//			csEventContext = _T("Digital Input ON");
//		}
//		break;
//	case EVENT_TYPE::VIDEO_LOST:
//		csEventContext = _T("Video Lost");
//		break;
//	case EVENT_TYPE::VIDEO_OPEN:
//		csEventContext = _T("Video Open");
//		break;
//	case EVENT_TYPE::REBOOT:
//		csEventContext = _T("Reboot");
//		break;
//	case EVENT_TYPE::DIGITAL_OUTPUT:
//		csEventContext = _T("Digital Output");
//		break;
//	case EVENT_TYPE::MISSING_OBJECT:
//		csEventContext = _T("Missing Object");
//		break;
//	case EVENT_TYPE::EVENT_TRIGGER:
//		csEventContext = _T("Event Trigger");
//		break;
//	default:
//		ASSERT(false);
//		break;
//	}
//	
//	//csEventStreamID.Format(_T("%d"), m_EventInfo->nStream);
//	csEventCamIDX.Format(_T("%d"), m_EventInfo->nCamIdx);
//
//	if(csEventIP == _T(""))	return;
//	
//	CString csDBEventCode = pThis->csGetEventTypeDB(csEventContext);
//
//	m_CameraEvent = pThis->GetCameraEventDB(csEventCamIDX, csDBEventCode, csEventStreamID);
//	
//	int nItem =0;
//	CString csTime(_T("")), csEventID(_T("")), csEvent(_T(""));
//	CTime timeTime = 0;
//
//	if(pEventList->GetItemCount() > 19)
//	{
//		pEventList->DeleteItem((pEventList->GetItemCount() -1));
//	}
//
//	if(pThis->EventIDCount > 100000)	pThis->EventIDCount = 1;
//		csEventID.Format(_T("%06d"), pThis->EventIDCount);
//
//	timeTime = CTime::GetCurrentTime();
//	csTime.Format(_T("%02d:%02d:%02d"), timeTime.GetHour(), timeTime.GetMinute(), timeTime.GetSecond());
//
//	if( pThis->bCheckCameraExist(csEventIP) )
//	{
//		nItem = pEventList->InsertItem(0, _T(""), (m_CameraEvent.iAlertLevel-1));
//		pEventList->SetItemText(nItem, 1, csEventID);
//		pEventList->SetItemText(nItem, 2, csEventIP);
//		pEventList->SetItemText(nItem, 3, csEventContext + _T(" ") + csEventStreamID);
//		pEventList->SetItemText(nItem, 4, csTime);
//		pThis->UpdateWindow();
//
//		pThis->EventIDCount++;
//	}
//	//CString csDBEventType = pThis->csGetEventTypeDB(csEventContext);
//
//	//pThis->vChangeTreeCtrl(csEventIP, csEventContext + _T(" ") + csEventStreamID, csEventCamIDX, m_CameraEvent);
//	//CString csSQLInsertEventLog;
//	//csSQLInsertEventLog.Format(_T("INSERT INTO ec_event_log (eventid, event_type_code, alert_level, device_id, occur_time) VALUES('%s', '%s %s', '%d', '%s', '%s')"), csEventID, csDBEventType, csEventStreamID, m_CameraEvent.iAlertLevel, csEventCamIDX, csTime);
//	//pThis->vInsertEventLogDB(csSQLInsertEventLog);
//}

CString CEcBaseTab::csEventCode(CString csCode)
{
	if(csCode == _T("m"))
		return _T("Motion Detect");
	else if(csCode == _T("i"))
		return _T("Digital Input");
	else if(csCode == _T("v"))
		return _T("Video");
	else if(csCode == _T("r"))
		return _T("Reboot");
	else if(csCode == _T("o"))
		return _T("Digital Output");
	else
		return _T("Missing Object");
}

void CEcBaseTab::vDeleteEventRec(CString csIP)
{
	
}

void CEcBaseTab::vEventReConnection(CString csIP)
{
	
}

//Popup a menu after mouse right click ...
void CEcBaseTab::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	//AfxMessageBox(_T("POPUP"));
	CMenu Menu;
	Menu.LoadMenu(IDR_MENU_EVENT_POPUP);

	CPoint Point = point;
	GetCursorPos(&Point);

	CMenu *pMenu = Menu.GetSubMenu(0);
	if (pMenu)
	pMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, Point.x,Point.y, this);
}

//CameraEvent CEcBaseTab::GetCameraEventDB(CString csEventCameraIDX, CString csEventType, CString csStreamID)
//{
//	CString csDSN, csDBUser, csDBPass, csODBC;
//
//	CameraEvent m_CameraEvent;
//
//	CString csAlertLevel;
//	//CDatabase DBQuery;
//
//	if(m_DB.IsOpen() == 0)	//Check DB is Open
//	{this->vDBConnect();}
//
//	try
//	{
//		//if( DBQuery.Open((csDSN), FALSE, FALSE, csODBC) )
//		//{/*AfxMessageBox(_T("Connection OK"));*/}
//		CString SQLAlertLevel;
//	//	SQLAlertLevel.Format(_T("SELECT camera_eventid, alert_level, event_source_code, device_action_code FROM ec_camera_event WHERE camera_idx = '%s' AND event_type_code = '%s' AND streamid = '%s'"), csEventCameraIDX, csEventType, csStreamID);
//		SQLAlertLevel.Format(_T("SELECT camera_eventid, alert_level, event_source_code, device_action_code FROM ec_camera_event WHERE camera_idx = '%s' AND event_type_code = '%s'"), csEventCameraIDX, csEventType);
//		CRecordset	rAlertLevel(&m_DB);
//  		rAlertLevel.Open(CRecordset::forwardOnly, SQLAlertLevel);
//		while (!rAlertLevel.IsEOF()) {
//			rAlertLevel.GetFieldValue(_T("camera_eventid"), m_CameraEvent.csEventID);
//			rAlertLevel.GetFieldValue(_T("alert_level"), csAlertLevel);
//			rAlertLevel.GetFieldValue(_T("event_source_code"), m_CameraEvent.csEventSourceCode);
//			rAlertLevel.GetFieldValue(_T("device_action_code"), m_CameraEvent.csDeviceActionCode);
//			m_CameraEvent.csEventID.TrimRight();
//			csAlertLevel.TrimRight();
//			//AfxMessageBox(Model);
//			rAlertLevel.MoveNext();
//		//}
//		//DBQuery.Close();
//		} //while
//
//	} 
//	catch(CDBException * p)
//	{
//		AfxMessageBox(p->m_strError);
//		p->Delete();
//		//DBQuery.Close();
//	}
//
//	if(csAlertLevel == _T(""))
//	{
//		m_CameraEvent.csEventID = _T("0");
//		m_CameraEvent.iAlertLevel = 3;
//		m_CameraEvent.csEventSourceCode = _T("CAMERA");
//	}
//	else 
//	{
//		int iAlertLevel = _ttoi (csAlertLevel.GetBuffer(0));
//		m_CameraEvent.iAlertLevel = iAlertLevel;
//	}
// 
//	return m_CameraEvent;
//}

CString CEcBaseTab::csGetEventAlarm(int iAlertLevel)
{
	CString csAlarmLevel;
	CString csDSN, csDBUser, csDBPass, csODBC;

	if(m_DB.IsOpen() == 0)	//Check DB is Open 
	{this->vDBConnect();}

	try
	{
		CString SQLAlertLevel;
		SQLAlertLevel.Format(_T("SELECT alarm_level_type FROM ec_hevent_alarm WHERE sort_order = '%d'"), iAlertLevel);
		CRecordset rAlertLevel(&m_DB);//&DBQuery);
		rAlertLevel.Open(CRecordset::forwardOnly, SQLAlertLevel);
		while (!rAlertLevel.IsEOF()) {
			rAlertLevel.GetFieldValue(_T("alarm_level_type"), csAlarmLevel);
			csAlarmLevel.TrimRight();
			//AfxMessageBox(Model);
			rAlertLevel.MoveNext();
		}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	if(csAlarmLevel == _T(""))
		return _T("No Match Alarm Level");

	return csAlarmLevel;
}

void CEcBaseTab::vInsertEventLogDB(CString SQLInsert)
{
	CString csDSN, csDBUser, csDBPass, csODBC;

	if(m_DB.IsOpen() == 0)	//Check DB is Open
	{this->vDBConnect();}

	try
	{
		TRACE(SQLInsert);
		m_DB.ExecuteSQL(SQLInsert);
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

CString CEcBaseTab::csGetEventTypeDB(CString csCGIEventType)
{
	CString csDBEventType;
	CString csDSN, csDBUser, csDBPass, csODBC;

	if(m_DB.IsOpen() == 0)	//Check DB is Open
	{this->vDBConnect();}

	try
	{
		CString SQLEventTypeCode;
		SQLEventTypeCode.Format(_T("SELECT event_type_code FROM ec_hevent_type WHERE event_type_desc = '%s'"), csCGIEventType);
		//CRecordset rEventTypeCode(&DBQuery);
		CRecordset rEventTypeCode(&m_DB);
		rEventTypeCode.Open(CRecordset::forwardOnly, SQLEventTypeCode);
		while (!rEventTypeCode.IsEOF()) {
			rEventTypeCode.GetFieldValue(_T("event_type_code"), csDBEventType);
			csDBEventType.TrimRight();
			//AfxMessageBox(Model);
			rEventTypeCode.MoveNext();
		} //Try
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return csDBEventType;
}

//void CEcBaseTab::OnEventPopSetting()
//{
//	DlgSettingEvent lDlg;
//	lDlg.DoModal();
//}

void CEcBaseTab::OnEventClearLog()
{
		//CString ls_confirm = _T("Are you sure to clear all the events log from database ?");
		int ll_ret = AfxMessageBox(IDS_CLEAR_EVENT_DB, MB_YESNO|MB_ICONQUESTION);
		if (ll_ret==IDYES) 	
		{				
			try
			{
				//Clear DB log
				CString ls_sql;
				ls_sql = _T("delete from ec_event_log where 1=1");
				m_DB.ExecuteSQL(ls_sql);
//				pEventList->DeleteAllItems();

			}
			catch(CDBException * p)
			{
				AfxMessageBox(p->m_strError);
				p->Delete();
			}
		}
	
}



void CEcBaseTab::OnTabPTZButtonClicked(UINT nCmdID)
{
	((CEcPtz*)GetParent())->OnTabPTZButtonClicked(nCmdID);
}

void CEcBaseTab::OnTabZoomButtonClicked(UINT nCmdID)
{
	((CEcPtz*)GetParent())->OnTabZoomButtonClicked(nCmdID);
}

void CEcBaseTab::OnCbnSelchangeComboPTZPreset()
{	
	int nPreset =  pCBPreset->GetCurSel() + 1;
	((CEcPtz*)GetParent())->OnCbnSelchangeComboPTZPreset(nPreset);
}

void CEcBaseTab::OnBnClickedPatrolBut()
{
	((CEcPtz*)GetParent())->OnBnClickedPatrolBut();
}

void CEcBaseTab::OnBnClickedPresetSet()
{
	int nPreset(0);
	CString csPreset;
	pPTZInput->GetWindowTextW(csPreset);
	nPreset = _ttoi(csPreset);
	if( nPreset < 0 || nPreset > 255) return;
	((CEcPtz*)GetParent())->vSetPresetPoint(nPreset);
}
void CEcBaseTab::OnBnClickedPresetGo()
{
	int nPreset(0);
	CString csPreset;
	pPTZInput->GetWindowTextW(csPreset);
	nPreset = _ttoi(csPreset);
	if( nPreset < 0 || nPreset > 255) return;
	((CEcPtz*)GetParent())->OnCbnSelchangeComboPTZPreset(nPreset);
}

void CEcBaseTab::OnBnClickedPTZSettingBut()
{
	((CEcPtz*)GetParent())->OnBnClickedPTZSettingBut();
}

//void CEcBaseTab::OnTabPTZButtonClickedStop(UINT nCmdID)
//{
//			((CEcPtz*)GetParent())->OnTabPTZButtonClickedStop(nCmdID);

//}
/* Modification 2010/12/01

1.Modify List Control


/////////////////////*/
bool CEcBaseTab::vDBConnect(void)
{
	try {
		
		CIniReader m_ini;
		m_ini.setINIFullFileName(_T("\ec.ini"));
		
		CString cpDns = m_ini.getKeyValue(_T("DSN"),_T("database"));
		CString cpUid = m_ini.getKeyValue(_T("UID"),_T("database"));
		CString cpPwd = m_ini.getKeyValue(_T("PWD"),_T("database"));
	 
		CString ls_connect;
		ls_connect.Format(_T("ODBC;UID=%s;PWD=%s"), cpUid, cpPwd);
		return m_DB.Open((LPCTSTR)cpDns, FALSE, FALSE, ls_connect );
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
	return true;
}

int CEcBaseTab::vGoLiveView(CString oIP, CString oType)
{
	if (oIP.Trim()==_T("")) return -1;
	CString lsType = oType.Trim().MakeUpper();
	if (lsType ==_T("MOTION DETECT") || lsType ==_T("DIGITAL OUTPUT")  || lsType ==_T("MOTION DETECTION") || lsType ==_T("MD"))
	//if (lsType ==_T("MOTION DETECT") ||  lsType ==_T("MOTION DETECTION") || lsType ==_T("MD"))
	{
			CMainFrame * m_CMainFrame = (CMainFrame*)(::AfxGetApp()->GetMainWnd());
			CTaskTreeCtrl* pTreeCtrl = (CTaskTreeCtrl *)m_CMainFrame->GetTreeView();
			NODEITEM* pNode = pTreeCtrl->vGetNodeitemFromIP(oIP);
			if (pNode)
			{
				//pTreeCtrl->vLiveView();
				/*	m_CMainFrame->vFreeVideoSet();
					m_CMainFrame->vAlocateViewCell(1);*/
			}
	}
	return 0;
}

//void CEcBaseTab::ParseEvent(CString lParam)
//{
//	CString csEvent, csEventIP, csEventType, csCamIDX;
//	
//	TCHAR tcEvent[512]={0};
//
//	::_tcscpy(tcEvent, lParam);
//
//	TCHAR *tcToken={0};
//
//	tcToken = wcstok(tcEvent, _T(","));
//	csCamIDX.Format(_T("%s"), tcToken);
//
//	tcToken = wcstok(NULL, _T(","));
//	csEventIP.Format(_T("%s"), tcToken);
//
//	tcToken = wcstok(NULL, _T(","));
//	csEventType.Format(_T("%s"), tcToken);
//
//	if(this->bCameraEnable(csCamIDX))
//	{
//		CString csCGIEventType = csEventCode(csEventType.Left(1));
//		
//		CString csStreamID = csEventType.Right(1);
//		int iCurPos = 0, iTemp = csEventType.Find(_T("Open"));
//		if(csEventType.Left(1) == _T("m"))	//Motion Detection
//		{
//			CString csTypeMD;
//			AfxExtractSubString(csTypeMD, (LPCTSTR)csEventType, 1, 'd'); 
//			csStreamID = csTypeMD.Left(1);
//		}
//		else if( iTemp != -1 && csEventType.Left(1) == _T("v"))		//Video Signal
//		{
//			csEventType = csEventType.Tokenize(_T(" "), iCurPos);
//			csStreamID = csEventType.Right(1);
//		}
//		else if( csStreamID == _T("s") )	//video Loss
//		{	
//			csStreamID = _T("");
//			csCGIEventType = csCGIEventType + _T(" Loss");
//		}
//
//		CString csDBEventType = csGetEventTypeDB(csCGIEventType);
//
//		CameraEvent m_CameraEvent = GetCameraEventDB(csCamIDX, csDBEventType, csStreamID);
//
//		//vInsertEventList(csEventIP, csCGIEventType, csStreamID, csCamIDX, csDBEventType, m_CameraEvent);
//		vChangeTreeCtrl(csEventIP, csEventType, csCamIDX, m_CameraEvent);
//	}
//}

//UINT _stdcall CEcBaseTab::GetEventFromService(LPVOID lpParam)
//{
//	CEcBaseTab * pThis = (CEcBaseTab*) lpParam;
//
//	pThis->m_EventRec->vSetCallBack(&pThis->vCallBack, pThis);
//	pThis->m_EventRec->vStartReceive();
//
//	return 0;
//}

//void CEcBaseTab::vInsertEventList(CString csEventIP, CString csCGIEventType, CString csStreamID, CString csCameraIDX, CString csDBEventType, CameraEvent m_CameraEvent)
//{
//	int nItem;
//	CString csTime(_T("")), csEventID(_T(""));
//
//	CTime timeTime = CTime::GetCurrentTime();
//	csTime.Format(_T("%02d:%02d:%02d"), timeTime.GetHour(), timeTime.GetMinute(), timeTime.GetSecond());
//
//	if(pEventList->GetItemCount() > 19)	pEventList->DeleteItem((pEventList->GetItemCount() - 1));
//	csEventID.Format(_T("%06d"), EventIDCount);
//
//	nItem = pEventList->InsertItem(0, _T(""), m_CameraEvent.iAlertLevel - 1);
//	pEventList->SetItemText(nItem, 1, csEventID);
//	pEventList->SetItemText(nItem, 2, csEventIP);
//	pEventList->SetItemText(nItem, 3, (csCGIEventType + _T(" ") + csStreamID));
//	pEventList->SetItemText(nItem, 4, csTime);
//	UpdateWindow();
//
//	EventIDCount++;
//
//	return;
//}

bool CEcBaseTab::bCameraEnable(CString csCamIDX)
{
	if(m_DB.IsOpen() == 0)	//Check DB is Open
	{this->vDBConnect();}

	try
	{
		CString csActive, csSQL;
		csSQL.Format(_T("SELECT * FROM ec_camera WHERE camera_idx = '%s'"), csCamIDX);
		CRecordset rCameraEnable(&m_DB);
		rCameraEnable.Open(CRecordset::forwardOnly, csSQL);
		while (!rCameraEnable.IsEOF()) {
			rCameraEnable.GetFieldValue(_T("active_"), csActive);
			csActive.TrimRight();
			rCameraEnable.MoveNext();
		} 

		if(csActive == _T("Y"))return true;
		else return false;
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return true;
}

void CEcBaseTab::vChangeTreeCtrl(CString o_csEventIP, CString csEventType, CString o_csCamIDX, CameraEvent m_CameraEvent)
{
	CString csEventIP = o_csEventIP;
	CString csCamIDX = o_csCamIDX;

	CTaskTreeCtrl* pTreeCrl = (CTaskTreeCtrl* )this->m_CMainFrame->GetTreeView();
	if (pTreeCrl == NULL) return;
	
	int iAlertLevel = 4;

	if(iAlertLevel > m_CameraEvent.iAlertLevel)
		iAlertLevel = m_CameraEvent.iAlertLevel;

	if(EventReceiveFlag == false)
					return;

	HTREEITEM hParent = pTreeCrl->GetRootItem();
	while(hParent != NULL)
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem;
		hChildItem = pTreeCrl->GetChildItem(hParent);

		while(hChildItem != NULL)
		{
			hNextItem = pTreeCrl->GetNextItem(hChildItem, TVGN_NEXT);

			CString csNodeIP;
			csNodeIP = pTreeCrl->GetItemText(hChildItem);
			//csEventIP = m_CEventReceive->m_csEventIP;

			CString csTempIDX, csTempIDX2;
			AfxExtractSubString(csTempIDX, (LPCTSTR)csNodeIP, 1, '(');
			AfxExtractSubString(csTempIDX2, (LPCTSTR)csTempIDX, 0, ')');

			if(csNodeIP.Find(csEventIP) != -1 && csTempIDX2 == csCamIDX)
			{
				//AfxMessageBox(_T("Got It!"));

				pTreeCrl->Expand(hParent, TVE_EXPAND);
				pTreeCrl->SetItemImage(hChildItem, iAlertLevel, 0);
				break;
			}
			hChildItem = hNextItem;
		}
		hParent = pTreeCrl->GetNextSiblingItem(hParent);
	}
	
	
}

//UINT CEcBaseTab::PushToListCtrl(LPVOID lpParam)
//{
//	CEcBaseTab* pThis = (CEcBaseTab*)lpParam;
//	
//	HANDLE hStartInsertItem = ::CreateEvent(NULL, false, false, _T("Global\\START_INSERT"));
//	HANDLE hStopInsertItem = ::CreateEvent(NULL, false, false, _T("Global\\STOP_INSERT"));
//
//	HANDLE hThreads[2] = {hStartInsertItem, hStopInsertItem};
//
//	HANDLE hWaitEvent = NULL;
//
//	DWORD dwResult = ::WaitForMultipleObjects(2, &hThreads[0], false, INFINITE);
//
//	int nItem =0;
//	CString csTime(_T("")), csEventID(_T("")), csEvent(_T(""));
//	CTime timeTime = 0;
//
//	CString csEventIP(_T("")), csEventContext(_T("")), csEventStreamID(_T("")), csEventCamIDX(_T(""));
//	csEventIP.Format(_T("%s"), pThis->m_EventInfo.tcIP);
//	csEventContext.Format(_T("%s"), pThis->m_EventInfo.tcEventType);
//	csEventStreamID.Format(_T("%s"), pThis->m_EventInfo.tcStream);
//	csEventCamIDX.Format(_T("%s"), pThis->m_EventInfo.tcCamIDX);
//
//	while(EventReceiveFlag)
//	{
//		switch(dwResult)
//		{
//		case WAIT_OBJECT_0:
//
//			if(pEventList->GetItemCount() > 19)
//			{
//				pEventList->DeleteItem((pEventList->GetItemCount() -1));
//			}
//
//			if(pThis->EventIDCount > 100000)	pThis->EventIDCount = 1;
//				csEventID.Format(_T("%06d"), pThis->EventIDCount);
//
//			timeTime = CTime::GetCurrentTime();
//			csTime.Format(_T("%02d:%02d:%02d"), timeTime.GetHour(), timeTime.GetMinute(), timeTime.GetSecond());
//
//			nItem = pEventList->InsertItem(0, _T(""), (m_CameraEvent.iAlertLevel-1));
//			pEventList->SetItemText(nItem, 1, csEventID);
//			pEventList->SetItemText(nItem, 2, csEventIP);
//			pEventList->SetItemText(nItem, 3, csEventContext + _T(" ") + csEventStreamID);
//			pEventList->SetItemText(nItem, 4, csTime);
//			pThis->UpdateWindow();
//
//			pThis->EventIDCount++;
//
//			break;
//		case WAIT_OBJECT_0+1:
//			return 0;
//			break;
//		};
//	}
//
//	return 0;
//}

bool CEcBaseTab::bCheckCameraExist(CString csIP)
{
	try
	{
		CString csSQL, csValue;
		csSQL.Format(_T("SELECT * FROM ec_camera WHERE ipaddress = '%s'"), csIP);
	
		CRecordset rCheckCam(&m_DB);
		rCheckCam.Open(CRecordset::forwardOnly, csSQL);
		while(!rCheckCam.IsEOF()) {

			rCheckCam.GetFieldValue(_T("camera_idx"), csValue);
			csValue.TrimRight();

			rCheckCam.MoveNext();
		}

		if(csValue.GetLength() == 0)
		{
			return false;
		}

	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
		return false;
	}

	return true;
}
void CEcBaseTab::OnDestroy()
{
	//m_EventRec->vStopEventReceive();
    CTabCtrl::OnDestroy();
}
