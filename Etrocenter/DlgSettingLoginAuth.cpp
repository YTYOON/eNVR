// DlgSettingLoginAuth.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSettingLoginAuth.h"


// CDlgSettingLoginAuth dialog

IMPLEMENT_DYNAMIC(CDlgSettingLoginAuth, CDialog)

CDlgSettingLoginAuth::CDlgSettingLoginAuth(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSettingLoginAuth::IDD, pParent)
{
	m_Crypto.DeriveKey(_T("Encrypt!@#$%^&*()_+-~`"));
}

CDlgSettingLoginAuth::~CDlgSettingLoginAuth()
{
	delete m_ToolTip;
}

void CDlgSettingLoginAuth::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSettingLoginAuth, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_AUTH, &CDlgSettingLoginAuth::OnLvnItemchangedListAuth)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_AUTH, &CDlgSettingLoginAuth::OnLvnItemActivateListAuth)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CDlgSettingLoginAuth::OnHdnItemclickListAuth)
	ON_NOTIFY(NM_CLICK, IDC_LIST_AUTH, &CDlgSettingLoginAuth::OnNMClickListAuth)
	ON_BN_CLICKED(IDC_BUTTON_AUTH_ADD, &CDlgSettingLoginAuth::OnBnClickedButtonAuthAdd)
	ON_BN_CLICKED(IDC_BUTTON_AUTH_DELETE, &CDlgSettingLoginAuth::OnBnClickedButtonAuthDelete)
	ON_BN_CLICKED(IDC_BUTTON_AUTH_MODIFY, &CDlgSettingLoginAuth::OnBnClickedButtonAuthModify)
	ON_BN_CLICKED(IDC_CHECK_LOGIN_AUTO, &CDlgSettingLoginAuth::OnBnClickedCheckAutoLogin)
END_MESSAGE_MAP()


// CDlgSettingLoginAuth message handlers



