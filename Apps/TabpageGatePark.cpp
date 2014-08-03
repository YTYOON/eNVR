// TabpageGatePark.cpp : implementation file
//
#include "stdafx.h"
#include "TabpageGatePark.h"


// CTabpageGatePark dialog

IMPLEMENT_DYNAMIC(CTabpageGatePark, CBaseDialog)

CTabpageGatePark::CTabpageGatePark(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CTabpageGatePark::IDD, pParent)
{
}

CTabpageGatePark::~CTabpageGatePark()
{
}

void CTabpageGatePark::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, m_tree);
	DDX_Control(pDX, IDC_BUTCHAINADD, m_butchainadd);
	DDX_Control(pDX, IDC_BUTCHAINMOD, m_butchainmod);
	DDX_Control(pDX, IDC_BUTCHAINDEL, m_butchaindel);
	DDX_Control(pDX, IDC_BUTPARKADD, m_butparkadd);
	DDX_Control(pDX, IDC_BUTPARKMOD, m_butparkmod);
	DDX_Control(pDX, IDC_BUTPARKDEL, m_butparkdel);
}


BEGIN_MESSAGE_MAP(CTabpageGatePark, CBaseDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &CTabpageGatePark::OnTvnSelChangedTree)
	ON_BN_CLICKED(IDC_BUTCHAINADD, &CTabpageGatePark::OnBnClickedButchainadd)
	ON_BN_CLICKED(IDC_BUTCHAINMOD, &CTabpageGatePark::OnBnClickedButchainmod)
	ON_BN_CLICKED(IDC_BUTCHAINDEL, &CTabpageGatePark::OnBnClickedButchaindel)
	ON_BN_CLICKED(IDC_BUTPARKADD, &CTabpageGatePark::OnBnClickedButparkadd)
	ON_BN_CLICKED(IDC_BUTPARKMOD, &CTabpageGatePark::OnBnClickedButparkmod)
	ON_BN_CLICKED(IDC_BUTPARKDEL, &CTabpageGatePark::OnBnClickedButparkdel)
END_MESSAGE_MAP()


// CTabpageGatePark message handlers

void CTabpageGatePark::OnTvnSelChangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	//In the middle of the add department
	TVITEM TreeItem;
	HTREEITEM  hItem = m_tree.GetSelectedItem();	
	HTREEITEM  hItemRoot = m_tree.GetRootItem();
	
	if (m_tree.GetParentItem(hItem) == NULL) //Chain Items
	{
		NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
		TreeItem.hItem = hItem;
		TreeItem.mask = TVIF_HANDLE ;
		m_tree.GetItem(&TreeItem);
		STRCHAINPARK* lpchain = (STRCHAINPARK*) TreeItem.lParam;
 
		((CEdit*)GetDlgItem(IDC_EDITCHAIN))->SetWindowTextW( lpchain->chaindesc );
		((CEdit*)GetDlgItem(IDC_EDITPARK))->SetWindowTextW( _T("") );
		((CEdit*)GetDlgItem(IDC_EDITGROUP))->SetWindowTextW( _T("") );
		((CEdit*)GetDlgItem(IDC_EDITMAP))->SetWindowTextW( _T("") );
		m_chainid = lpchain->chainid;
	}
	else	//Parking Item
	{
		NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
		TreeItem.hItem = hItem;
		TreeItem.mask = TVIF_HANDLE ;
		m_tree.GetItem(&TreeItem);

		//Set selected item into edit controls
		STRCHAINPARK* lpchain = (STRCHAINPARK*) TreeItem.lParam;
		((CEdit*)GetDlgItem(IDC_EDITCHAIN))->SetWindowTextW( lpchain->chaindesc );
		((CEdit*)GetDlgItem(IDC_EDITPARK))->SetWindowTextW( lpchain->parklotdesc );
		((CEdit*)GetDlgItem(IDC_EDITGROUP))->SetWindowTextW( lpchain->groupdesc );
		((CEdit*)GetDlgItem(IDC_EDITMAP))->SetWindowTextW( lpchain->mapfile );

		m_chainid = lpchain->chainid;
		m_parklotid = lpchain->parklotid;
		m_groupid = lpchain->groupid;
 	}

	*pResult = 0;
}

