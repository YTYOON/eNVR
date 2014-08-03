// DlgSettingFtpEmail.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSettingFtpEmail.h"
#include "MainFrm.h"

// CDlgSettingFtpEmail dialog

IMPLEMENT_DYNAMIC(CDlgSettingFtpEmail, CBaseDialog)

CDlgSettingFtpEmail::CDlgSettingFtpEmail(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgSettingFtpEmail::IDD, pParent), DirtyFlag(false)
{
}

CDlgSettingFtpEmail::~CDlgSettingFtpEmail()
{
//	if (&m_DB) m_DB.Close();
	if(m_ToolTip) delete m_ToolTip;
}

void CDlgSettingFtpEmail::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSettingFtpEmail, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUT_SERVERADD, &CDlgSettingFtpEmail::OnBnClickedButServeradd)
	ON_BN_CLICKED(IDC_BUT_SERVERDEL, &CDlgSettingFtpEmail::OnBnClickedButServerdel)
	ON_BN_CLICKED(IDC_BUT_SERVERMOD, &CDlgSettingFtpEmail::OnBnClickedButServermod)
	ON_BN_CLICKED(IDC_BUT_ADD, &CDlgSettingFtpEmail::OnBnClickedButAdd)
	ON_BN_CLICKED(IDC_BUT_MOD, &CDlgSettingFtpEmail::OnBnClickedButMod)
	ON_BN_CLICKED(IDC_BUT_DEL, &CDlgSettingFtpEmail::OnBnClickedButDel)
END_MESSAGE_MAP()


// CDlgSettingFtpEmail message handlers

BOOL CDlgSettingFtpEmail::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CBaseDialog::OnEraseBkgnd(pDC);
}

HBRUSH CDlgSettingFtpEmail::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

int CDlgSettingFtpEmail::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	
	return 0;
}