BOOL CDlgSettingLoginAuth::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	//--Initial List Control
	m_pAuthList = (CListCtrl *)this->GetDlgItem(IDC_LIST_AUTH);
	LV_COLUMN ListColumn;

	CString strUser, strPass, strPermission;
	LOADSTRING(strUser, IDC_USERNAME);
	LOADSTRING(strPass, IDC_PASSWORD);
	LOADSTRING(strPermission, IDC_PERMISSION);

	WCHAR * ListTitles[7] = {_T(""), strUser.GetBuffer(), strPass.GetBuffer(), strPermission.GetBuffer()};

	DWORD nWidth[]={20, 90, 90, 200};
	ListColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	ListColumn.fmt = LVCFMT_LEFT;	
	for(int i = 0; i < 4; i++)
	{
		ListColumn.iSubItem = i;
		ListColumn.cx = nWidth[i];
		ListColumn.pszText = (LPWSTR)(LPCTSTR)ListTitles[i];
		m_pAuthList->InsertColumn(i, &ListColumn);
	}

	DWORD dwStyle = m_pAuthList->GetStyle();
	dwStyle |= LVS_EX_FULLROWSELECT; 
	m_pAuthList->SetExtendedStyle(dwStyle);

	m_pDB = &(((CMainFrame*)AfxGetMainWnd())->m_DB);
	this->vGetAllUser();

	//m_pAuthCombo = (CComboBox*) GetDlgItem(IDC_COMBO_AUTH);			//Set Auth Combo Box
	//m_pAuthCombo->AddString(_T("LiveView, Application"));
	//m_pAuthCombo->AddString(_T("LiveView, Setting, Application"));
	//m_pAuthCombo->SetCurSel(0);

	//--------------Initial Bitmap Button-----------------*/
	if (((CMainFrame*)AfxGetMainWnd())->m_iAuth == LV_SET_APP)
	{
		m_cbDelete.Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(68, 55, 68+24 ,55+24), this, IDC_BUTTON_AUTH_DELETE);
		m_cbAdd.Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(35, 55, 35+24, 55+24), this, IDC_BUTTON_AUTH_ADD);
		m_cbEdit.Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(101, 55, 101+24 ,55+24), this, IDC_BUTTON_AUTH_MODIFY);
		m_cbDelete.LoadBitmaps(IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL);
		m_cbAdd.LoadBitmaps(IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD);
		m_cbEdit.LoadBitmaps(IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT);


		//--Microhelp on buttons--*/
		m_ToolTip= new CToolTipCtrl();
		m_ToolTip->Create(this);
		CString ls_st;
		LOADSTRING(ls_st, IDS_TOOL_DEL_STORE_LOC);
		m_ToolTip->AddTool(&m_cbDelete, ls_st);

		LOADSTRING(ls_st, IDS_TOOL_ADD_STORE_LOC);
		m_ToolTip->AddTool(&m_cbAdd, ls_st);

		LOADSTRING(ls_st, IDS_TOOL_MOD_STORE_LOC);
		m_ToolTip->AddTool(&m_cbEdit, ls_st);
	}

	CString csAuth;
	AfxExtractSubString(csAuth, ((CMainFrame*)AfxGetMainWnd())->szCurrentUser, 0, ';');
	((CButton*)GetDlgItem(IDC_CHECK_LOGIN_AUTO))->SetCheck( ::_ttoi(csAuth) );
	LOADSTRING(csAuth,IDC_CHECK_LOGIN_AUTO);
	SetDlgItemText(IDC_CHECK_LOGIN_AUTO, csAuth);

	if (((CMainFrame*)AfxGetMainWnd())->m_iAuth != LV_SET_APP)
	{
		//GetDlgItem(1075)->EnableWindow(0);
		GetDlgItem(IDC_LIST_AUTH)->EnableWindow(0);

	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSettingLoginAuth::vGetAllUser()
{
	try{

		CString csValue, csStar, csUser, csPass;
		int nItem;

		CRecordset rAuthDB(m_pDB); 
		rAuthDB.Open(CRecordset::forwardOnly, _T("SELECT * FROM ec_user"));
		while(!rAuthDB.IsEOF())
		{
			nItem = m_pAuthList->InsertItem(0, _T(""), 0);
			
			rAuthDB.GetFieldValue(_T("user_name"), csUser);
			m_pAuthList->SetItemText(nItem, 1, csUser.GetBuffer());

			rAuthDB.GetFieldValue(_T("user_pword"), csPass);
			for(int iLength = 0; iLength < csPass.GetLength(); iLength++)
			{
				csStar.Insert(0, _T("*"));
			}
			m_pAuthList->SetItemText(nItem, 2, csStar.GetBuffer());
			csStar = _T("");

			rAuthDB.GetFieldValue(_T("user_group_code"), csValue);
			switch(::_ttoi(csValue.GetBuffer()))
			{
			case 11:
	
				m_pAuthList->SetItemText(nItem, 3, _T("LiveView, Setting, Application"));
				break;
			case 1:
				
				m_pAuthList->SetItemText(nItem, 3, _T("LiveView, Application"));
				break;
			case 10:
				m_pAuthList->SetItemText(nItem, 3, _T("LiveView, Setting"));
				break;
			case 0:
				m_pAuthList->SetItemText(nItem, 3, _T("LiveView"));
				break;
			default:
				ASSERT(true);
				break;
			};

			mapUserPwd.insert(::pair<CString, CString>(csUser, csPass));

			rAuthDB.MoveNext();
		}

		rAuthDB.Close();

	}catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}
void CDlgSettingLoginAuth::OnLvnItemchangedListAuth(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	

	

	*pResult = 0;
}

void CDlgSettingLoginAuth::OnLvnItemActivateListAuth(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	

	*pResult = 0;
}

void CDlgSettingLoginAuth::OnHdnItemclickListAuth(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	

	*pResult = 0;
}

void CDlgSettingLoginAuth::OnNMClickListAuth(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	int nItem = m_pAuthList->GetSelectionMark();
	CString csUser = m_pAuthList->GetItemText(nItem, 1);
	CString csPass = m_pAuthList->GetItemText(nItem, 2);
	CString csAuth = m_pAuthList->GetItemText(nItem, 3);

	/*this->SetDlgItemTextW(IDC_EDIT_AUTH_USER, csUser.GetBuffer());

	this->SetDlgItemTextW(IDC_EDIT_AUTH_PWD, csPass.GetBuffer());

	int iComboNum = 0;
	for(iComboNum = 0; iComboNum < m_pAuthCombo->GetCount(); iComboNum++)
	{
		CString csComboText;
		m_pAuthCombo->GetLBText(iComboNum, csComboText);

		if( csAuth.Compare( csComboText.GetBuffer()) == 0)	break;
	}

	m_pAuthCombo->SetCurSel(iComboNum);*/

	*pResult = 0;
}

void CDlgSettingLoginAuth::OnBnClickedButtonAuthAdd()
{
	// TODO: Add your control notification handler code here
	CDlgSettingPermission m_CDlgSettingPermission;
	
	if( m_CDlgSettingPermission.DoModal() == IDOK )
	{
		if( bCheckUserExist(m_CDlgSettingPermission.m_csUser) )
		{
			CString csString;
			LOADSTRING(csString, IDS_AUTH_ADD_ERROR_1);
			//csString.LoadString(CGS::g_hResStr, IDS_AUTH_ADD_ERROR_1);
			AfxMessageBox(csString);
			return;
		}
		else if( m_CDlgSettingPermission.m_csUser.GetLength() == 0 )
		{
			CString csString;
			LOADSTRING(csString, IDS_AUTH_ADD_ERROR_2);
			//csString.LoadString(CGS::g_hResStr, IDS_AUTH_ADD_ERROR_2);
			AfxMessageBox(csString);
			return;
		}
		else if( m_CDlgSettingPermission.m_csUser.GetLength() == 0 )
		{
			CString csString;
			LOADSTRING(csString, IDS_AUTH_ADD_ERROR_3);
			//csString.LoadString(CGS::g_hResStr, IDS_AUTH_ADD_ERROR_3);
			AfxMessageBox(csString);
			return;
		}
		else if( m_CDlgSettingPermission.m_csPass.GetLength() > 8 || m_CDlgSettingPermission.m_csPass.GetLength() < 4)
		{
			CString csString;
			LOADSTRING(csString, IDS_PWD_LENGTH);
			//csString.LoadString(CGS::g_hResStr, IDS_PWD_LENGTH);
			AfxMessageBox(csString);
			return;
		}

		//-- Encrypt Password
		CByteArray cbArray;
		CString csEncryptPwd;
		m_Crypto.Encrypt(m_CDlgSettingPermission.m_csPass, cbArray);

		for(int iTemp = 0; iTemp < cbArray.GetSize(); iTemp++)
		{
			CString csTemp;
			csTemp.Format(_T("%02X"), cbArray[iTemp]);
			csEncryptPwd = csEncryptPwd + csTemp;				//csEncrypt is the cypher text.
		}

		CString csPermission;
		if(m_CDlgSettingPermission.m_bLV)
		{
			if(csPermission.GetLength() == 0)
			{
				csPermission = csPermission + _T("LiveView");
			}
			else
			{
				csPermission = csPermission + _T(", LiveView");
			}
		}
		if(m_CDlgSettingPermission.m_bSetting)
		{
			if(csPermission.GetLength() == 0)
			{
				csPermission = csPermission + _T("Setting");
			}
			else
			{
				csPermission = csPermission + _T(", Setting");
			}
		}
		if(m_CDlgSettingPermission.m_bApp)
		{
			if(csPermission.GetLength() == 0)
			{
				csPermission = csPermission + _T("Application");
			}
			else
			{
				csPermission = csPermission + _T(", Application");
			}
		}

		int nItem = m_pAuthList->InsertItem(0, _T(""), 0);
		m_pAuthList->SetItemText(nItem, 1, m_CDlgSettingPermission.m_csUser.GetBuffer());	//-- Insert User Name

		CString csStar;
		for(int iTemp = 0; iTemp < csEncryptPwd.GetLength(); iTemp++)
		{
			csStar.Insert(0, _T("*"));
		}
		m_pAuthList->SetItemText(nItem, 2, csStar.GetBuffer());		//--Insert Stars as Password

		m_pAuthList->SetItemText(nItem, 3, csPermission.GetBuffer());	//--Insert Permissions

		mapUserPwd.insert(::pair<CString, CString>(m_CDlgSettingPermission.m_csUser, csEncryptPwd));

	}


	/*CString csUser, csPass, csAuth;
	this->GetDlgItemTextW(IDC_EDIT_AUTH_USER, csUser);
	this->GetDlgItemTextW(IDC_EDIT_AUTH_PWD, csPass);
	m_pAuthCombo->GetLBText( m_pAuthCombo->GetCurSel(), csAuth);
	
	if( bCheckUserExist(csUser) ) 
	{
		AfxMessageBox(_T("User Name exist."));
		return;
	}
	else if( csUser.GetLength() == 0 )
	{
		AfxMessageBox(_T("User Name cannot be blank."));
		return;
	}
	else if( csPass.GetLength() == 0 )
	{
		AfxMessageBox(_T("Password cannot be blank."));
		return;
	}

	int nItem = m_pAuthList->InsertItem(0, _T(""), 0);
	m_pAuthList->SetItemText(nItem, 1, csUser.GetBuffer());
	m_pAuthList->SetItemText(nItem, 2, csPass.GetBuffer());
	m_pAuthList->SetItemText(nItem, 3, csAuth.GetBuffer());

*/
}

void CDlgSettingLoginAuth::OnBnClickedButtonAuthDelete()
{
	// TODO: Add your control notification handler code here
	int nItem = m_pAuthList->GetSelectionMark();
	if( m_pAuthList->GetItemText(nItem, 1).Find(_T("root")) != -1)
	{
		CString csString;
		LOADSTRING(csString, IDS_AUTH_ADD_ERROR_4);
		//csString.LoadString(CGS::g_hResStr, IDS_AUTH_ADD_ERROR_4);
		AfxMessageBox(csString);
		return;
	}

	m_pAuthList->DeleteItem(nItem);
	
}

void CDlgSettingLoginAuth::OnBnClickedButtonAuthModify()
{	
	// TODO: Add your control notification handler code here
	CString csUser, csPass, csAuth;
	bool bLV=0, bSetting=0, bApp=0;
	int nItem = m_pAuthList->GetSelectionMark();
	csUser = m_pAuthList->GetItemText(nItem, 1);
	
	map<CString, CString>::iterator iMapPos = mapUserPwd.begin();
	for(; iMapPos != mapUserPwd.end(); iMapPos++)
	{
		if( csUser.Compare( iMapPos->first ) == 0)
		{
			csPass = iMapPos->second;
			break;
		}
	}

	csAuth = m_pAuthList->GetItemText(nItem, 3);

	if( csAuth.Find(_T("LiveView")) != -1)
	{
		bLV = 1;
	}
	if( csAuth.Find(_T("Setting")) != -1)
	{
		bSetting = 1;
	}
	if( csAuth.Find(_T("Application")) != -1)
	{
		bApp = 1;
	}

	CDlgSettingPermission m_CDlgSettingPermission;
	m_CDlgSettingPermission.vAttach(csUser, csPass, bLV, bSetting, bApp);
	
	if( m_CDlgSettingPermission.DoModal() == IDOK )
	{
		/*if( bCheckUserExist(m_CDlgSettingPermission.m_csUser) )
		{
			AfxMessageBox(_T("User Name exist."));
			return;
		}*/
		if( m_CDlgSettingPermission.m_csUser.GetLength() == 0 )
		{
			CString csString;
			//csString.LoadString(CGS::g_hResStr, IDS_AUTH_ADD_ERROR_2);
			LOADSTRING(csString, IDS_AUTH_ADD_ERROR_2);
			AfxMessageBox(csString);
			//AfxMessageBox(_T("User Name cannot be blank."));
			return;
		}
		else if( m_CDlgSettingPermission.m_csUser.GetLength() == 0 )
		{
			CString csString;
			//csString.LoadString(CGS::g_hResStr, IDS_AUTH_ADD_ERROR_3);
			LOADSTRING(csString, IDS_AUTH_ADD_ERROR_3);
			AfxMessageBox(csString);
			//AfxMessageBox(_T("Password cannot be blank."));
			return;
		}
	
		CString csEncryptPwd;
		if(!m_CDlgSettingPermission.m_bORG)
		{
			if( m_CDlgSettingPermission.m_csPass.GetLength() > 8 || m_CDlgSettingPermission.m_csPass.GetLength() < 4)
			{
				CString csString;
				//csString.LoadString(CGS::g_hResStr, IDS_PWD_LENGTH);
				LOADSTRING(csString, IDS_PWD_LENGTH);
				AfxMessageBox(csString);
				//AfxMessageBox(_T("Password must be between 4 and 8 characters."));
				return;
			}

			//-- Encrypt Password
			CByteArray cbArray;
			
			m_Crypto.Encrypt(m_CDlgSettingPermission.m_csPass, cbArray);

			for(int iTemp = 0; iTemp < cbArray.GetSize(); iTemp++)
			{
				CString csTemp;
				csTemp.Format(_T("%02X"), cbArray[iTemp]);
				csEncryptPwd = csEncryptPwd + csTemp;				//csEncrypt is the cypher text.
			}
		}
		else
		{
			csEncryptPwd = m_CDlgSettingPermission.m_csPass;
		}

		CString csPermission;
		if(m_CDlgSettingPermission.m_bLV)
		{
			if(csPermission.GetLength() == 0)
			{
				csPermission = csPermission + _T("LiveView");
			}
			else
			{
				csPermission = csPermission + _T(", LiveView");
			}
		}
		if(m_CDlgSettingPermission.m_bSetting)
		{
			if(csPermission.GetLength() == 0)
			{
				csPermission = csPermission + _T("Setting");
			}
			else
			{
				csPermission = csPermission + _T(", Setting");
			}
		}
		if(m_CDlgSettingPermission.m_bApp)
		{
			if(csPermission.GetLength() == 0)
			{
				csPermission = csPermission + _T("Application");
			}
			else
			{
				csPermission = csPermission + _T(", Application");
			}
		}

		//int nItem = m_pAuthList->InsertItem(0, _T(""), 0);
		m_pAuthList->SetItemText(nItem, 1, m_CDlgSettingPermission.m_csUser.GetBuffer());	//-- Insert User Name

		CString csStar;
		for(int iTemp = 0; iTemp < csEncryptPwd.GetLength(); iTemp++)
		{
			csStar.Insert(0, _T("*"));
		}
		m_pAuthList->SetItemText(nItem, 2, csStar.GetBuffer());		//--Insert Stars as Password
		iMapPos->second = csEncryptPwd;

		m_pAuthList->SetItemText(nItem, 3, csPermission.GetBuffer());	//--Insert Permissions
		
		//mapUserPwd.insert(::pair<CString, CString>(m_CDlgSettingPermission.m_csUser, csEncryptPwd));

	}

	/*int nItem = m_pAuthList->GetSelectionMark();
	CString csUser, csPass, csAuth, csStar;
	this->GetDlgItemTextW(IDC_EDIT_AUTH_USER, csUser);
	this->GetDlgItemTextW(IDC_EDIT_AUTH_PWD, csPass);
	m_pAuthCombo->GetLBText( m_pAuthCombo->GetCurSel(), csAuth);

	if(csUser.GetLength() == 0)
	{
		AfxMessageBox(_T("User Name cannot be blank."));
		return;
	}
	else if( csPass.GetLength() == 0 )
	{
		AfxMessageBox(_T("Password cannot be blank."));
		return;
	}

	m_pAuthList->SetItemText(nItem, 1, csUser.GetBuffer());
	
	for(int iTemp = 0; iTemp < csPass.GetLength(); iTemp++)
	{
		csStar.Insert(0, _T("*"));
	}
	m_pAuthList->SetItemText(nItem, 2, csStar.GetBuffer());


	map<CString, CString>::iterator iMapPos = mapUserPwd.begin();
	for(; iMapPos != mapUserPwd.end(); iMapPos++)
	{
		if( csUser.Compare( iMapPos->first ) == 0)
		{
			iMapPos->second = csPass;
			break;
		}
	}


	m_pAuthList->SetItemText(nItem, 3, csAuth.GetBuffer());
*/
}

bool CDlgSettingLoginAuth::bCheckUserExist(CString &csUserName)
{
	for(int nItem = 0; nItem < m_pAuthList->GetItemCount(); nItem++)
	{
		CString csListUser = m_pAuthList->GetItemText(nItem, 1);

		if( csListUser.Compare(csUserName.GetBuffer()) == 0)
		{
			return true;
		}
	}

	return false;
}

bool CDlgSettingLoginAuth::bSaveInfo2DB()
{
	CString csSQL, csUser, csPass, csAuth, csGroupCode;

	try{
		
		csSQL.Format(_T("DELETE FROM ec_user"));
		m_pDB->ExecuteSQL(csSQL.GetBuffer());

		for(int iListNum = 0; iListNum < m_pAuthList->GetItemCount(); iListNum++)
		{
			csUser = m_pAuthList->GetItemText(iListNum, 1);

			map<CString, CString>::iterator iMapPos = mapUserPwd.begin();
			for(; iMapPos != mapUserPwd.end(); iMapPos++)
			{
				if( csUser.Compare( iMapPos->first ) == 0)
				{
					csPass = iMapPos->second;
					break;
				}
			}

			//csPass = mapUserPwd.find(csUser.GetBuffer())->second;
			csAuth = m_pAuthList->GetItemText(iListNum, 3);

			if( csAuth.Find(_T("Setting")) != -1 )
			{
				csGroupCode = csGroupCode + _T("1");
			}else
			{
				csGroupCode = csGroupCode + _T("0");
			}

			if( csAuth.Find(_T("Application")) != -1 )
			{
				csGroupCode = csGroupCode + _T("1");
			}else
			{
				csGroupCode = csGroupCode + _T("0");
			}

			//if(csAuth.Compare(_T("LiveView, Setting, Application")) == 0)
			/*if( csAuth.Find(_T("Setting")) != -1 )
			{
				csSQL.Format(_T("INSERT INTO ec_user(userid, user_name, user_pword, user_group_code) VALUES (%d, '%s', '%s', '%s')"), iListNum+1, csUser.GetBuffer(), csPass.GetBuffer() );
			}
			else
			{
				csSQL.Format(_T("INSERT INTO ec_user(userid, user_name, user_pword, user_group_code) VALUES (%d, '%s', '%s', 2)"), iListNum+1, csUser.GetBuffer(), csPass.GetBuffer() );
			}*/

			csSQL.Format(_T("INSERT INTO ec_user(userid, user_name, user_pword, user_group_code) VALUES (%d, '%s', '%s', '%s')"), iListNum+1, csUser.GetBuffer(), csPass.GetBuffer(), csGroupCode.GetBuffer());
			m_pDB->ExecuteSQL(csSQL.GetBuffer());

			csGroupCode = _T("");
		}

	}catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
		return false;
	}

	CRegKey regKey;
	const DWORD len = 1024;
	DWORD nActualLen(len);

	CString strForder;
	vGetForderString(strForder);

	LONG nRet = regKey.Open(HKEY_LOCAL_MACHINE, CProductDefine::m_tzRegistSubFolder);
	if(nRet != ERROR_SUCCESS)
	{
		if( regKey.Create(HKEY_LOCAL_MACHINE, (LPCTSTR)strForder) != 0)
		{
			TRACE(_T("Create Registry Fail."));
			//return 0; 
		}
	}
	
	if(m_csRegValue.GetLength() == 0)
	{
		m_csRegValue.Format(_T("%s"), ((CMainFrame*)AfxGetMainWnd())->szCurrentUser);
	}
	else
	{
		::_tcscpy(((CMainFrame*)AfxGetMainWnd())->szCurrentUser, m_csRegValue.GetBuffer());
	}
	regKey.SetValue(m_csRegValue, _T("USERPWD"));
	regKey.Close();

	return true;
}

