// DlgSettingCamGroup_Ext.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSettingCamGroup_Ext.h"
#include "MainFrm.h"

// CDlgSettingCamGroup_Ext dialog

IMPLEMENT_DYNAMIC(CDlgSettingCamGroup_Ext, CDialog)

CDlgSettingCamGroup_Ext::CDlgSettingCamGroup_Ext(int nGroupId, const CString& strName, CWnd* pParent)
	: CBaseDialog(CDlgSettingCamGroup_Ext::IDD, pParent)
	, m_Groupid(nGroupId)
	, m_GroupName(strName)
	, mb_OK(false)
{
	m_EntryMode = _T("");
}

CDlgSettingCamGroup_Ext::~CDlgSettingCamGroup_Ext()
{
}

void CDlgSettingCamGroup_Ext::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSettingCamGroup_Ext, CDialog)
	//ON_BN_CLICKED(IDC_CHECK1, &CDlgSettingCamGroup_Ext::OnBnClickedCheck1)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CDlgSettingCamGroup_Ext message handlers

int CDlgSettingCamGroup_Ext::vPopulateModule(void)
{
	CString ls_desc, ls_code;

	int ci=0;
	CComboBox* lModule = (CComboBox*)GetDlgItem(IDC_COMBOMODULE);
	CRecordset m_rsModule(&(((CMainFrame*)AfxGetMainWnd())->m_DB));
	m_rsModule.Open(CRecordset::dynaset,_T("select module_desc, module_code, sort_order from ec_hmodule order by sort_order"));
	int ll_rows = m_rsModule.GetRecordCount();

	m_rsModule.MoveFirst();

	while(!m_rsModule.IsEOF()   ) 
	{ 
		m_rsModule.GetFieldValue(_T("module_desc"), ls_desc);
		m_rsModule.GetFieldValue(_T("module_code"), ls_code);

		lModule->InsertString(ci, ls_desc);
		ma_Module[ci] = ls_code;
		ci++;
		m_rsModule.MoveNext();
	}
	return 0;
}

BOOL CDlgSettingCamGroup_Ext::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	
	// TODO:  Add extra initialization here
	CString csDialogCaption(_T(""));
	//csDialogCaption.LoadString(CGS::g_hResStr, IDS_DLG_CAM_GROUP_NAME);
	LOADSTRING(csDialogCaption, IDS_DLG_CAM_GROUP_NAME);
	this->SetWindowTextW(csDialogCaption);
    CString csHint;
    LOADSTRING(csHint, IDS_GROUP_NAME);
    SetDlgItemText(IDC_STATIC_GROUP_NAME, csHint);
    LOADSTRING(csHint, IDS_MODULE);
    SetDlgItemText(IDC_STATIC_MODULE, csHint);
    LOADSTRING(csHint, IDS_DEFAULT);
    SetDlgItemText(IDC_STATIC_DEFAULT, csHint);

	//vDBConnect();	
	vPopulateModule();
	
	vInitGroup(m_EntryMode);

	CComboBox* pGorupModule = (CComboBox*)this->GetDlgItem(IDC_COMBOMODULE);
	pGorupModule->SetCurSel(0);
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CDlgSettingCamGroup_Ext::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CBaseDialog::OnEraseBkgnd(pDC);
}

//int CDlgSettingCamGroup_Ext::vDBConnect(void)
//{
//	//Database m_DB - 
//	CIniReader m_ini(ECINI);
//	//char* cpDns = m_ini.getKeyValue(_T("DSN"),_T("database"));
//	//char* cpUid = m_ini.getKeyValue(_T("UID"),_T("database"));
//	//char* cpPwd = m_ini.getKeyValue(_T("PWD"),_T("database"));
//
//	CString cpDns = m_ini.getKeyValue(_T("DSN"),_T("database")); 
//	CString cpUid = m_ini.getKeyValue(_T("UID"),_T("database"));
//	CString cpPwd = m_ini.getKeyValue(_T("PWD"),_T("database"));
// 
//	CString ls_connect;
//	ls_connect.Format(_T("ODBC;UID=%s;PWD=%s"), cpUid, cpPwd);
//	bool lb_ret = m_DB.Open((LPCTSTR)cpDns, FALSE, FALSE, ls_connect );
//	if (!lb_ret) return -1;
//	return 0;
// 
//}

HBRUSH CDlgSettingCamGroup_Ext::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