void CTabpageGatePark::fnBuildTree()
{
	if (!psDB) return;	//Check DB Availabel

	CString ls_sql, ls_sql1, ls_sql2, ls_desc, ls_acname, ls_chain, ls_idx, ls_park, ls_group;
	CString csGroupDesc, ls_predept=_T(""), ls_masterid, ls_active, ls_deptCode, ls_wfrom, ls_wto, ls_map;
	int	ll_chainid, ll_prechainid=-1;
	char cIp[16];
	bool lb_ret;	
	
	USES_CONVERSION;
	HTREEITEM lprvItem;
	 
	int ci=0, ll_groupid;
	CDBVariant	varChainid, varParklotid, varGroupid;	
	HTREEITEM itemDefault;

  	CRecordset m_rs(psDB);
	
	ls_sql = _T( "SELECT DISTINCT ecgt_chain.chainid, ecgt_chainparklot.parklotid, ecgt_chainparklot.chainid, ecgt_chainparklot.parklotid, ecgt_chain.chain_desc, ecgt_parklot .parklot_desc, ecgt_chainparklot.cameragroupid, ec_camera_group.camera_group_desc, ecgt_chainparklot.gatemap_file FROM ecgt_chain LEFT OUTER JOIN  ecgt_chainparklot");
	ls_sql1 =  _T( " ON ecgt_chain.chainid = ecgt_chainparklot.chainid  LEFT OUTER JOIN ecgt_parklot ON ecgt_parklot.parklotid = ecgt_chainparklot.parklotid  ");		
	ls_sql2 = _T("  LEFT OUTER JOIN ec_camera_group ON ec_camera_group.cameragroupid = ecgt_chainparklot.cameragroupid  ORDER BY ecgt_chain.chainid, ecgt_chainparklot.parklotid");
	ls_sql = ls_sql + ls_sql1 + ls_sql2;

	TRACE(ls_sql);
	m_rs.Open(CRecordset::dynaset, ls_sql);
 
	LPWSTR mynew;
	HTREEITEM ChainItem;

	while (!m_rs.IsEOF()) { 

		STRCHAINPARK* m_strchain = new STRCHAINPARK;
		//::ZeroMemory(m_strchain, sizeof(STRCHAINPARK));
		TVINSERTSTRUCTW itemStru={0};

		m_rs.GetFieldValue(_T("chainid"), varChainid,  SQL_C_SSHORT );
		m_strchain->chainid = varChainid.m_iVal;
		ll_chainid = m_strchain->chainid;

		m_rs.GetFieldValue(_T("parklotid"), varParklotid,  SQL_C_SSHORT );
		m_strchain->parklotid = varParklotid.m_iVal;
		
		m_rs.GetFieldValue(_T("chain_desc"), ls_chain );
		wcscpy(	m_strchain->chaindesc , ls_chain);

		m_rs.GetFieldValue(_T("parklot_desc"), ls_park );
		wcscpy(m_strchain->parklotdesc , ls_park);

		m_rs.GetFieldValue(_T("cameragroupid"), varGroupid,  SQL_C_SSHORT );
		m_strchain->groupid = varGroupid.m_iVal;

		m_rs.GetFieldValue(_T("camera_group_desc"), ls_group );
		wcscpy(m_strchain->groupdesc , ls_group);

		m_rs.GetFieldValue(_T("gatemap_file"), ls_map );
		wcscpy(m_strchain->mapfile , ls_map);
	 
		itemStru.hParent = TVGN_ROOT;
		itemStru.item.lParam =  (LPARAM)m_strchain; //Save groupid into lparam
		itemStru.item.mask = TVIF_PARAM | TVIF_TEXT;
	 
		itemStru.item.pszText = (LPWSTR)(LPCTSTR)ls_chain;
		itemStru.item.cchTextMax =ls_desc.GetLength();

		m_vpChain.push_back(m_strchain);
 
		if (ll_prechainid == ll_chainid) //Same Chainid
		{
			 m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)ls_park, 0,0,0,0, (LPARAM)m_strchain, ChainItem, TVI_LAST);
		}
		else 
		{
			ChainItem = m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)ls_chain, 0,0,0,0, (LPARAM)m_strchain, NULL, TVI_LAST);
			if (ls_park !=_T(""))
					m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)ls_park, 0,0,0,0, (LPARAM)m_strchain, ChainItem, TVI_LAST);
		}
	 
			ll_prechainid = ll_chainid;
			m_rs.MoveNext();  		
 	}
	m_rs.Close();
}


