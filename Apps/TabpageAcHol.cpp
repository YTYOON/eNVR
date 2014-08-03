 // TabpageAcHol.cpp : implementation file
//

#include "stdafx.h"
#include "TabpageAcHol.h"
#include "DlgAcSearch.h"


// CTabpageAcHol dialog

IMPLEMENT_DYNAMIC(CTabpageAcHol, CBaseDialog)

CTabpageAcHol::CTabpageAcHol(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CTabpageAcHol::IDD, pParent)
{

}

CTabpageAcHol::~CTabpageAcHol()
{
		if (m_pToolTipCtrl != NULL) delete m_pToolTipCtrl ;
}

void CTabpageAcHol::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTADD, m_butadd);
	DDX_Control(pDX, IDC_BUTDEL, m_butdel);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_date);
	DDX_Control(pDX, IDC_CHECK1, m_butSamedate);
	DDX_Control(pDX, IDC_COMBOGATE, m_comGate);
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Control(pDX, IDC_BUTMOD, m_butmod);
	DDX_Control(pDX, IDC_LISTHOL, m_list);
}


BEGIN_MESSAGE_MAP(CTabpageAcHol, CBaseDialog)
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTHOL, &CTabpageAcHol::OnLvnItemchangedListhol)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CTabpageAcHol::OnDtnDatetimechangeDatetimepicker1)
	ON_BN_CLICKED(IDC_CKWEEKEND, &CTabpageAcHol::OnBnClickedCkweekend)
	ON_BN_CLICKED(IDC_BUTADD, &CTabpageAcHol::OnBnClickedButadd)
 
	ON_BN_CLICKED(IDC_BUTDEL, &CTabpageAcHol::OnBnClickedButdel)
	ON_CBN_SELCHANGE(IDC_COMBOSTART, &CTabpageAcHol::OnCbnSelchangeCombostart)
	ON_CBN_SELCHANGE(IDC_COMBOGATE, &CTabpageAcHol::OnCbnSelchangeCombogate)
//	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CTabpageAcHol::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_BUTMOD, &CTabpageAcHol::OnBnClickedButmod)
//	ON_NOTIFY(HDN_ITEMCHANGED, 0, &CTabpageAcHol::OnHdnItemchangedListhol)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTHOL, &CTabpageAcHol::OnLvnItemchangedListhol)
END_MESSAGE_MAP()


// CTabpageAcHol message handlers

