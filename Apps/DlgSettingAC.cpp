// DlgSettingAC.cpp : implementation file
//
#include "resource.h"
#include "stdafx.h"
#include "DlgSettingAC.h"
#include "MainFrm.h"
#define	TABW 680
#define	TABH 460


// CDlgSettingAC dialog

IMPLEMENT_DYNAMIC(CDlgSettingAC, CBaseDialog)

CDlgSettingAC::CDlgSettingAC(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgSettingAC::IDD, pParent)
{

}

CDlgSettingAC::~CDlgSettingAC()
{
	fnFreeMemory();
}

void CDlgSettingAC::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABMAIN, m_tab);
}


BEGIN_MESSAGE_MAP(CDlgSettingAC, CBaseDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CDlgSettingAC::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTSAVE, &CDlgSettingAC::OnBnClickedButsave)
	ON_BN_CLICKED(IDC_BUTDEL, &CDlgSettingAC::OnBnClickedButdel)
	ON_WM_ERASEBKGND()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABMAIN, &CDlgSettingAC::OnTcnSelchangeTabmain)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTAPPLY, &CDlgSettingAC::OnBnClickedButapply)
END_MESSAGE_MAP()


// CDlgSettingAC message handlers

BOOL CDlgSettingAC::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	//m_CamList = (CListCtrl*)GetDlgItem(IDC_LIST1);

	pDB = &(((CMainFrame*)AfxGetMainWnd())->m_DB);
	mb_camChanged = false;	
	mb_acChanged = false;	
	mb_holChanged = false;
	mb_priChanged = false;
	
	//Assign db pointer into tabpage
	m_pagecam.psDB = pDB;
	m_pageac.psDB = pDB;
	m_pageemp.psDB = pDB;
	m_pagehol.psDB = pDB;

	//fnDisplayBrand();	//Display values of combobox from ecac_hbrandmodel
	
	fnInitControl(); //Tabpage Initialize
	fnInitAccess();
 
	if (!mb_openNormal)
	{
		m_tab.SetCurSel(1);
		pDialog[0]->ShowWindow(SW_HIDE);
		pDialog[1]->ShowWindow(SW_SHOW);
		pDialog[2]->ShowWindow(SW_HIDE);
		pDialog[3]->ShowWindow(SW_HIDE);
	}
	return TRUE;  
}