//For Access Control tree
void CTabpageGatePark::fnBuildTreeAC()
{
	if (!psDB) return;	//Check DB Availabel

	CString ls_sql, ls_sql1, ls_sql2, ls_desc, ls_acname, ls_chain, ls_idx, ls_park, ls_group;
	CString csGroupDesc, ls_predept=_T(""), ls_masterid, ls_active, ls_deptCode, ls_wfrom, ls_wto, ls_map;
	int	ll_chainid, ll_prechainid=-1;
	char cIp[16];
	bool lb_ret;	
	
	USES_CONVERSION;
	HTREEITEM lprvItem;
	 
	int ci=0, ll_groupid;
	CDBVariant	varChainid, varParklotid, varGroupid;	
	HTREEITEM itemDefault;

  	CRecordset m_rs(psDB);
	
	ls_sql = _T( "SELECT DISTINCT ecac_chain.chainid, ecac_chainac.acid, ecac_chainac.chainid, ecac_chainac.acid, ecac_chain.chain_desc, ecac_ac .ac_desc, ecac_chainac.cameragroupid, ec_camera_group.camera_group_desc, ecac_chainac.gatemap_file FROM ecac_chain LEFT OUTER JOIN  ecac_chainac");
	ls_sql1 =  _T( " ON ecac_chain.chainid = ecac_chainac.chainid  LEFT OUTER JOIN ecac_ac ON ecac_ac.acid = ecac_chainac.acid  ");		
	ls_sql2 = _T("  LEFT OUTER JOIN ec_camera_group ON ec_camera_group.cameragroupid = ecac_chainac.cameragroupid  ORDER BY ecac_chain.chainid, ecac_chainac.acid");
	ls_sql = ls_sql + ls_sql1 + ls_sql2;

	TRACE(ls_sql);
	m_rs.Open(CRecordset::dynaset, ls_sql);
 
	LPWSTR mynew;
	HTREEITEM ChainItem;

	while (!m_rs.IsEOF()) { 

		STRCHAINPARK* m_strchain = new STRCHAINPARK;
		//::ZeroMemory(m_strchain, sizeof(STRCHAINPARK));
		TVINSERTSTRUCTW itemStru={0};

		m_rs.GetFieldValue(_T("chainid"), varChainid,  SQL_C_SSHORT );
		m_strchain->chainid = varChainid.m_iVal;
		ll_chainid = m_strchain->chainid;

		m_rs.GetFieldValue(_T("acid"), varParklotid,  SQL_C_SSHORT );
		m_strchain->parklotid = varParklotid.m_iVal;
		
		m_rs.GetFieldValue(_T("chain_desc"), ls_chain );
		wcscpy(	m_strchain->chaindesc , ls_chain);

		m_rs.GetFieldValue(_T("ac_desc"), ls_park );
		wcscpy(m_strchain->parklotdesc , ls_park);

		m_rs.GetFieldValue(_T("cameragroupid"), varGroupid,  SQL_C_SSHORT );
		m_strchain->groupid = varGroupid.m_iVal;

		m_rs.GetFieldValue(_T("camera_group_desc"), ls_group );
		wcscpy(m_strchain->groupdesc , ls_group);

		m_rs.GetFieldValue(_T("gatemap_file"), ls_map );
		wcscpy(m_strchain->mapfile , ls_map);
	 
		itemStru.hParent = TVGN_ROOT;
		itemStru.item.lParam =  (LPARAM)m_strchain; //Save groupid into lparam
		itemStru.item.mask = TVIF_PARAM | TVIF_TEXT;
	 
		itemStru.item.pszText = (LPWSTR)(LPCTSTR)ls_chain;
		itemStru.item.cchTextMax =ls_desc.GetLength();

		m_vpChain.push_back(m_strchain);
 
		if (ll_prechainid == ll_chainid) //Same Chainid
		{
			 m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)ls_park, 0,0,0,0, (LPARAM)m_strchain, ChainItem, TVI_LAST);
		}
		else 
		{
			ChainItem = m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)ls_chain, 0,0,0,0, (LPARAM)m_strchain, NULL, TVI_LAST);
			if (ls_park !=_T(""))
					m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)ls_park, 0,0,0,0, (LPARAM)m_strchain, ChainItem, TVI_LAST);
		}
	 
			ll_prechainid = ll_chainid;
			m_rs.MoveNext();  		
 	}
	m_rs.Close();
}



BOOL CTabpageGatePark::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
		int lposx=25, lposy=15;
	
	m_butchainadd.LoadBitmaps(IDB_CHAINADD,IDB_CHAINADD,IDB_CHAINADD,IDB_CHAINADD);
	m_butchainadd.MoveWindow(lposx, lposy,24,24);
	lposx +=28; 
	m_butchainmod.LoadBitmaps(IDB_CHAINMOD,IDB_CHAINMOD,IDB_CHAINMOD,IDB_CHAINMOD);
 
	m_butchainmod.MoveWindow(lposx, lposy,24,24);
	lposx +=28; 
	m_butchaindel.LoadBitmaps(IDB_CHAINDEL,IDB_CHAINDEL,IDB_CHAINDEL,IDB_CHAINDEL);
	m_butchaindel.MoveWindow(lposx, lposy,24,24);
	lposx +=38; 

if (mc_Pmode=='A')
		m_butparkadd.LoadBitmaps(IDB_AC24ADD,IDB_AC24ADD,IDB_AC24ADD,IDB_AC24ADD);
	else
		m_butparkadd.LoadBitmaps(IDB_PARKADD,IDB_PARKADD,IDB_PARKADD,IDB_PARKADD);
	m_butparkadd.MoveWindow(lposx, lposy,24,24);

	lposx +=28; 

	if (mc_Pmode=='A')
		m_butparkmod.LoadBitmaps(IDB_AC24MOD,IDB_AC24MOD,IDB_AC24MOD,IDB_AC24MOD);
	else	
		m_butparkmod.LoadBitmaps(IDB_PARKMOD,IDB_PARKMOD,IDB_PARKMOD,IDB_PARKMOD);
	m_butparkmod.MoveWindow(lposx, lposy,24,24);

	lposx +=28; 

	if (mc_Pmode=='A')
	{
		m_butparkdel.LoadBitmaps(IDB_AC24DEL,IDB_AC24DEL,IDB_AC24DEL,IDB_AC24DEL);
		fnBuildTreeAC();	
	}
	else
	{
		m_butparkdel.LoadBitmaps(IDB_PARKDEL,IDB_PARKDEL,IDB_PARKDEL,IDB_PARKDEL);
		fnBuildTree();	
	}
	m_butparkdel.MoveWindow(lposx, lposy,24,24);
 
	fnLocalize();

		//Tooltips
		CString ls_tooltip;

		m_pToolTipCtrl = new CToolTipCtrl;
		if (!m_pToolTipCtrl->Create(this))		
			TRACE(_T("Unable To create ToolTip\n"));



		if (mc_Pmode=='A')		//For Access Control
		{
			//Chain
			LOADSTRING( ls_tooltip, IDS_ACSETCHAINADD);
			m_pToolTipCtrl->AddTool(&m_butchainadd, ls_tooltip);

			LOADSTRING( ls_tooltip, IDS_ACSETCHAINMOD);		
			m_pToolTipCtrl->AddTool(&m_butchainmod, ls_tooltip);	

			LOADSTRING( ls_tooltip, IDS_ACSETCHAINDEL);
			m_pToolTipCtrl->AddTool(&m_butchaindel, ls_tooltip);		

			//Parking Gate
			LOADSTRING( ls_tooltip, IDS_ACSETGATEADD);
			m_pToolTipCtrl->AddTool(&m_butparkadd, ls_tooltip);

			LOADSTRING( ls_tooltip, IDS_ACSETGATEMOD);		
			m_pToolTipCtrl->AddTool(&m_butparkmod, ls_tooltip);	

			LOADSTRING( ls_tooltip, IDS_ACSETGATEDEL);
			m_pToolTipCtrl->AddTool(&m_butparkdel, ls_tooltip);		
		}
		else if (mc_Pmode=='G')	//For Gate Control
		{
			//Chain
 			LOADSTRING( ls_tooltip, IDS_GTSETCHAINADD);		
			m_pToolTipCtrl->AddTool(&m_butchainadd, ls_tooltip);

			LOADSTRING( ls_tooltip, IDS_GTSETCHAINMOD);		
			m_pToolTipCtrl->AddTool(&m_butchainmod, ls_tooltip);

			LOADSTRING( ls_tooltip, IDS_GTSETCHAINDEL);		
			m_pToolTipCtrl->AddTool(&m_butchaindel, ls_tooltip);			
		
			//Parking Gate
			LOADSTRING( ls_tooltip, IDS_GTSETGATEADD);		
			m_pToolTipCtrl->AddTool(&m_butparkadd, ls_tooltip);	

			LOADSTRING( ls_tooltip, IDS_GTSETGATEMOD);
			m_pToolTipCtrl->AddTool(&m_butparkmod, ls_tooltip);

			LOADSTRING( ls_tooltip, IDS_GTSETGATEDEL);		
			m_pToolTipCtrl->AddTool(&m_butparkdel, ls_tooltip);		
		}

	fnLocalize();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//Add New Chain
