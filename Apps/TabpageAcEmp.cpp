// TabpageAcEmp.cpp : implementation file
//

#include "stdafx.h"
#include "TabpageAcEmp.h"
#include "MainFrm.h"

#define GetRandom( min, max ) ((rand() % (int)(((max) + 1) - (min))) + (min))

// CTabpageAcEmp dialog

IMPLEMENT_DYNAMIC(CTabpageAcEmp, CBaseDialog)

	/*struct STREMP{
		char	csDept[50];
		char	csName[50];
		char		cFlag;		
	};	*/

CTabpageAcEmp::CTabpageAcEmp(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CTabpageAcEmp::IDD, pParent)
{
 
}

CTabpageAcEmp::~CTabpageAcEmp()
{
	fnFreeMemory();
}

void CTabpageAcEmp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREEEMP, m_tree);
	DDX_Control(pDX, IDC_BUTADDDEPT, m_butdeptadd);
	DDX_Control(pDX, IDC_BUTDEPTMOD, m_butdeptmod);
	DDX_Control(pDX, IDC_BUTDELDEPT, m_butdeptdel);

	DDX_Control(pDX, IDC_BUTEMPADD, m_butempadd);
	DDX_Control(pDX, IDC_BUTEMPMOD, m_butempmod);
	DDX_Control(pDX, IDC_BUTEMPDEL, m_butempdel);
	DDX_Control(pDX, IDC_BUTREFRESH, m_butRefresh);
	//	DDX_Control(pDX, IDC_EDITDEPTNAME, m_editDept);
	DDX_Control(pDX, IDC_COMBOGATE, m_comboGate);
	DDX_Control(pDX, IDC_STPIC, m_pic);
}


BEGIN_MESSAGE_MAP(CTabpageAcEmp, CBaseDialog)

	ON_NOTIFY(TVN_SELCHANGED, IDC_TREEEMP, &CTabpageAcEmp::OnTvnSelchangedTreeemp)
	ON_BN_CLICKED(IDC_BUTADDDEPT, &CTabpageAcEmp::OnBnClickedButdeptadd)
	ON_BN_CLICKED(IDC_BUTDELDEPT, &CTabpageAcEmp::OnBnClickedButdeptdel)
	ON_BN_CLICKED(IDC_BUTEMPADD, &CTabpageAcEmp::OnBnClickedButempadd)
	ON_BN_CLICKED(IDC_BUTEMPDEL, &CTabpageAcEmp::OnBnClickedButempdel)
	ON_BN_CLICKED(IDC_BUTDEPTMOD, &CTabpageAcEmp::OnBnClickedButdeptmod)

	ON_BN_CLICKED(IDC_BUTEMPMOD, &CTabpageAcEmp::OnBnClickedButempmod)
//	ON_NOTIFY(TVN_ITEMCHANGED, IDC_TREEEMP, &CTabpageAcEmp::OnTvnItemChangedTreeemp)
//ON_NOTIFY(NM_CLICK, IDC_TREEEMP, &CTabpageAcEmp::OnNMClickTreeemp)
ON_CBN_SELCHANGE(IDC_COMBOGATE, &CTabpageAcEmp::OnCbnSelchangeCombogate)
 
ON_BN_CLICKED(IDC_PCHECK1, &CTabpageAcEmp::OnBnClickedPcheck1)
ON_BN_CLICKED(IDC_PCHECK2, &CTabpageAcEmp::OnBnClickedPcheck2)
ON_BN_CLICKED(IDC_PCHECK3, &CTabpageAcEmp::OnBnClickedPcheck3)
ON_BN_CLICKED(IDC_PCHECK4, &CTabpageAcEmp::OnBnClickedPcheck4)
ON_BN_CLICKED(IDC_PCHECK5, &CTabpageAcEmp::OnBnClickedPcheck5)
ON_BN_CLICKED(IDC_PCHECK6, &CTabpageAcEmp::OnBnClickedPcheck6)
ON_BN_CLICKED(IDC_PCHECK7, &CTabpageAcEmp::OnBnClickedPcheck7)
ON_BN_CLICKED(IDC_PCHECK8, &CTabpageAcEmp::OnBnClickedPcheck8)
ON_BN_CLICKED(IDC_PCHECK9, &CTabpageAcEmp::OnBnClickedPcheck9)
ON_BN_CLICKED(IDC_PCHECK10, &CTabpageAcEmp::OnBnClickedPcheck10)
ON_BN_CLICKED(IDC_BUTREFRESH, &CTabpageAcEmp::OnBnClickedButrefresh)
END_MESSAGE_MAP()


BOOL CTabpageAcEmp::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	DWORD dwStyle =  TVS_LINESATROOT|WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASBUTTONS |  TVS_HASLINES|  TVS_SHOWSELALWAYS;
	//m_tree.SetBkColor(GLOBALTREEBKCOLOR);
	
	m_editDept = (CEdit*)GetDlgItem(IDC_EDITDEPTNAME);
	//m_editDept = m_tree.EditLabel( m_tree.GetSelectedItem());

	int lposx=25, lposy=15;

	m_butdeptadd.LoadBitmaps(IDB_DEPTADD,IDB_DEPTADD,IDB_DEPTADD,IDB_DEPTADD);
	m_butdeptadd.MoveWindow(lposx, lposy,24,24);
	lposx +=28; 
	m_butdeptmod.LoadBitmaps(IDB_DEPTMOD, IDB_DEPTMOD, IDB_DEPTMOD, IDB_DEPTMOD);
	m_butdeptmod.MoveWindow(lposx, lposy,24,24);
	lposx +=28; 
	m_butdeptdel.LoadBitmaps(IDB_DEPTDEL, IDB_DEPTDEL,IDB_DEPTDEL,IDB_DEPTDEL);
	m_butdeptdel.MoveWindow(lposx, lposy,24,24);
	lposx +=28;
	m_butempadd.LoadBitmaps(IDB_EMPADD, IDB_EMPADD, IDB_EMPADD, IDB_EMPADD);
	m_butempadd.MoveWindow(lposx, lposy,24,24);
	lposx +=28;
	m_butempmod.LoadBitmaps(IDB_EMPMOD, IDB_EMPMOD, IDB_EMPMOD, IDB_EMPMOD);
	m_butempmod.MoveWindow(lposx, lposy,24,24);
	lposx +=28; 
	m_butempdel.LoadBitmaps(IDB_EMPDEL, IDB_EMPDEL, IDB_EMPDEL, IDB_EMPDEL);
	//m_butdeptdel.LoadBitmaps(IDB_DEPTDEL, IDB_DEPTDEL,IDB_DEPTDEL,IDB_DEPTDEL);
	m_butempdel.MoveWindow(lposx, lposy,24,24);
	lposx +=28;
	m_butRefresh.LoadBitmaps(IDB_REFRESH,IDB_REFRESH, IDB_REFRESH, IDB_REFRESH);
	m_butRefresh.MoveWindow(lposx, lposy,24,24);

 	((CEdit*)GetDlgItem(IDC_EDITNAME))->EnableWindow(0);
	((CEdit*)GetDlgItem(IDC_EDITDEPTNAME))->EnableWindow(0);
	((CEdit*)GetDlgItem(IDC_EDITACID))->EnableWindow(0);
	((CButton*)GetDlgItem(IDC_CHECK1))->ShowWindow(0);

	//fnBuildTree();

	//Localization
	fnLocalize();

	//Tooltips
	m_pToolTipCtrl = new CToolTipCtrl;
	if (!m_pToolTipCtrl->Create(this))		
			TRACE(_T("Unable To create ToolTip\n"));
		
	CString ls_tooltip;
	LOADSTRING(ls_tooltip, IDS_ACDEPTADD);
	m_pToolTipCtrl->AddTool(&m_butdeptadd, ls_tooltip);

	LOADSTRING(ls_tooltip, IDS_ACDEPTMOD);
	m_pToolTipCtrl->AddTool(&m_butdeptmod, ls_tooltip);
	
	LOADSTRING(ls_tooltip, IDS_ACDEPTDEL);
	m_pToolTipCtrl->AddTool(&m_butdeptdel, ls_tooltip);

	LOADSTRING(ls_tooltip, IDS_ACEMPADD);
	m_pToolTipCtrl->AddTool(&m_butempadd, ls_tooltip);
	
	LOADSTRING(ls_tooltip, IDS_ACEMPMOD);
	m_pToolTipCtrl->AddTool(&m_butempmod, ls_tooltip);

	LOADSTRING(ls_tooltip, IDS_ACEMPDEL);
	m_pToolTipCtrl->AddTool(&m_butempdel, ls_tooltip);

	LOADSTRING(ls_tooltip, IDS_ACREFRESH);
	m_pToolTipCtrl->AddTool(&m_butRefresh, ls_tooltip);

	m_newDeptCode = _T("");
	
	fnInitPriv();
	fnHideAllCheckbox();
	fnBuildTree();
	m_curdeptid = -1;
	mb_privdirty = false;
	return TRUE;  // return TRUE unless you set the focus to a control
	
}