void CDlgSettingAC::fnPopulateCam(void)
{
	CString ls_name, ls_ip, ls_desc;
	TCHAR lc_desc[255];
	CDBVariant varIdx;
	CDBVariant varCamidx, varid, varNull;
	CString ls_sql1, ls_sql2, ls_sql3, ls_sql,  ls_full;
	int ci=0;

	try{
		LV_COLUMN ListColumn;
		CString csLoc1(_T(""));//ipcam
		LOADSTRING(csLoc1, IDS_EMAPIPCAM);			

		CString csLoc2(_T(""));//id
		LOADSTRING(csLoc2, IDS_EMAPID);

 		WCHAR * ListTitles[3] = {_T(""), csLoc1.GetBuffer(),  csLoc2.GetBuffer()};
		//WCHAR * ListTitles[3] = {_T(""), _T("IP Cameras"),_T("ID")};

		for(int i = 0; i < 3; i++)
		{
			ListColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			ListColumn.fmt = LVCFMT_LEFT;
			ListColumn.iSubItem = i;

			if(i == 1 )
				{ListColumn.cx = 220;}
			else if(i == 0)
				{ListColumn.cx = 16;}
			else if(i == 2)
				{ListColumn.cx = 20;}
			
			ListColumn.pszText = ListTitles[i];
			//m_CamList->InsertColumn(i, &ListColumn);
			m_pagecam.m_CamList.InsertColumn(i, &ListColumn);

		}

		DWORD	dwStyle = m_CamList->GetStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;
		m_pagecam.m_CamList.SetExtendedStyle(dwStyle);
		m_pagecam.m_CamList.ModifyStyle(0, LVS_SINGLESEL);
		//m_CamList->SetExtendedStyle(dwStyle);
		//m_CamList->ModifyStyle(0, LVS_SINGLESEL);

		//Image Checkbox
		m_SmallImg.Create(16, 16, ILC_COLOR | ILC_MASK, 0, 1);
 		CBitmap bIcon;
		bIcon.LoadBitmapW(IDB_KEY);
		m_SmallImg.Add(&bIcon, RGB(0, 0, 0));
		//m_CamList->SetImageList(&m_SmallImg, LVSIL_SMALL);
		m_pagecam.m_CamList.SetImageList(&m_SmallImg, LVSIL_SMALL);

		CRecordset m_rsCam(&((CMainFrame*)AfxGetMainWnd())->m_DB);
		 		 
		ls_sql = _T("SELECT ec_camera.camera_idx, ec_camera.cameraname, ec_camera.ipaddress,  ecac_camera.active_ ");
		ls_sql += _T(" FROM ec_camera LEFT OUTER JOIN ecac_camera ON ecac_camera.camera_idx = ec_camera.camera_idx");
		m_rsCam.Open(CRecordset::dynaset,ls_sql);

 		int cnt=0;	
		CString	ls_active;
		
		int ll_rows = m_rsCam.GetRecordCount();
		m_rsCam.MoveFirst();

		while(!m_rsCam.IsEOF()   ) 
		{ 
			m_rsCam.GetFieldValue(_T("camera_idx"), varIdx, SQL_C_SLONG);		
			m_rsCam.GetFieldValue(_T("cameraname"), ls_name);

			m_rsCam.GetFieldValue(_T("ipaddress"), ls_ip);
			m_rsCam.GetFieldValue(_T("active_"), ls_active);

			ls_full.Format(_T("(%d) - %s / %s"), varIdx.m_iVal, ls_name, ls_ip);
			wsprintf(lc_desc,_T("(%d) %s - %s"), varIdx.m_iVal,ls_name.Trim(), ls_ip.Trim());
			LVITEMW* pLitem = new LVITEM;
			pLitem->mask = LVIF_PARAM | LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
			pLitem->iItem = cnt;

			int 	nItem;
			if (ls_active == _T("Y"))
				nItem = m_pagecam.m_CamList.InsertItem(cnt, _T(""), 0);
			else
				nItem = m_pagecam.m_CamList.InsertItem(cnt, _T(""), 1);
			//nItem = m_listcam.InsertItem(cnt, _T(""), 0);
				
			CString ls_idx;
			ls_idx.Format(_T("%d"),varIdx.m_iVal);

			m_pagecam.m_CamList.SetItemText(nItem, 1, lc_desc);
			m_pagecam.m_CamList.SetItemText(nItem, 2, ls_idx);
			//m_CamList->SetItemText(nItem, 1, lc_desc);
			//m_CamList->SetItemText(nItem, 2, ls_idx);
			cnt++;
			m_rsCam.MoveNext();
		}	
	} //try
		catch(CDBException * p) 	{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}	
	return ;
}

void CDlgSettingAC::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	//UINT nSelCount = m_CamList->getit
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;	 
	 CString ls_name = m_CamList->GetItemText( pNMListView->iItem, 1);
	 CString ls_idx = m_CamList->GetItemText( pNMListView->iItem, 2);
	 m_camidx = _ttoi(ls_idx);
	 CStatic* lpName = (CStatic*)GetDlgItem(IDC_STNAME);
	 lpName->SetWindowTextW(ls_name);

	fnClearAc();
	fnRetrieveData();
	*pResult = 0;
}


