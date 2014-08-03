// DlgNASSetting.cpp : implementation file
//

#include "stdafx.h"
#include "DlgNASSetting.h"


// CDlgNASSetting dialog

IMPLEMENT_DYNAMIC(CDlgNASSetting, CDialog)

CDlgNASSetting::CDlgNASSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNASSetting::IDD, pParent)
{
	m_CCrypto.DeriveKey(_T("Encrypt!@#$%^&*()_+-~`"));
	bModFlag = false;
}

CDlgNASSetting::CDlgNASSetting(NASInfo NASInfo, CWnd* pParent)
	: CDialog(CDlgNASSetting::IDD, pParent)
{
	m_CCrypto.DeriveKey(_T("Encrypt!@#$%^&*()_+-~`"));
	::memcpy(&m_NASInfo, &NASInfo, sizeof(NASInfo));
	bModFlag = true;
}

CDlgNASSetting::~CDlgNASSetting()
{
}

void CDlgNASSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_NAS_SERVER_TYPE, m_comboServerType);
}


BEGIN_MESSAGE_MAP(CDlgNASSetting, CDialog)
	ON_BN_CLICKED(IDC_BTN_NAS_LAN, &CDlgNASSetting::OnBnClickedBtnNasLan)
	ON_BN_CLICKED(IDOK, &CDlgNASSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgNASSetting::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgNASSetting message handlers

void CDlgNASSetting::OnBnClickedBtnNasLan()
{
	// TODO: Add your control notification handler code here
	
	LPITEMIDLIST pFolderRoot = NULL;
	LPITEMIDLIST pFolderSelected = NULL;
	BROWSEINFO bPath = {0};
	CString csAllPath, csSelectDialogTitle;

	LOADSTRING(csSelectDialogTitle, IDS_SELECT_FOLDER);

	bPath.hwndOwner = this->m_hWnd;
	bPath.pidlRoot = pFolderRoot;
	//bPath.pszDisplayName = szDisplayName;
	//bPath.lpszTitle = _T("Select a folder");
	bPath.lpszTitle = csSelectDialogTitle;
	bPath.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bPath.lpfn = NULL;
	bPath.lParam = 0;

	pFolderSelected = SHBrowseForFolder(&bPath);

	//if root is set up.
	/*if(pFolderRoot)
    {
        CoTaskMemFree(pFolderRoot);
    }*/

	if(pFolderSelected)
	{
		SHGetPathFromIDList(pFolderSelected, csAllPath.GetBuffer(1024));
	}

	IMalloc *pmal =0;
	if(SHGetMalloc(&pmal) == S_OK)
	{
		pmal->Free(pFolderSelected);
		pmal->Release();
	}
	
	/*--Check the Path--*/
	CFileFind finder;
	if(csAllPath == _T(""))
		return;
	else if(csAllPath.GetLength() > 130)	
	{
		AfxMessageBox(IDS_PATH_TOOLONG);
		//AfxMessageBox(_T("The Path is too long.\r\nPlease Check the Path."));
		return;
	}

	CString csPCName, csPath, csIP;
	::AfxExtractSubString(csPCName, csAllPath.GetBuffer(), 2, '\\');

	if(csPCName.GetLength() == 0)
	{	
		AfxMessageBox(_T("This folder you selected is in local PC."));
		return;
	}

	CStringArray csaIPAddr;
	GetIpAddress(csPCName, csaIPAddr);
	//csIP.Format(_T("%s"), csaIPAddr.m_pData);
	csIP = csaIPAddr[0];

	bool bEnd;
	int iSub = 0;
	do{
		CString csSub;
		bEnd = ::AfxExtractSubString(csSub, csAllPath.GetBuffer(), iSub, '\\');
		if(iSub == 0 && bEnd)	
		{
			iSub++;
			continue;
		}
		else if(iSub == 2 && bEnd)
		{
			csPath += _T("\\")+csIP;
		}
		else if(bEnd)
		{
			csPath += _T("\\")+csSub;
		}
		iSub++;

	}while(bEnd);

	this->SetDlgItemTextW(IDC_EDIT_NAS_SERVER_ADDRESS, csPath.GetBuffer());



}

int CDlgNASSetting::GetIpAddress(const CString &sHostName, CStringArray &sIpAddress)
{
	USES_CONVERSION;

	struct hostent FAR * lpHostEnt=gethostbyname(W2A(sHostName));
	sIpAddress.RemoveAll();
	if(lpHostEnt==NULL)
	{
		return GetLastError();
	}
	
	int i=0;
	LPSTR lpAddr=lpHostEnt->h_addr_list[i];
	CString temp;
	while(lpAddr)
	{
		i++;
		struct in_addr inAddr;
		memmove(&inAddr,lpAddr,4);
		
		temp=inet_ntoa(inAddr);
		if(temp.IsEmpty())
		{
			break;
		}
		sIpAddress.Add(temp);
		lpAddr=lpHostEnt->h_addr_list[i];
	}
	return 0;
}

BOOL CDlgNASSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_comboServerType.AddString(_T("Samba"));
	m_comboServerType.AddString(_T("NFS"));
	m_comboServerType.SetCurSel(0);

	if(bModFlag)
	{
		if(_tcscmp(m_NASInfo.tcServerType, _T("Samba")) == 0)
		{
			m_comboServerType.SetCurSel(0);

		}else
		{
			m_comboServerType.SetCurSel(1);
		}
		
		this->SetDlgItemTextW(IDC_EDIT_NAS_SERVER_ADDRESS, m_NASInfo.tcServerAddr);

		this->SetDlgItemTextW(IDC_EDIT_NAS_USER_NAME, m_NASInfo.tcServerUser);

		CString csPass;
		csPass.Format(_T("%s"), m_NASInfo.tcServerPass);
		CString csDecrypt = DecryptPassword(csPass);

		this->SetDlgItemTextW(IDC_EDIT_NAS_PASSWORD, csDecrypt.GetBuffer());

		
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNASSetting::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CByteArray cbArray;
	CString csEncrypt, csPassword;
	this->GetDlgItemTextW(IDC_EDIT_NAS_PASSWORD, csPassword);

	m_CCrypto.Encrypt(csPassword, cbArray);
	for(int iTemp = 0; iTemp < cbArray.GetSize(); iTemp++)
	{
		CString csTemp;
		csTemp.Format(_T("%02X"), cbArray[iTemp]);
		csEncrypt = csEncrypt + csTemp;				//csEncrypt is the cypher text.
	}
	
	//--Get NAS Info
    _tcscpy(this->m_NASInfo.tcServerType, (m_comboServerType.GetCurSel() == 0) ? _T("Samba") : _T("NFS"));

	CString csUser, csAddress;
	this->GetDlgItemTextW(IDC_EDIT_NAS_SERVER_ADDRESS, csAddress);
	::_tcscpy(m_NASInfo.tcServerAddr, csAddress.GetBuffer());

	this->GetDlgItemTextW(IDC_EDIT_NAS_USER_NAME, csUser);
	::_tcscpy(m_NASInfo.tcServerUser, csUser.GetBuffer());

	//this->GetDlgItemTextW(IDC_EDIT_NAS_USER_NAME, csUIText);
	::_tcscpy(m_NASInfo.tcServerPass, csEncrypt.GetBuffer());

	NETRESOURCE nr;
	memset(&nr,0,sizeof(nr));
	nr.dwType = RESOURCETYPE_DISK;
    nr.lpRemoteName = (LPWSTR)(LPCTSTR)csAddress;
	int nRet(0);
	nRet =  WNetAddConnection2(&nr, csPassword , csUser ,0);
	if( nRet != NO_ERROR)
	{
		//ERRLOG(_T("WNetAddConnection2 error @ vConnectNASDirectory. Error code:%d "),nRet);
		AfxMessageBox(_T("NAS Connection Error!"));
		return;
	}
	AfxMessageBox(_T("NAS Connection Successful!"));
	

	OnOK();
}

void CDlgNASSetting::strtohex(const char *str, unsigned char *hex)
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

CString CDlgNASSetting::DecryptPassword(CString csEncrypt)
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
void CDlgNASSetting::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