void CTabpageAcEmp::fnBuildTree(void)
{
	if (!psDB) return;	//Check DB Availabel
 	//CDBVariant varCamidx, varStreamPort, varCamIdx, varCamid,varWebPort;
	CString ls_sql, ls_sql1, ls_sql2, ls_desc, ls_acname, ls_dept, ls_idx, ls_username, ls_password, ls_sprotocol,ls_webPort;
	CString csGroupDesc, ls_predept=_T(""), ls_masterid, ls_active, ls_deptCode, ls_wfrom, ls_wto, ls_picfile;
	int	i_accessid;
	char cIp[16];
	bool lb_ret;	
	
	USES_CONVERSION;
	HTREEITEM lprvItem;
	 
	int ci=0, ll_groupid;
	CDBVariant	varValue, varAcId, varAcGroupid;	
	HTREEITEM itemDefault;

  	CRecordset m_rs(psDB);
	
	ls_sql = _T( "SELECT DISTINCT ecac_access_employee.accessid, ecac_camera_group.access_group_code, ecac_camera_group.work_from, ecac_camera_group.work_to, ecac_camera_group.acgroupid  ,ecac_camera_group.access_group_desc, access_name, ecac_access_employee.access_masterid, ecac_access_employee.pic_file, ecac_access_employee.active_ FROM ecac_camera_group LEFT OUTER JOIN ecac_access_employee ");
	ls_sql1.Format( _T( " ON ecac_camera_group.access_group_code = ecac_access_employee.access_group_code"));		
	ls_sql2 = _T(" ORDER BY ecac_camera_group.access_group_desc");
	ls_sql = ls_sql + ls_sql1 + ls_sql2;

	TRACE(ls_sql);
	m_rs.Open(CRecordset::dynaset, ls_sql);
 
	LPWSTR mynew;
	HTREEITEM DeptItem;

	m_tree.DeleteAllItems();

	while (!m_rs.IsEOF()) { 

		STREMP* m_stremp = new STREMP;
		//::ZeroMemory(m_stremp, sizeof(STREMP));
		TVINSERTSTRUCTW itemStru={0};

		m_rs.GetFieldValue(_T("accessid"), varAcId,  SQL_C_SSHORT );
		m_stremp->acid = varAcId.m_iVal;

		//Group Code - Department
		m_rs.GetFieldValue(_T("access_group_code"), ls_deptCode);
		ls_deptCode = ls_deptCode.Trim();
		wcscpy(m_stremp->csCode , ls_deptCode);
		 
		//work from
		m_rs.GetFieldValue(_T("work_from"), ls_wfrom);		
		ls_wfrom = ls_wfrom.Trim() + '\0';
		wcscpy(m_stremp->csFrom , ls_wfrom);

		//work to
		m_rs.GetFieldValue(_T("work_to"), ls_wto);		
		ls_wto = ls_wto + '\0';
		wcscpy(m_stremp->csTo , ls_wto);

		m_rs.GetFieldValue(_T("acgroupid"), varAcGroupid,  SQL_C_SSHORT );		
		m_stremp->acgroupid = varAcGroupid.m_iVal;

		m_rs.GetFieldValue(_T("access_group_desc"), ls_dept);
		ls_dept = ls_dept.Trim() +'\0';		 
		//strncpy(m_stremp->csDept , W2A(ls_dept), ls_dept.GetLength());
		wcscpy(m_stremp->csDept , ls_dept);

		m_rs.GetFieldValue(_T("access_name"), ls_acname);
		ls_acname = ls_acname.Trim() + '\0';
		//strncpy(m_stremp->csName , W2A(ls_acname), ls_acname.GetLength());
		wcscpy(m_stremp->csName , ls_acname);

		m_rs.GetFieldValue(_T("access_masterid"), ls_masterid);
		ls_masterid = ls_masterid.Trim() + '\0';
		strncpy(m_stremp->csID , W2A(ls_masterid), ls_masterid.GetLength());

		m_rs.GetFieldValue(_T("pic_file"), ls_picfile);
		ls_picfile = ls_picfile.Trim() + '\0';
		wcscpy(m_stremp->picFile , ls_picfile);

		m_rs.GetFieldValue(_T("active_"), ls_active);
		if (ls_active ==_T("N"))
			m_stremp->csActive = 'N';
		else
			m_stremp->csActive = 'Y';

	/*	ls_active = ls_active.Trim() + '\0';
		strncpy(m_stremp->csActive , W2A(ls_active), ls_active.GetLength());*/

		itemStru.hParent = TVGN_ROOT;
		itemStru.item.lParam =  (LPARAM)m_stremp; //Save groupid into lparam
		itemStru.item.mask = TVIF_PARAM | TVIF_TEXT;
	 
		itemStru.item.pszText = (LPWSTR)(LPCTSTR)ls_dept;
		itemStru.item.cchTextMax =ls_desc.GetLength();

		m_vpEmp.push_back(m_stremp);

 
		if (ls_predept == ls_dept) //Same group as Previous department
		{
			 m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)ls_acname, 0,0,0,0, (LPARAM)m_stremp, DeptItem, TVI_LAST);
			m_rs.MoveNext();  					
		}
		else	//Different group
		{
			DeptItem = m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)ls_dept, 0,0,0,0, (LPARAM)m_stremp, NULL, TVI_LAST);
			if (ls_acname != _T(""))
			{
				m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)ls_acname, 0,0,0,0, (LPARAM)m_stremp, DeptItem, TVI_LAST);
			}
			ci++;
			ls_predept = ls_dept;
			m_rs.MoveNext();  		
		}		 
 
	}

	m_rs.Close();
 
}