void CTabpageGatePark::OnBnClickedButchainadd()
{
 	USES_CONVERSION;	 
 
	CDlgChain	lChainEdit;
	lChainEdit.m_mode = _T("CA");
	lChainEdit.mc_Pmode = mc_Pmode;
	if ( lChainEdit.DoModal()== IDOK)
	{
 		//Insert the new department into tree
		STRCHAINPARK*	pnewChain = new STRCHAINPARK;

		pnewChain->cFlag = 'A';
		CString ls_newChain = lChainEdit.m_chain;
		wcscpy(pnewChain->chaindesc , ls_newChain);
		
		int ll_newid;
		if (mc_Pmode=='A')
		{
			if (m_newchainid>0) 
				ll_newid = m_newchainid;
			else
				ll_newid = fnGetIdent(_T("chainid"), _T("ecac_chain"));
		}
		else
		{
				if (m_newchainid>0) 
					ll_newid = m_newchainid;
				else					
					ll_newid = fnGetIdent(_T("chainid"), _T("ecgt_chain"));
		}
		m_newchainid = ll_newid + 1;		
		pnewChain->chainid = ll_newid+1;
 
		HTREEITEM hitem = m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, ls_newChain, 0,0,0,0, (LPARAM)pnewChain, NULL, TVI_ROOT);
	
		//Modify Chain edit text
		((CEdit*)GetDlgItem(IDC_EDITCHAIN))->SetWindowTextW(ls_newChain);
		m_tree.SelectItem(hitem);

		m_vpModChain.push_back(pnewChain);
	}
}

//Modify Chain
void CTabpageGatePark::OnBnClickedButchainmod()
{
	CDlgChain	lChainEdit;

	//Make sure selec the chain item
	HTREEITEM	hItem = m_tree.GetSelectedItem();
	if (!hItem) return;
	if (m_tree.GetParentItem(hItem) != NULL) //Not Department Items
			return;

	lChainEdit.m_mode = _T("CM"); //Chain Modified
	lChainEdit.mc_Pmode = mc_Pmode;
	CString ls_chain;
	((CEdit*)GetDlgItem(IDC_EDITCHAIN))->GetWindowTextW(ls_chain);
	lChainEdit.m_chain = ls_chain;
 
	if ( lChainEdit.DoModal()== IDOK)
	{
		//Insert the new department into tree
		STRCHAINPARK*	pnewChain = new STRCHAINPARK;
		pnewChain->cFlag = 'M';		//Modify
		CString ls_newChain = lChainEdit.m_chain;
		wcscpy(pnewChain->chaindesc , ls_newChain);
		pnewChain->chainid = m_chainid;
		m_tree.SetItemText(m_tree.GetSelectedItem(),ls_newChain.Trim());
 	
		//Modify Chain edit text
		((CEdit*)GetDlgItem(IDC_EDITCHAIN))->SetWindowTextW(ls_newChain);
 
		m_vpModChain.push_back(pnewChain);
	}
}

void CTabpageGatePark::OnBnClickedButchaindel()
{
	HTREEITEM	hItem = m_tree.GetSelectedItem();
	if (!hItem) return;
	if (m_tree.GetParentItem(hItem) == NULL) //Make sure it's chain Item
	{
		if (AfxMessageBox(_T("Delete chain will also delete all the related parking gate, ok to process?"), MB_OKCANCEL, MB_ICONQUESTION) == IDOK)
		{		
				//Save tje delete flag into vector, fn_save will handle later 
				USES_CONVERSION;

 				STRCHAINPARK*	pnewChain = new STRCHAINPARK;
				pnewChain->cFlag = 'D';		//Delete
				pnewChain->chainid = m_chainid;
					
				m_tree.DeleteItem(hItem);
				m_vpModChain.push_back(pnewChain); 
		}
	}
}

