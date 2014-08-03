// TabpageAcCam.cpp : implementation file

#include "stdafx.h"
#include "TabpageAcCam.h"
#include "curl.h"

#define DEFAULTBRAND	"ETROVISION"
#define DEFAULTMODEL	"AC-EV721H"


// CTabpageAcCam dialog

IMPLEMENT_DYNAMIC(CTabpageAcCam, CBaseDialog)

static CURL* g_curl;

CTabpageAcCam::CTabpageAcCam(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CTabpageAcCam::IDD, pParent)
{
	m_pbk = CNVRBk::GetBkObject();
}

CTabpageAcCam::~CTabpageAcCam()
{
	if(m_pToolTipCtrl) delete m_pToolTipCtrl;
}

void CTabpageAcCam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_CamList);
	DDX_Control(pDX, IDC_BUTDEL, m_butdel);
	DDX_Control(pDX, IDC_BUTADD, m_butadd);
}


BEGIN_MESSAGE_MAP(CTabpageAcCam, CDialog)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CTabpageAcCam::OnLvnItemchangedList1)
	ON_CBN_SELCHANGE(IDC_COMBOBRAND, &CTabpageAcCam::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_CHECKACTIVE, &CTabpageAcCam::OnBnClickedCheckactive)
	ON_BN_CLICKED(IDC_BUTADD, &CTabpageAcCam::OnBnClickedButadd)
	ON_CBN_SELCHANGE(IDC_COMBOMODEL, &CTabpageAcCam::OnCbnSelchangeCombomodel)
	ON_BN_CLICKED(IDC_BUTDEL, &CTabpageAcCam::OnBnClickedButdel)
END_MESSAGE_MAP()


// CTabpageAcCam message handlers

HBRUSH CTabpageAcCam::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

void CTabpageAcCam::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{	 
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;	 

	 CString ls_name = m_CamList.GetItemText( pNMListView->iItem, 1);
	 CString ls_idx = m_CamList.GetItemText( pNMListView->iItem, 2);
	 m_camidx = _ttoi(ls_idx);
	m_curitem = pNMListView->iItem;

	fnClearControl();

	 CStatic* lpName = (CStatic*)GetDlgItem(IDC_STNAME);
	 lpName->SetWindowTextW(ls_name);

	fnRetrieveData();
	*pResult = 0;
}

void CTabpageAcCam::fnRetrieveData(void)
{
		//bool lb_delete = false;
		for (int i=0; i<m_vpCamAccess.size(); i++)
		{
				if (m_vpCamAccess[i].cam_idx == m_camidx)
				{
					if (mc_parentMode=='G')	//Gate
					{
						if (m_vpCamAccess[i].gatecontrol !='Y') 					 
							return;
					}
					else
					{
						if (m_vpCamAccess[i].active_ =='Y') 
							((CButton*)(GetDlgItem( IDC_CHECKACTIVE )))->SetCheck(1);
					}
					

					((CButton*)(GetDlgItem( IDC_CHECKACTIVE )))->EnableWindow(1);
 
					//Brand Model Record
					((CComboBox*) GetDlgItem(IDC_COMBOBRAND))->EnableWindow(1);
					((CComboBox*) GetDlgItem(IDC_COMBOMODEL))->EnableWindow(1);

					CComboBox* comboBrand = (CComboBox*) GetDlgItem(IDC_COMBOBRAND);					
					int nIndex = comboBrand->FindString(0,m_vpCamAccess[i].brand);
					if (nIndex>=0) comboBrand->SetCurSel(nIndex);

					CComboBox* comboModel = (CComboBox*) GetDlgItem(IDC_COMBOMODEL);					
					nIndex = comboModel->FindString(0,m_vpCamAccess[i].model);
					if (nIndex>=0) comboModel->SetCurSel(nIndex);

	/*				CComboBox* comboRecord = (CComboBox*) GetDlgItem(IDC_COMBORECORD);					
					nIndex = comboRecord->FindString(0,m_vpCamAccess[i].recordtype);
					if (nIndex>=0) comboRecord->SetCurSel(nIndex);*/

				}
		}
	//	((CButton*)GetDlgItem(IDC_BUTDEL))->EnableWindow(lb_delete);
}

void CTabpageAcCam::OnCbnSelchangeCombo1()
{
	mb_dirty = true;
	for (int i=0; i<m_vpCamAccess.size(); i++)
	{
		if (m_vpCamAccess[i].cam_idx == m_camidx)
		{
			int litem = ((CComboBox*) GetDlgItem(IDC_COMBOBRAND))->GetCurSel();
			if (litem != CB_ERR)
			{
				CString	ls_brand;
				((CComboBox*) GetDlgItem(IDC_COMBOBRAND))->GetLBText(litem, ls_brand);
				m_vpCamAccess[i].brand = ls_brand.Trim();
			}
			
		}
	}

}

