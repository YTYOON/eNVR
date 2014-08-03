// TabpageGatePlate.cpp : implementation file
//

#include "stdafx.h"
#include "TabpageGatePlate.h"


// CTabpageGatePlate dialog

IMPLEMENT_DYNAMIC(CTabpageGatePlate, CBaseDialog)

CTabpageGatePlate::CTabpageGatePlate(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CTabpageGatePlate::IDD, pParent)
{

}

CTabpageGatePlate::~CTabpageGatePlate()
{
}

void CTabpageGatePlate::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTADD, m_butadd);
	DDX_Control(pDX, IDC_BUTMOD, m_butmod);
	DDX_Control(pDX, IDC_BUTDEL, m_butdel);
	DDX_Control(pDX, IDC_TREE, m_tree);
}


BEGIN_MESSAGE_MAP(CTabpageGatePlate, CBaseDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &CTabpageGatePlate::OnTvnSelchangedTree)
 
	ON_BN_CLICKED(IDC_BUTADD, &CTabpageGatePlate::OnBnClickedButadd)
	ON_BN_CLICKED(IDC_BUTMOD, &CTabpageGatePlate::OnBnClickedButmod)
	ON_BN_CLICKED(IDC_BUTDEL, &CTabpageGatePlate::OnBnClickedButdel)
END_MESSAGE_MAP()


// CTabpageGatePlate message handlers

void CTabpageGatePlate::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
 	//In the middle of the add department
	TVITEM TreeItem;
	HTREEITEM  hItem = m_tree.GetSelectedItem();	
	HTREEITEM  hItemRoot = m_tree.GetRootItem();
	
	//if (m_tree.GetParentItem(hItem) == NULL) //Chain Items
	//{
//		NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
		TreeItem.hItem = hItem;
		TreeItem.mask = TVIF_HANDLE ;
		m_tree.GetItem(&TreeItem);
		STRPLATE* lpplate = (STRPLATE*) TreeItem.lParam;
 
		((CEdit*)GetDlgItem(IDC_EDITCARD))->SetWindowTextW( lpplate->cardcode );
		((CEdit*)GetDlgItem(IDC_EDITNAME))->SetWindowTextW( lpplate->name );
		((CEdit*)GetDlgItem(IDC_EDITPLATE1))->SetWindowTextW( lpplate->plate1 );
		((CEdit*)GetDlgItem(IDC_EDITPLATE2))->SetWindowTextW( lpplate->plate2 );
		((CEdit*)GetDlgItem(IDC_EDITPLATE3))->SetWindowTextW( lpplate->plate3 );
		m_cardid = lpplate->id;

	*pResult = 0;
}

BOOL CTabpageGatePlate::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	int lposx=25, lposy=15;
	//
	m_butadd.LoadBitmaps(IDB_CAMGROUP_ADD,IDB_CAMGROUP_ADD,IDB_CAMGROUP_ADD,IDB_CAMGROUP_ADD);
	m_butadd.MoveWindow(lposx, lposy,24,24);
	
	lposx +=28; 
	m_butmod.LoadBitmaps(IDB_CAMGROUP_EDIT,IDB_CAMGROUP_EDIT,IDB_CAMGROUP_EDIT,IDB_CAMGROUP_EDIT);
	m_butmod.MoveWindow(lposx, lposy,24,24);

	lposx +=28; 
	m_butdel.LoadBitmaps(IDB_CAMGROUP_DEL,IDB_CAMGROUP_DEL,IDB_CAMGROUP_DEL,IDB_CAMGROUP_DEL);
	m_butdel.MoveWindow(lposx, lposy,24,24);
	lposx +=38; 

	fnBuildTree();

		//Tooltips
		m_pToolTipCtrl = new CToolTipCtrl;
		if (!m_pToolTipCtrl->Create(this))		
			TRACE(_T("Unable To create ToolTip\n"));

		CString ls_tooltip;
 		LOADSTRING( ls_tooltip, IDS_GTSETPLATEADD);		
		m_pToolTipCtrl->AddTool(&m_butadd, ls_tooltip);

		LOADSTRING( ls_tooltip, IDS_GTSETPLATEMOD);		
		m_pToolTipCtrl->AddTool(&m_butmod, ls_tooltip);

		LOADSTRING( ls_tooltip, IDS_GTSETPLATEDEL);		
		m_pToolTipCtrl->AddTool(&m_butdel, ls_tooltip);

		fnLocalize();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTabpageGatePlate::fnLocalize()
{
		CString ls_loc;
		//card name LP
		LOADSTRING(ls_loc, IDS_STLP);
		GetDlgItem( IDC_STCNL )->SetWindowTextW(ls_loc);

		//card/name group box
		LOADSTRING(ls_loc, IDS_GBOXCARDNAME );
		GetDlgItem( IDC_GBOXCARDNAME )->SetWindowTextW(ls_loc);

		LOADSTRING(ls_loc, IDS_STCARD);
		GetDlgItem( IDC_STCARD )->SetWindowTextW(ls_loc);
		
		LOADSTRING(ls_loc, IDS_STNAME);
		GetDlgItem( IDC_STNAME )->SetWindowTextW(ls_loc);

		//license plate gbox
		LOADSTRING(ls_loc, IDS_LPLATE);
		GetDlgItem( IDC_STLP )->SetWindowTextW(ls_loc);
		

}