//Modify Parking Lot
void CTabpageGatePark::OnBnClickedButparkadd()
{
		USES_CONVERSION;	 
	 
		CDlgChain	lChainEdit;
		lChainEdit.m_mode = _T("PA");
		lChainEdit.mc_Pmode = mc_Pmode;
		lChainEdit.pDB = psDB;

		//Make sure selec the park item
		HTREEITEM	hItem = m_tree.GetSelectedItem();
		if (!hItem) return;
		HTREEITEM	hpItem;
		
		if (m_tree.GetParentItem(hItem) == NULL)
			hpItem = hItem;
		else
			hpItem = m_tree.GetParentItem(hItem);

		CString ls_chain, ls_map;
		((CEdit*)GetDlgItem(IDC_EDITCHAIN))->GetWindowTextW(ls_chain);
		lChainEdit.m_chain = ls_chain;

		if ( lChainEdit.DoModal()== IDOK)
		{
 				//Insert the new department into tree
				STRCHAINPARK*	pnewPark = new STRCHAINPARK;
				pnewPark->cFlag = 'A';
				CString ls_newParklot = lChainEdit.m_parklot;
		
				pnewPark->chainid = m_chainid;		
				pnewPark->parklotid = m_parklotid;
				pnewPark->groupid = lChainEdit.m_groupid;
				m_groupid = lChainEdit.m_groupid;

				wcscpy(pnewPark->parklotdesc ,ls_newParklot );
				wcscpy(  pnewPark->groupdesc , lChainEdit.m_group+'\0');
				wcscpy(  pnewPark->chaindesc , ls_chain+'\0');
				wcscpy( pnewPark->mapfile, lChainEdit.m_map+'\0');

				//Modify BACK edit text
				((CEdit*)GetDlgItem(IDC_EDITPARK))->SetWindowTextW(ls_newParklot );
				((CEdit*)GetDlgItem(IDC_EDITGROUP))->SetWindowTextW(lChainEdit.m_group);
				((CEdit*)GetDlgItem(IDC_EDITMAP))->SetWindowTextW(lChainEdit.m_map);
			 
	 			 HTREEITEM hnewItem = m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, ls_newParklot, 0,0,0,0, (LPARAM)pnewPark, hpItem, TVI_LAST);
				 m_tree.SelectItem(hnewItem);

				m_vpModPark.push_back(pnewPark);
		}
}

//Modify Parking Lot info
void CTabpageGatePark::OnBnClickedButparkmod()
{
	USES_CONVERSION;
 
	CDlgChain	lChainEdit;
	lChainEdit.m_mode = _T("PM");	//Parklot Modify
	lChainEdit.mc_Pmode = mc_Pmode;
	lChainEdit.pDB = psDB;

		//Make sure selec the park item
		HTREEITEM	hItem = m_tree.GetSelectedItem();
		if (!hItem) return;
		HTREEITEM	hpItem = m_tree.GetParentItem(hItem);
		if ( hpItem == NULL) //Not Parking Item
				return;

		TVITEM tvItem;
		tvItem.hItem = hItem;
		tvItem.mask = TVIF_HANDLE| TVIF_PARAM;
		m_tree.GetItem(&tvItem);

		//Assign related info into edit window
		CString ls_chain, ls_parklot, ls_group;
		((CEdit*)GetDlgItem(IDC_EDITCHAIN))->GetWindowTextW(ls_chain);
		((CEdit*)GetDlgItem(IDC_EDITPARK))->GetWindowTextW(ls_parklot);
		((CEdit*)GetDlgItem(IDC_EDITGROUP))->GetWindowTextW(m_group);
		((CEdit*)GetDlgItem(IDC_EDITMAP))->GetWindowTextW(m_map);
		lChainEdit.m_chain = ls_chain;
		lChainEdit.m_parklot = ls_parklot;
		lChainEdit.m_groupid = m_groupid;
		lChainEdit.m_map = m_map;

	if ( lChainEdit.DoModal()== IDOK)
	{
 		//Insert the new department into tree
		STRCHAINPARK*	pnewPark = new STRCHAINPARK;
		pnewPark->cFlag = 'M';
		CString ls_newParklot = lChainEdit.m_parklot;
		wcscpy(pnewPark->parklotdesc, ls_newParklot );
		
		CString ls_map = lChainEdit.m_map;
		if (m_map != lChainEdit.m_map) // map changed
		{
			CString ls_newmap = lChainEdit.m_map;
			wcscpy(pnewPark->mapfile, ls_newmap );
			((CEdit*)GetDlgItem(IDC_EDITMAP))->SetWindowTextW( lChainEdit.m_map);
		}
 
		m_tree.SetItemText(hItem, ls_newParklot );

		//Modify Chain edit text
		((CEdit*)GetDlgItem(IDC_EDITPARK))->SetWindowTextW(ls_newParklot );
		m_tree.SelectItem(hItem);
		
		//Camera Group
		m_groupid = lChainEdit.m_groupid;
		((CEdit*)GetDlgItem(IDC_EDITGROUP))->SetWindowTextW( lChainEdit.m_group);
		pnewPark->chainid = m_chainid;		
		pnewPark->parklotid = m_parklotid;
		pnewPark->groupid = m_groupid;

		wcscpy(  pnewPark->groupdesc , lChainEdit.m_group+'\0');
		wcscpy(  pnewPark->chaindesc , ls_chain+'\0');
		wcscpy(  pnewPark->mapfile , ls_map+'\0');

		tvItem.lParam = (LPARAM)pnewPark;
		bool lb_ret = m_tree.SetItem(hItem, TVIF_HANDLE| TVIF_PARAM, ls_newParklot,  0,0,0, 0, tvItem.lParam);

 		m_vpModPark.push_back(pnewPark);
	}
}