void CTabpageAcCam::fnDisplayBrand(void)
{
	CComboBox* pBrand = (CComboBox*) GetDlgItem(IDC_COMBOBRAND);
	CComboBox* pModel = (CComboBox*) GetDlgItem(IDC_COMBOMODEL);

	try
	{
		CString csBrand, csModel;
		CRecordset rBrand(psDB);

		rBrand.Open(CRecordset::forwardOnly, _T("SELECT ac_brand, ac_model FROM ecac_hbrand_model"));

		//pBrand->AddString(_T(""));
		//pModel->AddString(_T(""));

		while (!rBrand.IsEOF()) { 
			rBrand.GetFieldValue(_T("ac_brand"), csBrand);
			rBrand.GetFieldValue(_T("ac_model"), csModel);
			
			pBrand->AddString(csBrand);
			pModel->AddString(csModel);

			rBrand.MoveNext();
		}
		pBrand->SetCurSel(0);
		pModel->SetCurSel(0);
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CTabpageAcCam::fnSave(CString o_mode)
{
	CString	ls_sql;
	try{
			for (int i=0; i<m_vpCamAccess.size(); i++)
			{
				if (m_vpCamAccess[i].mode == 'A')
				{
						ls_sql.Format(_T("DELETE FROM ecac_camera WHERE camera_idx=%d; INSERT INTO ecac_camera(active_,camera_idx, access_brand, access_model, ac_control )  VALUES('%c', %d, '%s', '%s', 'Y')"), m_vpCamAccess[i].cam_idx,m_vpCamAccess[i].active_,m_vpCamAccess[i].cam_idx,  m_vpCamAccess[i].brand, m_vpCamAccess[i].model);							
						psDB->ExecuteSQL(ls_sql);

						//Set camera default info by cgi
						CStatic* lpNote = new CStatic;
						CRect		lrect(350,350,750,370);
						DWORD dwStyle = WS_CHILD | WS_VISIBLE ;
						lpNote->Create(_T(""), dwStyle, lrect, this, 200);

						CString csTxt;
						LOADSTRING(csTxt, IDS_ACSETNOTE);
						lpNote->SetWindowTextW(csTxt.GetBuffer());
						//lpNote->SetWindowTextW(_T("Setting new access control, please wait..."));
						//lpNote->MoveWindow(100,300,1200,32,true);
						lpNote->ShowWindow(true);
						fnSetTrans(m_vpCamAccess[i].cam_idx);
						lpNote->ShowWindow(false);
						lpNote->DestroyWindow();
						delete lpNote;

						//Add exising groups into ecac_privilege
						fnInsertPriv(m_vpCamAccess[i].cam_idx);
				}
				else if (m_vpCamAccess[i].mode == 'D')
				{
						//Also delete the group camera stuff
						CString ls_exesql, ls_sql1;
						ls_sql.Format(_T("DELETE FROM ecac_camera WHERE camera_idx = %d ; DELETE FROM ecac_camera_group  WHERE camera_idx = %d ; UPDATE ec_camera_group_camera SET acmap_x = NULL, acmap_y = NULL WHERE camera_idx = %d;   "),m_vpCamAccess[i].cam_idx, m_vpCamAccess[i].cam_idx, m_vpCamAccess[i].cam_idx);							
						ls_sql1.Format(_T("DELETE FROM ecac_privilege WHERE camera_idx = %d ;"),m_vpCamAccess[i].cam_idx)  ;
						ls_exesql  =  ls_sql + ls_sql1;
						psDB->ExecuteSQL(ls_exesql);						
				}
				else if (m_vpCamAccess[i].mode == 'C') //change
				{
						ls_sql.Format(_T("UPDATE ecac_camera SET access_brand = '%s',  access_model= '%s'  WHERE camera_idx = %d "),m_vpCamAccess[i].brand, m_vpCamAccess[i].model, m_vpCamAccess[i]);							
						psDB->ExecuteSQL(ls_sql);
				}
			}

			if (o_mode != _T("close"))
			{
				m_CamList.DeleteAllItems();
				fnRefreshCam();
			}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
		return ;
	}
	m_vpCamAccess.clear();
	fnRebuildCam();
}//void CTabpageAcCam::fnSave(void)


//Save for Gate Settings
void CTabpageAcCam::fnSaveGate(bool o_close)
{
	CString	ls_sql;
	try{
			for (int i=0; i<m_vpCamAccess.size(); i++)
			{
				if (m_vpCamAccess[i].mode == 'A')
				{						
						ls_sql.Format(_T("DELETE FROM ecgt_camera WHERE camera_idx=%d; INSERT INTO ecgt_camera(active_,camera_idx, access_brand, access_model, gate_control)  VALUES('%c', %d, '%s', '%s', 'Y')"), m_vpCamAccess[i].cam_idx, m_vpCamAccess[i].active_,m_vpCamAccess[i].cam_idx,  m_vpCamAccess[i].brand, m_vpCamAccess[i].model);							
						psDB->ExecuteSQL(ls_sql);

						//Set camera default info by cgi
						CStatic* lpNote = new CStatic;
						CRect		lrect(350,350,750,370);
						DWORD dwStyle = WS_CHILD | WS_VISIBLE ;
						lpNote->Create(_T(""), dwStyle, lrect, this, 200);

						CString csTxt;
						LOADSTRING(csTxt, IDS_ACSETNOTE);
						lpNote->SetWindowTextW(csTxt.GetBuffer());
						//lpNote->SetWindowTextW(_T("Setting new gate control, please wait..."));

						lpNote->ShowWindow(true);
						fnSetTrans(m_vpCamAccess[i].cam_idx);
						lpNote->ShowWindow(false);
						lpNote->DestroyWindow();
						delete lpNote;

						//Add exising groups into ecac_privilege
						fnInsertPriv(m_vpCamAccess[i].cam_idx);
				}
				else if (m_vpCamAccess[i].mode == 'D') // Delete
				{
						CString ls_sql;
						ls_sql.Format(_T("DELETE FROM ecgt_camera WHERE camera_idx = %d"), m_vpCamAccess[i].cam_idx) ;
						TRACE(ls_sql);
						psDB->ExecuteSQL(ls_sql);
 				}
				else if (m_vpCamAccess[i].mode == 'C') //change
				{
						ls_sql.Format(_T("UPDATE ecgt_camera SET access_brand = '%s',  access_model= '%s'  WHERE camera_idx = %d "),m_vpCamAccess[i].brand, m_vpCamAccess[i].model, m_vpCamAccess[i]);							
						psDB->ExecuteSQL(ls_sql);
				}
			}

			if (!o_close ) //Not Close
			{
				m_CamList.DeleteAllItems();
				fnRefreshCamGate();
			}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
		return ;
	}
	m_vpCamAccess.clear();
	fnRebuildCamGate();
}//void CTabpageAcCam::fnSave(void)


void CTabpageAcCam::OnBnClickedCheckactive()
{
 	mb_dirty = true;
}

void CTabpageAcCam::fnInitCam(void)
{
		//CString	ls_sql, ls_recordtype, ls_active, ls_db, ls_brand, ls_model, ls_ac;
		CDBVariant	varIdx;

		//Load Delete
		int	ll_posx = 250, ll_posy=30;
		m_butadd.LoadBitmaps(IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD);
		m_butadd.MoveWindow(ll_posx, ll_posy, 24,24);		
		
		m_butdel.LoadBitmaps(IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL,IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL);
		m_butdel.MoveWindow(ll_posx + 30, ll_posy, 24,24);

		//Hide active checkbox
		(CButton*)GetDlgItem(IDC_CHECKACTIVE)->ShowWindow(false);

		//Display Brand Model combo
		fnDisplayBrand();

		m_CamList.SetExtendedStyle(LVS_SHOWSELALWAYS);
		
		char	cActive[1];		
		fnRebuildCam();

		fnLocalize();

		//Tooltips
		m_pToolTipCtrl = new CToolTipCtrl;
		if (!m_pToolTipCtrl->Create(this))		
			TRACE(_T("Unable To create ToolTip\n"));

		CString ls_tooltip;
		//ls_tooltip.LoadString(CGS::g_hResStr, IDS_ACATTACH);
		LOADSTRING(ls_tooltip, IDS_ACATTACH);
		m_pToolTipCtrl->AddTool(&m_butadd, ls_tooltip);

		//ls_tooltip.LoadString(CGS::g_hResStr, IDS_ACDETACH);
		LOADSTRING(ls_tooltip, IDS_ACDETACH);
		m_pToolTipCtrl->AddTool(&m_butdel, ls_tooltip);

		CRect	lrect, llrect;
		
		m_CamList.GetClientRect(lrect);
 		m_CamList.MoveWindow(20, 59, lrect.Width() + 23, lrect.Height(),true);
}



void CTabpageAcCam::fnInitCamGate(void)
{
		CString	ls_sql, ls_recordtype,  ls_db, ls_brand, ls_model, ls_gcontrol;
		CDBVariant	varIdx;

		//Load Delete
		int	ll_posx = 250, ll_posy=30;
		m_butadd.LoadBitmaps(IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD);
		m_butadd.MoveWindow(ll_posx, ll_posy, 24,24);		
		
		m_butdel.LoadBitmaps(IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL,IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL);
		m_butdel.MoveWindow(ll_posx + 30, ll_posy, 24,24);

		//Hide active checkbox
		(CButton*)GetDlgItem(IDC_CHECKACTIVE)->ShowWindow(false);

		//Display Brand Model combo
		fnDisplayBrand();

		m_CamList.SetExtendedStyle(LVS_SHOWSELALWAYS);
		
		char	cActive[1];		
		fnRebuildCamGate();
		fnLocalize();
//		CRecordset	m_rsCam(psDB);
//		ls_sql = _T("SELECT ec_camera.camera_idx, ecgt_camera.access_brand, ecgt_camera.access_model, ecgt_camera.gate_control  FROM ec_camera LEFT OUTER JOIN ecgt_camera  ON ec_camera.camera_idx =  ecgt_camera.camera_idx ORDER BY camera_idx");
//		m_rsCam.Open(CRecordset::dynaset,ls_sql);
//
//		int ll_rows = m_rsCam.GetRecordCount();
//		if (ll_rows > 0) 
//		{
//			m_rsCam.MoveFirst();
//
//			STRCAM	lCamAc;
//			m_vpCamAccess.clear();
//
//			while(!m_rsCam.IsEOF()   ) 
//			{ 
//				m_rsCam.GetFieldValue(_T("camera_idx"), varIdx, SQL_C_SLONG);
//				m_rsCam.GetFieldValue(_T("access_brand"), ls_brand);
//				m_rsCam.GetFieldValue(_T("access_model"), ls_model);
//				//m_rsCam.GetFieldValue(_T("record_type"), ls_recordtype);
//				//m_rsCam.GetFieldValue(_T("save_db"), ls_db);
//				m_rsCam.GetFieldValue(_T("gate_control"),ls_gcontrol);
//				//m_rsCam.GetFieldValue(_T("active_"),ls_active);
//
//				lCamAc.cam_idx = varIdx.m_iVal;
//		 
//				//Gate Control
//				if (ls_gcontrol == _T("Y")) lCamAc.gatecontrol = 'Y';
//				else
//					lCamAc.gatecontrol = 'N';
//
//				lCamAc.brand = ls_brand;
//				lCamAc.model = ls_model;
//	 		
//				//Construct the vector
//				m_vpCamAccess.push_back(lCamAc);
//				//m_vpCamAccess.push_back(lCamAc);
//				m_rsCam.MoveNext();
//		}
//}
		//Tooltips
		m_pToolTipCtrl = new CToolTipCtrl;
		if (!m_pToolTipCtrl->Create(this))		
			TRACE(_T("Unable To create ToolTip\n"));

		CString ls_tooltip;
		//ls_tooltip.LoadString(CGS::g_hResStr, IDS_GTATTACH);
		LOADSTRING( ls_tooltip, IDS_GTATTACH);		
		m_pToolTipCtrl->AddTool(&m_butadd, ls_tooltip);

		//ls_tooltip.LoadString(CGS::g_hResStr, IDS_GTDETACH);
		LOADSTRING(ls_tooltip, IDS_GTDETACH);
		m_pToolTipCtrl->AddTool(&m_butdel, ls_tooltip);

		CRect	lrect, llrect;
		
		m_CamList.GetClientRect(lrect);
 		m_CamList.MoveWindow(20, 59, lrect.Width() + 23, lrect.Height(),true);
}


void CTabpageAcCam::OnBnClickedButadd()
{
		bool	lb_found = false;
		mb_dirty = true;

		if (m_camidx<=0 || m_camidx>300 ) return;  //prevent saved an weird camera

		m_CamList.SetFocus();
		//Check if the cam already in the list, if not, then add into the
		for (int i=0; i<m_vpCamAccess.size(); i++)
		{
			if (m_vpCamAccess[i].cam_idx == m_camidx)
			{
				if (m_vpCamAccess[i].gatecontrol == 'Y')
				{
					lb_found = true;
					break;
				}
			}
		}

		if (lb_found) return;

				STRCAM	lCamAc;
				lCamAc.active_ = 'Y';
				lCamAc.mode = 'A'; //Set to add mode
				lCamAc.cam_idx = m_camidx;
				lCamAc.brand = DEFAULTBRAND;
				lCamAc.model = DEFAULTMODEL;

				m_vpCamAccess.push_back(lCamAc);
	 
				((CButton*) GetDlgItem(IDC_CHECKACTIVE))->SetCheck(1);
				((CButton*) GetDlgItem(IDC_CHECKACTIVE))->EnableWindow(1);
									
				((CComboBox*) GetDlgItem(IDC_COMBOBRAND))->EnableWindow(1);
				((CComboBox*) GetDlgItem(IDC_COMBOBRAND))->SetCurSel(0);
 				
				((CComboBox*) GetDlgItem(IDC_COMBOMODEL))->EnableWindow(1);
				((CComboBox*) GetDlgItem(IDC_COMBOMODEL))->SetCurSel(0);
				if (m_curitem>=0)
				{
						LVITEM lvItem;
						lvItem.mask = LVIF_IMAGE ;
						lvItem.iItem = m_curitem;
						lvItem.pszText = _T("");
						lvItem.iSubItem = 0;
						lvItem.iImage = 0; //blank
						int ll_ret = m_CamList.SetItem(&lvItem);
				}
		

}

void CTabpageAcCam::OnCbnSelchangeCombomodel()
{
		mb_dirty = true;
		for (int i=0; i<m_vpCamAccess.size(); i++)
		{
			if (m_vpCamAccess[i].cam_idx == m_camidx)
			{
				int litem = ((CComboBox*) GetDlgItem(IDC_COMBOMODEL))->GetCurSel();
				if (litem != CB_ERR)
				{
					CString	ls_model;
					((CComboBox*) GetDlgItem(IDC_COMBOMODEL))->GetLBText(litem, ls_model);
					m_vpCamAccess[i].model = ls_model.Trim();
				}
				//change mode - if not ADD mode, change to CHANGE mode for saving...
				if (	m_vpCamAccess[i].mode !='A') m_vpCamAccess[i].mode = 'C';
			}//if (m_vpCamAccess[i].cam_idx == m_camidx)

		}
}

void CTabpageAcCam::fnPopulateCam(void)
{
	CString ls_name, ls_ip, ls_desc, str;
	TCHAR lc_desc[255];
	CDBVariant varIdx;
	CDBVariant varCamidx, varid, varNull;
	CString ls_sql1, ls_sql2, ls_sql3, ls_sql,  ls_full, ls_brand, ls_model, ls_accontrol;
	int	ci=0;
	int 	nItem;

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

		if(i == 0)
				{ListColumn.cx = 16;}			
		else if(i == 1 )
				{ListColumn.cx = 220;}	
		else if(i == 2)
				{ListColumn.cx = 30;}
			
			ListColumn.pszText = ListTitles[i];
			//m_CamList->InsertColumn(i, &ListColumn);
			m_CamList.InsertColumn(i, &ListColumn);
		}

		DWORD	dwStyle = m_CamList.GetStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;
		m_CamList.SetExtendedStyle(dwStyle);
		m_CamList.ModifyStyle(0, LVS_SINGLESEL);
 

		//Image Checkbox
		m_SmallImg.Create(16, 16, ILC_COLOR | ILC_MASK, 0, 1);
 		CBitmap bIcon;
		bIcon.LoadBitmapW(IDB_KEY);
		m_SmallImg.Add(&bIcon, RGB(0, 0, 0));
				
		m_CamList.SetImageList(&m_SmallImg, LVSIL_SMALL);

		CRecordset m_rsCam(psDB);
		 		 
		ls_sql = _T("SELECT ec_camera.video_format, ec_camera.camera_idx, ec_camera.cameraname, ec_camera.ipaddress,  ecac_camera.active_ ,ecac_camera.access_brand, ecac_camera.access_model, ecac_camera.ac_control");
		ls_sql += _T(" FROM ec_camera LEFT OUTER JOIN ecac_camera ON ecac_camera.camera_idx = ec_camera.camera_idx");
		m_rsCam.Open(CRecordset::dynaset,ls_sql);

 		int cnt=0;	
		CString	ls_active;
		
		int ll_rows = m_rsCam.GetRecordCount();
		if (ll_rows>0) m_rsCam.MoveFirst();

		while(!m_rsCam.IsEOF()   ) 
		{ 
			m_rsCam.GetFieldValue(_T("video_format"), str);
			if (m_pbk->GetDevice(str) == eNVR || m_pbk->GetDevice(str) == IPVSTATION)
			{
				if (!m_pbk->IsChannel(str))
				{
					m_rsCam.MoveNext();
					continue;
				}
			}

			m_rsCam.GetFieldValue(_T("camera_idx"), varIdx, SQL_C_SLONG);		
			m_rsCam.GetFieldValue(_T("cameraname"), ls_name);

			m_rsCam.GetFieldValue(_T("ipaddress"), ls_ip);

			m_rsCam.GetFieldValue(_T("active_"), ls_active);

			m_rsCam.GetFieldValue(_T("access_brand"), ls_brand);
			m_rsCam.GetFieldValue(_T("access_model"), ls_model);
			m_rsCam.GetFieldValue(_T("ac_control"), ls_accontrol);
			ls_full.Format(_T("(%d) - %s / %s"), varIdx.m_iVal, ls_name, ls_ip);
			wsprintf(lc_desc,_T("(%d) %s - %s"), varIdx.m_iVal,ls_name.Trim(), ls_ip.Trim());			

			LVITEMW	pLitem;
			STRCAM*	lpStrCam = new STRCAM;	
			pLitem.mask = LVIF_PARAM | LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
			pLitem.iItem = cnt;

			//ac control
		if (ls_accontrol.Trim().MakeUpper() == _T("Y"))
			lpStrCam->gatecontrol = 'Y';
			else
				lpStrCam->gatecontrol = 'N';

			//Active
			if (ls_active.Trim().MakeUpper() == _T("Y"))
				lpStrCam->active_ = 'Y';
			else
				lpStrCam->active_ = 'N';

			lpStrCam->cam_idx = varIdx.m_iVal;
			lpStrCam->brand = ls_brand.Trim().MakeUpper();
			lpStrCam->model = ls_model.Trim().MakeUpper();

			pLitem.lParam = (LPARAM)lpStrCam;

			if (lpStrCam->gatecontrol == 'Y')
				nItem =  m_CamList.InsertItem( cnt, _T(""), 0); //, 0, 0, pLitem.lParam);
			else
				nItem =  m_CamList.InsertItem( cnt, _T(""), 1);
				
			CString ls_idx;
			ls_idx.Format(_T("%d"),varIdx.m_iVal);

			m_CamList.SetItemText(nItem, 1, lc_desc);
			m_CamList.SetItemText(nItem, 2, ls_idx);
	
			cnt++;
			delete lpStrCam;
			m_rsCam.MoveNext();
		}	
	} //try
		catch(CDBException * p) 	{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}	
	return ;
}

