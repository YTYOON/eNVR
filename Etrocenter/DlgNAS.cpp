// DlgNAS.cpp : implementation file
//

#include "stdafx.h"
#include "DlgNAS.h"

// CDlgNAS dialog

IMPLEMENT_DYNAMIC(CDlgNAS, CDialog)

CDlgNAS::CDlgNAS(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNAS::IDD, pParent)
{

}

CDlgNAS::~CDlgNAS()
{
}

void CDlgNAS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgNAS, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADD_NAS, &CDlgNAS::OnBnClickedBtnAddNas)
	ON_BN_CLICKED(IDC_BTN_MODIFY_NAS, &CDlgNAS::OnBnClickedBtnModifyNas)
	ON_BN_CLICKED(IDC_BTN_DELETE_NAS, &CDlgNAS::OnBnClickedBtnDeleteNas)
	ON_LBN_SELCHANGE(IDC_LIST_NAS_STORAGE, &CDlgNAS::OnLbnSelchangeListNasStorage)
	ON_BN_CLICKED(IDOK, &CDlgNAS::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgNAS message handlers

BOOL CDlgNAS::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	mp_DB = &(((CMainFrame*)AfxGetMainWnd())->m_DB);

	//--------------Initial Bitmap Button-----------------*/
	m_cbNASDelete.SubclassDlgItem(IDC_BTN_DELETE_NAS, this);
	m_cbNASAdd.SubclassDlgItem(IDC_BTN_ADD_NAS, this);
	m_cbNASEdit.SubclassDlgItem(IDC_BTN_MODIFY_NAS, this);

	m_cbNASDelete.LoadBitmaps(IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL);
	m_cbNASAdd.LoadBitmaps(IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD);
	m_cbNASEdit.LoadBitmaps(IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT);

	//--Microhelp on buttons--*/
	m_ToolTip= new CToolTipCtrl();
	m_ToolTip->Create(this);
	CString ls_st;
	//ls_st.LoadString(CGS::g_hResStr, IDS_TOOL_DEL_STORE_LOC);
	LOADSTRING(ls_st, IDS_TOOL_DEL_NAS);
	m_ToolTip->AddTool(&m_cbNASDelete, ls_st);

	//ls_st.LoadString(CGS::g_hResStr, IDS_TOOL_ADD_STORE_LOC);
	LOADSTRING(ls_st, IDS_TOOL_ADD_NAS);
	m_ToolTip->AddTool(&m_cbNASAdd, ls_st);

	//ls_st.LoadString(CGS::g_hResStr, IDS_TOOL_MOD_STORE_LOC);
	LOADSTRING(ls_st, IDS_TOOL_MOD_NAS);
	m_ToolTip->AddTool(&m_cbNASEdit, ls_st);

	m_ToolTip->Activate(TRUE);

	//--Initial NAS List Control
	
	//CCheckListBox * pNASList = (CCheckListBox*)this->GetDlgItem(IDC_LIST_NAS_STORAGE);
	pNASList.SubclassDlgItem(IDC_LIST_NAS_STORAGE, this);
	pNASList.SetCheckStyle(BS_3STATE);
	pNASList.ResetContent();

	int nItem = 0;

	try{
		CString csNAS_Value;
		CRecordset rAllNAS(mp_DB); 
		rAllNAS.Open(CRecordset::forwardOnly, _T("SELECT * FROM ec_storage WHERE storage_type = 'Samba' OR storage_type = 'NFS'"));
		while (!rAllNAS.IsEOF()) {
		
			rAllNAS.GetFieldValue(_T("storage_type"), csNAS_Value);
			csNAS_Value.TrimRight();
			::_tcscpy(m_NASInfo.tcServerType, csNAS_Value.GetBuffer());

			rAllNAS.GetFieldValue(_T("store_location"), csNAS_Value);
			csNAS_Value.TrimRight();
			::_tcscpy(m_NASInfo.tcServerAddr, csNAS_Value.GetBuffer());

			rAllNAS.GetFieldValue(_T("server_user"), csNAS_Value);
			csNAS_Value.TrimRight();
			::_tcscpy(m_NASInfo.tcServerUser, csNAS_Value.GetBuffer());

			rAllNAS.GetFieldValue(_T("server_password"), csNAS_Value);
			csNAS_Value.TrimRight();
			::_tcscpy(m_NASInfo.tcServerPass, csNAS_Value.GetBuffer());

			veNASInfo.push_back(m_NASInfo);

			nItem = pNASList.AddString(m_NASInfo.tcServerAddr);
			pNASList.SetCheck(nItem, 0);
			pNASList.Enable(nItem, 1);

			rAllNAS.MoveNext();
		}
		rAllNAS.Close();

	}catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNAS::OnBnClickedOk()
{
	::ZeroMemory(&m_NASInfo, sizeof(NASInfo));

	CString csCheckAddr;
	for(int nItem = 0; nItem < pNASList.GetCount(); nItem++)
	{
		if(pNASList.GetCheck(nItem))
		{
			pNASList.GetText(nItem, csCheckAddr);
		}
	}

	try
	{
		CString csSQL;
		csSQL.Format(_T("DELETE FROM ec_storage WHERE storage_type = 'Samba' OR storage_type = 'NFS'"));

		mp_DB->ExecuteSQL(csSQL.GetBuffer());

		for(int iTemp = 0; iTemp < veNASInfo.size(); iTemp++)
		{
			if( csCheckAddr.Find(veNASInfo[iTemp].tcServerAddr) != -1)
			{
				m_NASInfo = veNASInfo[iTemp];
			}
			
			csSQL.Format(_T("INSERT INTO ec_storage (storage_type, store_location, server_user, server_password) VALUES ('%s', '%s', '%s', '%s')"), veNASInfo[iTemp].tcServerType, veNASInfo[iTemp].tcServerAddr, veNASInfo[iTemp].tcServerUser, veNASInfo[iTemp].tcServerPass);

			mp_DB->ExecuteSQL(csSQL.GetBuffer());
		}

	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	OnOK();
}

void CDlgNAS::OnBnClickedBtnAddNas()
{
	CDlgNASSetting m_CDlgNASSetting;
	if(m_CDlgNASSetting.DoModal() == IDOK)
	{
		m_NASInfo = m_CDlgNASSetting.m_NASInfo;
		int nItem = pNASList.AddString(m_NASInfo.tcServerAddr);
		pNASList.SetCheck(nItem, 0);
		pNASList.Enable(nItem, 1);

		veNASInfo.push_back(m_NASInfo);
	}


}

void CDlgNAS::OnBnClickedBtnModifyNas()
{
	int nItem = pNASList.GetCurSel();
	if(nItem < 0)	
	{
		CString csWarnning;
		LOADSTRING(csWarnning, IDS_NAS_WARNNING) 
		AfxMessageBox(csWarnning);
		return;
	}

	CString csCurSelText;
	pNASList.GetText(nItem, csCurSelText);

	int iTemp = 0;
	for(; iTemp < veNASInfo.size(); iTemp++)
	{
		if(csCurSelText.Compare(veNASInfo[iTemp].tcServerAddr) == 0)
		{
			break;
		}
	}

	CDlgNASSetting m_CDlgNASSetting(veNASInfo[iTemp]);
	if(m_CDlgNASSetting.DoModal() == IDOK)
	{
		veNASInfo.erase(veNASInfo.begin() + iTemp);
		veNASInfo.push_back(m_CDlgNASSetting.m_NASInfo);
		//veNASInfo[iTemp] = m_CDlgNASSetting.m_NASInfo;
		pNASList.DeleteString(nItem);
		pNASList.AddString(m_CDlgNASSetting.m_NASInfo.tcServerAddr);
	}
}

void CDlgNAS::OnBnClickedBtnDeleteNas()
{
	int nItem = pNASList.GetCurSel();
	if(nItem < 0)	
	{
		CString csWarnning;
		LOADSTRING(csWarnning, IDS_NAS_WARNNING) 
		AfxMessageBox(csWarnning);
		return;
	}

	CString csCurSelText;
	pNASList.GetText(nItem, csCurSelText);

	vector<NASInfo>::iterator iPos = veNASInfo.begin();
	int iTemp = 0;
	for(; iTemp < veNASInfo.size(); iTemp++)
	{
		if(csCurSelText.Compare(veNASInfo[iTemp].tcServerAddr) == 0)
		{
			break;
		}
	}

	pNASList.DeleteString(nItem);
	veNASInfo.erase(veNASInfo.begin() + iTemp);
}


void CDlgNAS::OnLbnSelchangeListNasStorage()
{
	// TODO: Add your control notification handler code here
	int nItem = pNASList.GetCurSel();
	if(pNASList.GetCheck(nItem) == 0)
	{
		for(int iTemp = 0; iTemp <= pNASList.GetCount(); iTemp++)
		{
			pNASList.SetCheck(iTemp, 0);
		}

		pNASList.SetCheck(nItem, 1);
	}
	else if(pNASList.GetCheck(nItem) == 1)
	{
		pNASList.SetCheck(nItem, 0);
	}
}
