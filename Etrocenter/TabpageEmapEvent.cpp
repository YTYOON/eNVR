// TabpageEmapEvent.cpp : implementation file
//

#include "stdafx.h"
#include "TabpageEmapEvent.h"


// CTabpageEmapEvent dialog

IMPLEMENT_DYNAMIC(CTabpageEmapEvent, CBaseDialog)

CTabpageEmapEvent::CTabpageEmapEvent(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CTabpageEmapEvent::IDD, pParent), m_ToolTip(NULL)
{

}

CTabpageEmapEvent::~CTabpageEmapEvent()
{
	if (m_ToolTip != NULL) 
		delete m_ToolTip;
}

void CTabpageEmapEvent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTCLEAR, m_butClear);
	DDX_Control(pDX, IDC_LISTEVENT, m_listevent);
}


BEGIN_MESSAGE_MAP(CTabpageEmapEvent, CBaseDialog)
	//ON_BN_CLICKED(IDC_BUTSAVEAS, &CTabpageEmapEvent::OnBnClickedButsaveas)
	//ON_BN_CLICKED(IDC_BUTCLEAR, &CTabpageEmapEvent::OnBnClickedButclear)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CTabpageEmapEvent message handlers

void CTabpageEmapEvent::OnBnClickedButsaveas()
{
	// TODO: Add your control notification handler code here
	//	fnSaveEventLog();
}

void CTabpageEmapEvent::OnBnClickedButclear()
{
//	m_listevent.DeleteAllItems();
 
}

BOOL CTabpageEmapEvent::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	int ll_posx=2, ll_posy=2;
	DWORD dwStyle =WS_CHILD | WS_VISIBLE | BS_OWNERDRAW; 

	m_butSaveas.AutoLoad(IDC_BUTSAVEAS, this);
	m_butSaveas.MoveWindow(ll_posx, ll_posy, 24, 24, true);

	ll_posx += 26;
	m_butClear.LoadBitmaps(IDB_CAMGROUP_DEL,IDB_CAMGROUP_DEL,IDB_CAMGROUP_DEL,IDB_CAMGROUP_DEL);
	m_butClear.MoveWindow(ll_posx, ll_posy, 24, 24, true);

	CTabCtrl* lpTab = (CTabCtrl*)GetParent();
	CRect lrect;
	lpTab->GetClientRect(lrect);
	int ll_gap=28, ll_gapw=14, ll_gaph=58;
	m_listevent.MoveWindow(0, ll_gap, lrect.Width()-ll_gapw, lrect.Height() - ll_gaph );

	LV_COLUMN ListColumn;

	WCHAR * ListTitles1[5] = {_T(""),_T("Time"), _T("ID"),_T("Camera IP"),_T("Event Type")};
	for(int i = 0; i < 5; i++)
		{
			ListColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			ListColumn.fmt = LVCFMT_LEFT;
			ListColumn.iSubItem = i;

			if (i==0)  ListColumn.cx = 20;
			else if (i==1)  ListColumn.cx = 73; //Time
			else if (i==2)  ListColumn.cx = 25; //ID
			else if (i==3)  ListColumn.cx = 80; // Camera IP
			else if (i==4)  ListColumn.cx = 130; //Event

			ListColumn.pszText = ListTitles1[i];
			m_listevent.InsertColumn(i, &ListColumn);
		}

		dwStyle = m_listevent.GetStyle();
		dwStyle |= LVS_EX_FULLROWSELECT|LVS_SINGLESEL;
		m_listevent.SetExtendedStyle(dwStyle);
 
		//Tooltips
		 m_ToolTip = new CToolTipCtrl;
		if (!m_ToolTip->Create(this))
		{
			TRACE(_T("Unable To create ToolTip\n"));
		}
		CString ls_st;
		ls_st.LoadString(CProductDefine::g_hResStr, IDS_CLEAR_EVENT);
		m_ToolTip->AddTool(&m_butClear, ls_st);

		ls_st.LoadString(CProductDefine::g_hResStr, IDS_EMAPSAVEAS);
		m_ToolTip->AddTool(&m_butSaveas, ls_st);

		return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTabpageEmapEvent::OnSize(UINT nType, int cx, int cy)
{
	CBaseDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
}