// For Parking Gate Control System
void CTabpageAcCam::fnPopulateCamGate(void)
{
	CString ls_name, ls_ip, ls_desc, ls_gcontrol;
	TCHAR lc_desc[255];
	CDBVariant varIdx;
	CDBVariant varCamidx, varid, varNull;
	CString ls_sql1, ls_sql2, ls_sql3, ls_sql,  ls_full, ls_brand, ls_model;
	int	ci=0;
	int 	nItem;

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

		if(i == 0)
				{ListColumn.cx = 16;}			
		else if(i == 1 )
				{ListColumn.cx = 220;}	
		else if(i == 2)
				{ListColumn.cx = 30;}
			
			ListColumn.pszText = ListTitles[i];
			//m_CamList->InsertColumn(i, &ListColumn);
			m_CamList.InsertColumn(i, &ListColumn);
		}

		DWORD	dwStyle = m_CamList.GetStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;
		m_CamList.SetExtendedStyle(dwStyle);
		m_CamList.ModifyStyle(0, LVS_SINGLESEL);
 
		//Image Checkbox
		m_SmallImg.Create(16, 16, ILC_COLOR | ILC_MASK, 0, 1);
 		CBitmap bIcon;
		bIcon.LoadBitmapW(IDB_KEY);
		m_SmallImg.Add(&bIcon, RGB(0, 0, 0));
				
		m_CamList.SetImageList(&m_SmallImg, LVSIL_SMALL);

		CRecordset m_rsCam(psDB);
		 		 
		ls_sql = _T("SELECT ec_camera.video_format, ec_camera.camera_idx, ec_camera.cameraname, ec_camera.ipaddress ,ecgt_camera.access_brand, ecgt_camera.access_model,  ecgt_camera.gate_control, ecgt_camera.active_");
		ls_sql += _T(" FROM ec_camera LEFT OUTER JOIN ecgt_camera ON ecgt_camera.camera_idx = ec_camera.camera_idx");
		TRACE(ls_sql);
		m_rsCam.Open(CRecordset::dynaset,ls_sql);

 		int cnt=0;	
		CString	ls_active,str;
		
		int ll_rows = m_rsCam.GetRecordCount();
		if (ll_rows>0) m_rsCam.MoveFirst();

		while(!m_rsCam.IsEOF()   ) 
		{ 
			m_rsCam.GetFieldValue(_T("video_format"), str);
			if (m_pbk->GetDevice(str) == eNVR || m_pbk->GetDevice(str) == IPVSTATION)
			{
				if (!m_pbk->IsChannel(str))
				{
					m_rsCam.MoveNext();
					continue;
				}
			}

			m_rsCam.GetFieldValue(_T("camera_idx"), varIdx, SQL_C_SLONG);		
			m_rsCam.GetFieldValue(_T("cameraname"), ls_name);

			m_rsCam.GetFieldValue(_T("ipaddress"), ls_ip);

			m_rsCam.GetFieldValue(_T("access_brand"), ls_brand);
			m_rsCam.GetFieldValue(_T("access_model"), ls_model);

			m_rsCam.GetFieldValue(_T("gate_control"), ls_gcontrol);
			m_rsCam.GetFieldValue(_T("active_"), ls_active);

			ls_full.Format(_T("(%d) - %s / %s"), varIdx.m_iVal, ls_name, ls_ip);
			wsprintf(lc_desc,_T("(%d) %s - %s"), varIdx.m_iVal,ls_name.Trim(), ls_ip.Trim());			

			LVITEMW	pLitem;
			STRCAM*	lpStrCam = new STRCAM;	
			pLitem.mask = LVIF_PARAM | LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
			pLitem.iItem = cnt;

			//Gate Control
			if (ls_gcontrol.Trim().MakeUpper() == _T("Y"))
				lpStrCam->gatecontrol = 'Y';
			else
				lpStrCam->gatecontrol = 'N';

			lpStrCam->cam_idx = varIdx.m_iVal;
			lpStrCam->brand = ls_brand.Trim().MakeUpper();
			lpStrCam->model = ls_model.Trim().MakeUpper();

			pLitem.lParam = (LPARAM)lpStrCam;

			if (lpStrCam->gatecontrol == 'Y')
				nItem =  m_CamList.InsertItem( cnt, _T(""), 0); //
			else
				nItem =  m_CamList.InsertItem( cnt, _T(""), 1);
				
			CString ls_idx;
			ls_idx.Format(_T("%d"),varIdx.m_iVal);

			m_CamList.SetItemText(nItem, 1, lc_desc);
			m_CamList.SetItemText(nItem, 2, ls_idx);
	
			cnt++;
			delete lpStrCam;
			m_rsCam.MoveNext();
		}	
	} //try
		catch(CDBException * p) 	{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}	
	return ;
}