void CTabpageAcEmp::fnBuildTree(int oCamidx)
{
	if (!psDB) return;	//Check DB Availabel
 	//CDBVariant varCamidx, varStreamPort, varCamIdx, varCamid,varWebPort;
	CString ls_sql, ls_sql1, ls_sql2, ls_desc, ls_acname, ls_dept, ls_idx, ls_username, ls_password, ls_sprotocol,ls_webPort;
	CString csGroupDesc, ls_predept=_T(""), ls_masterid, ls_active, ls_deptCode, ls_wfrom, ls_wto;
	int	i_accessid;
	char cIp[16];
	bool lb_ret;	
	
	USES_CONVERSION;
	HTREEITEM lprvItem;
	 
	int ci=0, ll_groupid;
	CDBVariant	varValue, varAcId, varAcGroupid;	
	HTREEITEM itemDefault;

  	CRecordset m_rs(psDB);
	
	ls_sql = _T( "SELECT DISTINCT ecac_access_employee.accessid, ecac_access_employee.access_group_code, ecac_camera_group.work_from, ecac_camera_group.work_to, ecac_camera_group.acgroupid  ,ecac_camera_group.access_group_desc, access_name, ecac_access_employee.access_masterid, ecac_access_employee.active_ FROM ecac_camera_group LEFT OUTER JOIN ecac_access_employee ");
	ls_sql1.Format( _T( " ON ecac_camera_group.access_group_code = ecac_access_employee.access_group_code WHERE ecac_camera_group.camera_idx=%d"), oCamidx);		
	ls_sql2 = _T(" ORDER BY ecac_access_employee.access_group_code");
	ls_sql = ls_sql + ls_sql1 + ls_sql2;

	TRACE(ls_sql);
	m_rs.Open(CRecordset::dynaset, ls_sql);
 
	LPWSTR mynew;
	HTREEITEM DeptItem;

	while (!m_rs.IsEOF()) { 

		STREMP* m_stremp = new STREMP;
		//::ZeroMemory(m_stremp, sizeof(STREMP));
		TVINSERTSTRUCTW itemStru={0};

		m_rs.GetFieldValue(_T("accessid"), varAcId,  SQL_C_SSHORT );
		m_stremp->acid = varAcId.m_iVal;

		//Group Code - Department
		m_rs.GetFieldValue(_T("access_group_code"), ls_deptCode);
		ls_deptCode = ls_deptCode.Trim();
		wcscpy(m_stremp->csCode , ls_deptCode);
		 
		//work from
		m_rs.GetFieldValue(_T("work_from"), ls_wfrom);		
		ls_wfrom = ls_wfrom.Trim() + '\0';
		wcscpy(m_stremp->csFrom , ls_wfrom);

		//work to
		m_rs.GetFieldValue(_T("work_to"), ls_wto);		
		ls_wto = ls_wto + '\0';
		wcscpy(m_stremp->csTo , ls_wto);

		m_rs.GetFieldValue(_T("acgroupid"), varAcGroupid,  SQL_C_SSHORT );		
		m_stremp->acgroupid = varAcGroupid.m_iVal;

		m_rs.GetFieldValue(_T("access_group_desc"), ls_dept);
		ls_dept = ls_dept.Trim() +'\0';		 
		//strncpy(m_stremp->csDept , W2A(ls_dept), ls_dept.GetLength());
		wcscpy(m_stremp->csDept , ls_dept);

		m_rs.GetFieldValue(_T("access_name"), ls_acname);
		ls_acname = ls_acname.Trim() + '\0';
		//strncpy(m_stremp->csName , W2A(ls_acname), ls_acname.GetLength());
		wcscpy(m_stremp->csName , ls_acname);

		m_rs.GetFieldValue(_T("access_masterid"), ls_masterid);
		ls_masterid = ls_masterid.Trim() + '\0';
		strncpy(m_stremp->csID , W2A(ls_masterid), ls_masterid.GetLength());

		m_rs.GetFieldValue(_T("active_"), ls_active);
		if (ls_active ==_T("N"))
			m_stremp->csActive = 'N';
		else
			m_stremp->csActive = 'Y';

	/*	ls_active = ls_active.Trim() + '\0';
		strncpy(m_stremp->csActive , W2A(ls_active), ls_active.GetLength());*/

		itemStru.hParent = TVGN_ROOT;
		itemStru.item.lParam =  (LPARAM)m_stremp; //Save groupid into lparam
		itemStru.item.mask = TVIF_PARAM | TVIF_TEXT;
	 
		itemStru.item.pszText = (LPWSTR)(LPCTSTR)ls_dept;
		itemStru.item.cchTextMax =ls_desc.GetLength();

		m_vpEmp.push_back(m_stremp);

 
		if (ls_predept == ls_dept) //Same group as Previous department
		{
			 m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)ls_acname, 0,0,0,0, (LPARAM)m_stremp, DeptItem, TVI_LAST);
			m_rs.MoveNext();  					
		}
		else	//Different group
		{
			DeptItem = m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)ls_dept, 0,0,0,0, (LPARAM)m_stremp, NULL, TVI_LAST);
			if (ls_acname != _T(""))
			{
				m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)ls_acname, 0,0,0,0, (LPARAM)m_stremp, DeptItem, TVI_LAST);
			}
			ci++;
			ls_predept = ls_dept;
			m_rs.MoveNext();  		
		}		 
	} 

	m_rs.Close();
//	if (itemDefault != NULL) SelectItem(itemDefault);
}


void CTabpageAcEmp::fnInsertTreeEmp(CString oGroupCode)
{
}

void CTabpageAcEmp::fnSave(CString oMode)
{
 
	if (psDB==NULL) return;
	USES_CONVERSION;
	CString	ls_sql, ls_sql1;
 

	try{
			//Department Stuff
			for (int i=0; i<m_vpDept.size(); i++)
			{
				CString	ls_code;
 				if (m_vpDept[i]->cFlag == 'D')
				{
					if (m_vpDept[i]->acgroupid<=0 || m_vpDept[i]->acgroupid>100000)
					{
						if (m_vpDept[i]->csCode != _T(""))
						{
								ls_sql.Format(_T("DELETE FROM ecac_access_employee  WHERE  access_group_code='%s' ; "), CString(m_vpDept[i]->csCode));						
									ls_sql1.Format(_T("DELETE FROM ecac_camera_group  WHERE  access_group_code='%s' ; DELETE FROM ecac_privilege  WHERE  access_group_code='%s' "), CString(m_vpDept[i]->csCode), CString(m_vpDept[i]->csCode) );
									ls_sql = ls_sql + ls_sql1;
 
									TRACE(ls_sql);
									psDB->ExecuteSQL(ls_sql);
						}
					}
					else
					{
								ls_sql.Format(_T("SELECT access_group_code FROM ecac_camera_group  WHERE  acgroupid=%d"), m_vpDept[i]->acgroupid  );	
								CRecordset rCode(psDB);
								rCode.Open(CRecordset::forwardOnly, ls_sql);

								while (!rCode.IsEOF()) {
									rCode.GetFieldValue(_T("access_group_code"), ls_code);
									ls_code.TrimRight();

									ls_sql.Format(_T("DELETE FROM ecac_access_employee  WHERE  access_group_code='%s' ; "), ls_code);						
									ls_sql1.Format(_T("DELETE FROM ecac_camera_group  WHERE  access_group_code='%s' ; DELETE FROM ecac_privilege  WHERE  access_group_code='%s' "), ls_code, ls_code );
									ls_sql = ls_sql + ls_sql1;
 
									TRACE(ls_sql);
									psDB->ExecuteSQL(ls_sql);

									break;
								}
					}
				}
				else if (m_vpDept[i]->cFlag == 'A') // Add 
				{
					CString ls_dept, ls_Code;
					TCHAR		lcdept[50];
					wcscpy(lcdept, m_vpDept[i]->csDept);
 
					ls_dept = CString( m_vpDept[i]->csDept);
 
					ls_sql.Format(_T("INSERT INTO ecac_camera_group ( access_group_code, work_from, work_to, access_group_desc ) VALUES('%s', '%s', '%s', '%s')"),  CString(m_vpDept[i]->csCode), CString(m_vpDept[i]->csFrom).Left(5) ,  CString(m_vpDept[i]->csTo).Left(5), ls_dept )  ;	
					TRACE(ls_sql);
					psDB->ExecuteSQL(ls_sql);		 

					 fnInsertDeptPriv(CString(m_vpDept[i]->csCode));
	
				}
				else if (m_vpDept[i]->cFlag == 'M') // Modify 
				{
					ls_sql.Format(_T("UPDATE ecac_camera_group SET access_group_desc = '%s' , work_from = '%s', work_to = '%s' WHERE  acgroupid = %d "), CString(m_vpDept[i]->csDept),  CString(m_vpDept[i]->csFrom).Left(5) ,  CString(m_vpDept[i]->csTo).Left(5),  m_vpDept[i]->acgroupid ) ;	
					TRACE(ls_sql);
					psDB->ExecuteSQL(ls_sql);
				}
				m_vpDept[i]->cFlag = ' ';
			} // Department

			//Employee 
			for (int i=0; i<m_vpEmp.size(); i++)
			{
			//	ls_debug.Format(_T("flag: %c"), m_vpEmp[i]->cFlag);
 				if (m_vpEmp[i]->cFlag == 'D')
				{
					ls_sql.Format(_T("DELETE FROM ecac_access_employee  WHERE  accessid =%d "), m_vpEmp[i]->acid );
 
					psDB->ExecuteSQL(ls_sql);
				}
				else if (m_vpEmp[i]->cFlag == 'A')
				{
					CString	ls_code;
					ls_sql.Format(_T("INSERT INTO ecac_access_employee (access_group_code, access_name, access_masterid, camera_idx, pic_file )  VALUES('%s', '%s',  '%s',  %d, '%s' )"),CString(m_vpEmp[i]->csCode),  CString(m_vpEmp[i]->csName), A2W(m_vpEmp[i]->csID), m_vpEmp[i]->camera_idx, m_vpEmp[i]->picFile  );	
					TRACE(ls_sql);
					psDB->ExecuteSQL(ls_sql);
				}
	 			else if (m_vpEmp[i]->cFlag == 'M')
				{					
					ls_sql.Format(_T("UPDATE ecac_access_employee SET access_name = '%s', access_masterid = '%s', pic_file = '%s'  WHERE  accessid = %d  "), CStringW(m_vpEmp[i]->csName), A2W(m_vpEmp[i]->csID),  m_vpEmp[i]->picFile, m_vpEmp[i]->acid )  ;						
					TRACE(ls_sql);
					psDB->ExecuteSQL(ls_sql);
				}
				m_vpEmp[i]->cFlag = ' ';
			} //for (int i=0; i<m_vpEmp.size(); i++)

	}//try
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

		//Privilege stuff
		if (mb_privdirty && m_curDeptCode!= _T(""))
		{
			for (int i=0; i<m_maxcam; i++)
			{
				if (ma_pricheck[i])	//Only do for the changed item
				{
						CString ls_check;
						ls_check = fnGetCheckState(i);
						ls_sql.Format(_T("UPDATE ecac_privilege SET privilege_ = '%s'  WHERE  camera_idx= %d AND access_group_code = '%s'  "),  ls_check, ma_camidx[i], m_curDeptCode )  ;						
						TRACE(ls_sql);
						psDB->ExecuteSQL(ls_sql);
				}
			}			

		if (oMode != _T("close"))
		{
			m_tree.DeleteAllItems();
			//fnBuildTree(m_curcamidx);
			fnBuildTree();
		}
		m_vpEmp.empty();
		m_vpDept.empty();


		}
		//Reset 
		for (int i=0; i<m_maxcam; i++)
		{
			ma_pricheck[i] = false;
		}
		mb_privdirty = false;

		fnInitPriv();
		fnHideAllCheckbox();
		fnBuildTree();
}