BOOL CTabpageAcHol::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	mb_dirty = false;
	mb_datedirty = false;
	mb_eydirty = false;
	
	fnLocalize();
	fnInitCtrl();
	//fnBuildCamCombo();
 
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTabpageAcHol::fnInitCtrl(void)
{
	try{
 
		int	ll_posx = 220, ll_posy=30;
	
		m_butadd.LoadBitmaps(IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD);
		m_butadd.MoveWindow(ll_posx, ll_posy, 24,24);		
		ll_posx +=28;

		m_butmod.LoadBitmaps(IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT);
		m_butmod.MoveWindow(ll_posx, ll_posy, 24,24);		
		ll_posx +=28;
		
		m_butdel.LoadBitmaps(IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL);
		m_butdel.MoveWindow(ll_posx, ll_posy, 24,24);		

		//Weekend checkbox retrieve from ini 
	/*	m_ini.setINIFullFileName(_T("\ec.ini"));
		CString csweekend = m_ini.getKeyValue(_T("weekend_off"),_T("access control"));
		if (csweekend == _T("Y"))
			((CButton*)GetDlgItem(IDC_CKWEEKEND))->SetCheck(1);
		else
			((CButton*)GetDlgItem(IDC_CKWEEKEND))->SetCheck(0);*/


		m_date.EnableWindow(false);
		m_butSamedate.ShowWindow(false);
		m_list.SetExtendedStyle(LVS_SHOWSELALWAYS);
		fnPopulateHol();
		//fnRefreshList();

			//ToolTips
		m_pToolTipCtrl = new CToolTipCtrl;
		if (!m_pToolTipCtrl->Create(this))		
		TRACE(_T("Unable To create ToolTip\n"));
		CString ls_st;
		LOADSTRING(ls_st, IDS_ADD_HOLIDAY);
		m_pToolTipCtrl->AddTool(&m_butadd, ls_st);
		m_pToolTipCtrl->BringWindowToTop();
		
		LOADSTRING(ls_st, IDS_DEL_HOLIDAY);
		m_pToolTipCtrl->AddTool(&m_butdel, ls_st);
		m_pToolTipCtrl->BringWindowToTop();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CTabpageAcHol::fnSave(CString oMode)
{
	CString	ls_sql, ls_sqlAll;
	//CString	ls_sql, ls_sqlAll=_T("");

	for (int i=0; i<m_vpHol.size(); i++)
	{
		if (m_vpHol[i]->mode == 'A' ) //Add
		{			 
				ls_sql.Format(_T("INSERT INTO ecac_hholiday(date_)  VALUES('%s') "), m_vpHol[i]->date_ );					
				ls_sqlAll = ls_sqlAll + ls_sql;
				//psDB->ExecuteSQL(ls_sql);
		}
		else if (m_vpHol[i]->mode == 'M' ) //Modify
		{
			ls_sql.Format(_T("UPDATE ecac_hholiday SET date_='%s'  WHERE holidayid = %d ; "),m_vpHol[i]->date_,   m_vpHol[i]->holid );							
				ls_sqlAll = ls_sqlAll + ls_sql;
				//psDB->ExecuteSQL(ls_sql);
		}
		else if (m_vpHol[i]->mode == 'D' ) //Delete
		{
				ls_sql.Format(_T("DELETE from ecac_hholiday WHERE holidayid = %d ; "), m_vpHol[i]->holid );	
				ls_sqlAll = ls_sqlAll + ls_sql;
				//psDB->ExecuteSQL(ls_sql);
		}
	}//for loop

	TRACE(ls_sqlAll);
	psDB->ExecuteSQL(ls_sqlAll);

	m_vpHol.clear();

	if (oMode == _T("open"))
		fnDisplayList();

}

void CTabpageAcHol::fnPopulateHol(void)
{
		CString ls_name, ls_ip, ls_desc;
		TCHAR lc_desc[255];
		CDBVariant	varIdx;
		//CDBVariant	varID, varDate, varNull;
		CString ls_sql1, ls_sql2, ls_sql3, ls_sql,  ls_ey, ls_dt;
		int ci=0;

		LV_COLUMN ListColumn;
		WCHAR * ListTitles[2] = {_T("Date"), _T("")};

		for(int i = 0; i < 2; i++)
		{
				ListColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
				ListColumn.fmt = LVCFMT_LEFT;
				ListColumn.iSubItem = i;

				if(i == 0)
					{ListColumn.cx = 275;}
				else if ( i ==1)
					{ListColumn.cx = 0;}
	 			
				ListColumn.pszText = ListTitles[i];
				m_list.InsertColumn(i, &ListColumn);
		}

		DWORD	dwStyle = m_list.GetStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;
		m_list.SetExtendedStyle(dwStyle);
		m_list.ModifyStyle(0, LVS_SINGLESEL);

		
		fnDisplayList();	
		return ;
}

//To display holiday list
void CTabpageAcHol::fnDisplayList()
{
	m_list.DeleteAllItems();
	//Display holiday info
		CString	ls_date;
		CDBVariant	varHolId;
  		CRecordset m_rs(psDB);

	CString 	ls_sql = _T( "SELECT DISTINCT  holidayid,  date_  FROM  ecac_hholiday ORDER BY date_ DESC");
	 
		m_rs.Open(CRecordset::dynaset, ls_sql);
	 
		while (!m_rs.IsEOF()) { 

				STRHOL* m_strHol = new STRHOL;
				//::ZeroMemory(m_strHol, sizeof(STREMP));
				TVINSERTSTRUCTW itemStru={0};

				m_rs.GetFieldValue(_T("holidayid"), varHolId,  SQL_C_SSHORT );	
				int	iholid = varHolId.m_iVal;
				m_strHol->holid = varHolId.m_iVal;

				m_rs.GetFieldValue(_T("date_"), ls_date  );
				ls_date = ls_date.Left(10);
				wcscpy(m_strHol->date_, ls_date );

				m_list.InsertItem(0, (LPCTSTR) ls_date);
				CString ls_holid;
				ls_holid.Format(_T("%d"), varHolId.m_iVal);
				m_list.SetItemText(0, 1,  (LPCTSTR) ls_holid);

				m_strHol->mode = 'S';  //Already in DB
				m_strHol->holid =  _ttoi(ls_holid);
				wcscpy(  m_strHol->date_ , ls_date  );
				m_vpHol.push_back(m_strHol);

				m_rs.MoveNext();  		
		}
}

//void CTabpageAcHol::OnLvnItemchangedListhol(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
// 
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;	 
//	 
//	//Display Date
//	//STRHOL* lpHol = (STRHOL*)pNMListView->lParam;
//	mp_curHol = (STRHOL*)pNMListView->lParam;
//
//	CString	ls_date = mp_curHol->date_;
//	//CString	ls_date = lpHol->date_;
//// 	CString ls_date = m_listhol.GetItemText( pNMListView->iItem, 0);
//	int ll_first = ls_date.Find(_T("/"));
//	
//	CString ls_year = ls_date.Mid(0, ll_first);
//	int ll_year = _ttoi(ls_year);
//	int ll_second = ls_date.Find(_T("/"), ll_first+1);
//	CString	ls_mon = ls_date.Mid(ll_first + 1, ll_second - ll_first - 1);
//	int ll_mon = _ttoi(ls_mon);
//	CString	ls_day = ls_date.Mid(ll_second+ 1 );
//	int ll_day = _ttoi(ls_day);
//	CTime ltime(ll_year,ll_mon,ll_day,0,0,0);
//
//	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER1))->SetTime(&ltime);
//
//	m_curitem = pNMListView->iItem;	//Current Item
//	if (m_curitem>=0) 	m_date.EnableWindow(true);
//
//	//Every Year
// 	 CString ls_ey = m_listhol.GetItemText( pNMListView->iItem, 1);
//	if (ls_ey == _T("Y"))
//		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(1);
//	else
//		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(0);
//	*pResult = 0;
//}

int CTabpageAcHol::fnparse(CString oSource, CString oSeperator)
{
	return 0;
}

void CTabpageAcHol::OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
 
	mb_dirty = true;
	mb_datedirty = true;

	 CTime tmDate;	
    UpdateData();

    m_date.GetTime(tmDate);

	*pResult = 0;
}

void CTabpageAcHol::OnBnClickedCkweekend()
{
	mb_dirty = true;
	mb_weekenddirty = true;
}

//Click on Add button
void CTabpageAcHol::OnBnClickedButadd()
{
	 	CDlgAcSearch lpEditHol; 		
		lpEditHol.psDB = psDB;
		lpEditHol.m_parent = this;

		int ll_ret = lpEditHol.DoModal();

		if (ll_ret == IDOK)
		{
			CString ls_date;
			mb_dirty = true;
			ls_date = lpEditHol.m_hol;

			((CEdit*)GetDlgItem(IDC_EDITDATE))->SetWindowTextW(lpEditHol.m_hol );

			//Save new data into vector - for saving
			STRHOL*	pnewHol = new STRHOL;
			
			wcscpy(  pnewHol->date_ , ls_date  );
 
			pnewHol->mode = 'A';
			m_list.InsertItem(0, (LPCTSTR) ls_date);
			
			m_list.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED); //selected the new added item
			m_vpHol.push_back(pnewHol); 	
		}
}