//Delete camera access control
void CTabpageAcCam::OnBnClickedButdel()
{
	if (m_camidx<=0 || m_camidx>300 ) return;  

	for (int i=0; i<m_vpCamAccess.size(); i++)
	{
		if (m_vpCamAccess[i].cam_idx == m_camidx)
		{
			mb_dirty = true;
			m_vpCamAccess[i].mode = 'D'; //mark it as delete
		}
	}


	
	if (m_curitem>=0)
	{	 
			m_CamList.SetItem(m_curitem, 0, 0, _T(""),1,0,0, NULL);
			((CComboBox*) GetDlgItem(IDC_COMBOBRAND))->SetCurSel(0);
			((CComboBox*) GetDlgItem(IDC_COMBOBRAND))->EnableWindow(0);

			((CComboBox*) GetDlgItem(IDC_COMBOMODEL))->SetCurSel(0);
			((CComboBox*) GetDlgItem(IDC_COMBOMODEL))->EnableWindow(0);
			((CButton*)(GetDlgItem( IDC_CHECKACTIVE )))->SetCheck( 0 );
			((CButton*)(GetDlgItem( IDC_CHECKACTIVE )))->EnableWindow(0);

					LVITEM lvItem;
		lvItem.mask = LVIF_IMAGE ;
		lvItem.iItem = m_curitem;
		lvItem.pszText = _T("");
		lvItem.iSubItem = 0;
		lvItem.iImage = 2; //blank
		int ll_ret = m_CamList.SetItem(&lvItem);
	}
}