void CTabpageAcEmp::OnTvnSelchangedTreeemp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	//In the middle of the add department

	TVITEM TreeItem;
	HTREEITEM  hItem = m_tree.GetSelectedItem();	
	HTREEITEM  hItemRoot = m_tree.GetRootItem();
	
	if (m_tree.GetParentItem(hItem) == NULL) //Department Items
	{
		NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
		TreeItem.hItem = hItem;
		TreeItem.mask = TVIF_HANDLE ;
		m_tree.GetItem(&TreeItem);
		STREMP* lpstremp = (STREMP*) TreeItem.lParam;
		CStringW ls_dept = CStringW(lpstremp->csDept);
 
		((CEdit*)GetDlgItem(IDC_EDITDEPTNAME))->SetWindowTextW(CStringW( lpstremp->csDept));
		m_deptname = CString( lpstremp->csDept);
		m_curdeptid = lpstremp->acgroupid;
	 
		((CEdit*)GetDlgItem(IDC_EDITNAME))->SetWindowTextW( (LPCTSTR)_T(""));
		((CEdit*)GetDlgItem(IDC_EDITACID))->SetWindowTextW( (LPCTSTR)_T(""));
		((CEdit*)GetDlgItem(IDC_EDITNAME))->EnableWindow(0);
		((CEdit*)GetDlgItem(IDC_EDITACID))->EnableWindow(0);

		//work From- To
		CString ls_from = CString( lpstremp->csFrom);
		((CEdit*)GetDlgItem(IDC_EDITFROM))->SetWindowTextW( ls_from.Left(5));
		CString ls_to = CString( lpstremp->csTo);
		((CEdit*)GetDlgItem(IDC_EDITTO))->SetWindowTextW( ls_to);

		fnAllCheck(true, true);
		fnPrivilege(CString(lpstremp->csCode));
		m_curDeptCode = CString(lpstremp->csCode);
		m_pic.ShowWindow(false);
	}
	else	//Employee Item
	{
		NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
		TreeItem.hItem = hItem;
		TreeItem.mask = TVIF_HANDLE ;
		m_tree.GetItem(&TreeItem);

		STREMP* lpstremp = (STREMP*) TreeItem.lParam;
		((CEdit*)GetDlgItem(IDC_EDITDEPTNAME))->SetWindowTextW(CString( lpstremp->csDept));
		m_deptname = CString( lpstremp->csDept);
		CString ls_name = CString( lpstremp->csName);
 
		((CEdit*)GetDlgItem(IDC_EDITNAME))->SetWindowTextW( CString( lpstremp->csName));

		m_empname = CString(lpstremp->csName);
		((CEdit*)GetDlgItem(IDC_EDITACID))->SetWindowTextW( CString( lpstremp->csID));
		m_empid = CString(lpstremp->csID);
		m_acid = lpstremp->acid;
		m_curdeptid = lpstremp->acgroupid;

		//work From- To
		CString ls_from = CString( lpstremp->csFrom);
		((CEdit*)GetDlgItem(IDC_EDITFROM))->SetWindowTextW( ls_from.Left(5));
		CString ls_to = CString( lpstremp->csTo);
		((CEdit*)GetDlgItem(IDC_EDITTO))->SetWindowTextW( ls_to);
	
		//Get parents Code
		HTREEITEM hpItem = m_tree.GetParentItem(hItem);
		TVITEM TreePItem;
		TreePItem.hItem = hpItem;
		TreePItem.mask = TVIF_HANDLE ;
		m_tree.GetItem(&TreePItem);
		STREMP* lpPstremp = (STREMP*) TreePItem.lParam;
		CStringW ls_pdept = CStringW(lpPstremp->csDept);
				
		if (CString(lpstremp->picFile) !=_T(""))
		{
			fnLoadBmp(lpstremp->picFile);
			m_pic.ShowWindow(true);
		}
		else
			m_pic.ShowWindow(false);

		fnPrivilege(CString(lpPstremp->csCode));

		fnAllCheck(true, false);
 	}

//	fnGetPermission(m_curdeptid );
	*pResult = 0;
}


void CTabpageAcEmp::OnBnClickedButdeptadd()
{
	USES_CONVERSION;	 
	//int ll_sel = m_comboGate.GetCurSel();
	//if (ll_sel<0) return;
	CString	ls_newdept = _T("New Department");
	mc_deptmode = 'A';	//Add Mode

	CDlgAcEmpEdit	lEmpEdit;
	lEmpEdit.m_entryMode = _T("DA");
	lEmpEdit.m_vtCheck.clear();

	int ll_ret = lEmpEdit.DoModal();

	if (ll_ret == IDOK)
	{
		//Get department
		CString ls_dept, ls_from, ls_to;
		
		ls_dept = lEmpEdit.m_dept;
		ls_from = lEmpEdit.m_from;
		ls_to = lEmpEdit.m_to;

		//Insert the new department into tree
		STREMP*	pnewEmp = new STREMP;
		pnewEmp->cFlag = 'A';
		CString	ls_empty = _T("") +'\0';
		CString ls_code = fnGenString(ls_dept, 20);
		m_newDeptCode = ls_code+ '\0';
		ls_code = ls_code + '\0';
		ls_dept = ls_dept + '\0';
		m_curdeptid = 0;
		mb_dirty = true;
 
		//strncpy(  pnewEmp->csDept , W2A(ls_dept), 50);
		wcscpy(pnewEmp->csFrom , ls_from.Left(5));
		wcscpy(pnewEmp->csTo , ls_to.Left(5));
		wcscpy(pnewEmp->csDept , ls_dept);
		strncpy(  pnewEmp->csID ,W2A(ls_empty), 18);
		//strncpy(  pnewEmp->csName , W2A(ls_empty), 50);
		wcscpy(  pnewEmp->csName , ls_empty);

		wcscpy(pnewEmp->csCode,  ls_code);
 
		m_newItem = m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, ls_dept, 0,0,0,0, (LPARAM)pnewEmp, NULL, TVI_ROOT);
	
		((CEdit*)GetDlgItem(IDC_EDITDEPTNAME))->SetWindowTextW(ls_dept);
		m_tree.SelectItem(m_newItem);

		fnAllCheck(true, true, true);
		((CEdit*)GetDlgItem(IDC_EDITFROM))->SetWindowTextW(ls_from);
		((CEdit*)GetDlgItem(IDC_EDITTO))->SetWindowTextW(ls_to);

		//Add into vp_dept
		STRDEPT*	pnewDept = new STRDEPT;

		//strncpy(  pnewDept->csDept , W2A(ls_dept), 50);
		pnewDept->camidx = m_curcamidx;

		wcscpy(  pnewDept->csFrom , ls_from+'\0');	
		wcscpy(  pnewDept->csTo , ls_to+'\0');
		wcscpy(  pnewDept->csDept , ls_dept);
		wcscpy( pnewDept->csCode,  ls_code);
		wcscpy( pnewDept->csPriv,  m_camList);
		wcscpy( pnewDept->csYN,  m_privyn);

		pnewDept->cFlag = 'A';		
		m_vpDept.push_back(pnewDept);
	}

}

