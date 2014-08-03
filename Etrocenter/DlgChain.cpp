// DlgChain.cpp : implementation file
//

#include "stdafx.h"
#include "DlgChain.h"
#include "MainFrm.h"

// CDlgChain dialog

IMPLEMENT_DYNAMIC(CDlgChain, CDialog)

CDlgChain::CDlgChain(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgChain::IDD, pParent)
{

}

CDlgChain::~CDlgChain()
{
}

void CDlgChain::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOGROUP, m_comboGroup);
	//DDX_Control(pDX, IDC_BUTFOLDER, m_butfolder);
	//DDX_Control(pDX, IDC_BUTMAP, m_butmap);
}


BEGIN_MESSAGE_MAP(CDlgChain, CBaseDialog)
 	ON_BN_CLICKED(IDC_BUTFOLDER, &CDlgChain::OnBnClickedButfolder)
	ON_BN_CLICKED(IDC_BUTMAP, &CDlgChain::OnBnClickedButmap)
	ON_CBN_SELCHANGE(IDC_COMBOGROUP, &CDlgChain::OnCbnSelchangeCombogroup)
END_MESSAGE_MAP()


// CDlgChain message handlers

BOOL CDlgChain::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	if (m_mode == _T("CA") || m_mode == _T("CM"))		//Chain Mode
	{		
 				if (m_mode == _T("CM")) //Chain Modify
					((CEdit*)GetDlgItem(IDC_EDITCHAIN))->SetWindowTextW(m_chain);

				//Focus CHAIN FIELD
				((CEdit*)GetDlgItem(IDC_EDITCHAIN))->SetFocus();
				
				fnHidePark();
	}
	else	//Parking Mode
	{		
				fnPopulateGroup();
				fnFolder();
				((CEdit*)GetDlgItem(IDC_EDITCHAIN))->SetReadOnly(1); //Chain Readonly
				((CEdit*)GetDlgItem(IDC_EDITCHAIN))->SetWindowTextW(m_chain);	
				int ll_sel =fnGetGroupidx(m_groupid);
				if (ll_sel> -1)
						((CComboBox*)GetDlgItem(IDC_COMBOGROUP))->SetCurSel(ll_sel);
				//((CComboBox*)GetDlgItem(IDC_COMBOGROUP))->SetCurSel(>SetWindowTextW(m_group);	
				if (m_mode == _T("PM")) //Park Modify
				{
					((CEdit*)GetDlgItem(IDC_EDITPARK))->SetWindowTextW(m_parklot);	
					((CEdit*)GetDlgItem(IDC_EDITMAP))->SetWindowTextW(m_map);	
				}

				//Focus CHAIN FIELD
				((CEdit*)GetDlgItem(IDC_EDITCHAIN))->EnableWindow(false);
				((CEdit*)GetDlgItem(IDC_EDITPARK))->SetFocus();
	 								
				int ll_x = 350, ll_y = 172;
				m_butfolder.AutoLoad( IDC_BUTFOLDER, this);
				//m_butfolder.MoveWindow(ll_x, ll_y, 24,24);

				ll_y +=30;
				m_butmap.AutoLoad( IDC_BUTMAP, this);
				//m_butmap.MoveWindow(ll_x, ll_y, 24,24);
 	}

	if (mc_Pmode=='A') //Access Control
		fnChangeTxt();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgChain::fnHidePark()
{
	GetDlgItem(IDC_STPARKG)->ShowWindow(false);
	GetDlgItem(IDC_EDITPARK )->ShowWindow(false);
	GetDlgItem( 	IDC_STGATEINFO )->ShowWindow(false);
	GetDlgItem(IDC_STGROUP)->ShowWindow(false);
	GetDlgItem(IDC_COMBOGROUP )->ShowWindow(false);
	GetDlgItem(IDC_STFOLDER)->ShowWindow(false);
	GetDlgItem(IDC_EDITFOLDER)->ShowWindow(false);
	GetDlgItem(IDC_STMAP)->ShowWindow(false);
	GetDlgItem(IDC_EDITMAP)->ShowWindow(false);

}

void CDlgChain::OnOK()
{
	int ll_sel;
	if (m_mode == _T("CA") || m_mode == _T("CM"))		//Chain Mode		
		 ((CEdit*)GetDlgItem(IDC_EDITCHAIN))->GetWindowTextW(m_chain);

	if (m_mode == _T("PA") || m_mode == _T("PM"))		//Park Mode		
	{	
		((CEdit*)GetDlgItem(IDC_EDITPARK))->GetWindowTextW(m_parklot);
		((CComboBox*)GetDlgItem(IDC_COMBOGROUP))->GetWindowTextW(m_group);
		ll_sel = m_comboGroup.GetCurSel();
		if (ll_sel<0 || m_parklot == _T("") || m_group==_T(""))
		{
			CString ls_warn;
			LOADSTRING(ls_warn, 	IDS_ACSET_WARNEMPTY );
			AfxMessageBox(ls_warn, 0, 0);
			return;
		}
		m_groupid = m_vtpGroup[ll_sel].cameragroupid;
	}
	CBaseDialog::OnOK();
}