void CTabpageAcCam::fnRefreshCam(void)
{
	 	int cnt=0;	
		TCHAR lc_desc[255];
		CDBVariant varIdx;
		CDBVariant varCamidx, varid, varNull;
		CString ls_sql1, ls_sql2, ls_sql3, ls_sql,  ls_full, ls_name, ls_active, ls_ip,str;
		
		try{
		m_CamList.DeleteAllItems();
		CRecordset m_rsCam(psDB);
		 		 
		ls_sql = _T("SELECT ec_camera.video_format, ec_camera.camera_idx, ec_camera.cameraname, ec_camera.ipaddress,  ecac_camera.ac_control ");
		ls_sql += _T(" FROM ec_camera LEFT OUTER JOIN ecac_camera ON ecac_camera.camera_idx = ec_camera.camera_idx");
		m_rsCam.Open(CRecordset::dynaset,ls_sql);


		int ll_rows = m_rsCam.GetRecordCount();
		if (ll_rows>0) m_rsCam.MoveFirst();

		while(!m_rsCam.IsEOF()   ) 
		{ 
			m_rsCam.GetFieldValue(_T("video_format"), str);
			if (m_pbk->GetDevice(str) == eNVR || m_pbk->GetDevice(str) == IPVSTATION)
			{
				if (!m_pbk->IsChannel(str))
				{
					m_rsCam.MoveNext();
					continue;
				}
			}
			m_rsCam.GetFieldValue(_T("camera_idx"), varIdx, SQL_C_SLONG);		
			m_rsCam.GetFieldValue(_T("cameraname"), ls_name);

			m_rsCam.GetFieldValue(_T("ipaddress"), ls_ip);
			m_rsCam.GetFieldValue(_T("ac_control"), ls_active);

			ls_full.Format(_T("(%d) - %s / %s"), varIdx.m_iVal, ls_name, ls_ip);
			wsprintf(lc_desc,_T("(%d) %s - %s"), varIdx.m_iVal,ls_name.Trim(), ls_ip.Trim());
			LVITEMW* pLitem = new LVITEM;
			pLitem->mask = LVIF_PARAM | LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
			pLitem->iItem = cnt;

			int 	nItem;
			if (ls_active == _T("Y"))
				nItem =  m_CamList.InsertItem(cnt, _T(""), 0);
			else
				nItem =  m_CamList.InsertItem(cnt, _T(""), 1);
				
			CString ls_idx;
			ls_idx.Format(_T("%d"),varIdx.m_iVal);

			m_CamList.SetItemText(nItem, 1, lc_desc);
			m_CamList.SetItemText(nItem, 2, ls_idx);
 
			cnt++;
			m_rsCam.MoveNext();
		}	
	} //try
		catch(CDBException * p) 	{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}	

	fnClearControl();
	return ;
}