//Delete Department
void CTabpageAcEmp::OnBnClickedButdeptdel()
{
	//if (m_curcamidx<0) return;
	HTREEITEM	hItem = m_tree.GetSelectedItem();
	if (!hItem) return;
	if (m_tree.GetParentItem(hItem) == NULL) //Department Items
	{
		if (AfxMessageBox(_T("Delete department will also delete all the related employees, ok to process?"), MB_OKCANCEL, MB_ICONQUESTION) == IDOK)
		{		
				USES_CONVERSION;
				//Create m_vpDelDept vector
				mb_dirty = true;
				TVITEM TreeItem;
				TreeItem.hItem = hItem;
				TreeItem.mask = TVIF_HANDLE ;
				m_tree.GetItem(&TreeItem);
				STREMP* lpstremp = (STREMP*) TreeItem.lParam;
				STRDEPT* lpdept = new STRDEPT();
		 
				wcscpy( lpdept->csDept , lpstremp->csDept);
				 
				lpdept->cFlag = 'D'; //Delete flag
				//lpdept->camidx = m_curcamidx;   Obsolete
				
				lpdept->acgroupid  = m_curdeptid; //lpstremp->acgroupid;
		
				wcscpy( lpdept->csCode , lpstremp->csCode);
				//lpdept->csCode = CString(lpstremp->csCode);
				m_vpDept.push_back(lpdept);

				m_tree.DeleteItem(hItem);
 
		}
	}
}

void CTabpageAcEmp::fnFreeMemory()
{	
	for (int i=0; i<m_vpDept.size(); i++)
	{
		delete m_vpDept[i];
	}
	for (int i=0; i<m_vpEmp.size(); i++)
	{
		if (m_vpEmp[i]) delete m_vpEmp[i];
	}
	if (m_pToolTipCtrl ) delete m_pToolTipCtrl ;

}

//Add Employee
void CTabpageAcEmp::OnBnClickedButempadd()
{	
	if ( m_curdeptid<=0 )
	{
		if (mc_deptmode!= 'A') return;
	}
	CString	ls_code, ls_from, ls_to;
	USES_CONVERSION;

	mb_dirty = true;
 
	CDlgAcEmpEdit	lEmpEdit;
	lEmpEdit.m_entryMode = _T("EA");
	lEmpEdit.m_dept = m_deptname;

	((CEdit*)GetDlgItem(IDC_EDITFROM))->GetWindowTextW(ls_from);
	lEmpEdit.m_from = ls_from;

	((CEdit*)GetDlgItem(IDC_EDITTO))->GetWindowTextW(ls_to);
	lEmpEdit.m_to = ls_to;
 
	int ll_ret = lEmpEdit.DoModal();

	if (ll_ret == IDOK)
	{
		HTREEITEM	hItem = m_tree.GetSelectedItem();
		if (m_tree.GetParentItem(hItem) != NULL) 
			hItem = 	m_tree.GetParentItem(hItem);

		//Grab lEmpedit info might be more than 1 records

		for (int i=0; i<lEmpEdit.m_vtEmp.size(); i++)
		{
					
 				STREMP*	pnewEmp = new STREMP;
				pnewEmp->cFlag = 'A';
				pnewEmp->camera_idx = m_curcamidx;
				wcscpy(  pnewEmp->csDept , m_deptname+'\0');	
				
				//strncpy(  pnewEmp->csID ,W2A(lEmpEdit.m_empid + '\0'), 20);
				strncpy(  pnewEmp->csID ,W2A(lEmpEdit.m_vtEmp[i]->csAcid + '\0'), 20);
				//wcscpy(  pnewEmp->csName , lEmpEdit.m_empname + '\0');
				wcscpy(  pnewEmp->csName , lEmpEdit.m_vtEmp[i]->csEmp + '\0');

				//Picture File
				wcscpy(  pnewEmp->picFile , lEmpEdit.m_vtEmp[i]->picFile + '\0');

				((CEdit*)GetDlgItem(IDC_EDITFROM))->GetWindowTextW(ls_from);
				wcscpy(  pnewEmp->csFrom , ls_from + '\0'  );
				((CEdit*)GetDlgItem(IDC_EDITTO))->GetWindowTextW(ls_to);
				wcscpy(  pnewEmp->csTo , ls_to+ '\0' );
			
				//CString ls_debug;
				//ls_debug.Format(_T("Dept ID: %d"), m_curdeptid);
				//AfxMessageBox(ls_debug);

				if (m_curdeptid>0 && m_curdeptid<100000){
						ls_code =  fnGetDeptCode(m_curdeptid);
 
						if (ls_code == _T(""))
						{
 								TVITEM TreeItem;
								TreeItem.hItem = hItem;
								TreeItem.mask = TVIF_HANDLE ;
								m_tree.GetItem(&TreeItem);
								STREMP* lpstremp = (STREMP*) TreeItem.lParam;
								CString ls_code;
								ls_code = CString(lpstremp->csCode);
								wcscpy(  pnewEmp->csCode , ls_code + '\0');				
						}
						else
						{
							wcscpy( pnewEmp->csCode , ls_code+ '\0');
							m_curDeptCode = ls_code;
						}
						
						CString ls_dept = m_deptname;									
						wcscpy(  pnewEmp->csDept , ls_dept);
				}
				else{
	/*				ls_debug.Format(_T("ls code: %s;    m_newDeptCode: %s "), ls_code, m_newDeptCode);
					AfxMessageBox(ls_debug);*/

					if (ls_code != _T(""))
					{
						wcscpy(  pnewEmp->csCode ,ls_code );
					}
					else if (m_newDeptCode != _T(""))
					{
						TVITEM TreeItem;
						TreeItem.hItem = hItem;
						TreeItem.mask = TVIF_HANDLE ;
						m_tree.GetItem(&TreeItem);
						STREMP* lpstremp = (STREMP*) TreeItem.lParam;
						CString ls_dcode;
						ls_dcode = CString(lpstremp->csCode);
						wcscpy(  pnewEmp->csCode , ls_dcode + '\0');
						m_curDeptCode = m_newDeptCode;
					}
 
				}

	 			m_newItem = m_tree.InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM,lEmpEdit.m_vtEmp[i]->csEmp, 0,0,0,0, (LPARAM)pnewEmp, hItem, TVI_LAST);
 				//m_tree.SelectItem(m_newItem);
				m_vpEmp.push_back(pnewEmp);		
		}
	}
}

//Delete Employee
void CTabpageAcEmp::OnBnClickedButempdel()
{
		HTREEITEM	hItem = m_tree.GetSelectedItem();
		if (m_tree.GetParentItem(hItem) == NULL) //Department Items
			return;

		USES_CONVERSION;
		//Create m_vpDelDept vector
		TVITEM TreeItem;
		TreeItem.hItem = hItem;
		TreeItem.mask = TVIF_HANDLE ;
		m_tree.GetItem(&TreeItem);
		STREMP* lpstremp = (STREMP*) TreeItem.lParam;
	 
		lpstremp->cFlag = 'D';	
		//m_vpEmp.push_back(pnewEmp);		
		mb_dirty = true;
		m_tree.DeleteItem(hItem);
}