bool CTabpageEmapEvent::fnDisplayEvent(CString oCamIdx, CString oCamIP, CString oEventType, CString o_curHMS)
{
			LVITEM lvi;
		CString strItem;
 
		//To prevent event triggered after window closed
//		if (m_EventReceiveFlag == false) return false;
		// Insert the first item
		lvi.mask =  LVIF_IMAGE | LVIF_TEXT;
 
		lvi.iItem = 0;
		lvi.iSubItem = 0;
		lvi.pszText = _T("");

		//If there's no row defined in the DB (priority -1), ignore the event
		int ll_priority=0;
//		ll_priority = CheckEventPriority(oCamIdx, oEventType);

		ll_priority--; //the imagelist is the priority - 1
		m_listevent.InsertItem(0,_T(""),ll_priority);

 		// Set subitem 1
		strItem.Format(_T("%s"),o_curHMS );
		lvi.iSubItem = 1;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_listevent.SetItem(&lvi);

		// Set subitem 2
		strItem.Format(_T("%s"), oCamIdx);
		lvi.iSubItem =2;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_listevent.SetItem(&lvi);
		
		// Set subitem 3
		strItem.Format(_T("%s"),  oCamIP );
		lvi.iSubItem =3;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_listevent.SetItem(&lvi);

		// Set subitem 4
		strItem.Format(_T("%s"),  oEventType );
		lvi.iSubItem =4;
		//CString ls_type = SmartEventType(strItem, true);
		lvi.pszText = (LPTSTR)(LPCTSTR)(oEventType);
		m_listevent.SetItem(&lvi);

	return false;
}

CString CTabpageEmapEvent::SmartEventType(CString oOrgEventType)
{
	return SmartEventType(oOrgEventType, false);
}

//oLong true will return long string ex. Motion Detect
CString CTabpageEmapEvent::SmartEventType(CString oOrgEventType, bool obLong)
{
	CString ls_ret=_T("");
	oOrgEventType = oOrgEventType.MakeLower();

	if ( oOrgEventType.Find(_T("input")) != -1)  //Digital Input
	{	
		if (obLong) 
			ls_ret = _T("Digital Input");
		else
			ls_ret = _T("DI");
	}
	else if ( oOrgEventType.Find(_T("signal")) != -1) //Video Loss
	{
		if (obLong) 
			ls_ret = _T("Video Loss");
		else
			ls_ret = _T("VL");
	}
	else if ( oOrgEventType.Find(_T("output")) != -1)  //Digital Output
	{
		if (obLong) 
			ls_ret = _T("Digital Output");
		else
			ls_ret = _T("DO");
	}
	else if ( oOrgEventType.Find(_T("md")) != -1)  //Motion Detect
	{
		if (obLong) 
			ls_ret = _T("Motion Detect");
		else
			ls_ret = _T("MD");
	}

	return ls_ret;
}


bool CTabpageEmapEvent::fnSaveEventLog()
{
	//Under Construction.....................
		CString	ls_tfolder =_T("");
		CFileDialog fOpenDlg(TRUE, _T("txt"), _T("event_log"), OFN_HIDEREADONLY|OFN_CREATEPROMPT, 	_T("Select Bitmap File (*.txt)|*.txt|"), this);
		fOpenDlg.m_pOFN->lpstrTitle=_T("Save Event Log As");	  
		fOpenDlg.m_pOFN->lpstrInitialDir=_T("c:");
	  
		if(fOpenDlg.DoModal()==IDOK)
		{
			CString ls_path = fOpenDlg.GetPathName();
			CFile cLogFile;
			
			cLogFile.Open( ls_path,  CFile::modeCreate|CFile:: modeReadWrite);
			
			//Loop into list Event
			for(int iRow = 0; iRow<  m_listevent.GetItemCount(); iRow++)
			{	
				CString	ls_data, ls_time, ls_cam, ls_event;
				ls_time = m_listevent.GetItemText(iRow, 1);
				//ls_pri = m_listevent.GetItemText(iRow, 2);
				ls_cam= m_listevent.GetItemText(iRow, 3);
				ls_event= m_listevent.GetItemText(iRow, 4);
				ls_data.Format(_T("Time:%s  Camera:%s   Event:%s \r\n"),ls_time.Trim(),ls_cam.Trim(), ls_event.Trim()); 
					
				USES_CONVERSION;		

				char*	lcp_data = new char[80];	
				strncpy(lcp_data , W2A(ls_data), ls_data.GetLength());
 
				cLogFile.Write(lcp_data,ls_data.GetLength());  
				delete lcp_data;
			}
			cLogFile.Close();

		}
		return false;

 }

BOOL CTabpageEmapEvent::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if ( m_ToolTip != NULL)
		m_ToolTip ->RelayEvent(pMsg);
	return false;
}