BOOL CDlgSettingFtpEmail::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_CCrypto.DeriveKey(_T("Encrypt!@#$%^&*()_+-~`"));
	//vDBConnect();
	vInitEmail();
	vRetrieveEmail();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSettingFtpEmail::vInitEmail(void)
{
	pServerList = (CListCtrl*) GetDlgItem(IDC_LISTSERVER);
	pEmailList = (CListCtrl*) GetDlgItem(IDC_LIST1);	
	
	DWORD dwStyle = pServerList->GetStyle();
	dwStyle |= LVS_EX_FULLROWSELECT ;	
	pServerList->SetExtendedStyle(dwStyle);

	pEmailList->SetExtendedStyle(LVS_EX_FULLROWSELECT);

	int ll_y=30;
	//CBitmapButton* m_butSAdd= new CBitmapButton ;
	dwStyle =  WS_CHILD|WS_VISIBLE|BS_OWNERDRAW ;
	m_butSAdd.Create(NULL, dwStyle, CRect(20, ll_y, 44, ll_y+24) ,this ,IDC_BUT_SERVERADD);
	m_butSAdd.LoadBitmaps(IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD,IDB_CAMGROUP_ADD);
	m_butSAdd.ShowWindow(SW_SHOW);

	//CBitmapButton* m_butSDel = new CBitmapButton ;
	m_butSDel.Create(NULL, dwStyle, CRect(50, ll_y, 74, ll_y+24) ,this ,IDC_BUT_SERVERDEL);
	m_butSDel.LoadBitmaps(IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL,IDB_CAMGROUP_DEL_DIS);
	m_butSDel.ShowWindow(SW_SHOW);

	//CBitmapButton* m_butSMod = new CBitmapButton ;
	m_butSMod.Create(NULL, dwStyle, CRect(80, ll_y, 104, ll_y+24) ,this , IDC_BUT_SERVERMOD);
	m_butSMod.LoadBitmaps(IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT,IDB_CAMGROUP_EDIT_DIS);
	m_butSMod.ShowWindow(SW_SHOW);

	ll_y = 185;
	//CBitmapButton* m_butAdd= new CBitmapButton ;

	//DWORD dwStyle =  WS_CHILD|WS_VISIBLE|BS_OWNERDRAW ;
	m_butAdd.Create(NULL, dwStyle, CRect(20, ll_y, 44, ll_y+24) ,this ,IDC_BUT_ADD);
	m_butAdd.LoadBitmaps(IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD,IDB_CAMGROUP_ADD);
	m_butAdd.ShowWindow(SW_SHOW);

	//CBitmapButton* m_butDel = new CBitmapButton ;
	m_butDel.Create(NULL, dwStyle, CRect(50, ll_y, 74, ll_y+24) ,this ,IDC_BUT_DEL);
	m_butDel.LoadBitmaps(IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL,IDB_CAMGROUP_DEL_DIS);
	m_butDel.ShowWindow(SW_SHOW);

	//CBitmapButton* m_butMod = new CBitmapButton ;
	m_butMod.Create(NULL, dwStyle, CRect(80, ll_y, 104, ll_y+24) ,this , IDC_BUT_MOD);
	m_butMod.LoadBitmaps(IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT,IDB_CAMGROUP_EDIT_DIS);
	m_butMod.ShowWindow(SW_SHOW);

	//Create columns for server
	LV_COLUMN ListColumn;
	WCHAR * ListTitles[3] = {_T("SMTP Server"), _T("User Name"), _T("Password")};
	int ll_colw=176;

	for(int i = 0; i < 3; i++)
	{
		ListColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		ListColumn.fmt = LVCFMT_LEFT;
		ListColumn.iSubItem = i;

		{ListColumn.cx = ll_colw;}
		
		ListColumn.pszText = ListTitles[i];
		pServerList->InsertColumn(i, &ListColumn);
	}

		//Create columns for email
 	WCHAR * ListTit[1] = {_T("Email Address")};
	ll_colw=530;

		ListColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		ListColumn.fmt = LVCFMT_LEFT;
		ListColumn.iSubItem = 0;

		ListColumn.cx = ll_colw;
		
		ListColumn.pszText = ListTit[0];
		pEmailList->InsertColumn(0, &ListColumn);

	/*--Microhelp on buttons--*/
	m_ToolTip= new CToolTipCtrl();
	m_ToolTip->Create(this);
	CString ls_st; //string table
	LOADSTRING(ls_st, IDS_TOOL_ADD_SMTP_SERVER);
	//ls_st.LoadString(CGS::g_hResStr, IDS_TOOL_ADD_SMTP_SERVER);
	m_ToolTip->AddTool(&m_butSAdd,ls_st);

	LOADSTRING(ls_st, IDS_TOOL_DEL_SMTP_SERVER);
	//ls_st.LoadString(CGS::g_hResStr, IDS_TOOL_DEL_SMTP_SERVER);
	m_ToolTip->AddTool(&m_butSDel,ls_st);

	LOADSTRING(ls_st, IDS_TOOL_MOD_SMTP_SERVER);
	//ls_st.LoadString(CGS::g_hResStr, IDS_TOOL_MOD_SMTP_SERVER);
 	m_ToolTip->AddTool(&m_butSMod,ls_st);

	LOADSTRING(ls_st, IDS_TOOL_ADD_EMAIL_ADDRESS);
	//ls_st.LoadString(CGS::g_hResStr, IDS_TOOL_ADD_EMAIL_ADDRESS);
	m_ToolTip->AddTool(&m_butAdd,ls_st);

	LOADSTRING(ls_st, IDS_TOOL_DEL_EMAIL_ADDRESS);
	//ls_st.LoadString(CGS::g_hResStr, IDS_TOOL_DEL_EMAIL_ADDRESS);
	m_ToolTip->AddTool(&m_butDel,ls_st);

	LOADSTRING(ls_st, IDS_TOOL_MOD_EMAIL_ADDRESS);
	//ls_st.LoadString(CGS::g_hResStr, IDS_TOOL_MOD_EMAIL_ADDRESS);
	m_ToolTip->AddTool(&m_butMod,ls_st);

	//m_ToolTip->AddTool(&m_butSAdd,_T("Add New SMTP Server"));
	//m_ToolTip->AddTool(&m_butSDel,_T("Delete SMTP Server"));
	//m_ToolTip->AddTool(&m_butSMod,_T("Modify SMTP Server"));
	//m_ToolTip->AddTool(&m_butAdd,_T("Add new email address"));
	//m_ToolTip->AddTool(&m_butDel,_T("Delete email address"));
	//m_ToolTip->AddTool(&m_butMod,_T("Modify Email Server"));
	m_ToolTip->Activate(TRUE);

}