void CTabpageAcCam::fnRefreshCamGate(void)
{
	 	int cnt=0;	
		TCHAR lc_desc[255];
		CDBVariant varIdx;
		CDBVariant varCamidx, varid, varNull;
		CString ls_sql1, ls_sql2, ls_sql3, ls_sql,  ls_full, ls_name, ls_gcontrol, ls_ip, str;
		
		try{
		m_CamList.DeleteAllItems();
		CRecordset m_rsCam(psDB);
		 		 
		ls_sql = _T("SELECT ec_camera.video_format, ec_camera.camera_idx, ec_camera.cameraname, ec_camera.ipaddress,  ecgt_camera.gate_control ");
		ls_sql += _T(" FROM ec_camera LEFT OUTER JOIN ecgt_camera ON ecgt_camera.camera_idx = ec_camera.camera_idx");
		m_rsCam.Open(CRecordset::dynaset,ls_sql);


		int ll_rows = m_rsCam.GetRecordCount();
		if (ll_rows>0) m_rsCam.MoveFirst();

		while(!m_rsCam.IsEOF()   ) 
		{ 
			m_rsCam.GetFieldValue(_T("video_format"), str);
			if (m_pbk->GetDevice(str) == eNVR || m_pbk->GetDevice(str) == IPVSTATION)
			{
				if (!m_pbk->IsChannel(str))
				{
					m_rsCam.MoveNext();
					continue;
				}
			}
			m_rsCam.GetFieldValue(_T("camera_idx"), varIdx, SQL_C_SLONG);		
			m_rsCam.GetFieldValue(_T("cameraname"), ls_name);

			m_rsCam.GetFieldValue(_T("ipaddress"), ls_ip);
			m_rsCam.GetFieldValue(_T("gate_control"), ls_gcontrol);

			ls_full.Format(_T("(%d) - %s / %s"), varIdx.m_iVal, ls_name, ls_ip);
			wsprintf(lc_desc,_T("(%d) %s - %s"), varIdx.m_iVal,ls_name.Trim(), ls_ip.Trim());
			LVITEMW* pLitem = new LVITEM;
			pLitem->mask = LVIF_PARAM | LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
			pLitem->iItem = cnt;

			int 	nItem;
			if (ls_gcontrol == _T("Y"))
				nItem =  m_CamList.InsertItem(cnt, _T(""), 0);
			else
				nItem =  m_CamList.InsertItem(cnt, _T(""), 1);
				
			CString ls_idx;
			ls_idx.Format(_T("%d"),varIdx.m_iVal);

			m_CamList.SetItemText(nItem, 1, lc_desc);
			m_CamList.SetItemText(nItem, 2, ls_idx);
 
			cnt++;
			m_rsCam.MoveNext();
		}	
	} //try
		catch(CDBException * p) 	{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}	

	fnClearControl();
	return ;
}

