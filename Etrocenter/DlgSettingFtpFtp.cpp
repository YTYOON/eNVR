// DlgSettingFtpFtp.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSettingFtpFtp.h"
#include "resource.h"
#include "MainFrm.h"

// CDlgSettingFtpFtp dialog

IMPLEMENT_DYNAMIC(CDlgSettingFtpFtp, CBaseDialog)

CDlgSettingFtpFtp::CDlgSettingFtpFtp(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgSettingFtpFtp::IDD, pParent), DirtyFlag(false)
{
	
}

CDlgSettingFtpFtp::~CDlgSettingFtpFtp()
{
	//if (&m_DB) m_DB.Close();
	if(m_ToolTip) delete m_ToolTip;
}

void CDlgSettingFtpFtp::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDC_BUTADD, &m_butAdd);
	//	DDX_Control(pDX, IDC_BUTDELETE, &m_butDel);
//	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_Pick);
}


BEGIN_MESSAGE_MAP(CDlgSettingFtpFtp, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTADD, &CDlgSettingFtpFtp::OnBnClickedButadd)
	ON_BN_CLICKED(IDC_BUTDELETE, &CDlgSettingFtpFtp::OnBnClickedButdelete)
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CDlgSettingFtpFtp::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTTONEDIT, &CDlgSettingFtpFtp::OnBnClickedButtonedit)
END_MESSAGE_MAP()


// CDlgSettingFtpFtp message handlers

BOOL CDlgSettingFtpFtp::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CBaseDialog::OnEraseBkgnd(pDC);
}

HBRUSH CDlgSettingFtpFtp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