void CDlgSettingLoginAuth::vGetForderString(CString& strFolder)
{
	if (CNVRBk::GetBkObject()->GetOEMCompany() == GNC_Company)
	{
		strFolder.Format(_T("SOFTWARE\\Company\\CMS\\"));
	}
	else if(CNVRBk::GetBkObject()->GetOEMCompany() == XTS_Company)
	{
		strFolder.Format(_T("SOFTWARE\\XTS\\CMS\\"));
	}
	else
	{
		if(CNVRBk::GetBkObject()->isNVRType())
		{
			strFolder.Format(_T("SOFTWARE\\Etrovision Technology\\NVR\\"));
		}
		else
		{
			strFolder.Format(_T("SOFTWARE\\Etrovision Technology\\EtroCenter\\"));
		}
		
	}
}

void CDlgSettingLoginAuth::OnBnClickedCheckAutoLogin()
{
	// TODO: Add your control notification handler code here
	CDlgSettingLogin m_CDlgSettingLogin;
	//CString csRegValue;
	if( ((CButton*)this->GetDlgItem(IDC_CHECK_LOGIN_AUTO))->GetCheck() )
	{
		if(m_CDlgSettingLogin.DoModal() == IDOK)
		{
			if(m_CDlgSettingLogin.m_bOK == true)
			{
				//-- Encrypt Password
				CByteArray cbArray;
				CString csEncryptPwd;
				m_Crypto.Encrypt(m_CDlgSettingLogin.m_csPass, cbArray);

				for(int iTemp = 0; iTemp < cbArray.GetSize(); iTemp++)
				{
					CString csTemp;
					csTemp.Format(_T("%02X"), cbArray[iTemp]);
					csEncryptPwd = csEncryptPwd + csTemp;				//csEncrypt is the cypher text.
				}

				m_csRegValue.Format(_T("1;%s;%s;"), m_CDlgSettingLogin.m_csUser, csEncryptPwd);
			}
			else
			{
				((CButton*)this->GetDlgItem(IDC_CHECK_LOGIN_AUTO))->SetCheck(0);
			}
		}
	}
	else
	{
		CString csCurrentUser, csUser, csPass;
		
		AfxExtractSubString(csUser, ((CMainFrame*)AfxGetMainWnd())->szCurrentUser, 1, ';');
		AfxExtractSubString(csPass, ((CMainFrame*)AfxGetMainWnd())->szCurrentUser, 2, ';');

		m_csRegValue.Format(_T("0;%s;%s;"), csUser, csPass);
	}
}