void CTabpageAcHol::OnBnClickedButdel()
{
		mb_dirty = true;

		STRHOL* lpstrhol = new STRHOL;
	 
		lpstrhol->mode = 'D';
		CString ls_holid = m_list.GetItemText(m_list.GetSelectionMark(),1);
		lpstrhol->holid =  _ttoi(ls_holid);

		m_list.DeleteItem(m_list.GetSelectionMark());
		GetDlgItem(IDC_EDITDATE)->SetWindowTextW(_T(""));
		m_vpHol.push_back(lpstrhol);
}

 
BOOL CTabpageAcHol::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if ( m_pToolTipCtrl != NULL)
		m_pToolTipCtrl->RelayEvent(pMsg);
	return CBaseDialog::PreTranslateMessage(pMsg);
}

void CTabpageAcHol::OnCbnSelchangeCombostart()
{
	// TODO: Add your control notification handler code here
}

void CTabpageAcHol::OnCbnSelchangeCombogate()
{
		int i_idx = m_comGate.GetCurSel();
		int i_camidx = m_vtpcam[i_idx]->cam_idx;
		m_curcamidx = i_camidx;
		m_tree.DeleteAllItems();
		//fnBuildTree( i_camidx);
}

void CTabpageAcHol::OnBnClickedButmod()
{
		bool	lb_newmod = false;
		int		ll_cnt=-1;
		CString	ls_orgdate=_T("");
		((CEdit*)GetDlgItem(IDC_EDITDATE))->GetWindowTextW(ls_orgdate);
		if (ls_orgdate == _T("")) return;

		//check if modify newly added item
		for (int i=0; i<m_vpHol.size(); i++)
		{
			if (m_vpHol[i]->mode =='A' && m_vpHol[i]->date_==ls_orgdate)
			{
				ll_cnt = i;
				lb_newmod = true;
				break;
			}
		}

		CDlgAcSearch lpEditHol; 	
		lpEditHol.m_hol = ls_orgdate;
		lpEditHol.psDB = psDB;
		lpEditHol.m_parent = this;

		int ll_ret = lpEditHol.DoModal();

		if (ll_ret == IDOK)
		{
			CString ls_newdate;
			mb_dirty = true;
			ls_newdate = lpEditHol.m_hol;				

			((CEdit*)GetDlgItem(IDC_EDITDATE))->SetWindowTextW(lpEditHol.m_hol );

			if (lb_newmod)	//modify the newly add
			{
				wcscpy(  m_vpHol[ll_cnt]->date_  , ls_newdate  );
				m_list.SetItemText(m_curitem, 0, (LPCTSTR)ls_newdate);
			}
			else
			{
				for (int i=0; i<m_vpHol.size(); i++)
				{
					if (m_vpHol[i]->date_ == ls_orgdate)
					{
						wcscpy(  m_vpHol[i]->date_ , ls_newdate  );
						m_list.SetItemText( m_list.GetSelectionMark(),0, (LPCTSTR) ls_newdate);
						m_vpHol[i]->mode = 'M';
						break;
					}
				}
					////Save new data into vector - for saving
					//STRHOL*	pnewHol = new STRHOL;

					//CString ls_id = m_list.GetItemText( m_list.GetSelectionMark(), 1);
					//
					//pnewHol->holid =   _ttoi(ls_id);
					//wcscpy(  pnewHol->date_ , ls_newdate  );

					//m_list.SetItemText( m_list.GetSelectionMark(),0, (LPCTSTR) ls_newdate);
					//pnewHol->mode = 'M';	//Modify
					//m_vpHol.push_back(pnewHol);
			}
			//delete pnewHol;

		}
	 
}


void CTabpageAcHol::OnLvnItemchangedListhol(NMHDR *pNMHDR, LRESULT *pResult)
{
		LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;	 

		m_curDate = m_list.GetItemText( pNMListView->iItem, 0);
		m_curitem = pNMListView->iItem;
	//CString ls_date = m_list.GetItemText(m_list.GetSelectionMark(),0);
	//CString csSelectMac = pScanList->GetItemText(pScanList->GetSelectionMark(), 5);
		((CEdit*)GetDlgItem(IDC_EDITDATE))->SetWindowTextW(m_curDate);
		*pResult = 0;
}

void CTabpageAcHol::fnLocalize()
{
		CString ls_loc;
	
		LOADSTRING(ls_loc, IDS_AC_TAB_HOLIDAYS);
		GetDlgItem( IDC_STTITLEHOLIDAY )->SetWindowTextW(ls_loc);
		GetDlgItem( IDC_STGHOLIDAY )->SetWindowTextW(ls_loc);

		LOADSTRING(ls_loc,  IDS_AC_TAB_DATE);
		GetDlgItem( IDC_STDATE )->SetWindowTextW(ls_loc);
				
}