void CTabpageGatePlate::fnBuildTree()
{
	if (!psDB) return;	//Check DB Availabel

	CString ls_sql, ls_code, ls_desc, ls_name, ls_plate1, ls_plate2, ls_plate3;
 
	int	ll_cardid, ll_prechainid=-1;
	char cIp[16];
	bool lb_ret;	
	
	USES_CONVERSION;
	HTREEITEM lprvItem;
	 
	int ci=0, ll_groupid;
	CDBVariant	varCardid;
	HTREEITEM itemDefault;

  	CRecordset m_rs(psDB);
	
	ls_sql = _T( "SELECT DISTINCT ecgt_card.cardid, cardcode, name, lplate1, lplate2, lplate3 FROM ecgt_card  ORDER BY name");

	TRACE(ls_sql);
	m_rs.Open(CRecordset::dynaset, ls_sql);
 
	LPWSTR mynew;
	HTREEITEM ChainItem;

	while (!m_rs.IsEOF()) { 

		STRPLATE* m_strcard = new STRPLATE;
		::ZeroMemory(m_strcard, sizeof(STRPLATE));
		TVINSERTSTRUCTW itemStru={0};

		m_rs.GetFieldValue(_T("cardid"), varCardid,  SQL_C_SSHORT );
		m_strcard->id = varCardid.m_iVal;
		ll_cardid = m_strcard->id;
		
		m_rs.GetFieldValue(_T("cardcode"), ls_code );
		wcscpy(	m_strcard->cardcode , ls_code);

		m_rs.GetFieldValue(_T("name"), ls_name );
		wcscpy(m_strcard->name , ls_name);

		m_rs.GetFieldValue(_T("lplate1"), ls_plate1 );
		wcscpy(m_strcard->plate1 , ls_plate1);

 		m_rs.GetFieldValue(_T("lplate2"), ls_plate2 );
		wcscpy(m_strcard->plate2 , ls_plate2);

		m_rs.GetFieldValue(_T("lplate3"), ls_plate3 );
		wcscpy(m_strcard->plate3 , ls_plate3);

		itemStru.hParent = TVGN_ROOT;
		itemStru.item.lParam =  (LPARAM)m_strcard; //Save groupid into lparam
		itemStru.item.mask = TVIF_PARAM | TVIF_TEXT;
	 
		itemStru.item.pszText = (LPWSTR)(LPCTSTR)ls_name;
		itemStru.item.cchTextMax =ls_name.GetLength();

		m_vtpPlate.push_back(m_strcard);
 		ls_desc =  ls_name +  _T(" - ") + ls_code ;

		ChainItem = m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)ls_desc, 0,0,0,0, (LPARAM)m_strcard, NULL, TVI_LAST);

		if (ls_plate1 != _T(""))
			m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)ls_plate1, 0,0,0,0, (LPARAM)m_strcard, ChainItem, TVI_LAST);

		if (ls_plate2 != _T(""))
			m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)ls_plate2, 0,0,0,0, (LPARAM)m_strcard, ChainItem, TVI_LAST);

		if (ls_plate3 != _T(""))
			m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)ls_plate3, 0,0,0,0, (LPARAM)m_strcard, ChainItem, TVI_LAST);
 
			m_rs.MoveNext();  		
 	}
	m_rs.Close();
}