void	CDlgChain::fnPopulateGroup()
{
	if (!pDB) return;

	CDBVariant	varGroupid;	
	CString	ls_desc;
  	CRecordset m_rs(pDB);
	
	CString ls_sql = _T( "SELECT DISTINCT cameragroupid, camera_group_desc FROM  ec_camera_group ORDER BY 1");

	TRACE(ls_sql);
	m_rs.Open(CRecordset::dynaset, ls_sql);
	int  newitem=0;
 
	while (!m_rs.IsEOF()) { 

		TVINSERTSTRUCTW itemStru={0};

		m_rs.GetFieldValue(_T("cameragroupid"), varGroupid,  SQL_C_SSHORT );
		m_rs.GetFieldValue(_T("camera_group_desc"), ls_desc );
		m_comboGroup.InsertString(newitem, ls_desc);

		//build vector
		STRGROUP	lgroup;
		lgroup.idx = newitem;
		lgroup.cameragroupid =  varGroupid.m_iVal;
		wcscpy(lgroup.desc , ls_desc);

		m_vtpGroup.push_back(lgroup);

		newitem++;
		m_rs.MoveNext();
	}
}

// Get combobox index from group desc
int CDlgChain::fnGetGroupidx(int oGroupid)
{
	for (int i=0; i<m_vtpGroup.size(); i++)
	{
		if (m_vtpGroup[i].cameragroupid == oGroupid)
			return i;
	}
	return -1;
}


void CDlgChain::OnBnClickedButfolder()
{
	LPMALLOC pMalloc = NULL;
	LPITEMIDLIST pidl = NULL;
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));

	// set the bi's default values
	bi.hwndOwner = m_hWnd;
	bi.lpszTitle = _T("Modify access control map storage folder:");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
 
	pidl = SHBrowseForFolder(&bi);
	if(pidl != NULL)
	{
		 CString pszPath;
		// free memory
		if(SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc)
		{
			SHGetPathFromIDListW(pidl, pszPath.GetBuffer(256));
			GetDlgItem(IDC_EDITFOLDER)->SetWindowTextW(pszPath.Trim());
			
			pMalloc->Free(pidl);  
			pMalloc->Release(); 
	
		//Save into DB
		try{
		CString ls_sql;

		ls_sql.Format(_T("DELETE FROM ec_storage WHERE storage_type='GATE' ; INSERT INTO ec_storage (storage_type, store_location) VALUES ('GATE', '%s') "), pszPath.Trim() ); 
			TRACE(ls_sql);
			pDB->ExecuteSQL(ls_sql);
		}
		catch(CDBException * p) 	{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}
	}
	} //	if(pidl != NULL)


}

void CDlgChain::OnBnClickedButmap()
{
		CString	ls_tfolder =_T("");
 
		CFileDialog fOpenDlg(TRUE, _T("bmp"), _T("Access Control Map File"), OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, 	_T("Select Bitmap File (*.bmp)|*.bmp|"), this);
	  
		fOpenDlg.m_pOFN->lpstrTitle=_T("Select Bitmap File");
		CString ls_appPath = ((CMainFrame*)AfxGetMainWnd())->m_csAppPath;
		if (ls_appPath !=_T(""))
				fOpenDlg.m_pOFN->lpstrInitialDir = ls_appPath;
		else
				fOpenDlg.m_pOFN->lpstrInitialDir=_T("c:");
	  
		if(fOpenDlg.DoModal()==IDOK)
		{
				CString ls_path = fOpenDlg.GetPathName();
				CString ls_name = fOpenDlg.GetFileName();
				CString ls_sfile, ls_tfile;
	
				((CEdit*)GetDlgItem(IDC_EDITMAP))->SetWindowTextW(ls_name.Trim());

	 			((CEdit*)GetDlgItem(IDC_EDITFOLDER))->GetWindowTextW(ls_tfolder);
				ls_sfile.Format(_T("%s\\%s"),ls_tfolder,ls_name);		
				bool ls_ret = CopyFile(ls_path,ls_sfile, false); 
				m_map = ls_name;
 
		}	//		if(fOpenDlg.DoModal()==IDOK)
}

//Get Folder from ec_storage
void CDlgChain::fnFolder()
{
		if (!pDB) return;

		CDBVariant	varGroupid;	
		CString	ls_folder, ls_sql;
  		CRecordset m_rs(pDB);

		ls_sql = _T( "SELECT store_location FROM  ec_storage WHERE storage_type='GATE'");

		TRACE(ls_sql);
		m_rs.Open(CRecordset::dynaset, ls_sql);

		while (!m_rs.IsEOF()) { 
 			m_rs.GetFieldValue(_T("store_location"), ls_folder );
			((CEdit*)GetDlgItem(IDC_EDITFOLDER))->SetWindowTextW(ls_folder.Trim());
			break;
		}
}	//void CDlgChain::fnFolder()
 


void CDlgChain::OnCbnSelchangeCombogroup()
{
	int ll_idx=((CComboBox*)GetDlgItem(IDC_COMBOGROUP))->GetCurSel();
	for (int i=0; i< m_vtpGroup.size();i++)
	{
			if (m_vtpGroup[i].idx == ll_idx)
			{
				m_groupid = m_vtpGroup[i].cameragroupid;
				break;
			}
	}
}

void CDlgChain::fnChangeTxt(void)
{
	CString lstr;
	//Dialog title
	LOADSTRING(lstr, IDS_ACSET_WINCAP);
	SetWindowText(lstr);	
 
	LOADSTRING(lstr, 	IDS_ACSET_STATIC );
	GetDlgItem( IDC_STATIC )->SetWindowTextW(lstr);

	LOADSTRING(lstr, IDS_ACSET_STPARKG);
	GetDlgItem( IDC_STPARKG )->SetWindowTextW(lstr);

	LOADSTRING(lstr,  IDS_ACSET_STGATEINFO);
	GetDlgItem( IDC_STGATEINFO )->SetWindowTextW(lstr);

	LOADSTRING(lstr, IDS_ACSET_STGROUP	 );
	GetDlgItem( IDC_STGROUP	 )->SetWindowTextW(lstr);

}