CString CDlgSettingLoginAuth::csDecryptPassword(CString &csEncrypt)
{
	CString csDecrypt;
	CByteArray cbArray;

	LPCSTR lpSTR;
	USES_CONVERSION;
	lpSTR = W2A(csEncrypt.LockBuffer());
	csEncrypt.UnlockBuffer();

	unsigned char hexdata[32];

	memset(hexdata, 0, sizeof(unsigned char)*32);

	strtohex(lpSTR, hexdata);

	cbArray.SetSize(csEncrypt.GetLength()/2);

	for(int iTemp = 0; iTemp < (csEncrypt.GetLength()/2); iTemp++)
	{
		cbArray[iTemp] = (BYTE)hexdata[iTemp];
	}

	m_Crypto.Decrypt(cbArray, csDecrypt);

	return csDecrypt;
}

void CDlgSettingLoginAuth::strtohex(const char *str, unsigned char *hex)
{
	int i,len = strlen(str);
	char* t;
	unsigned char* x;

	for(i=0,t=(char *)str,x=hex;i<len;i+=2,x++,t+=2)
	{
		if(*t >= '0' && *t <= '9')
		{
			*x = ((*t & 0x0f) << 4);
		}
		else
		{
			char h = 0x0a + tolower(*t) - 'a';
			*x = (h << 4) ;
		}
		if(*(t+1) >= '0' && *(t+1) <= '9')
		{
			*x |= (*(t+1) & 0x0f);
		}
		else
		{
			char l = 0x0a + tolower(*(t+1)) - 'a';
			*x += l;
		}
	}
}