void CDlgSettingFtpEmail::OnBnClickedButServeradd()
{
	// TODO: Add your control notification handler code here
	CDlgAddEditMailServer *m_CDlgAddEditMailServer = new CDlgAddEditMailServer(AddServer);

	if(m_CDlgAddEditMailServer->DoModal() == IDOK)
	{
		memcpy(&m_MailInfo, &m_CDlgAddEditMailServer->m_MailInfo, sizeof(m_MailInfo));
		DirtyFlag = true;
	}
	else
	{delete m_CDlgAddEditMailServer;return;}

	delete m_CDlgAddEditMailServer;

	/*--Encrypt Password--*/
	CByteArray cbArray;
	CString csEncrypt, csPassword;

	csPassword.Format(_T("%s"), m_MailInfo.cPassword);
	m_CCrypto.Encrypt(csPassword, cbArray);

	for(int iTemp = 0; iTemp < cbArray.GetSize(); iTemp++)
	{
		CString csTemp;
		csTemp.Format(_T("%02X"), cbArray[iTemp]);
		csEncrypt = csEncrypt + csTemp;				//csEncrypt is the cypher text.
	}

	_tcscpy(m_MailInfo.cPassword, csEncrypt);

	int nItem = pServerList->InsertItem(0, (LPCTSTR)m_MailInfo.cSMTPServer);
	pServerList->SetItemText(nItem, 1, (LPCTSTR)m_MailInfo.cUserName);
	pServerList->SetItemText(nItem, 2, (LPCTSTR)m_MailInfo.cPassword);
	//pServerList->SetItemText(nItem, 2, _T("********"));
	pServerList->SetCheck(nItem, 1);
	
	//veMailServer.push_back(m_MailInfo);
}

void CDlgSettingFtpEmail::OnBnClickedButServerdel()
{
	// TODO: Add your control notification handler code here
	int nItem = pServerList->GetSelectionMark();
	
	pServerList->DeleteItem(nItem);
	DirtyFlag = true;
}