void CTabpageGatePark::OnBnClickedButparkdel()
{
		//Make sure selec the park item
		HTREEITEM	hItem = m_tree.GetSelectedItem();
		if (!hItem) return;
		if (m_tree.GetParentItem(hItem) == NULL) //Not Parking Item
				return;
 
		if (AfxMessageBox(_T("Delete related parking gate, ok to process?"), MB_OKCANCEL, MB_ICONQUESTION) == IDOK)
		{		
				//Save tje delete flag into vector, fn_save will handle later 
				USES_CONVERSION;

 				STRCHAINPARK*	pnewChain = new STRCHAINPARK;
				pnewChain->cFlag = 'D';		//Delete
				pnewChain->chainid = m_chainid;
				pnewChain->parklotid = m_parklotid;
					
				m_tree.DeleteItem(hItem);
				m_vpModPark.push_back(pnewChain); 
		}
	
}

void CTabpageGatePark::fnSaveAC(bool obClose)
{
	//Save new Chains
	CString ls_sql;
	for (int i=0; i<m_vpModChain.size(); i++)
	{
		// Insert new chains
		if (m_vpModChain[i]->cFlag == 'A')
		{
				CString ls_bsql;
				ls_bsql.Format(_T(" INSERT INTO ecac_chain(chain_desc) VALUES('%s'); "), m_vpModChain[i]->chaindesc);
				ls_sql += ls_bsql;
		}
		else if (m_vpModChain[i]->cFlag == 'M') //Modify Chain
		{
			if (m_vpModChain[i]->chainid < 0) 
				continue;
				
			CString ls_bsql;
			ls_bsql.Format(_T(" UPDATE ecac_chain SET chain_desc = '%s'  WHERE chainid=%d; "), m_vpModChain[i]->chaindesc, m_vpModChain[i]->chainid);
			ls_sql += ls_bsql;
		}
		else if (m_vpModChain[i]->cFlag == 'D') //Delete Chain
		{
			if (m_vpModChain[i]->chainid < 0) 
				continue;
				
			fnDeleteTB(_T("ecac_chainac"), _T("acid"), m_vpModChain[i]->chainid);

			CString ls_bsql;
			ls_bsql.Format(_T(" DELETE FROM ecac_chain WHERE chainid=%d; "),  m_vpModChain[i]->chainid);
			ls_sql += ls_bsql;
		
			ls_bsql.Format(_T(" DELETE FROM ecac_chainac WHERE chainid=%d; "),  m_vpModChain[i]->chainid);
			ls_sql += ls_bsql;
		}
	}

	//Save new Parking 
	for (int i=0; i<m_vpModPark.size(); i++)
	{
		// Insert new chains
		if (m_vpModPark[i]->cFlag == 'A')
		{
				CString ls_bsql;
 
				ls_bsql.Format(_T(" INSERT INTO ecac_ac(ac_desc) VALUES('%s'); "), m_vpModPark[i]->parklotdesc);
				psDB->ExecuteSQL(ls_bsql);
				int ll_newparklotid =0;
				ll_newparklotid = fnGetMaxParklotid();
				if (ll_newparklotid >0)
				{
					ls_bsql.Format(_T(" INSERT INTO ecac_chainac(chainid, acid, cameragroupid, gatemap_file) VALUES(%d, %d, %d, '%s'); "), m_vpModPark[i]->chainid, ll_newparklotid, m_vpModPark[i]->groupid, m_vpModPark[i]->mapfile);
					ls_sql += ls_bsql;
				}
		}
		else if (m_vpModPark[i]->cFlag == 'M') //Modify Chain
		{
			if (m_vpModPark[i]->chainid < 0) 
				continue;
				
			CString ls_bsql;
			ls_bsql.Format(_T(" UPDATE ecac_ac SET ac_desc = '%s'  WHERE acid=%d; "), m_vpModPark[i]->parklotdesc, m_vpModPark[i]->parklotid);
			ls_sql += ls_bsql;
			ls_bsql.Format(_T(" UPDATE ecac_chainac SET cameragroupid=%d, gatemap_file='%s' WHERE chainid =%d AND acid=%d; "), m_vpModPark[i]->groupid, m_vpModPark[i]->mapfile, m_vpModPark[i]->chainid, m_vpModPark[i]->parklotid);
			ls_sql += ls_bsql;
		}
		else if (m_vpModPark[i]->cFlag == 'D') //Delete Chain
		{
			if (m_vpModPark[i]->chainid < 0) 
				continue;
 			CString ls_bsql;
			ls_bsql.Format(_T(" DELETE FROM ecac_chainac WHERE chainid=%d AND acid=%d; "),  m_vpModPark[i]->chainid, m_vpModPark[i]->parklotid);
			ls_sql += ls_bsql;
		}
	}

	TRACE(ls_sql);
	psDB->ExecuteSQL(ls_sql);
	m_vpModChain.clear();		//Clear Chain Vector
	m_vpModPark.clear();

	if (!obClose)
	{
		m_tree.DeleteAllItems();
		fnBuildTreeAC();
	}

}