int CDlgSettingCamGroup_Ext::vInitGroup(CString o_EntryMode)
{

	//Set the combobox disable if opened from LiveView Tree
	if (o_EntryMode=="L")
		((CComboBox*)GetDlgItem(IDC_COMBOMODULE))->EnableWindow(false);

	if (m_Groupid <= 0) //New Group
	{
        CString csTmp( CProductDefine::m_tzProductName);
        csTmp += _T(" New Group");
		((CEdit*)GetDlgItem(IDC_EDITNAME))->SetWindowTextW( csTmp );
		
		//Select the Liveview Module
		if (o_EntryMode=="L") 
			for (int i=0; i<MAXMODULE; i++)
			{
				if (ma_Module[i].Trim()==_T("LIVEVIEW"))
				{
					((CComboBox*)GetDlgItem(IDC_COMBOMODULE))->SetCurSel(i); //SetWindowTextW(m_sGroup.module);
					break;
				}
			}		
		//((CComboBox*)GetDlgItem(IDC_COMBOMODULE))->SetCurSel(0);
	}
	else if (m_Groupid>0) //Modify Group
	{
		//Group Name
		if (!m_GroupName.IsEmpty())
		{
			((CEdit*)GetDlgItem(IDC_EDITNAME))->SetWindowTextW(m_GroupName);
		}
		else
		{
			CString csTmp( CProductDefine::m_tzProductName);
			csTmp += _T(" New Group");
			((CEdit*)GetDlgItem(IDC_EDITNAME))->SetWindowTextW( csTmp );
		}
		
		//Default - 	Get Default from DB
		CString ls_sql, ls_default, ls_Group;
		ls_sql.Format(_T("SELECT default_ FROM ec_camera_group WHERE cameragroupid= %d "), m_Groupid);
		CRecordset m_rs(&(((CMainFrame*)AfxGetMainWnd())->m_DB));		
		m_rs.Open(CRecordset::forwardOnly, ls_sql);

		while (!m_rs.IsEOF())
		{
			m_rs.GetFieldValue(_T("default_"), ls_default);			
			break;
		}

		if (ls_default.Trim()==_T("Y")) 
			((CButton*)GetDlgItem(IDC_CHECKDEFAULT))->SetCheck(BST_CHECKED);
		else
			((CButton*)GetDlgItem(IDC_CHECKDEFAULT))->SetCheck(BST_UNCHECKED);

		m_rs.Close();
	 
		//Module
		if (o_EntryMode=="L") 
		{
 			for (int i=0; i<MAXMODULE; i++)
			{
				if (ma_Module[i].Trim()==_T("LIVEVIEW"))
				{
					((CComboBox*)GetDlgItem(IDC_COMBOMODULE))->SetCurSel(i); //SetWindowTextW(m_sGroup.module);
					break;
				}
			}
		}
		else
		{
			for (int i=0; i<MAXMODULE; i++)
			{
				ls_Group.Format(_T("%s"),m_sGroup.module);  
				if (ma_Module[i]==ls_Group.Trim())
				{
					((CComboBox*)GetDlgItem(IDC_COMBOMODULE))->SetCurSel(i); //SetWindowTextW(m_sGroup.module);
					break;
				}
			}
		}//if (o_EntryMode=="L") 
		
	}
	return 0;
}

void CDlgSettingCamGroup_Ext::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	//if( AfxMessageBox(_T("WARNING: Camera Group Can not be remove in this version. \r\nAre you sure to add this group ?"), MB_OKCANCEL|MB_ICONQUESTION) == IDOK)
	//{
		mb_OK =true;

		((CEdit*)GetDlgItem(IDC_EDITNAME))->GetWindowTextW(m_GroupName);
		//int ll_idx = ((CComboBox*)GetDlgItem(IDC_COMBOMODULE))->GetCurSel();

		int ll_idx = 1;
		if (ll_idx > MAXMODULE) return;
		m_ModuleCode = ma_Module[ll_idx];
	//}
	CBaseDialog::OnOK();
}

void CDlgSettingCamGroup_Ext::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	mb_OK = false;
	CBaseDialog::OnCancel();
}

INT_PTR CDlgSettingCamGroup_Ext::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class
	//To change font here...

	return CBaseDialog::DoModal(IDD);
}

INT_PTR CDlgSettingCamGroup_Ext::DoModal(HTREEITEM hitem)
{
 
	return CBaseDialog::DoModal(IDD);
}

int CDlgSettingCamGroup_Ext::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}
