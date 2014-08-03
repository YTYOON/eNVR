// EventLogView.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"
#include "EventLogView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEventLogView

IMPLEMENT_DYNCREATE(CEventLogView, CView)

CEventLogView::CEventLogView()
{
}

CEventLogView::~CEventLogView()
{
}


BEGIN_MESSAGE_MAP(CEventLogView, CView)
	//{{AFX_MSG_MAP(CEventLogView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEventLogView drawing

void CEventLogView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CEventLogView diagnostics

#ifdef _DEBUG
void CEventLogView::AssertValid() const
{
	CView::AssertValid();
}

void CEventLogView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEventLogView message handlers

int CEventLogView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	DWORD nStyle =  LVS_REPORT | WS_BORDER | WS_TABSTOP | LVS_SHOWSELALWAYS |LVS_SINGLESEL;
	//LVS_REPORT   | TVS_HASBUTTONS  | TVS_HASLINES| TVS_LINESATROOT | WS_BORDER | TVS_CHECKBOXES | TVS_SHOWSELALWAYS; 

	CRect rect(0,0,1500,500);
//	GetClientRect(&rect);
	BOOL bRet = m_listctrl.Create(nStyle, rect, this, 10001);
	vSetListStyle();
	return 0;
}

void CEventLogView::vSetListStyle()
{
#define MAX_DISPLAY_NUM			5

	LV_COLUMN lvcolumn;
	TCHAR* listcolumn[MAX_DISPLAY_NUM] = {_T("Alias"),_T("mac"),_T("date-time"),_T("Event"),_T("Memo")};
    int width[MAX_DISPLAY_NUM] = {120, 200, 130, 180, 200};
	int i;
	for( i= 0; i < MAX_DISPLAY_NUM; i++)
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = listcolumn[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = width[i];
		int nCol = m_listctrl.InsertColumn(i,&lvcolumn);
	}
	// Enable whole Line Select |LVS_EX_GRIDLINES
	DWORD nRet = m_listctrl.SetExtendedStyle(LBS_OWNERDRAWVARIABLE | WS_TABSTOP | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP );//| LVS_EX_ONECLICKACTIVATE
//	LBS_OWNERDRAWVARIABLE | WS_TABSTOP|LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT
	m_listctrl.SetTextColor(RGB(0,0,0));
//	m_listctrl.	SetBkColor(BKCOLOR);
	m_listctrl.ShowWindow(SW_SHOW);

	LV_ITEM lvi;
	TCHAR szItem[256];
	lvi.mask = LVIF_TEXT;
	for ( i=0; i  < 15; i++)
	{
		// First item
		lvi.iItem = i;
		lvi.iSubItem = 0;
		_stprintf(szItem, _T("IPCam-%02d"), i);
		lvi.pszText = szItem;
		m_listctrl.InsertItem(&lvi);

		// SubItem 1
		lvi.iSubItem = 1;
		_stprintf(szItem, _T("%02x-%02x-%02x-%02x-%02x-%02x"), rand()%256,rand()%256,rand()%256,rand()%256,rand()%256,rand()%256);
		lvi.pszText = szItem;
		m_listctrl.SetItem(&lvi);

		// SubItem 2
		lvi.iSubItem = 2;
		float f = (float)((i%4)*10 + 10);
		_stprintf(szItem, _T("%.2f"), f);
		m_listctrl.SetItem(&lvi);

		// SubItem 3
		lvi.iSubItem = 3;
		f = (float)((i%4)*10 + 10);
		_stprintf(szItem, _T("%.2f"), f);
		m_listctrl.SetItem(&lvi);

		// SubItem 4
		lvi.iSubItem = 4;
		_stprintf(szItem, _T("test"));
		m_listctrl.SetItem(&lvi);

	}
}

void CEventLogView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	CRect rect;
	m_listctrl.GetClientRect(&rect);
	m_listctrl.MoveWindow(0,0,cx,cy);	
	
}