void CTabpageAcCam::fnClearControl(void)
{
	 CStatic* lpName = (CStatic*)GetDlgItem(IDC_STNAME);
	 lpName->SetWindowTextW(_T(""));

	 ((CButton*)(GetDlgItem( IDC_CHECKACTIVE )))->SetCheck( 0 );
	((CButton*)(GetDlgItem( IDC_CHECKACTIVE )))->EnableWindow(0);
 
	((CComboBox*) GetDlgItem(IDC_COMBOBRAND))->SetCurSel(0);
	((CComboBox*) GetDlgItem(IDC_COMBOBRAND))->EnableWindow(0);

	((CComboBox*) GetDlgItem(IDC_COMBOMODEL))->SetCurSel(0);
	((CComboBox*) GetDlgItem(IDC_COMBOMODEL))->EnableWindow(0);

}

BOOL CTabpageAcCam::PreTranslateMessage(MSG* pMsg)
{

	if ( m_pToolTipCtrl != NULL)
		m_pToolTipCtrl->RelayEvent(pMsg);

	return CBaseDialog::PreTranslateMessage(pMsg);
}

// Set transparent mode cgi 
void CTabpageAcCam::fnSetTrans(int oCamidx)
{
		if (oCamidx <=0) return;

		CString	ls_sql, ls_ipaddress, ls_user, ls_pw;
		CRecordset		rsCam(psDB);
		CString	ls_defualt = _T("/config/rs485.cgi?proto=Transparent&baudrate=9600&databits=8&stopbits=1&comm=RS485&Parity=None&");

		ls_sql.Format(_T("SELECT ipaddress, username, password FROM ec_camera WHERE camera_idx = %d"),  oCamidx);

		rsCam.Open(CRecordset::dynamic, ls_sql);

		while (!rsCam.IsEOF()) { 

			//ipAddress
			rsCam.GetFieldValue(_T("ipaddress"), ls_ipaddress);
			rsCam.GetFieldValue(_T("username"), ls_user);
			rsCam.GetFieldValue(_T("password"), ls_pw);
	 	
  			CString csURL = _T("http://") + ls_ipaddress  + ls_defualt;
		 
			char *chUrl = new char[csURL.GetLength() + 1];
			wcstombs_s(NULL, chUrl, csURL.GetLength() + 1, csURL, _TRUNCATE);

			CString csUserPwd = ls_user + _T(":") + ls_pw;
			char *chUserPwd = new char[csUserPwd.GetLength() + 1];
			wcstombs_s(NULL, chUserPwd, csUserPwd.GetLength() + 1, csUserPwd, _TRUNCATE);

			CURLcode gCode = curl_global_init(CURL_GLOBAL_ALL);
			CURL* ms_curl = curl_easy_init();

			if(ms_curl == NULL)
					return;

			CURLINFO	m_curlInfo;

			curl_easy_setopt(ms_curl, CURLOPT_URL, chUrl);
			curl_easy_setopt(ms_curl, CURLOPT_USERPWD, chUserPwd);
			CURLcode pCode = curl_easy_perform(ms_curl); 

			curl_easy_cleanup(ms_curl);
			
			delete chUrl;
			delete chUserPwd;
			return ;
		}

		return;

}