//Modify Department
void CTabpageAcEmp::OnBnClickedButdeptmod()
{
 
	//	if (m_curcamidx<0) return;
	USES_CONVERSION;
	CDlgAcEmpEdit*	lEmpEdit = new CDlgAcEmpEdit;

	HTREEITEM	hItem = m_tree.GetSelectedItem();
	if (!hItem) return;
	if (m_tree.GetParentItem(hItem) != NULL) //Not Department Items
			return;

	//Pass info into edit dialog
	lEmpEdit->m_entryMode = _T("DM");
	
	CString ls_dept, ls_orgdept, ls_from, ls_to;
	((CEdit*)GetDlgItem(IDC_EDITDEPTNAME))->GetWindowTextW(ls_dept);
	ls_dept = m_tree.GetItemText(hItem);
	ls_orgdept = ls_dept +'\0';
	lEmpEdit->m_dept = ls_dept; 
	lEmpEdit->m_vtCheck.clear();
	
	((CEdit*)GetDlgItem(IDC_EDITFROM))->GetWindowTextW(ls_from);
	lEmpEdit->m_from = ls_from;

	((CEdit*)GetDlgItem(IDC_EDITTO))->GetWindowTextW(ls_to);
	lEmpEdit->m_to = ls_to;

	//Open Dialog to edit
	int ll_ret = lEmpEdit->DoModal();

	if (ll_ret == IDOK)
	{
		//Get department
		CString ls_dept, ls_from, ls_to;
		ls_dept = lEmpEdit->m_dept;
		ls_from = lEmpEdit->m_from;
		ls_to = lEmpEdit->m_to;

		//Insert the new department into tree
		CString	ls_empty = _T("") +'\0';
		mb_dirty = true;
		//ls_dept = ls_dept + '\0';

		m_tree.SetItemText(m_tree.GetSelectedItem(),ls_dept.Trim());
		((CEdit*)GetDlgItem(IDC_EDITDEPTNAME))->SetWindowTextW(ls_dept);
		//m_tree.SelectItem(m_newItem);

		((CEdit*)GetDlgItem(IDC_EDITFROM))->SetWindowTextW(ls_from);
		((CEdit*)GetDlgItem(IDC_EDITTO))->SetWindowTextW(ls_to);

		//Add into vp_dept - for saveing
		STRDEPT*	pmodDept = new STRDEPT;

 		wcscpy(  pmodDept->csDept , ls_dept); 
		wcscpy(  pmodDept->csOrgDept , ls_orgdept);	
		wcscpy(  pmodDept->csFrom , ls_from);	
		wcscpy(  pmodDept->csTo , ls_to);	
		pmodDept->acgroupid = m_curdeptid;

		pmodDept->cFlag = 'M';		
		m_vpDept.push_back(pmodDept);
	
	}

	delete lEmpEdit;
}

CString CTabpageAcEmp::fnGenString(CString osource, int olen)
{

	CString ls_bas = _T("E0ZDJ1FGWN2AXVU3YTOH4PB5MQ6CIRL7S89K");
	CTime nowTime = CTime::GetCurrentTime();
	CString ls_ret;
	const int nMaxIndex = (ls_bas.GetLength() -sizeof(TCHAR))/sizeof(TCHAR);
 
	if (nMaxIndex>0)
	{
		srand((unsigned int)GetTickCount());
	 
			for (int i=0; i<olen; i++)
			{
				ls_ret += ls_bas[GetRandom(0,nMaxIndex)];
			}
		return ls_ret;	
	}
}


BOOL CTabpageAcEmp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if ( m_pToolTipCtrl != NULL)
		m_pToolTipCtrl->RelayEvent(pMsg);

	return CBaseDialog::PreTranslateMessage(pMsg);
}

void CTabpageAcEmp::OnBnClickedButempmod()
{
	CString	ls_code;
	USES_CONVERSION;

	HTREEITEM  hItem = m_tree.GetSelectedItem();		
	if (m_tree.GetParentItem(hItem) == NULL) //Department Items
		return;
			
	TVITEM tvItem;
	tvItem.hItem = hItem;
	tvItem.mask = TVIF_HANDLE| TVIF_PARAM;
	m_tree.GetItem(&tvItem);

	STREMP* lpEmp = (STREMP*)tvItem.lParam;

	CDlgAcEmpEdit	lEmpEdit;
	lEmpEdit.m_entryMode = _T("EM");
	lEmpEdit.m_dept = m_deptname;
	ls_code = CString(lpEmp->csCode);

	CString ls_from, ls_to, ls_picFile;

	((CEdit*)GetDlgItem(IDC_EDITFROM))->GetWindowTextW(ls_from);
	lEmpEdit.m_from = ls_from;

	((CEdit*)GetDlgItem(IDC_EDITTO))->GetWindowTextW(ls_to);
	lEmpEdit.m_to = ls_to;
		
	CString ls_empname = m_tree.GetItemText(hItem);
	lEmpEdit.m_empname = ls_empname + '\0';
	//lEmpEdit.m_empname = m_empname;

	m_empid = lpEmp->csID;
	lEmpEdit.m_empid = m_empid;

	lEmpEdit.m_picFile = lpEmp->picFile;

	int ll_ret = lEmpEdit.DoModal();

	if (ll_ret == IDOK)
	{
		mb_dirty = true;
		HTREEITEM	hItem = m_tree.GetSelectedItem();
		if (m_tree.GetParentItem(hItem) != NULL) 
			hItem = 	m_tree.GetParentItem(hItem);
		
 		STREMP*	pmodEmp = new STREMP;
		pmodEmp->cFlag = 'M';
 
		wcscpy(  pmodEmp->csDept , m_deptname+'\0');
		strncpy(  pmodEmp->csID ,W2A(lEmpEdit.m_empid + '\0'), 20);
		wcscpy(  pmodEmp->csOrgName , m_empname + '\0');
		wcscpy(  pmodEmp->csName , lEmpEdit.m_empname + '\0');
		wcscpy(  pmodEmp->csCode,  ls_code);
		wcscpy(  pmodEmp->csFrom,  ls_from);
		wcscpy(  pmodEmp->csTo,  ls_to);

		//Picture File
		wcscpy(  pmodEmp->picFile, lEmpEdit.m_picFile + '\0');
		if (lEmpEdit.m_picFile == _T(""))
			m_pic.ShowWindow(SW_HIDE);
		else
		{
			m_pic.ShowWindow(SW_SHOW);
			fnLoadBmp(lEmpEdit.m_picFile);
		}
		 
		pmodEmp->acgroupid = m_curdeptid;
		pmodEmp->acid = m_acid;
		
		tvItem.lParam = (LPARAM)pmodEmp;
		bool lb_ret = m_tree.SetItem(hItem, TVIF_HANDLE| TVIF_PARAM, m_empname,  0,0,0, 0, tvItem.lParam);
		//m_tree.SetItem(&tvItem);			
		
		m_tree.SetItemText(m_tree.GetSelectedItem(),lEmpEdit.m_empname + '\0');
		((CEdit*)GetDlgItem(IDC_EDITNAME))->SetWindowTextW(lEmpEdit.m_empname + '\0');
		((CEdit*)GetDlgItem(IDC_EDITACID))->SetWindowTextW(lEmpEdit.m_empid + '\0');

 		m_vpEmp.push_back(pmodEmp);		
	}
}

// get department code from department ID

CString CTabpageAcEmp::fnGetDeptCode(int oDeptid)
{
	CRecordset m_rs(psDB);
	CString ls_sql, ls_code=_T("");
	ls_sql.Format( _T("SELECT DISTINCT  access_group_code FROM ecac_camera_group  WHERE acgroupid = %d "), oDeptid);

	m_rs.Open(CRecordset::dynaset, ls_sql);
	while (!m_rs.IsEOF()) { 

		//Group Code - Department		
		m_rs.GetFieldValue(_T("access_group_code"), ls_code);
		ls_code = ls_code.Trim();
		break;
	}
	return ls_code;

}


void CTabpageAcEmp::fnClearCheck()
{
	for (int i=0; i<10; i++)
	{
		ma_check[i]->ShowWindow(false);
		ma_check[i]->SetWindowText(_T(""));
	}
}