void CDlgSettingAC::fnInitControl(void)
{
		CString csTxt =_T("");		
		
		//Initialize Tab Control
		m_tab.ShowWindow(SW_SHOW);

		LOADSTRING(csTxt,  IDS_ACSETCAM);
		m_tab.InsertItem(0,csTxt.GetBuffer());

		 //Chain-Access Control
		LOADSTRING(csTxt, IDS_ACSETCHAIN);
		m_tab.InsertItem(1,csTxt.GetBuffer());

		LOADSTRING(csTxt, IDS_ACSETDWE);
		m_tab.InsertItem(2,csTxt.GetBuffer());

		LOADSTRING(csTxt, IDS_ACSETHOL);
		m_tab.InsertItem(3,csTxt.GetBuffer());

		m_pagecam.Create(IDD_TABPAGEACCAM, &m_tab);
		m_pageac.mc_Pmode='A';
		m_pageac.Create(IDD_TABPAGEGATEPARK, &m_tab);

		m_pageemp.Create(IDD_TABPAGEACEMP, &m_tab);   
		m_pagehol.Create(IDD_TABPAGEACHOL, &m_tab);   

		pDialog[0] = &m_pagecam;
		pDialog[1] = &m_pageac;
		pDialog[2] = &m_pageemp;
		pDialog[3] = &m_pagehol;
	 
		pDialog[0]->ShowWindow(SW_SHOW);
		pDialog[1]->ShowWindow(SW_HIDE);
		pDialog[2]->ShowWindow(SW_HIDE);
		pDialog[3]->ShowWindow(SW_HIDE);

		 CRect	rc;
		 int ll_gap = 1;
		m_tab.GetClientRect(rc);
		
		rc.top += 20;
		rc.bottom -= ll_gap;
		rc.left += ll_gap;
		rc.right -= ll_gap;

		m_pagecam.MoveWindow(&rc);
		m_pageac.MoveWindow(&rc);
		m_pageemp.MoveWindow(&rc);
		m_pagehol.MoveWindow(&rc);
		m_tab.SetCurSel(0);
}

//Port access data into structure
void CDlgSettingAC::fnInitAccess(void)
{
	m_pagecam.fnInitCam();
	m_pagecam.fnPopulateCam();

// *************** Obsolete Codes *****************************************
//		CString	ls_sql, ls_recordtype, ls_active, ls_db, ls_brand, ls_model;
//		CDBVariant	varIdx;
//
//		char	cActive[1];		
//		
//		CRecordset	m_rsCam(pDB);
//		ls_sql = _T("SELECT * FROM ecac_camera  ORDER BY camera_idx");
//		m_rsCam.Open(CRecordset::dynaset,ls_sql);
//
//		int ll_rows = m_rsCam.GetRecordCount();
//		if (ll_rows<=0) return;
//		m_rsCam.MoveFirst();
//
////		STRCAM	lCamAc;
//
//		while(!m_rsCam.IsEOF()   ) 
//		{ 
//			m_rsCam.GetFieldValue(_T("camera_idx"), varIdx, SQL_C_SLONG);
//			m_rsCam.GetFieldValue(_T("access_brand"), ls_brand);
//			m_rsCam.GetFieldValue(_T("access_model"), ls_model);
//			m_rsCam.GetFieldValue(_T("record_type"), ls_recordtype);
//			m_rsCam.GetFieldValue(_T("save_db"), ls_db);
//			m_rsCam.GetFieldValue(_T("active_"),ls_active);
//
//			lCamAc.cam_idx = varIdx.m_iVal;
//			if (ls_active == _T("N")) lCamAc.active_ = 'N';
//			else
//				lCamAc.active_ = 'Y';
//
//			if (ls_db == _T("N")) lCamAc.savedb = 'N';
//			else
//				lCamAc.savedb = 'Y';
//
//			lCamAc.brand = ls_brand;
//			lCamAc.model = ls_model;
//			lCamAc.recordtype = ls_recordtype;
//		
//			//Construct the vector
//			m_pagecam.m_vpCamAccess.push_back(lCamAc);
//			//m_vpCamAccess.push_back(lCamAc);
//			m_rsCam.MoveNext();
//		}
}

void CDlgSettingAC::fnClearAc(void)
{
 	((CButton*)(GetDlgItem( IDC_CKACTIVE )))->SetCheck( 0 );
	((CButton*)(GetDlgItem( IDC_CKDB )))->SetCheck( 0 );
	((CComboBox*) GetDlgItem(IDC_COMBOBRAND))->SetCurSel(0);
	((CComboBox*) GetDlgItem(IDC_COMBOMODEL))->SetCurSel(0);
	((CComboBox*) GetDlgItem(IDC_COMBORECORD))->SetCurSel(0);
 }

