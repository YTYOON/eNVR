// DlgUserLogin.cpp : implementation file
//

#include "stdafx.h"
#include "DlgUserLogin.h"

#define		TIMER_SETFOCUS		01

// CDlgUserLogin dialog

IMPLEMENT_DYNAMIC(CDlgUserLogin, CDialog)

CDlgUserLogin::CDlgUserLogin(CDatabase* pDB, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUserLogin::IDD, pParent),m_pVk(NULL)
{
	m_pDB = pDB;
	m_pMainFrm =(CMainFrame*) pParent;
	bool bret = m_pDB->IsOpen();
	m_Crypto.DeriveKey(_T("Encrypt!@#$%^&*()_+-~`"));
}

CDlgUserLogin::~CDlgUserLogin()
{
    DELETEOBJ(m_pVk);
}

void CDlgUserLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgUserLogin, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgUserLogin::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgUserLogin::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_LOGIN_AUTO, &CDlgUserLogin::OnBnClickedCheckLoginAuto)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgUserLogin message handlers

BOOL CDlgUserLogin::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString csString(_T(""));
	LOADSTRING(csString, IDS_ADD_CAM_USER);
	SetDlgItemText(IDC_STATIC_LOGIN_USER, csString);
	LOADSTRING(csString, IDS_ADD_CAM_PASSWORD);
	SetDlgItemText(IDC_STATIC_LOGIN_PWD, csString);
	LOADSTRING(csString, IDC_CHECK_LOGIN_AUTO);
	SetDlgItemText(IDC_CHECK_LOGIN_AUTO, csString);
	LOADSTRING(csString, IDC_STATIC_CONFIRM);
	SetDlgItemText(IDC_STATIC_CONFIRM, csString);

	CString csAuto;
	this->SetDlgItemText(IDC_EDIT_LOGIN_USER, m_csUserName);

	CString csWindowText;

	switch(m_iOption)
	{
    case LOGOUT: //logout
    case RESTART:
        ((CButton*)GetDlgItem(IDC_CHECK_LOGIN_AUTO))->ShowWindow(SW_HIDE);
        //don't break here
	case UNKNOWN3:
	case LOGIN: // normal user login
		GetDlgItem(IDC_STATIC_CONFIRM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_CONFIRM)->ShowWindow(SW_HIDE);
		AfxExtractSubString(csAuto, m_pMainFrm->szCurrentUser, 0, ';');
		if (m_iOption == 0)
		{
			if( ::_ttoi(csAuto) )
				((CButton*)GetDlgItem(IDC_CHECK_LOGIN_AUTO))->SetCheck(1);
			else
				((CButton*)GetDlgItem(IDC_CHECK_LOGIN_AUTO))->SetCheck(0);
		}
		else
		{
			GetDlgItem(IDC_CHECK_LOGIN_AUTO)->ShowWindow(SW_HIDE);
		}
        if (m_iOption == LOGOUT)
        {
		    csWindowText.Format(_T("%s Logout"), CProductDefine::m_tzProductName);
        }else if (m_iOption == RESTART)
        {
		    csWindowText.Format(_T("%s Restart"), CProductDefine::m_tzProductName);
        }
		this->SetWindowTextW(csWindowText);

		break;
	case PASSWORD_SETTING://Register root password
		((CButton*)GetDlgItem(IDC_CHECK_LOGIN_AUTO))->ShowWindow(SW_HIDE);
		this->SetDlgItemTextW(IDC_EDIT_LOGIN_USER, _T("root"));
		GetDlgItem(IDC_EDIT_LOGIN_USER)->EnableWindow(0);

		csWindowText.Format(_T("%s Register Password for 'root'."),  CProductDefine::m_tzProductName);
		this->SetWindowTextW(csWindowText);

		SetTimer(TIMER_SETFOCUS, 250, 0);

		break;
	case UNKNOWN2:
		GetDlgItem(IDC_STATIC_CONFIRM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_CONFIRM)->ShowWindow(SW_HIDE);
		AfxExtractSubString(csAuto, ((CMainFrame*)m_pMainFrm)->szCurrentUser, 0, ';');
		if( ::_ttoi(csAuto) )
		{
			((CButton*)GetDlgItem(IDC_CHECK_LOGIN_AUTO))->SetCheck(1);
		}
		else
			((CButton*)GetDlgItem(IDC_CHECK_LOGIN_AUTO))->SetCheck(0);

		csWindowText =  CProductDefine::m_tzProductName;
		this->SetWindowTextW(csWindowText);

		break;
	default:
		ASSERT(false);
		break;
	};
    m_pVk = new VK;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgUserLogin::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	this->GetDlgItemTextW(IDC_EDIT_LOGIN_USER, m_csUserName);
	this->GetDlgItemTextW(IDC_EDIT_LOGIN_PWD, m_csPassword);

	CByteArray cbArray;
	CString csEncryptPwd;
	m_Crypto.Encrypt(m_csPassword, cbArray);

	for(int iTemp = 0; iTemp < cbArray.GetSize(); iTemp++)
	{
		CString csTemp;
		csTemp.Format(_T("%02X"), cbArray[iTemp]);
		csEncryptPwd = csEncryptPwd + csTemp;				//csEncrypt is the cypher text.
	}


	CRegKey regKey;
	TCHAR szCurrentUser[1024]={0};
	const DWORD len = 1024;
	DWORD nActualLen(len);

    CString csForder(CProductDefine::m_tzRegistSubFolder);

	LONG nRet = regKey.Open(HKEY_LOCAL_MACHINE, csForder.GetBuffer());
	if(nRet != ERROR_SUCCESS)
	{
		if( regKey.Create(HKEY_LOCAL_MACHINE, csForder.GetBuffer()) != 0)
		{
			TRACE(_T("Create Registry Fail."));
			return; 
		}
	}

	if(m_iOption == LOGIN || m_iOption == LOGOUT || m_iOption == RESTART)
	{
		iCheck(m_csUserName, m_csPassword);

		CString csAuto;
		if( ((CButton*)GetDlgItem(IDC_CHECK_LOGIN_AUTO))->GetCheck() )
		{
			m_csRegValue.Format(_T("%d;%s;%s;"), 1, m_csUserName, csEncryptPwd);
		}
		else
		{
			m_csRegValue.Format(_T("%d;%s;%s;"), 0, m_csUserName, csEncryptPwd);
		}
	}
	else if(m_iOption == UNKNOWN2)
	{
		if( ((CButton*)GetDlgItem(IDC_CHECK_LOGIN_AUTO))->GetCheck() )
		{
			m_csRegValue.Format(_T("%d;%s;%s;"), 1, m_csUserName, csEncryptPwd);
		}
		else
		{
			m_csRegValue.Format(_T("%d;%s;%s;"), 0, m_csUserName, csEncryptPwd);
		}
	}
	else
	{
		this->GetDlgItemTextW(IDC_EDIT_CONFIRM, m_csConfirmPwd);
		if( m_csPassword.Compare(m_csConfirmPwd.GetBuffer()) != 0 )
		{
			AfxMessageBox(_T("Please Retype Password!"));
			SetDlgItemText(IDC_EDIT_LOGIN_PWD, _T(""));
			SetDlgItemText(IDC_EDIT_CONFIRM, _T(""));
			return;
		}

		m_csRegValue.Format(_T("0;%s;%s;"), m_csUserName, csEncryptPwd);
		this->m_iChecksum = LV_SET_APP;
	}
    if (m_iChecksum != LOGIN_UNAUTHORIZED)
    {
	    regKey.SetValue(m_csRegValue.GetBuffer(), _T("USERPWD"));
    }
	regKey.Close();

	OnOK();
}