void CTabpageAcEmp::fnAddCheckRow(CString oDeptCode)
{
			CStringW ls_sql, ls_sqlAll, ls_desc;
			int i_gid=0;

			CDBVariant	vargid, varidx;
		 CRecordset m_rsCam(psDB);

		 ls_sql.Format(_T("SELECT acgroupid FROM ecac_hgroup WHERE access_group_code = '%s' "), oDeptCode);
	 
		m_rsCam.Open(CRecordset::dynaset,ls_sql);

 		int cnt=0;	
		CString	ls_active;
		
		int ll_rows = m_rsCam.GetRecordCount();

		while(!m_rsCam.IsEOF()   ) 
		{ 
			m_rsCam.GetFieldValue(_T("acgroupid"), vargid, SQL_C_SLONG);		
			break;		//1 row only
		}

		if (vargid.m_iVal<=0) return;		
		
			TRACE(ls_sqlAll);
			psDB->ExecuteSQL(ls_sqlAll);

}

//Rebuild tree to display employee related to the camera index
void CTabpageAcEmp::OnCbnSelchangeCombogate()
{
	int i_idx = m_comboGate.GetCurSel();
	int i_camidx;
	//Get camera index
	for (int i=0; i<m_vtpcam.size(); i++)
	{
		if (m_vtpcam[i]->selitem == i_idx)
		{
			i_camidx = m_vtpcam[i]->cam_idx;
			break;
		}
	}

 	if (i_camidx<=0) return;	//shouldn't happen
	m_curcamidx = i_camidx;	
	m_tree.DeleteAllItems();

	//Empty edit control
	m_curdeptid = 0;
	((CEdit*)GetDlgItem(IDC_EDITDEPTNAME))->SetWindowTextW(_T(""));
	((CEdit*)GetDlgItem(IDC_EDITFROM))->SetWindowTextW(_T(""));
	((CEdit*)GetDlgItem(IDC_EDITTO))->SetWindowTextW(_T(""));

	fnBuildTree( i_camidx);

}

// Build Camera Checkbox
void CTabpageAcEmp::fnBuildCamCombo(void)
{
 	CDBVariant	vargid, varIdx, varAcId, varAcGroupid;		 
	CRecordset m_rs(psDB);
	CString ls_sql, ls_camname=_T(""), ls_permit;

	ls_sql = _T("SELECT DISTINCT ecac_camera.camera_idx, ec_camera.cameraname FROM ecac_camera  LEFT OUTER JOIN ec_camera ON ecac_camera.camera_idx = ec_camera.camera_idx ORDER BY ecac_camera.camera_idx ");
 
	m_rs.Open(CRecordset::dynaset, ls_sql);
	int iitem=0;
  
	if (m_rs.GetRecordCount() >0)
	{
  			m_rs.MoveFirst();
	
			while (!m_rs.IsEOF()) 
			{ 
						//build vector
						STRCAM* lstrcam= new STRCAM;

						m_rs.GetFieldValue(_T("camera_idx"), varIdx,  SQL_C_SSHORT );
						m_rs.GetFieldValue(_T("cameraname"), ls_camname );		
						
						//Build vector m_vtCheck
						lstrcam->cam_idx = varIdx.m_iVal;
						lstrcam->csname = ls_camname.Trim();	
						lstrcam->selitem = iitem;
						m_vtpcam.push_back(lstrcam);
						CString ls_data;
						ls_data.Format(_T("( %d )-%s"),varIdx.m_iVal,  ls_camname.Trim());
						//fnSetCheckText(iitem, ls_data);
						m_comboGate.AddString(ls_data);
						  iitem++;
						m_rs.MoveNext();
			} // while loop

//			m_maxcam = iitem;
	} // if rowcount>0
 
}



void CTabpageAcEmp::fnSetCheckText(int oItem, CString oText)
{
 		((CButton*)GetDlgItem(IDC_PCHECK1))->SetWindowTextW(oText);
 
}

//based on the deptcode, show camera checkbox
void CTabpageAcEmp::fnPrivilege(CString oGroupCode)
{
 	CDBVariant	 varIdx, varAcId, varAcGroupid;		 
	CRecordset m_rs(psDB);
	CString ls_sql,  ls_priv, ls_camname;
 
	try{

		ls_sql.Format(_T("SELECT  ec_camera.camera_idx, ec_camera.cameraname, ecac_privilege.privilege_ FROM ecac_privilege INNER JOIN ec_camera ON ec_camera.camera_idx = ecac_privilege.camera_idx  INNER JOIN ecac_camera ON ecac_camera.camera_idx= ecac_privilege.camera_idx WHERE ecac_camera.ac_control='Y' AND access_group_code='%s' ORDER BY ec_camera.camera_idx "), oGroupCode);
		TRACE(ls_sql);
		m_rs.Open(CRecordset::dynaset, ls_sql);
		int litem=0;
	  
		if (m_rs.GetRecordCount() >0)
		{
  				m_rs.MoveFirst();
		
				while (!m_rs.IsEOF()) 
				{ 
						m_rs.GetFieldValue(_T("camera_idx"), varIdx,  SQL_C_SSHORT );
						m_rs.GetFieldValue(_T("cameraname"), ls_camname );		
						m_rs.GetFieldValue(_T("privilege_"), ls_priv );		
						CString ls_txt;
						ls_txt.Format(_T("(%d) - %s"), varIdx.m_iVal, ls_camname);
 
						CButton* lpCheck;

		 					if (litem==0) 	lpCheck = (CButton*)GetDlgItem(IDC_PCHECK1);
 							else if (litem==1) 	lpCheck = (CButton*)GetDlgItem(IDC_PCHECK2);
							else if (litem==2) 	lpCheck = (CButton*)GetDlgItem(IDC_PCHECK3);
							else if (litem==3) 	lpCheck = (CButton*)GetDlgItem(IDC_PCHECK4);
							else if (litem==4) 	lpCheck = (CButton*)GetDlgItem(IDC_PCHECK5);
							else if (litem==5) 	lpCheck = (CButton*)GetDlgItem(IDC_PCHECK6);
							else if (litem==6) 	lpCheck = (CButton*)GetDlgItem(IDC_PCHECK7);
							else if (litem==7) 	lpCheck = (CButton*)GetDlgItem(IDC_PCHECK8);
							else if (litem==8) 	lpCheck = (CButton*)GetDlgItem(IDC_PCHECK9);
							else if (litem==9) 	lpCheck = (CButton*)GetDlgItem(IDC_PCHECK10);

							ma_camidx[litem] = varIdx.m_iVal;

	 						lpCheck->EnableWindow(true);
							lpCheck->ShowWindow(true);
							lpCheck->SetWindowTextW(ls_txt);

							if (ls_priv==_T("Y"))							
								lpCheck->SetCheck(1);							
							else
								lpCheck->SetCheck(0);	

							STRPRIV lpPriv;
							lpPriv.cam_idx = varIdx.m_iVal;
							lpPriv.camname = ls_txt;
							lpPriv.cYN = ls_priv[0];
							m_vpPriv.push_back(&lpPriv);
						litem++;
						m_rs.MoveNext();	 
			}
		}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CTabpageAcEmp::fnAllCheck(bool obShow, bool obEnable)
{
			for (int i=0; i<m_maxcam; i++)
			{
				CButton* lpCheck;
				if (i==0) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK1);
				else if (i==1) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK2);
				else if (i==2) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK3);
				else if (i==3) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK4);
				else if (i==4) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK5);
				else if (i==5) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK6);
				else if (i==6) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK7);
				else if (i==7) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK8);
				else if (i==8) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK9);
				else if (i==9) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK10);

					lpCheck->ShowWindow(obShow);
					lpCheck->EnableWindow(obEnable);
				
			}
}

void CTabpageAcEmp::fnAllCheck(bool obShow, bool obEnable, bool obCheck)
{
			for (int i=0; i<m_maxcam; i++)
			{
				CButton* lpCheck;
				if (i==0) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK1);
				else if (i==1) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK2);
				else if (i==2) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK3);
				else if (i==3) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK4);
				else if (i==4) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK5);
				else if (i==5) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK6);
				else if (i==6) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK7);
				else if (i==7) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK8);
				else if (i==8) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK9);
				else if (i==9) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK10);

					lpCheck->ShowWindow(obShow);
					lpCheck->EnableWindow(obEnable);
					lpCheck->SetCheck(obCheck);
				
			}
}