void CTabpageGatePlate::OnBnClickedButadd()
{
	CDlgPlate	lPlateEdit;

	//Make sure selec the chain item
//	HTREEITEM	hItem = m_tree.GetSelectedItem();
//	if (!hItem) return;

	lPlateEdit.mc_mode = 'A'; // Modified
	
	if ( lPlateEdit.DoModal()== IDOK)
	{
		//Insert the new department into tree
		STRPLATE*	pnewPlate = new STRPLATE;
		pnewPlate->cFlag = 'A';		//Modify
		
		//Passed data from edit screen
		m_card = lPlateEdit.m_card;
		m_name = lPlateEdit.m_name;
		m_plate1 = lPlateEdit.m_plate1;
		m_plate2 = lPlateEdit.m_plate2;
		m_plate3 = lPlateEdit.m_plate3;

		//To Structure
		pnewPlate->id = m_cardid;
		wcscpy(pnewPlate->cardcode , m_card);
		wcscpy(pnewPlate->name , m_name);
		wcscpy(pnewPlate->plate1 , m_plate1);
		wcscpy(pnewPlate->plate2 , m_plate2);
		wcscpy(pnewPlate->plate3 , m_plate3);

		//Modify Chain edit text
		((CEdit*)GetDlgItem(IDC_EDITCARD))->SetWindowTextW(m_card);
		((CEdit*)GetDlgItem(IDC_EDITNAME))->SetWindowTextW(m_name);
		((CEdit*)GetDlgItem(IDC_EDITPLATE1))->SetWindowTextW(m_plate1);
		((CEdit*)GetDlgItem(IDC_EDITPLATE2))->SetWindowTextW(m_plate2);
		((CEdit*)GetDlgItem(IDC_EDITPLATE3))->SetWindowTextW(m_plate3);
 
		m_vpModPlate.push_back(pnewPlate);

		//insert new item & child into tree
		CString ls_desc = m_name + _T(" - ") + m_card;
 		HTREEITEM hnewItem = m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)ls_desc, 0,0,0,0, (LPARAM)pnewPlate, NULL, TVI_LAST);

		if (m_plate1 != _T("")) 
			m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)m_plate1, 0,0,0,0, (LPARAM)pnewPlate, hnewItem, TVI_LAST);
		if (m_plate2 != _T("")) 
			m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)m_plate2, 0,0,0,0, (LPARAM)pnewPlate, hnewItem, TVI_LAST);
		if (m_plate3 != _T("")) 
			m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)m_plate3, 0,0,0,0, (LPARAM)pnewPlate, hnewItem, TVI_LAST);

		m_tree.SelectItem(hnewItem);
	}
}

void CTabpageGatePlate::OnBnClickedButmod()
{
	CDlgPlate	lPlateEdit;

	//Make sure selec the chain item
	HTREEITEM	hItem = m_tree.GetSelectedItem();
	if (!hItem) return;
	if (m_tree.GetParentItem(hItem) != NULL) //Not Department Items
			return;

	lPlateEdit.mc_mode = 'M'; // Modified
	
	((CEdit*)GetDlgItem(IDC_EDITCARD))->GetWindowTextW(m_card);
	lPlateEdit.m_card = m_card;
	((CEdit*)GetDlgItem(IDC_EDITNAME))->GetWindowTextW(m_name);
	lPlateEdit.m_name = m_name;
	((CEdit*)GetDlgItem(IDC_EDITPLATE1))->GetWindowTextW(m_plate1);
	lPlateEdit.m_plate1 = m_plate1;
	((CEdit*)GetDlgItem(IDC_EDITPLATE2))->GetWindowTextW(m_plate2);
	lPlateEdit.m_plate2 = m_plate2;
	((CEdit*)GetDlgItem(IDC_EDITPLATE3))->GetWindowTextW(m_plate3);
	lPlateEdit.m_plate3 = m_plate3;

 
	if ( lPlateEdit.DoModal()== IDOK)
	{
		//Insert the new department into tree
		STRPLATE*	pnewPlate = new STRPLATE;
		pnewPlate->cFlag = 'M';		//Modify
		
			//Passed data from edit screen
		m_card = lPlateEdit.m_card;
		m_name = lPlateEdit.m_name;
		m_plate1 = lPlateEdit.m_plate1;
		m_plate2 = lPlateEdit.m_plate2;
		m_plate3 = lPlateEdit.m_plate3;
 
	//To Structure
		pnewPlate->id = m_cardid;
		wcscpy(pnewPlate->cardcode , m_card);
		wcscpy(pnewPlate->name , m_name);
		wcscpy(pnewPlate->plate1 , m_plate1);
		wcscpy(pnewPlate->plate2 , m_plate2);
		wcscpy(pnewPlate->plate3 , m_plate3);
 	
		//Modify Back edit text
		((CEdit*)GetDlgItem(IDC_EDITCARD))->SetWindowTextW(m_card);
		((CEdit*)GetDlgItem(IDC_EDITNAME))->SetWindowTextW(m_name);
		((CEdit*)GetDlgItem(IDC_EDITPLATE1))->SetWindowTextW(m_plate1);
		((CEdit*)GetDlgItem(IDC_EDITPLATE2))->SetWindowTextW(m_plate2);
		((CEdit*)GetDlgItem(IDC_EDITPLATE3))->SetWindowTextW(m_plate3);
 
		//
		m_vpModPlate.push_back(pnewPlate);

		m_tree.SetItemText(hItem, m_name + _T(" - ") + m_card );
		// Delete all of the children of hmyItem.
		if (m_tree.ItemHasChildren(hItem))   
		{
				HTREEITEM hNextItem;
				HTREEITEM hChildItem = m_tree.GetChildItem(hItem);

				while (hChildItem != NULL)
				{
					hNextItem = m_tree.GetNextItem(hChildItem, TVGN_NEXT);
					m_tree.DeleteItem(hChildItem);
					hChildItem = hNextItem;
				}
		}
		if (m_plate1 != _T("")) 
			m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)m_plate1, 0,0,0,0, (LPARAM)pnewPlate, hItem, TVI_LAST);
		if (m_plate2 != _T("")) 
			m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)m_plate2, 0,0,0,0, (LPARAM)pnewPlate, hItem, TVI_LAST);
		if (m_plate3 != _T("")) 
			m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)m_plate3, 0,0,0,0, (LPARAM)pnewPlate, hItem, TVI_LAST);

	}
}