void CDlgSettingAC::fnRetrieveData()
{
		//bool lb_delete = false;
		//for (int i=0; i<m_vpCamAccess.size(); i++)
		//{
		//		if (m_vpCamAccess[i].cam_idx == m_camidx)
		//		{
		//			lb_delete = true;
		//			if (m_vpCamAccess[i].active_ =='Y') 
		//				((CButton*)(GetDlgItem( IDC_CKACTIVE )))->SetCheck(1);
		//			if (m_vpCamAccess[i].savedb =='Y') 
		//				((CButton*)(GetDlgItem( IDC_CKDB )))->SetCheck(1);

		//			//Brand Model Record
		//			CComboBox* comboBrand = (CComboBox*) GetDlgItem(IDC_COMBOBRAND);					
		//			int nIndex = comboBrand->FindString(0,m_vpCamAccess[i].brand);
		//			if (nIndex>=0) comboBrand->SetCurSel(nIndex);

		//			CComboBox* comboModel = (CComboBox*) GetDlgItem(IDC_COMBOMODEL);					
		//			nIndex = comboModel->FindString(0,m_vpCamAccess[i].model);
		//			if (nIndex>=0) comboModel->SetCurSel(nIndex);

		//			CComboBox* comboRecord = (CComboBox*) GetDlgItem(IDC_COMBORECORD);					
		//			nIndex = comboRecord->FindString(0,m_vpCamAccess[i].recordtype);
		//			if (nIndex>=0) comboRecord->SetCurSel(nIndex);

		//		}
		//}
		//((CButton*)GetDlgItem(IDC_BUTDEL))->EnableWindow(lb_delete);
}

void CDlgSettingAC::OnBnClickedButsave()
{
				//bool	lb_found = false;
				//int		i_rec=0;
				//CString ls_sql;

				//try{
				//	// Check if the alerady exist
				//	for (int i=0; i<m_vpCamAccess.size(); i++)
				//	{
				//		if (m_vpCamAccess[i].cam_idx == m_camidx)
				//		{
				//			lb_found = true;
				//			i_rec = i;
				//			break;
				//		}
				//	}
				//	
				//	//Get Info from Settings
				//	int i_active = ((CButton*)(GetDlgItem( IDC_CKACTIVE )))->GetCheck();
				//	int i_db = ((CButton*)(GetDlgItem( IDC_CKDB )))->GetCheck();
				//	
				//	char  lc_active='Y', lc_db='Y';	
				//	if (i_active==0) lc_active = 'N';
				//	if (i_db==0) lc_db = 'N';

				//	//brand, Model and Reocrd type
				//	CString	ls_brand, ls_model, ls_record;					
				//	CComboBox* pBrand = (CComboBox*) GetDlgItem(IDC_COMBOBRAND);
				//	CComboBox* pModel = (CComboBox*) GetDlgItem(IDC_COMBOMODEL);
				//	CComboBox* pRecord = (CComboBox*) GetDlgItem(IDC_COMBORECORD);
				//	
				//	pBrand->GetWindowTextW(ls_brand);
				//	pModel->GetWindowTextW(ls_model);
				//	pRecord->GetWindowTextW(ls_record);

				//	STRCAMAC lstr;
				//	lstr.active_ = lc_active;
				//	lstr.savedb = lc_db;
				//	lstr.cam_idx = m_camidx;
				//	lstr.brand = ls_brand;
				//	lstr.model = ls_model;
				//	lstr.recordtype = ls_record;

				//	if (lb_found) // Existing access data
				//	{
				//		ls_sql.Format(_T("UPDATE ecac_camera SET  access_brand='%s', access_model='%s', record_type='%s', save_db = '%c', active_='%c'  WHERE  camera_idx=%d"),ls_brand, ls_model, ls_record, lc_db, lc_active, m_camidx  );
				//			m_vpCamAccess[i_rec] = lstr;
				//	}
				//	else	//New access data
				//	{
				//			ls_sql.Format(_T("INSERT INTO ecac_camera ( camera_idx, save_db, active_,access_brand, access_model, record_type) VALUES (%d, '%c', '%c', '%s', '%s', '%s') "), m_camidx, lc_db, lc_active, ls_brand, ls_model, ls_record );
				//			TRACE(ls_sql);
				//			m_vpCamAccess.push_back(lstr);
				//	}
				//			
				//	TRACE(ls_sql);
				//	pDB->ExecuteSQL(ls_sql);
				//}
				//catch(CDBException * p)
				//{
				//	AfxMessageBox(p->m_strError);
				//	p->Delete();
				//}
}

void CDlgSettingAC::fnFreeMemory()
{
	 
}