BOOL CDlgSettingFtpFtp::OnInitDialog()
{
	
	CBaseDialog::OnInitDialog();
	m_CCrypto.DeriveKey(_T("Encrypt!@#$%^&*()_+-~`"));
	//vSetKey(_T("<(กรก`กร)>"));
	m_listCtrl = (CListCtrl*)GetDlgItem(IDC_LIST1);
	//vDBConnect();
	pDB = &(((CMainFrame*)AfxGetMainWnd())->m_DB);
	vInitFtp();
	vRetrieveFtp();
	

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//bool CDlgSettingFtpFtp::vDBConnect(void)
//{
//	CIniReader m_ini(ECINI);
//	CString cpDns = m_ini.getKeyValue(_T("DSN"),_T("database"));
//	CString cpUid = m_ini.getKeyValue(_T("UID"),_T("database"));
//	CString cpPwd = m_ini.getKeyValue(_T("PWD"),_T("database"));
// 
//	CString ls_connect;
//	ls_connect.Format(_T("ODBC;UID=%s;PWD=%s"), cpUid, cpPwd);
//	return m_DB.Open((LPCTSTR)cpDns, FALSE, FALSE, ls_connect );
//	//return true;
//}


void CDlgSettingFtpFtp::vRetrieveFtp(void)
{
	CString	csHost, csUser, csPassword, csActive;

	CMainFrame* lframe = (CMainFrame*)GetParentFrame();	
	CRecordset m_rs(&lframe->m_DB);
	//CRecordset m_rs(&m_DB);

	m_listCtrl->DeleteAllItems();
	m_rs.Open(CRecordset::dynaset,_T("select * from ec_ftp order by ftpid "));
	int ll_tot = m_rs.GetRecordCount();	
		if (ll_tot<=0) return;
		
	m_rs.MoveFirst();
	
	int ci=0;

	LPWSTR mynew;
	CDBVariant	idVal;
 

	while (!m_rs.IsEOF()) { 					
				strFTP* m_Ftp = new strFTP;
				m_rs.GetFieldValue(_T("ftpid"), idVal, SQL_C_SSHORT);
			
				m_rs.GetFieldValue(_T("ftp_host"), csHost);
				m_rs.GetFieldValue(_T("username"), csUser);
				m_rs.GetFieldValue(_T("ftp_pw"), csPassword);				
				m_rs.GetFieldValue(_T("active_"), csActive);
		
				//_tcscpy(m_FTPInfo.tcPassword , csPassword);
				//_tcscpy(m_FTPInfo.tcHost , csHost);
				//_tcscpy(m_FTPInfo.tcUser , csUser);

				m_listCtrl->InsertItem(ci, _T(""));
				if ( csActive.Trim()==_T("N"))
					m_listCtrl->SetCheck(ci,0); 
				else
					m_listCtrl->SetCheck(ci,1); 

				m_listCtrl->SetItemText(ci,1, csHost);
				m_listCtrl->SetItemText(ci,2, csUser);
				m_listCtrl->SetItemText(ci,3, csPassword);

				//veFTPInfo.push_back(m_FTPInfo);

				//Build Structure
				//m_Ftp->ftpid = idVal.m_iVal;
				//m_Ftp->csHost = csHost.Trim();
				//m_Ftp->csUser = csUser.Trim();
				//m_Ftp->csActive = csActive.Trim();
				//m_Ftp->csPassword = csPassword.Trim();

				/*--Get Passwrod From Registry and Decrypt--
				TCHAR tcFTPPassword[32];
				ReadPasswordFromRegistry(tcFTPPassword, m_Ftp->csUser);
				tcFTPPassword[(iPwdLength / 2)] = L'\0';
				m_Ftp->csPassword.Format(_T("%s"), tcFTPPassword);

				LVITEMW* pLitem = new LVITEM;
				pLitem->mask = LVIF_PARAM|LVIF_TEXT;
				pLitem->iItem = ci;
				pLitem->pszText = (LPWSTR)(LPCTSTR)csHost;
				pLitem->lParam = (LPARAM)m_Ftp;

				m_listCtrl->InsertItem(pLitem->mask ,ci, _T(""),0,0,0, pLitem->lParam);
				if ( csActive.Trim()==_T("N"))
					m_listCtrl->SetCheck(ci,0); 
				else
					m_listCtrl->SetCheck(ci,1); 
				m_listCtrl->SetItemText(ci,1, csHost);
				m_listCtrl->SetItemText(ci,2, csUser);
				m_listCtrl->SetItemText(ci,3, m_Ftp->csPassword);

				memcpy(m_FTPInfo.tcHost, csHost, sizeof(m_FTPInfo.tcHost)/sizeof(TCHAR));
				memcpy(m_FTPInfo.tcUser, csUser, sizeof(m_FTPInfo.tcUser)/sizeof(TCHAR));
				memcpy(m_FTPInfo.tcPassword, m_Ftp->csPassword, sizeof(m_FTPInfo.tcPassword)/sizeof(TCHAR));
				
				veFTPInfo.push_back(m_FTPInfo);*/

		ci++;
		m_rs.MoveNext();  		
	} 
	m_rs.Close();
}

bool CDlgSettingFtpFtp::vInitFtp(void)
{
	/*------------- Ftp Initial-----------*/
	pFtpList = (CListCtrl *)this->GetDlgItem(IDC_LIST1);

	DWORD dwStyle = pFtpList->GetStyle();
	dwStyle |= LVS_EX_FULLROWSELECT  | LVS_EX_CHECKBOXES;
	
	pFtpList->SetExtendedStyle(dwStyle);
	//m_butAdd = new CBitmapButton ;
	dwStyle =  WS_CHILD|WS_VISIBLE|BS_OWNERDRAW ;
	m_butAdd.Create(NULL, dwStyle, CRect(10, 5, 34, 29) ,this ,IDC_BUTADD);
	m_butAdd.LoadBitmaps(IDB_CAMGROUP_ADD,IDB_CAMGROUP_ADD,IDB_CAMGROUP_ADD,IDB_CAMGROUP_ADD);
	m_butAdd.ShowWindow(SW_SHOW);

	//m_butDel = new CBitmapButton ;
	dwStyle =  WS_CHILD|WS_VISIBLE|BS_OWNERDRAW ;
	m_butDel.Create(NULL, dwStyle, CRect(40, 5, 64, 29) ,this ,IDC_BUTDELETE);
	m_butDel.LoadBitmaps(IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL,IDB_CAMGROUP_DEL_DIS);
	m_butDel.ShowWindow(SW_SHOW);

	//m_butEdit = new CBitmapButton ;
	dwStyle =  WS_CHILD|WS_VISIBLE|BS_OWNERDRAW ;
	m_butEdit.Create(NULL, dwStyle, CRect(70, 5, 94, 29) ,this ,IDC_BUTTONEDIT);
	m_butEdit.LoadBitmaps(IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT,IDB_CAMGROUP_EDIT_DIS);
	m_butEdit.ShowWindow(SW_SHOW);

	LV_COLUMN ListColumn;
	WCHAR * ListTitles[4] = {_T("Active"), _T("Host"),_T("User Name"), _T("Password")};
	int ll_colw=180;

	for(int i = 0; i < 4; i++)
	{
		ListColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		ListColumn.fmt = LVCFMT_LEFT;
		ListColumn.iSubItem = i;

		if(i == 0 )
			{ListColumn.cx = 45;}
		else if (i==1)
			{ListColumn.cx = ll_colw;}
		else if (i==2)
			{ListColumn.cx = ll_colw;}
		else if (i==3)
			{ListColumn.cx = ll_colw;}
		
		ListColumn.pszText = ListTitles[i];
		pFtpList->InsertColumn(i, &ListColumn);
	}

	/*--Microhelp on buttons--*/
	m_ToolTip= new CToolTipCtrl();
	m_ToolTip->Create(this);
	CString ls_st;
	LOADSTRING(ls_st, IDS_TOOL_DEL_FTP);
	//ls_st.LoadString(CGS::g_hResStr, IDS_TOOL_DEL_FTP);
	m_ToolTip->AddTool(&m_butDel, ls_st);

	LOADSTRING(ls_st, IDS_TOOL_ADD_FTP);
	//ls_st.LoadString(CGS::g_hResStr, IDS_TOOL_ADD_FTP);
	m_ToolTip->AddTool(&m_butAdd,ls_st);
	
	LOADSTRING(ls_st, IDS_TOOL_MOD_FTP);
	//ls_st.LoadString(CGS::g_hResStr, IDS_TOOL_MOD_FTP);
	m_ToolTip->AddTool(&m_butEdit, ls_st);

	m_ToolTip->Activate(TRUE);

	return false;
}

void CDlgSettingFtpFtp::OnBnClickedButadd()
{
	// TODO: Add your control notification handler code here

	CDlgAddEditFTPServer *m_CDlgAddEditFTPServer = new CDlgAddEditFTPServer();

	if(m_CDlgAddEditFTPServer->DoModal() == IDOK)
	{
		memcpy(&m_FTPInfo, &m_CDlgAddEditFTPServer->m_FTPInfo, sizeof(m_FTPInfo));
		DirtyFlag = true;
	}
	else
	{
		delete m_CDlgAddEditFTPServer;
		return;
	}

	/*--Encrypt Password--*/
	CByteArray cbArray;
	CString csEncrypt, csPassword;

	csPassword.Format(_T("%s"), m_FTPInfo.tcPassword);
	m_CCrypto.Encrypt(csPassword, cbArray);

	for(int iTemp = 0; iTemp < cbArray.GetSize(); iTemp++)
	{
		CString csTemp;
		csTemp.Format(_T("%02X"), cbArray[iTemp]);
		csEncrypt = csEncrypt + csTemp;				//csEncrypt is the cypher text.
	}

	_tcscpy(m_FTPInfo.tcPassword , csEncrypt);

	int nItem = pFtpList->InsertItem(0, _T(""));
	pFtpList->SetItemText(nItem, 1, (LPCTSTR)m_FTPInfo.tcHost);
	pFtpList->SetItemText(nItem, 2, (LPCTSTR)m_FTPInfo.tcUser);
	pFtpList->SetItemText(nItem, 3, (LPCTSTR)m_FTPInfo.tcPassword);
	pFtpList->SetCheck( nItem, 1 );

	//veFTPInfo.push_back(m_FTPInfo);

	delete m_CDlgAddEditFTPServer;
}

void CDlgSettingFtpFtp::OnBnClickedButdelete()
{

	// TODO: Add your control notification handler code here
	if(pFtpList->GetSelectionMark() != -1)
	{
		pFtpList->DeleteItem(pFtpList->GetSelectionMark());
	}

	DirtyFlag = true;
}

void CDlgSettingFtpFtp::vSaveFtp(void)
{
	try
	{
		CString SQL_FTPDel(_T("DELETE FROM ec_ftp"));
		pDB->ExecuteSQL(SQL_FTPDel);

		for(int iTemp = 0; iTemp < pFtpList->GetItemCount(); iTemp++)
		{
			//m_FTPInfo = mGetFTPServerVector(pFtpList->GetItemText(iTemp, 1));

			CString csFTPListCheck;
			if(pFtpList->GetCheck(iTemp) == 1)
				csFTPListCheck = _T("Y");
			else
				csFTPListCheck = _T("N");

			CString SQL_FTPServer;
			SQL_FTPServer.Format(_T("INSERT INTO ec_ftp(ftp_host, username, active_, ftp_pw) VALUES ('%s', '%s', '%s', '%s')"), pFtpList->GetItemText(iTemp, 1), pFtpList->GetItemText(iTemp, 2), csFTPListCheck, pFtpList->GetItemText(iTemp, 3));
			pDB->ExecuteSQL(SQL_FTPServer);

		}

	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	DirtyFlag = false;

}//vSaveFtp

void CDlgSettingFtpFtp::OnBnClickedButtonedit()
{
	// TODO: Add your control notification handler code here
	int nItem = pFtpList->GetSelectionMark();

	if(nItem == -1)
	{
		AfxMessageBox(IDS_FTP_SERVER);return;
	//	AfxMessageBox(_T("Please Select a FTP Server"));return;
	}

	//m_FTPInfo = mGetFTPServerVector(pFtpList->GetItemText(nItem, 1));

	CDlgAddEditFTPServer *m_CDlgAddEditFTPServer = new CDlgAddEditFTPServer();

	//m_FTPInfo.tcPassword = DecryptFTPPassword();
	_tcscpy(m_FTPInfo.tcHost, pFtpList->GetItemText(nItem, 1));
	_tcscpy(m_FTPInfo.tcUser, pFtpList->GetItemText(nItem, 2));
	CString csDecrypt = DecryptFTPPassword(pFtpList->GetItemText(nItem, 3));
	_tcscpy(m_FTPInfo.tcPassword, csDecrypt);

	m_CDlgAddEditFTPServer->vSetFTPInfo(m_FTPInfo);

	if(m_CDlgAddEditFTPServer->DoModal() == IDOK)
	{
		memcpy(&m_FTPInfo, &m_CDlgAddEditFTPServer->m_FTPInfo, sizeof(m_FTPInfo));
		DirtyFlag = true;
	}
	else
	{
		delete m_CDlgAddEditFTPServer;
		return;
	}

	/*--Encrypt Password--*/
	CByteArray cbArray;
	CString csEncrypt, csPassword;

	csPassword.Format(_T("%s"), m_FTPInfo.tcPassword);
	m_CCrypto.Encrypt(csPassword, cbArray);

	for(int iTemp = 0; iTemp < cbArray.GetSize(); iTemp++)
	{
		CString csTemp;
		csTemp.Format(_T("%02X"), cbArray[iTemp]);
		csEncrypt = csEncrypt + csTemp;				//csEncrypt is the cypher text.
	}

	_tcscpy(m_FTPInfo.tcPassword , csEncrypt);

	pFtpList->SetItemText(nItem, 1, m_FTPInfo.tcHost);
	pFtpList->SetItemText(nItem, 2, m_FTPInfo.tcUser);
	pFtpList->SetItemText(nItem, 3, m_FTPInfo.tcPassword);

	delete m_CDlgAddEditFTPServer;
	return;
}

FTPInfo CDlgSettingFtpFtp::mGetFTPServerVector(CString csFTPServer)
{
	int iTemp;
	for(iTemp = 0; iTemp <= veFTPInfo.size(); iTemp++)
	{
		CString csVectorFTPServer;
		csVectorFTPServer.Format(_T("%s"), veFTPInfo[iTemp].tcHost);

		if(csVectorFTPServer == csFTPServer)
		{
			return veFTPInfo[iTemp];
		}
	}

	return veFTPInfo[iTemp];
}

void CDlgSettingFtpFtp::strtohex(const char *str, unsigned char *hex)
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

CString CDlgSettingFtpFtp::DecryptFTPPassword(CString csEncrypt)
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

BOOL CDlgSettingFtpFtp::PreTranslateMessage(MSG *pMsg)
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