void CDlgUserLogin::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//OnCancel();
	this->m_iChecksum = CANCEL;
	OnOK();
}

int CDlgUserLogin::iCheck(CString csUser, CString csPwd)
{
	CString csSQL, csValue, csDBPwd, csPass;
	try
	{
		csSQL.Format(_T("SELECT * FROM ec_user WHERE user_name = '%s';"), csUser.GetBuffer());
		CRecordset rChecksum(m_pDB);
		rChecksum.Open(CRecordset::forwardOnly, csSQL.GetBuffer());
		while(!rChecksum.IsEOF())
		{
			rChecksum.GetFieldValue(_T("user_pword"), csDBPwd);
			csDBPwd.TrimRight();

			rChecksum.GetFieldValue(_T("user_group_code"), csValue);//--User Group Code = limits to authority
			csValue.TrimRight();

			rChecksum.MoveNext();
		}
		rChecksum.Close();
	
		
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
	
	if(csValue.GetLength() == 0)
	{
		this->m_iChecksum = LOGIN_UNAUTHORIZED;
		return LOGIN_UNAUTHORIZED;
	}
		
	csPass = csDecryptAuthPassword(csDBPwd);
	if( csPwd.Compare(csPass.GetBuffer()) == 0 )
	{
		this->m_iChecksum = ::_ttoi(csValue);
	}
	else
	{
		this->m_iChecksum = LOGIN_UNAUTHORIZED;
	}

	return ::_ttoi(csValue);
}

bool CDlgUserLogin::bAttach(CString csUser, CString csPwd)
{
	m_csUserName = csUser;
	m_csPassword = csPwd;

	return true;
}

void CDlgUserLogin::OnBnClickedCheckLoginAuto()
{
	// TODO: Add your control notification handler code here
}

void CDlgUserLogin::vSetOption(int iOption)
{
	m_iOption = iOption;
}

CString CDlgUserLogin::csDecryptAuthPassword(CString &csEncrypt)
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

void CDlgUserLogin::strtohex(const char *str, unsigned char *hex)
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
void CDlgUserLogin::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == TIMER_SETFOCUS)
	{
		KillTimer(nIDEvent);
		
		HWND hWnd=m_hWnd;
		HWND hFrgWnd = ::GetForegroundWindow();
		AttachThreadInput( GetWindowThreadProcessId(hFrgWnd, NULL), GetCurrentThreadId(), TRUE );
		::SetForegroundWindow(hWnd);
		::BringWindowToTop(hWnd);

		if(!::BringWindowToTop(hWnd))
		{
			printf("BringWindowToTop Error %d/n", GetLastError());
			//AfxMessageBox(L"1");
		}
		else
		{
			printf("BringWindowToTop OK/n");
			//AfxMessageBox(L"2");
		}
		if(!::SetForegroundWindow(hWnd))
		{
			printf("SetForegroundWindow Error %d/n", GetLastError());
			//AfxMessageBox(L"3");
		}
		else
		{
			printf("SetForegroundWindow OK/n");
			//AfxMessageBox(L"4");
		}

		SwitchToThisWindow(hWnd, TRUE);

		AttachThreadInput(GetWindowThreadProcessId(hFrgWnd, NULL), GetCurrentThreadId(), FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}