void CDlgSettingAC::OnBnClickedButdel()
{
	// TODO: Add your control notification handler code here
	//Delete the Access Control of the highlight camera
	try
	{
			CString	ls_sql, ls_idx;
			if (m_camidx<=0)  return;

			ls_sql.Format(_T("DELETE FROM ecac_camera WHERE camera_idx=%d "), m_camidx );										
			TRACE(ls_sql);
	 		pDB->ExecuteSQL(ls_sql);

			for (int i=0; i<m_CamList->GetItemCount(); i++)
			{
				ls_idx  = m_CamList->GetItemText(i, 2);
				if  (_ttoi(ls_idx) == m_camidx)
				{
					m_CamList->SetItem(i, 0, 0,_T(""),1,0,0,NULL);
					break;
				}
			}

			fnClearAc();
	}
	catch(CDBException * p)
	{
					AfxMessageBox(p->m_strError);
					p->Delete();
	}
}

void CDlgSettingAC::fnDisplayBrand(void)
{
	//CComboBox* pBrand = (CComboBox*) GetDlgItem(IDC_COMBOBRAND);
	//CComboBox* pModel = (CComboBox*) GetDlgItem(IDC_COMBOMODEL);

	//try
	//{
	//	CString csBrand, csModel;
	//	CRecordset rBrand(pDB);

	//	rBrand.Open(CRecordset::forwardOnly, _T("SELECT ac_brand, ac_model FROM ecac_hbrand_model"));

	//	pBrand->AddString(_T(""));
	//	pModel->AddString(_T(""));


	//	while (!rBrand.IsEOF()) { 
	//		rBrand.GetFieldValue(_T("ac_brand"), csBrand);
	//		rBrand.GetFieldValue(_T("ac_model"), csModel);
	//		
	//		pBrand->AddString(csBrand);
	//		pModel->AddString(csModel);

	//		rBrand.MoveNext();
	//	}
	//	pBrand->SetCurSel(0);
	//	pModel->SetCurSel(0);
	//}
	//catch(CDBException * p)
	//{
	//	AfxMessageBox(p->m_strError);
	//	p->Delete();
	//}
}

//void CDlgSettingAC::OnCbnSelchangeCombo3()
//{
//	// TODO: Add your control notification handler code here
//}

BOOL CDlgSettingAC::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CBaseDialog::OnEraseBkgnd(pDC);
}

void CDlgSettingAC::OnTcnSelchangeTabmain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	int ll_sel = m_tab.GetCurSel();
	if (ll_sel ==0) 	{
		pDialog[0]->ShowWindow(SW_SHOW);
		pDialog[1]->ShowWindow(SW_HIDE);
		pDialog[2]->ShowWindow(SW_HIDE);
		pDialog[3]->ShowWindow(SW_HIDE);
	}
	else if (ll_sel ==1) 	{
		pDialog[0]->ShowWindow(SW_HIDE);
		pDialog[1]->ShowWindow(SW_SHOW);
		pDialog[2]->ShowWindow(SW_HIDE);
		pDialog[3]->ShowWindow(SW_HIDE);
	}
	else if (ll_sel ==2) 	{
		pDialog[0]->ShowWindow(SW_HIDE);
		pDialog[1]->ShowWindow(SW_HIDE);
		pDialog[2]->ShowWindow(SW_SHOW);
		pDialog[3]->ShowWindow(SW_HIDE);
	}
	else{
		pDialog[0]->ShowWindow(SW_HIDE);
		pDialog[1]->ShowWindow(SW_HIDE);
		pDialog[2]->ShowWindow(SW_HIDE);
		pDialog[3]->ShowWindow(SW_SHOW);
	}
}

HBRUSH CDlgSettingAC::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

void CDlgSettingAC::OnBnClickedButapply()
{
	// TODO: Add your control notification handler code here
	fnSave(_T("open"));
	//fnPopulateCam();
}

void CDlgSettingAC::fnSave(CString o_mode)
{
	if (m_pagecam.mb_dirty) {
		mb_camChanged = true;	
		m_pagecam.fnSave(o_mode);
	}
	
	m_pageac.fnSaveAC(false);

	if (m_pageemp.mb_dirty) 
	{
		m_pageemp.fnSave(o_mode);
		mb_priChanged = true; 
	}
	if (m_pagehol.mb_dirty) 
	{
		m_pagehol.fnSave(o_mode);
		mb_holChanged = true;
	}
}


void CDlgSettingAC::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	fnSave(_T("close"));
	CBaseDialog::OnOK();
}