void CTabpageGatePark::fnSave(bool obClose)
{
	//Save new Chains
	CString ls_sql;
	for (int i=0; i<m_vpModChain.size(); i++)
	{
		// Insert new chains
		if (m_vpModChain[i]->cFlag == 'A')
		{
				CString ls_bsql;
				ls_bsql.Format(_T(" INSERT INTO ecgt_chain(chain_desc) VALUES('%s'); "), m_vpModChain[i]->chaindesc);
				ls_sql += ls_bsql;
		}
		else if (m_vpModChain[i]->cFlag == 'M') //Modify Chain
		{
			if (m_vpModChain[i]->chainid < 0) 
				continue;
				
			CString ls_bsql;
			ls_bsql.Format(_T(" UPDATE ecgt_chain SET chain_desc = '%s'  WHERE chainid=%d; "), m_vpModChain[i]->chaindesc, m_vpModChain[i]->chainid);
			ls_sql += ls_bsql;
		}
		else if (m_vpModChain[i]->cFlag == 'D') //Delete Chain
		{
			if (m_vpModChain[i]->chainid < 0) 
				continue;
				
			//Delete ecgr_parklot data
			fnDeleteTB(_T("ecgt_chainparklot"), _T("parklotid"), m_vpModChain[i]->chainid);

			CString ls_bsql;
			ls_bsql.Format(_T(" DELETE FROM ecgt_chain WHERE chainid=%d; "),  m_vpModChain[i]->chainid);
			ls_sql += ls_bsql;
		
			ls_bsql.Format(_T(" DELETE FROM ecgt_chainparklot WHERE chainid=%d; "),  m_vpModChain[i]->chainid);
			ls_sql += ls_bsql;
		}
	}

	//Save new Parking 
	for (int i=0; i<m_vpModPark.size(); i++)
	{
		// Insert new chains
		if (m_vpModPark[i]->cFlag == 'A')
		{
				CString ls_bsql;
 
				ls_bsql.Format(_T(" INSERT INTO ecgt_parklot(parklot_desc) VALUES('%s'); "), m_vpModPark[i]->parklotdesc);
				psDB->ExecuteSQL(ls_bsql);
				int ll_newparklotid =0;
				ll_newparklotid = fnGetMaxParklotid();
				if (ll_newparklotid >0)
				{
					ls_bsql.Format(_T(" INSERT INTO ecgt_chainparklot(chainid, parklotid, cameragroupid, gatemap_file) VALUES(%d, %d, %d, '%s'); "), m_vpModPark[i]->chainid, ll_newparklotid, m_vpModPark[i]->groupid, m_vpModPark[i]->mapfile);
					ls_sql += ls_bsql;
				}
		}
		else if (m_vpModPark[i]->cFlag == 'M') //Modify Chain
		{
			if (m_vpModPark[i]->chainid < 0) 
				continue;
				
			CString ls_bsql;
			ls_bsql.Format(_T(" UPDATE ecgt_parklot SET parklot_desc = '%s'  WHERE parklotid=%d; "), m_vpModPark[i]->parklotdesc, m_vpModPark[i]->parklotid);
			ls_sql += ls_bsql;
			ls_bsql.Format(_T(" UPDATE ecgt_chainparklot SET cameragroupid=%d, gatemap_file='%s' WHERE chainid =%d AND parklotid=%d; "), m_vpModPark[i]->groupid, m_vpModPark[i]->mapfile, m_vpModPark[i]->chainid, m_vpModPark[i]->parklotid);
			ls_sql += ls_bsql;
		}
		else if (m_vpModPark[i]->cFlag == 'D') //Delete Chain
		{
			if (m_vpModPark[i]->chainid < 0) 
				continue;
 			CString ls_bsql;
			ls_bsql.Format(_T(" DELETE FROM ecgt_chainparklot WHERE chainid=%d AND parklotid=%d; "),  m_vpModPark[i]->chainid, m_vpModPark[i]->parklotid);
			ls_sql += ls_bsql;
		}
	}

	TRACE(ls_sql);
	psDB->ExecuteSQL(ls_sql);
	m_vpModChain.clear();		//Clear Chain Vector
	m_vpModPark.clear();
	m_newchainid = 0;

	if (!obClose)
	{
		m_tree.DeleteAllItems();
		fnBuildTree();
	}

}



void CTabpageGatePark::fnFreeMemory(void)
{
}

// Get Max parklot id from ecgt_parklot
int CTabpageGatePark::fnGetMaxParklotid(void)
{
		int	ll_ret=0;
		CDBVariant	varid;	
		CRecordset m_rs(psDB);
		CString	ls_sql;
		if (mc_Pmode=='A')
			ls_sql =  _T("SELECT acid FROM ecac_ac WHERE acid IN (SELECT MAX(acid) FROM ecac_ac)");
		else
			ls_sql =  _T("SELECT parklotid FROM ecgt_parklot WHERE parklotid IN (SELECT MAX(parklotid) FROM ecgt_parklot)");

		m_rs.Open(CRecordset::dynaset, ls_sql);
  
		while (!m_rs.IsEOF()) { 
			if (mc_Pmode=='A')
				m_rs.GetFieldValue(_T("acid"), varid,  SQL_C_SSHORT );
			else
				m_rs.GetFieldValue(_T("parklotid"), varid,  SQL_C_SSHORT );
			ll_ret = varid.m_iVal;
			break;
		}	
		return ll_ret;
}