void CTabpageAcCam::fnInsertPriv(int oCamidx)
{
		CString	ls_dcode, ls_exesql=_T("");
		CRecordset	m_rs(psDB);
		CString ls_sql = _T("SELECT access_group_code FROM ecac_camera_group");
		m_rs.Open(CRecordset::dynaset,ls_sql);

		int ll_rows = m_rs.GetRecordCount();
		if (ll_rows > 0) 
		{
				m_rs.MoveFirst();		 
				while(!m_rs.IsEOF()   )  //loop into every row of ecac_camera_group
				{ 						
						m_rs.GetFieldValue(_T("access_group_code"), ls_dcode);
						CString lstr=_T("");
						
						lstr.Format(_T("DELETE FROM ecac_privilege WHERE camera_idx=%d AND access_group_code='%s' ; INSERT INTO ecac_privilege(  camera_idx,  access_group_code  , privilege_) VALUES (%d, '%s', 'Y' );  "), oCamidx, ls_dcode, oCamidx, ls_dcode);
						ls_exesql += lstr;
						m_rs.MoveNext();
				}
		}
		if (ls_exesql != _T(""))
		{
				 	TRACE(ls_exesql);
					psDB->ExecuteSQL(ls_exesql);
		}		
}


void CTabpageAcCam::fnRebuildCam()
{
		CString	ls_sql, ls_recordtype, ls_active, ls_db, ls_brand, ls_model, ls_ac;
		CDBVariant	varIdx;	
		CRecordset	m_rsCam(psDB);
		ls_sql = _T("SELECT * FROM ecac_camera  ORDER BY camera_idx");
		m_rsCam.Open(CRecordset::dynaset,ls_sql);

		int ll_rows = m_rsCam.GetRecordCount();
		if (ll_rows > 0) 
		{
			m_rsCam.MoveFirst();

			STRCAM	lCamAc;
			m_vpCamAccess.clear();

			while(!m_rsCam.IsEOF()   ) 
			{ 
				m_rsCam.GetFieldValue(_T("camera_idx"), varIdx, SQL_C_SLONG);
				m_rsCam.GetFieldValue(_T("access_brand"), ls_brand);
				m_rsCam.GetFieldValue(_T("access_model"), ls_model);
				m_rsCam.GetFieldValue(_T("record_type"), ls_recordtype);
				m_rsCam.GetFieldValue(_T("save_db"), ls_db);
				m_rsCam.GetFieldValue(_T("ac_control"), ls_ac);
				m_rsCam.GetFieldValue(_T("active_"),ls_active);

				lCamAc.cam_idx = varIdx.m_iVal;
				if (ls_active == _T("N")) lCamAc.active_ = 'N';
				else
					lCamAc.active_ = 'Y';

				if (ls_ac == 'Y')
					lCamAc.gatecontrol = 'Y';
				else
					lCamAc.gatecontrol = 'N';

				lCamAc.brand = ls_brand;
				lCamAc.model = ls_model;
	 		
				//Construct the vector
				m_vpCamAccess.push_back(lCamAc);
				//m_vpCamAccess.push_back(lCamAc);
				m_rsCam.MoveNext();
		}
		}


}


void CTabpageAcCam::fnRebuildCamGate()
{
		CString	ls_sql, ls_recordtype,  ls_db, ls_brand, ls_model, ls_gcontrol, str;
		CDBVariant	varIdx;
		CRecordset	m_rsCam(psDB);
		ls_sql = _T("SELECT ec_camera.video_format, ec_camera.camera_idx, ecgt_camera.access_brand, ecgt_camera.access_model, ecgt_camera.gate_control  FROM ec_camera LEFT OUTER JOIN ecgt_camera  ON ec_camera.camera_idx =  ecgt_camera.camera_idx ORDER BY camera_idx");
		m_rsCam.Open(CRecordset::dynaset,ls_sql);
		
		int ll_rows = m_rsCam.GetRecordCount();
		if (ll_rows > 0) 
		{
			m_rsCam.MoveFirst();

			STRCAM	lCamAc;
			m_vpCamAccess.clear();

			while(!m_rsCam.IsEOF()   ) 
			{ 
				m_rsCam.GetFieldValue(_T("video_format"), str);
				if (m_pbk->GetDevice(str) == eNVR || m_pbk->GetDevice(str) == IPVSTATION)
				{
					if (!m_pbk->IsChannel(str))
					{
						m_rsCam.MoveNext();
						continue;
					}
				}

				m_rsCam.GetFieldValue(_T("camera_idx"), varIdx, SQL_C_SLONG);
				m_rsCam.GetFieldValue(_T("access_brand"), ls_brand);
				m_rsCam.GetFieldValue(_T("access_model"), ls_model);
				m_rsCam.GetFieldValue(_T("gate_control"),ls_gcontrol);
				lCamAc.cam_idx = varIdx.m_iVal;
		 
				//Gate Control
				if (ls_gcontrol == _T("Y")) lCamAc.gatecontrol = 'Y';
				else
					lCamAc.gatecontrol = 'N';

				lCamAc.brand = ls_brand;
				lCamAc.model = ls_model;
	 		
				//Construct the vector
				m_vpCamAccess.push_back(lCamAc);
				//m_vpCamAccess.push_back(lCamAc);
				m_rsCam.MoveNext();
		}
}


}

//Localization
void CTabpageAcCam::fnLocalize()
{	
		CString ls_loc;
		//camera list
		LOADSTRING(ls_loc, IDS_STCAMLIST);
		GetDlgItem( IDC_STCAMLIST )->SetWindowTextW(ls_loc);

		//group box access control
		if (mc_parentMode=='G')	//Gate Control 
		{
				LOADSTRING(ls_loc,	IDS_GATE_CONTROL);
				GetDlgItem( IDC_STGROUPAC )->SetWindowTextW(ls_loc);			
		}
		else
		{
				LOADSTRING(ls_loc,IDS_ACSET_STPARKG);
				GetDlgItem( IDC_STGROUPAC )->SetWindowTextW(ls_loc);			
		}

		LOADSTRING(ls_loc, IDS_AC_TAB_BRAND);
		GetDlgItem( IDC_STBRAND )->SetWindowTextW(ls_loc);

		LOADSTRING(ls_loc, IDS_AC_TAB_MODEL);
		GetDlgItem( IDC_STMODEL )->SetWindowTextW(ls_loc);

}