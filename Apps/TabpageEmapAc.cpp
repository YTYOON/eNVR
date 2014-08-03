// TabpageEmapAc.cpp : implementation file
//

#include "stdafx.h"
#include "TabpageEmapAc.h"
#include "DlgSettingAC.h"

// CTabpageEmapAc dialog

IMPLEMENT_DYNAMIC(CTabpageEmapAc, CBaseDialog)

CTabpageEmapAc::CTabpageEmapAc(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CTabpageEmapAc::IDD, pParent)
{

}

CTabpageEmapAc::~CTabpageEmapAc()
{
}

void CTabpageEmapAc::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTAC, m_listac);
}


BEGIN_MESSAGE_MAP(CTabpageEmapAc, CBaseDialog)
	ON_BN_CLICKED(IDC_BUTSET, &CTabpageEmapAc::OnBnClickedButset)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTREP, &CTabpageEmapAc::OnBnClickedButrep)
END_MESSAGE_MAP()


// CTabpageEmapAc message handlers

void CTabpageEmapAc::OnBnClickedButset()
{
	// TODO: Add your control notification handler code here
	CDlgSettingAC lDlg;
	lDlg.DoModal();
}

BOOL CTabpageEmapAc::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	//m_listac = (CListCtrl*)(GetDlgItem(IDC_LISTAC));
	// TODO:  Add extra initialization here
	int	ll_posx = 2, ll_posy=2;
	m_set.AutoLoad(IDC_BUTSET, this);
	m_set.MoveWindow( ll_posx, ll_posy, 24, 24, true); 

	ll_posx +=28;

	m_butrep.AutoLoad(IDC_BUTREP, this);
	m_butrep.MoveWindow(ll_posx, ll_posy, 24,24, true);

	CTabCtrl* lpTab = (CTabCtrl*)GetParent();
	CRect lrect;
	lpTab->GetClientRect(lrect);
	int ll_gap=28, ll_gapw=14, ll_gaph=58;
	m_listac.MoveWindow(0, ll_gap, lrect.Width()-ll_gapw, lrect.Height() - ll_gaph );

	LV_COLUMN ListColumn;
	WCHAR * ListTitles1[5] = {_T("Date"),_T("Time"), _T("ID"),_T("Department"),_T("Name")};
	for(int i = 0; i < 5; i++)
		{
			ListColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			ListColumn.fmt = LVCFMT_LEFT;
			ListColumn.iSubItem = i;

			if (i==0)  ListColumn.cx = 70;
			else if (i==1)  ListColumn.cx = 60; //Time
			else if (i==2)  ListColumn.cx = 30; //ID
			else if (i==3)  ListColumn.cx = 75; // Department
			else if (i==4)  ListColumn.cx = 130; //Name

			ListColumn.pszText = ListTitles1[i];
			m_listac.InsertColumn(i, &ListColumn);
		}

		DWORD dwStyle = m_listac.GetStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;
		m_listac.SetExtendedStyle(dwStyle);

		//Temp disable button 
		m_butrep.EnableWindow(true);
		m_set.EnableWindow(true);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CTabpageEmapAc::OnBnClickedButrep()
{
	// Run Report
	 TCHAR apppath[MAX_PATH]={0};
	GetModuleFileName(AfxGetInstanceHandle(),apppath,MAX_PATH-1);
	CString csAppPath(apppath);
	int nPos = csAppPath.ReverseFind('\\');
	csAppPath = csAppPath.Left(nPos + 1);
	CString csReport = csAppPath + _T("\\EcReport.exe");

	ShellExecuteW(NULL,_T(""),csReport,NULL,NULL,SW_SHOWNORMAL);
}

bool CTabpageEmapAc::fnInsertRow(CString oDate, CString oTime, CString oCamidx, CString oDept, CString oName)
{
	LVITEM lvi;
	lvi.mask = LVIF_IMAGE | LVIF_TEXT;
 
	TCHAR szItem[256];
	lvi.iItem = 0;
	lvi.iSubItem = 0;

	CString ldate;
	ldate.Format(_T("20%s-%s-%s"),oDate.Mid(0,2), oDate.Mid(2,2), oDate.Mid(4,2));
	_stprintf(szItem, _T("%s"), ldate);
	lvi.pszText =szItem;
	int nItem = m_listac.InsertItem(&lvi);

	lvi.iSubItem =1;		//Time
	CString csTime;
	csTime.Format(_T("%s:%s"), oTime.Mid(0,2), oTime.Mid(2,2));
	lvi.pszText = (LPTSTR)(LPCTSTR)(csTime);
	m_listac.SetItem(&lvi);

	lvi.iSubItem =2; //Camera Index
	lvi.pszText = (LPTSTR)(LPCTSTR)(oCamidx);
	m_listac.SetItem(&lvi);

	lvi.iSubItem =3; //Camera Index
	lvi.pszText = (LPTSTR)(LPCTSTR)(oDept);
	m_listac.SetItem(&lvi);

	lvi.iSubItem =4; //Camera Index
	lvi.pszText = (LPTSTR)(LPCTSTR)(oName);
	m_listac.SetItem(&lvi);

	//fnDisplay
	return false;
}


bool CTabpageEmapAc::fnDisplayAccess(CString oCamIdx, CString oCamIP, CString oEventType, CString o_curHMS)
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
		m_listac.InsertItem(0,_T("aa"));

 		// Set subitem 1
		strItem.Format(_T("%s"),o_curHMS );
		lvi.iSubItem = 1;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_listac.SetItem(&lvi);

		// Set subitem 2
		strItem.Format(_T("%s"), oCamIdx);
		lvi.iSubItem =2;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_listac.SetItem(&lvi);
		
		// Set subitem 3
		strItem.Format(_T("%s"),  oCamIP );
		lvi.iSubItem =3;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_listac.SetItem(&lvi);

		// Set subitem 4
		strItem.Format(_T("%s"),  oEventType );
		lvi.iSubItem =4;
		//CString ls_type = SmartEventType(strItem, true);
		lvi.pszText = (LPTSTR)(LPCTSTR)(oEventType);
		m_listac.SetItem(&lvi);

	return false;
}