int CTabpageAcEmp::fnInitPriv(void)
{
		CDBVariant	 varIdx, varAcId, varAcGroupid;		 
	CRecordset m_rs(psDB);
	CString ls_sql,  ls_priv, ls_camname;
	m_maxcam=0;
 
	ls_sql.Format(_T("SELECT  ec_camera.camera_idx, ec_camera.cameraname FROM ecac_camera, ec_camera WHERE ec_camera.camera_idx = ecac_camera.camera_idx"));
 
	m_rs.Open(CRecordset::dynaset, ls_sql);
	int litem=0;
	m_camList = _T("");
  
	if (m_rs.GetRecordCount() >0)
	{
  			m_rs.MoveFirst();
	
			while (!m_rs.IsEOF()) 
			{
					CString	ls_camname=_T("");
					m_rs.GetFieldValue(_T("camera_idx"), varIdx,  SQL_C_SSHORT );
					m_rs.GetFieldValue(_T("cameraname"),  ls_camname);
					CString ls_cam=_T("");

					ls_cam.Format(_T("%d"), varIdx.m_iVal);
					if (ls_cam!=_T(""));
					{
						m_camList += ls_cam + _T("|");
						m_privyn += "Y|";
					}

					CButton* lpCheck;
		 
						if (litem ==0) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK1);
				else if (litem ==1) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK2);
				else if (litem ==2) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK3);
				else if (litem ==3) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK4);
				else if (litem ==4) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK5);
				else if (litem ==5) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK6);
				else if (litem ==6) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK7);
				else if (litem ==7) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK8);
				else if (litem ==8) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK9);
				else if (litem ==9) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK10);

				lpCheck->SetWindowTextW(ls_camname);
				lpCheck->ShowWindow(true);
				lpCheck->EnableWindow(true);
					

					litem++;	
					m_rs.MoveNext();
			}
			m_maxcam = litem;
			return litem;
	}
}

bool CTabpageAcEmp::fnInsertDeptPriv(CString oDeptCode)
{			
	CString ls_str, ls_sql = _T(""), ls_sql1=_T(""), ls_cam;
	CString ls_yn;
	int	lpos=0, icnt=0;
	ls_str = m_camList;

	while 	(ls_str.Find('|', 0 )>0)
	{
		lpos = ls_str.Find('|', 0);
		ls_cam = ls_str.Left( lpos);
		ls_str = ls_str.Mid(lpos+ 1);
		ls_yn = m_privyn.Mid(icnt*2,1);

		if (_ttoi(ls_cam)<=0) continue;
		ls_sql1.Format(_T("DELETE FROM ecac_privilege WHERE access_group_code='%s' AND camera_idx=%d ; INSERT INTO ecac_privilege ( access_group_code,  camera_idx, privilege_ ) VALUES('%s', %d, '%s') ;"),   oDeptCode, _ttoi(ls_cam), oDeptCode, _ttoi(ls_cam), ls_yn  )  ;	
		ls_sql = ls_sql + ls_sql1;
		icnt++;
	}
	
		TRACE(ls_sql);
		psDB->ExecuteSQL(ls_sql);	
		return false;
}

void CTabpageAcEmp::OnBnClickedPcheck1()
{
	fnClickPrivilege(1);
}

void CTabpageAcEmp::OnBnClickedPcheck2()
{
	fnClickPrivilege(2);
}

void CTabpageAcEmp::OnBnClickedPcheck3()
{
	fnClickPrivilege(3);
}

void CTabpageAcEmp::OnBnClickedPcheck4()
{
	fnClickPrivilege(4);
}

void CTabpageAcEmp::OnBnClickedPcheck5()
{
	fnClickPrivilege(5);
}

void CTabpageAcEmp::OnBnClickedPcheck6()
{
	fnClickPrivilege(6);
}

void CTabpageAcEmp::OnBnClickedPcheck7()
{
	fnClickPrivilege(7);
}

void CTabpageAcEmp::OnBnClickedPcheck8()
{
	fnClickPrivilege(8);
}

void CTabpageAcEmp::OnBnClickedPcheck9()
{
	fnClickPrivilege(9);
}

void CTabpageAcEmp::OnBnClickedPcheck10()
{
	fnClickPrivilege(10);
}

void CTabpageAcEmp::fnClickPrivilege(int oCheckNum)
{
	mb_dirty = true;
	mb_privdirty = true;
	ma_pricheck[oCheckNum-1] = true;
}


CString CTabpageAcEmp::fnGetCheckState(int oIdx)
{
		CString	ls_ret;
		CButton*	lpCheck;
					
		
		if (oIdx==0) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK1);
		else if (oIdx ==1) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK2);
		else if (oIdx ==2) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK3);
		else if (oIdx ==3) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK4);
		else if (oIdx ==4) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK5);
		else if (oIdx ==5) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK6);
		else if (oIdx ==6) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK7);
		else if (oIdx ==7) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK8);
		else if (oIdx ==8) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK9);
		else if (oIdx ==9) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK10);

		if (lpCheck->GetCheck()>0) 
			ls_ret = _T("Y");
		else
			ls_ret = _T("N");

	return ls_ret;
}

void CTabpageAcEmp::OnBnClickedButrefresh()
{
	fnInitPriv();
	fnHideAllCheckbox();
	fnBuildTree();
}

void CTabpageAcEmp::fnHideAllCheckbox()
{
	for (int litem =0; litem<10; litem++)
	{
		CButton* lpCheck;

						if (litem ==0) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK1);
				else if (litem ==1) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK2);
				else if (litem ==2) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK3);
				else if (litem ==3) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK4);
				else if (litem ==4) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK5);
				else if (litem ==5) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK6);
				else if (litem ==6) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK7);
				else if (litem ==7) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK8);
				else if (litem ==8) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK9);
				else if (litem ==9) lpCheck = (CButton*)GetDlgItem(IDC_PCHECK10);
				lpCheck->ShowWindow(false);
	}
}

bool CTabpageAcEmp::fnLoadBmp(CString o_file)
{
		CString ls_appPath = ((CMainFrame*)AfxGetMainWnd())->m_csAppPath;
		CString ls_file;

		HBITMAP hbm = (HBITMAP) m_pic.GetBitmap();
		BITMAP bm;
		SIZE szBitmap;
		GetObject(hbm, sizeof(bm), &bm);
		CBitmap hbmp;
		 HBITMAP hbitmap;
	
		 //Load JPG File
		 ls_file.Format(_T("%sac\\%s"), ls_appPath, o_file);
		m_pic.Load(ls_file);
		m_pic.ShowWindow(SW_SHOW);

		return false;
}

//Localization
void CTabpageAcEmp::fnLocalize()
{	
		CString ls_loc;
		// 
		LOADSTRING(ls_loc, IDS_AC_TAB_DEPT_EMP);
		GetDlgItem( IDC_STTITLEDEPT )->SetWindowTextW(ls_loc);

		LOADSTRING(ls_loc, IDS_AC_TAB_DEPT);
		GetDlgItem( IDC_STDEPT )->SetWindowTextW(ls_loc);			

		LOADSTRING(ls_loc, IDS_AC_TAB_DEPT);
		GetDlgItem( IDC_STDEPTNAME )->SetWindowTextW(ls_loc);

		LOADSTRING(ls_loc,IDS_AC_TAB_FROM);
		GetDlgItem( IDC_STDEPTNAME2 )->SetWindowTextW(ls_loc);

		LOADSTRING(ls_loc, IDS_AC_TAB_TO);
		GetDlgItem( IDC_STDEPTNAME3 )->SetWindowTextW(ls_loc);

		//Employee
		LOADSTRING(ls_loc, IDS_AC_TAB_EMP	);
		GetDlgItem( IDC_STEMP )->SetWindowTextW(ls_loc);
 
		LOADSTRING(ls_loc, IDS_AC_TAB_NAME	);
		GetDlgItem( IDC_STNAME )->SetWindowTextW(ls_loc);

		LOADSTRING(ls_loc, IDS_AC_TAB_ACID	);
		GetDlgItem( IDC_STATICACID )->SetWindowTextW(ls_loc);

		LOADSTRING(ls_loc, IDS_AC_TAB_GATE_PRIVILEGE);
		GetDlgItem( IDC_STPRIV )->SetWindowTextW(ls_loc);
		

}