void CTabpageGatePlate::OnBnClickedButdel()
{
 	HTREEITEM	hItem = m_tree.GetSelectedItem();
	if (!hItem) return;
	if (m_tree.GetParentItem(hItem) == NULL) //Make sure it's Main Item
	{
				//Save tje delete flag into vector, fn_save will handle later 
				USES_CONVERSION;

 				STRPLATE*	pnewPlate = new STRPLATE;
				pnewPlate->cFlag = 'D';		//Delete
				pnewPlate->id = m_cardid;
					
				m_tree.DeleteItem(hItem);
				m_vpModPlate.push_back(pnewPlate); 
		}
}


void CTabpageGatePlate::fnSave(bool obClose)
{
	//Save new Chains
	CString ls_sql;
	for (int i=0; i<m_vpModPlate.size(); i++)
	{
		// Insert new chains
		if (m_vpModPlate[i]->cFlag == 'A')
		{
				CString ls_bsql;
				ls_bsql.Format(_T(" INSERT INTO ecgt_card(cardcode, name, lplate1, lplate2, lplate3) VALUES('%s', '%s', '%s', '%s', '%s' ); "), m_vpModPlate[i]->cardcode, m_vpModPlate[i]->name, m_vpModPlate[i]->plate1, m_vpModPlate[i]->plate2, m_vpModPlate[i]->plate3);
				ls_sql += ls_bsql;
		}
		else if (m_vpModPlate[i]->cFlag == 'M') //Modify Chain
		{
			if (m_vpModPlate[i]->id < 0) 
				continue;
				
			CString ls_bsql;
			ls_bsql.Format(_T(" UPDATE ecgt_card SET cardcode = '%s', name='%s', lplate1='%s', lplate2='%s', lplate3='%s'  WHERE cardid=%d; "), m_vpModPlate[i]->cardcode, m_vpModPlate[i]->name, m_vpModPlate[i]->plate1, m_vpModPlate[i]->plate2, m_vpModPlate[i]->plate3, m_vpModPlate[i]->id);
			ls_sql += ls_bsql;
		}
		else if (m_vpModPlate[i]->cFlag == 'D') //Delete Plate
		{
			if (m_vpModPlate[i]->id < 0) 
				continue;
				
			CString ls_bsql;
			ls_bsql.Format(_T(" DELETE FROM ecgt_card WHERE cardid=%d; "),  m_vpModPlate[i]->id);
			ls_sql += ls_bsql;
		}
	} //for loop

		TRACE(ls_sql);
		psDB->ExecuteSQL(ls_sql);
		m_vpModPlate.clear();

		if (!obClose)
		{	
			m_tree.DeleteAllItems();
			fnBuildTree();
		}
}
BOOL CTabpageGatePlate::PreTranslateMessage(MSG* pMsg)
{
		if ( m_pToolTipCtrl != NULL)
			m_pToolTipCtrl->RelayEvent(pMsg);

		return CBaseDialog::PreTranslateMessage(pMsg);
}