int CTabpageGatePark::fnGetIdent(CString oCol, CString oTable)
{
		CDBVariant	varid;	
		CRecordset m_rs(psDB);
		CString	ls_sql, csid;
		
		try{
			ls_sql.Format(_T("SELECT %s FROM %s WHERE %s = IDENT_CURRENT('%s'); "), oCol, oTable, oCol, oTable);
			m_rs.Open(CRecordset::dynaset, ls_sql);
			
			while (!m_rs.IsEOF()) { 			 
					m_rs.GetFieldValue(oCol,varid, SQL_C_SSHORT);
					m_newchainid = varid.m_iVal;
					return varid.m_iVal;			
			}

			//if there's empty row in the chain table, reset identity to 0
			ls_sql.Format(_T("DBCC CHECKIDENT (%s, reseed, 1) "), oTable );
			psDB->ExecuteSQL(ls_sql);
			return 0;
		}
			catch(CDBException * p)
		{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}
}

BOOL CTabpageGatePark::PreTranslateMessage(MSG* pMsg)
{
	if ( m_pToolTipCtrl != NULL)
		m_pToolTipCtrl->RelayEvent(pMsg);
	return CBaseDialog::PreTranslateMessage(pMsg);
}

void CTabpageGatePark::fnLocalize()
{
			CString csLoc;
		if (mc_Pmode=='A')
		{
			//Groupbox Chain
			LOADSTRING(csLoc, IDS_ACSETCHAIN  );
			GetDlgItem(IDC_GBOXCHAIN)->SetWindowTextW(csLoc);

			//Description
			GetDlgItem(IDC_STDESC)->SetWindowTextW(csLoc);

			//Camera Group
			LOADSTRING(csLoc,IDS_DLG_CAM_GROUP_NAME);
			GetDlgItem(IDC_STCAMGROUP)->SetWindowTextW(csLoc);

			//Map
			LOADSTRING(csLoc, IDS_HINT_EMAP);
			GetDlgItem(IDC_STMAP)->SetWindowTextW(csLoc);

			LOADSTRING(csLoc, IDS_ACSTTOP	);
			GetDlgItem(IDC_STTOP)->SetWindowTextW(csLoc);

 			LOADSTRING(csLoc, IDS_AC_TAB_AC	);
			GetDlgItem(IDC_STGBGATE)->SetWindowTextW(csLoc);	
 
			LOADSTRING(csLoc, IDS_AC_TAB_AC	);
			GetDlgItem(IDC_STGATE)->SetWindowTextW(csLoc);	
		}
		else if (mc_Pmode=='G')
		{
 			//Camera Group
			LOADSTRING(csLoc,IDS_DLG_CAM_GROUP_NAME);
			GetDlgItem(IDC_STCAMGROUP)->SetWindowTextW(csLoc);

			LOADSTRING(csLoc, IDC_STATIC_GATE_CHAIN	);
			GetDlgItem(IDC_STTOP)->SetWindowTextW(csLoc);

			//Chain
			GetDlgItem(IDC_GBOXCHAIN)->SetWindowTextW(csLoc);
			//Description
			GetDlgItem(IDC_STDESC)->SetWindowTextW(csLoc);


 			LOADSTRING(csLoc, IDS_HINT_GATE	);
			GetDlgItem( IDC_STGBGATE )->SetWindowTextW(csLoc);	
 
			//LOADSTRING(csLoc, IDS_AC_TAB_AC	);
			GetDlgItem( IDC_STGATE )->SetWindowTextW(csLoc);	

			LOADSTRING(csLoc, IDS_DLG_CAM_GROUP_NAME);
			GetDlgItem( IDC_STCAMGROUP  )->SetWindowTextW(csLoc);	

			LOADSTRING(csLoc, IDS_EMAPMAP);
			GetDlgItem(  IDC_STMAP )->SetWindowTextW(csLoc);	
						
		}
}

void CTabpageGatePark::fnDeleteTB(CString oTabname, CString oColname, int oChainid )
{
		CString	ls_sql, ls_newsql, ls_delsql;
		CDBVariant	varid;	
		CRecordset m_rs(psDB);
		ls_sql.Format(_T("SELECT %s FROM %s WHERE chainid=%d "), oColname, oTabname, oChainid);
			
		m_rs.Open(CRecordset::dynaset, ls_sql);
			
			while (!m_rs.IsEOF()) { 			 
					m_rs.GetFieldValue(oColname,varid, SQL_C_SSHORT);
					if (oTabname == _T("ecac_chainac"))			//Access Control			
						ls_newsql.Format(_T("DELETE FROM ecac_ac WHERE acid=%d; "), varid.m_iVal);
					else	//For Gate Control
						ls_newsql.Format(_T("DELETE FROM ecgt_parklot WHERE parklotid=%d; "), varid.m_iVal);

					ls_delsql = ls_delsql + ls_newsql;
					m_rs.MoveNext();
			}


			try
			{
 				TRACE(ls_delsql);
				psDB->ExecuteSQL(ls_delsql);
			}
			catch(CDBException * p)
			{
					AfxMessageBox(p->m_strError);
					p->Delete();
			}//try...catch
			
}