void CDlgSettingFtpEmail::vRetrieveEmail(void)
{
	CMainFrame* lframe = (CMainFrame*)GetParentFrame();
	CString ls_server, ls_mserver, ls_saccount, ls_spass, ls_active, ls_target_mail_address, csEmailCount, csMailAddress;
	int ll_srow=0, ll_row=0;

	try
	{
		CRecordset m_rs(&lframe->m_DB);
		m_rs.Open(CRecordset::forwardOnly, _T("SELECT COUNT(*) AS email_count FROM ec_email"));
		while (!m_rs.IsEOF()) { 
			m_rs.GetFieldValue(_T("email_count"), csEmailCount);
			m_rs.MoveNext();
		}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	if(csEmailCount == _T("0"))
	{return;}
 
	CRecordset m_rs(&lframe->m_DB);
	m_rs.Open(CRecordset::dynaset,_T("select * from ec_email"));
 
	m_rs.MoveFirst();
	while (!m_rs.IsEOF()) { 
				
		m_rs.GetFieldValue(_T("server_"), ls_server);
		m_rs.GetFieldValue(_T("mail_server"), ls_mserver);
		m_rs.GetFieldValue(_T("server_account"), ls_saccount);
		m_rs.GetFieldValue(_T("server_password"), ls_spass);
		m_rs.GetFieldValue(_T("target_email_addr"), ls_target_mail_address);
		m_rs.GetFieldValue(_T("active_"), ls_active);
		
		
		ls_spass.Trim();

		if (ls_server.Trim()==_T("Y"))
		{
			int nItem = pServerList->InsertItem(ll_srow,_T(""));
			pServerList->SetItemText(nItem, 0,  ls_mserver.Trim());
			pServerList->SetItemText(nItem, 1,  ls_saccount.Trim());
			pServerList->SetItemText(nItem, 2,  ls_spass.Trim());
			//pServerList->SetItemText(nItem, 2,  _T("********"));

			if(ls_active == _T("Y"))
				pServerList->SetCheck(nItem, 1);
		
			//memcpy(m_MailInfo.cSMTPServer, ls_mserver.Trim(), sizeof(m_MailInfo.cSMTPServer)/sizeof(TCHAR));
			//memcpy(m_MailInfo.cUserName, ls_saccount.Trim(), sizeof(m_MailInfo.cUserName)/sizeof(TCHAR));
			//memcpy(m_MailInfo.cPassword, ls_spass.Trim(), sizeof(m_MailInfo.cPassword)/sizeof(TCHAR));
			
			/*--Get Password and Decrypt From Registry--*/
			//ReadPasswordFromRegistry(m_MailInfo.cPassword, m_MailInfo.cSMTPServer); 
			//m_MailInfo.cPassword[(iPwdLength / 2)] = L'\0';

			//veMailServer.push_back(m_MailInfo);
		}
		else if(ls_server.Trim()==_T("N"))
		{
			int nItem = pEmailList->InsertItem(0, _T(""));
			pEmailList->SetItemText(nItem, 0, ls_target_mail_address.Trim());

			if(ls_active == _T("Y"))
				pEmailList->SetCheck(nItem, 1);
		}
		//m_rs.GetFieldValue(_T("default_"), ls_default);
		m_rs.MoveNext();
	}
 

}

void CDlgSettingFtpEmail::OnBnClickedButServermod()
{
	// TODO: Add your control notification handler code here
	int nItem = pServerList->GetSelectionMark();
	if(nItem == -1)
	{
		AfxMessageBox(IDS_SELECT_SERVER);
		//AfxMessageBox(_T("Please Select a Server"));
		return;
	}

	_tcscpy(m_MailInfo.cSMTPServer, pServerList->GetItemText(nItem, 0));
	_tcscpy(m_MailInfo.cUserName, pServerList->GetItemText(nItem, 1));
	CString csDecrypt = DecryptEMailServerPassword(pServerList->GetItemText(nItem, 2));
	_tcscpy(m_MailInfo.cPassword, csDecrypt);

	CDlgAddEditMailServer *m_CDlgAddEditMailServer = new CDlgAddEditMailServer(EditServer);
	m_CDlgAddEditMailServer->vSetServerInfo(m_MailInfo);
	
	if(m_CDlgAddEditMailServer->DoModal() == IDOK)
	{
		memcpy(&m_MailInfo, &m_CDlgAddEditMailServer->m_MailInfo, sizeof(m_MailInfo));
		DirtyFlag = true;
	}
	else
	{delete m_CDlgAddEditMailServer;return;}

	delete m_CDlgAddEditMailServer;

	/*--Encrypt Password--*/
	CByteArray cbArray;
	CString csEncrypt, csPassword;

	csPassword.Format(_T("%s"), m_MailInfo.cPassword);
	m_CCrypto.Encrypt(csPassword, cbArray);

	for(int iTemp = 0; iTemp < cbArray.GetSize(); iTemp++)
	{
		CString csTemp;
		csTemp.Format(_T("%02X"), cbArray[iTemp]);
		csEncrypt = csEncrypt + csTemp;				//csEncrypt is the cypher text.
	}

	_tcscpy(m_MailInfo.cPassword, csEncrypt);

	//int nItem = pServerList->InsertItem(0, (LPCTSTR)m_MailInfo.cSMTPServer);
	pServerList->SetItemText(nItem, 0, (LPCTSTR)m_MailInfo.cSMTPServer);
	pServerList->SetItemText(nItem, 1, (LPCTSTR)m_MailInfo.cUserName);
	pServerList->SetItemText(nItem, 2, (LPCTSTR)m_MailInfo.cPassword);
	//pServerList->SetItemText(nItem, 2, _T("********"));
	//pServerList->SetCheck(nItem, 1);
}

void CDlgSettingFtpEmail::OnBnClickedButAdd()
{
	// TODO: Add your control notification handler code here
	//AfxMessageBox(_T("Address Add"));

	CDlgAddEditMailServer *m_CDlgAddEditMailServer = new CDlgAddEditMailServer(AddMailAddress);

	if(m_CDlgAddEditMailServer->DoModal() == IDOK)
	{
		memcpy(&m_MailInfo, &m_CDlgAddEditMailServer->m_MailInfo, sizeof(m_MailInfo));
		DirtyFlag = true;
	}
	else
	{delete m_CDlgAddEditMailServer;return;}

	delete m_CDlgAddEditMailServer;

	int nItem = pEmailList->InsertItem(0, (LPCTSTR)m_MailInfo.cSMTPServer);
	//pServerList->SetItemText(nItem, 1, (LPCTSTR)m_MailInfo.cUserName);
	//pServerList->SetItemText(nItem, 2, (LPCTSTR)m_MailInfo.cPassword);
	pEmailList->SetCheck(nItem, 1);
}

void CDlgSettingFtpEmail::OnBnClickedButMod()
{
	// TODO: Add your control notification handler code here
	//AfxMessageBox(_T("Address Modify"));
	int nItem = pEmailList->GetSelectionMark();
	if(nItem == -1)
	{
		AfxMessageBox(IDS_SELECT_EMAIL);
		//AfxMessageBox(_T("Please Select a Mail Address"));
		return;
	}

	memcpy(m_MailInfo.cSMTPServer, pEmailList->GetItemText(nItem, 0), sizeof(m_MailInfo.cSMTPServer)/sizeof(TCHAR));
	//memcpy(m_MailInfo.cUserName, pServerList->GetItemText(nItem, 1), sizeof(m_MailInfo.cUserName)/sizeof(TCHAR));
	//memcpy(m_MailInfo.cPassword, pServerList->GetItemText(nItem, 2), sizeof(m_MailInfo.cPassword)/sizeof(TCHAR));

	CDlgAddEditMailServer *m_CDlgAddEditMailServer = new CDlgAddEditMailServer(EditMailAddress);
	m_CDlgAddEditMailServer->vSetServerInfo(m_MailInfo);
	
	if(m_CDlgAddEditMailServer->DoModal() == IDOK)
	{
		memcpy(&m_MailInfo, &m_CDlgAddEditMailServer->m_MailInfo, sizeof(m_MailInfo));
		DirtyFlag = true;
	}
	else
	{delete m_CDlgAddEditMailServer;return;}

	delete m_CDlgAddEditMailServer;

	//int nItem = pServerList->InsertItem(0, (LPCTSTR)m_MailInfo.cSMTPServer);
	pEmailList->SetItemText(nItem, 0, (LPCTSTR)m_MailInfo.cSMTPServer);
	//pServerList->SetItemText(nItem, 1, (LPCTSTR)m_MailInfo.cUserName);
	//pServerList->SetItemText(nItem, 2, (LPCTSTR)m_MailInfo.cPassword);
	//pServerList->SetCheck(nItem, 1);
}

void CDlgSettingFtpEmail::OnBnClickedButDel()
{
	// TODO: Add your control notification handler code here
	//AfxMessageBox(_T("Address Delete"));
	int nItem = pEmailList->GetSelectionMark();
	pEmailList->DeleteItem(nItem);
	DirtyFlag = true;
}

void CDlgSettingFtpEmail::vSaveEmail()
{
	try
	{
		CString SQL_DeleteEmail(_T("DELETE FROM ec_email"));
		(((CMainFrame*)AfxGetMainWnd())->m_DB).ExecuteSQL(SQL_DeleteEmail);

		for(int iTemp = 0; iTemp < pServerList->GetItemCount(); iTemp++)
		{
			//m_MailInfo = mGetMailServerVactor(pServerList->GetItemText(iTemp, 0));

			CString SQL_MailServer, csServerListCheck;

			csServerListCheck.Format(_T("%d"), pServerList->GetCheck(iTemp));

			if(pServerList->GetCheck(iTemp) == 0)
				csServerListCheck = _T("N");
			else
				csServerListCheck = _T("Y");

			SQL_MailServer.Format(_T("INSERT INTO ec_email(server_, mail_server, server_account, server_password, active_) VALUES('Y', '%s', '%s', '%s', '%s')"), pServerList->GetItemText(iTemp, 0), pServerList->GetItemText(iTemp, 1), pServerList->GetItemText(iTemp, 2), csServerListCheck);
			(((CMainFrame*)AfxGetMainWnd())->m_DB).ExecuteSQL(SQL_MailServer);

			/*--Encrypt Password and Save to Registry--*/
			//SavePasswordToRegistry(m_MailInfo.cPassword, m_MailInfo.cSMTPServer);
		}

		for(int iTemp = 0; iTemp < pEmailList->GetItemCount(); iTemp++)
		{
			CString SQL_MailAddress;
			SQL_MailAddress.Format(_T("INSERT INTO ec_email(server_, target_email_addr) VALUES('N', '%s')"), pEmailList->GetItemText(iTemp, 0));
			(((CMainFrame*)AfxGetMainWnd())->m_DB).ExecuteSQL(SQL_MailAddress);
		}	
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
	
	DirtyFlag = false;
}

MailInfo CDlgSettingFtpEmail::mGetMailServerVactor(CString csSMTPServer)
{
	int iTemp;
	for(iTemp = 0; iTemp <= veMailServer.size(); iTemp++)
	{
		CString csVectorSMTPServer;
		csVectorSMTPServer.Format(_T("%s"), veMailServer[iTemp].cSMTPServer);

		if(csVectorSMTPServer == csSMTPServer)
		{
			return veMailServer[iTemp];
		}
	}

	return veMailServer[iTemp];
}

//void CDlgSettingFtpEmail::vDBConnect()
//{
//	CIniReader m_ini(ECINI);
//	CString cpDns = m_ini.getKeyValue(_T("DSN"),_T("database"));
//	CString cpUid = m_ini.getKeyValue(_T("UID"),_T("database"));
//	CString cpPwd = m_ini.getKeyValue(_T("PWD"),_T("database"));
// 
//	CString ls_connect;
//	ls_connect.Format(_T("ODBC;UID=%s;PWD=%s"), cpUid, cpPwd);
//	m_DB.Open((LPCTSTR)cpDns, FALSE, FALSE, ls_connect );
//}

CString CDlgSettingFtpEmail::DecryptEMailServerPassword(CString csEncrypt)
{
	CString csDecrypt;
	CByteArray cbArray2;

	LPCSTR lpSTR;
	USES_CONVERSION;
	lpSTR = W2A(csEncrypt.LockBuffer());
	csEncrypt.UnlockBuffer();

	unsigned char hexdata[32];

	memset(hexdata, 0, sizeof(unsigned char)*32);

	strtohex(lpSTR, hexdata);

	cbArray2.SetSize(csEncrypt.GetLength()/2);

	for(int iTemp = 0; iTemp < (csEncrypt.GetLength()/2); iTemp++)
	{
		cbArray2[iTemp] = (BYTE)hexdata[iTemp];
	}

	m_CCrypto.Decrypt(cbArray2, csDecrypt);

	return csDecrypt;
}

void CDlgSettingFtpEmail::strtohex(const char *str, unsigned char *hex)
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

BOOL CDlgSettingFtpEmail::PreTranslateMessage(MSG *pMsg)
{
	if(m_ToolTip != NULL)
       m_ToolTip->RelayEvent(pMsg);

	if   (pMsg-> message   ==   WM_KEYDOWN) 
    { 
        if   (pMsg-> wParam   ==   VK_RETURN   ||   pMsg-> wParam   ==   VK_ESCAPE) 
            return   TRUE; 
    } 

    return   CDialog::PreTranslateMessage(pMsg); 
}