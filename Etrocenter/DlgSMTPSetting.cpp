// DlgSMTPSetting.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSMTPSetting.h"


// CDlgSMTPSetting dialog

IMPLEMENT_DYNAMIC(CDlgSMTPSetting, CDialog)

CDlgSMTPSetting::CDlgSMTPSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSMTPSetting::IDD, pParent)
{
	m_CCrypto.DeriveKey(_T("Encrypt!@#$%^&*()_+-~`"));
}

CDlgSMTPSetting::~CDlgSMTPSetting()
{
}

void CDlgSMTPSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSMTPSetting, CDialog)
//	ON_WM_CREATE()
END_MESSAGE_MAP()


// CDlgSMTPSetting message handlers

BOOL CDlgSMTPSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CRegKey regKey;
	TCHAR szSMTPValue[2048];
	::ZeroMemory(&szSMTPValue[0], sizeof(szSMTPValue));
	const DWORD len = 2048;
	DWORD nActualLen(len);
    CString csForder(CProductDefine::m_tzRegistSubFolder);

	LONG nRet = regKey.Open(HKEY_LOCAL_MACHINE, csForder.GetBuffer());
	if(nRet != ERROR_SUCCESS)
	{
		if( regKey.Create(HKEY_LOCAL_MACHINE, csForder.GetBuffer()) != 0)
		{
			TRACE(_T("Create Registry Fail."));
		}
	}
	
	nRet = regKey.QueryValue(szSMTPValue, _T("SMTPSETTING"), &nActualLen);
	if(nRet != ERROR_SUCCESS)
	{
		CString csBlankSMTP;
		regKey.SetValue(_T("0;;;;;"), _T("SMTPSETTING"));
		{
			if( regKey.QueryValue(szSMTPValue, _T("SMTPSETTING"), &nActualLen) != ERROR_SUCCESS)
			{
				TRACE(_T("Registry Query Fail!\r\n"));
			}
		}
	}
	regKey.Close();

	CString csSMTPSecure, csSMTPServer, csSMTPPort, csSMTPAccount, csSMTPPassword;

	AfxExtractSubString(csSMTPSecure, szSMTPValue, 0, ';');
	AfxExtractSubString(csSMTPServer, szSMTPValue, 1, ';');
	AfxExtractSubString(csSMTPPort, szSMTPValue, 2, ';');
	AfxExtractSubString(csSMTPAccount, szSMTPValue, 3, ';');
	AfxExtractSubString(csSMTPPassword, szSMTPValue, 4, ';');

	CString csDecrypt = DecryptSMTPPassword(csSMTPPassword);

	this->SetDlgItemText(IDC_EDIT_SMTP_IP, csSMTPServer);
	this->SetDlgItemText(IDC_EDIT_SMTP_PORT, csSMTPPort);
	this->SetDlgItemText(IDC_EDIT_SMTP_USER, csSMTPAccount);
	this->SetDlgItemText(IDC_EDIT_SMTP_PWD, csDecrypt);
	//this->SetDlgItemText(IDC_COMBO_SMTP_SECURITY, csSMTPSecure);

	m_cbSMTPSecure = (CComboBox*)GetDlgItem(IDC_COMBO_SMTP_SECURITY);
	m_cbSMTPSecure->AddString( _T("None") );
	m_cbSMTPSecure->AddString( _T("TLS") );
	m_cbSMTPSecure->AddString( _T("SSL") );
	m_cbSMTPSecure->SetCurSel(::_ttoi(csSMTPSecure.GetBuffer()));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSMTPSetting::vSetSMTP2Reg()
{
	CString csSMTPSecure, csSMTPServer, csSMTPPort, csSMTPAccount, csSMTPPassword, csRegValue;
	this->GetDlgItemTextW(IDC_EDIT_SMTP_IP, csSMTPServer);
	this->GetDlgItemTextW(IDC_EDIT_SMTP_PORT, csSMTPPort);
	this->GetDlgItemTextW(IDC_EDIT_SMTP_USER, csSMTPAccount);
	this->GetDlgItemTextW(IDC_EDIT_SMTP_PWD, csSMTPPassword);

	/*--Encrypt Password--*/
	CByteArray cbArray;
	CString csEncrypt;

	//csSMTPPassword.Format(_T("%s"), m_FTPInfo.tcPassword);
	m_CCrypto.Encrypt(csSMTPPassword, cbArray);

	for(int iTemp = 0; iTemp < cbArray.GetSize(); iTemp++)
	{
		CString csTemp;
		csTemp.Format(_T("%02X"), cbArray[iTemp]);
		csEncrypt = csEncrypt + csTemp;				//csEncrypt is the cypher text.
	}


	m_cbSMTPSecure = (CComboBox*)GetDlgItem(IDC_COMBO_SMTP_SECURITY);
	//csSMTPSecure.Format(_T("%d"), m_cbSMTPSecure->GetCurSel());

	csRegValue.Format(_T("%d;%s;%s;%s;%s;"), m_cbSMTPSecure->GetCurSel(), csSMTPServer.GetBuffer(), csSMTPPort.GetBuffer(), csSMTPAccount.GetBuffer(), csEncrypt.GetBuffer());

	CRegKey regKey;
	TCHAR szSMTPValue[2048];
	::ZeroMemory(&szSMTPValue[0], sizeof(szSMTPValue));
	const DWORD len = 2048;
	DWORD nActualLen(len);
    CString csForder(CProductDefine::m_tzRegistSubFolder);

	LONG nRet = regKey.Open(HKEY_LOCAL_MACHINE, csForder.GetBuffer());
	if(nRet != ERROR_SUCCESS)
	{
		if( regKey.Create(HKEY_LOCAL_MACHINE, csForder.GetBuffer()) != 0)
		{
			TRACE(_T("Create Registry Fail."));
		}
	}

	regKey.SetValue(csRegValue.GetBuffer(), _T("SMTPSETTING"));
	regKey.Close();
}
//BOOL CDlgSMTPSetting::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	return CDialog::Create(lpszTemplateName, pParentWnd);
//}

//int CDlgSMTPSetting::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CDialog::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//	// TODO:  Add your specialized creation code here
//
//	m_CCrypto.DeriveKey(_T("Encrypt!@#$%^&*()_+-~`"));
//
//	return 0;
//}

CString CDlgSMTPSetting::DecryptSMTPPassword(CString csEncrypt)
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

void CDlgSMTPSetting::strtohex(const char *str, unsigned char *hex)
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