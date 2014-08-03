// EncpSettingPage.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"

#include "NVSENCPLib.h"
#include "EncpSettingPage.h"
//#include "Authentication.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEncpSettingPage dialog


CEncpSettingPage::CEncpSettingPage(CWnd* pParent /*=NULL*/)
	: CDialog(CEncpSettingPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEncpSettingPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	memset(&m_nodeInfo, 0, sizeof(m_nodeInfo));
	memset(&m_netInfo, 0, sizeof(m_netInfo));
	m_dlgPage[0] = NULL;
	m_dlgPage[1] = NULL;
	m_dlgPage[2] = NULL;

	m_hENCPItem = NULL;
	m_bMsgReceived = FALSE;
}

/*
CEncpSettingPage::~CEncpSettingPage()
{
	if( m_dlgPage[0] )
	{
		m_dlgPage[0]->DestroyWindow();
		delete (CSettingPageNetwork*)m_dlgPage[0];
		m_dlgPage[0] = NULL;
	}

	if( m_dlgPage[1] )
	{
		m_dlgPage[1]->DestroyWindow();
		delete (CSettingPageHostname*)m_dlgPage[1];
		m_dlgPage[1] = NULL;
	}

	if( m_dlgPage[2] )
	{
		m_dlgPage[2]->DestroyWindow();
		delete (CSettingPageAccount*)m_dlgPage[2];
		m_dlgPage[2] = NULL;
	}
}*/

void CEncpSettingPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEncpSettingPage)
	DDX_Control(pDX, IDC_BUTTON_NETWORK, m_buttonNetwork);
	DDX_Control(pDX, IDC_BUTTON_HOSTNAME, m_buttonHostName);
	DDX_Control(pDX, IDC_BUTTON_ACCOUNT, m_buttonAccount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEncpSettingPage, CDialog)
	//{{AFX_MSG_MAP(CEncpSettingPage)
	ON_BN_CLICKED(IDC_BUTTON_NETWORK, OnButtonNetwork)
	ON_BN_CLICKED(IDC_BUTTON_HOSTNAME, OnButtonHostname)
	ON_BN_CLICKED(IDC_BUTTON_ACCOUNT, OnButtonAccount)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEncpSettingPage message handlers

BOOL CEncpSettingPage::OnInitDialog() 
{
	RECT rc, rcWindow;
	CDialog::OnInitDialog();

	// Get Item Information for encp item.
	encp_GetNetworkInfo(m_hENCPItem, &m_netInfo);
	encp_GetNodeInfo(m_hENCPItem, &m_nodeInfo);
	encp_GetGuestPermission( m_hENCPItem, &m_bGuestAllow);

	if( !m_dlgPage[0] )
	{
		m_dlgPage[0] = new CSettingPageNetwork;
		m_dlgPage[0]->Create(IDD_SETTING_PAGE_NETWORK, this);
	}
	if( !m_dlgPage[1] )
	{
		m_dlgPage[1] = new CSettingPageHostname;
		m_dlgPage[1]->Create(IDD_SETTING_PAGE_HOSTNAME, this);
	}
	if( !m_dlgPage[2] )
	{
		m_dlgPage[2] = new CSettingPageAccount;
		m_dlgPage[2]->Create(IDD_SETTING_PAGE_ACCOUNT, this);
	}


	GetDlgItem(IDC_OUTLINE)->GetClientRect(&rc);
	GetDlgItem(IDC_OUTLINE)->GetWindowRect(&rcWindow);
	ScreenToClient(&rcWindow);
	GetDlgItem(IDC_OUTLINE)->ShowWindow(SW_HIDE);

	m_dlgPage[0]->SetWindowPos(NULL, rcWindow.left, rcWindow.top, rc.right, rc.bottom, SWP_SHOWWINDOW);
	m_dlgPage[1]->SetWindowPos(NULL, rcWindow.left, rcWindow.top, rc.right, rc.bottom, SWP_HIDEWINDOW);
	m_dlgPage[2]->SetWindowPos(NULL, rcWindow.left, rcWindow.top, rc.right, rc.bottom, SWP_HIDEWINDOW);

	SelectPage( IDC_BUTTON_NETWORK );
#ifdef FACTORY
	GetDlgItem(IDC_BUTTON_HOSTNAME)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_ACCOUNT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_HOSTNAME)->ShowWindow(false);
	GetDlgItem(IDC_BUTTON_ACCOUNT)->ShowWindow(false);

#endif
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEncpSettingPage::OnButtonNetwork() 
{
	SelectPage( IDC_BUTTON_NETWORK );
}

void CEncpSettingPage::OnButtonHostname() 
{
	SelectPage( IDC_BUTTON_HOSTNAME );
}

void CEncpSettingPage::OnButtonAccount() 
{
	SelectPage( IDC_BUTTON_ACCOUNT );
}

void CEncpSettingPage::SelectPage(UINT button)
{
	UINT checkstate[3];
	switch( button )
	{
	case IDC_BUTTON_NETWORK:
		checkstate[0] = BST_CHECKED;
		checkstate[1] = BST_UNCHECKED;
		checkstate[2] = BST_UNCHECKED;
		break;
	case IDC_BUTTON_HOSTNAME:
		checkstate[0] = BST_UNCHECKED;
		checkstate[1] = BST_CHECKED;
		checkstate[2] = BST_UNCHECKED;
		break;
	case IDC_BUTTON_ACCOUNT:
		checkstate[0] = BST_UNCHECKED;
		checkstate[1] = BST_UNCHECKED;
		checkstate[2] = BST_CHECKED;
		break;
	}

	CheckDlgButton( IDC_BUTTON_NETWORK, checkstate[0] );
	CheckDlgButton( IDC_BUTTON_HOSTNAME, checkstate[1] );
	CheckDlgButton( IDC_BUTTON_ACCOUNT, checkstate[2] );

	m_dlgPage[0]->ShowWindow( checkstate[0]==BST_CHECKED?SW_SHOW:SW_HIDE );
	m_dlgPage[1]->ShowWindow( checkstate[1]==BST_CHECKED?SW_SHOW:SW_HIDE );
	m_dlgPage[2]->ShowWindow( checkstate[2]==BST_CHECKED?SW_SHOW:SW_HIDE );
}

void CEncpSettingPage::Close(UINT nID)
{
	m_hENCPItem = NULL;

	EndDialog(nID);
//	CDialog::OnClose();
}

void CEncpSettingPage::OnENCPItemReceived(HENCPITEM hItem, ENCPCBMSG msg, LPARAM lp)
{
	m_bMsgReceived = TRUE;
	if( m_hENCPItem != hItem )	goto ENDOF_OnENCPItemReceived;
	switch( msg )
	{
	case ENM_ENCPSEARCH:		goto ENDOF_OnENCPItemReceived;
	case ENM_TYPE_MISMATCH:
		AfxMessageBox(_T("Protocol Type mismatched!"));
		break;
	case ENM_AUTHFAIL:
		AfxMessageBox(_T("Authentication Failed!"));
		break;
	case ENM_SETFAIL:
		AfxMessageBox(_T("Update failed! Try later..."));
		break;
	case ENM_SETSUCCESS:
		break;
	case ENM_UNKNOWN_ERROR:
		AfxMessageBox(_T("Update failed! Unknown error..."));
		break;
	}

ENDOF_OnENCPItemReceived:
	Close( ( msg == ENM_SETSUCCESS ) ? IDOK : IDCANCEL );
	m_bMsgReceived = FALSE;
}

void CEncpSettingPage::SetENCPItem(	vector <HENCPITEM>* pvthENCPItem)
{
	m_pvthENCPItem = pvthENCPItem;
//	if (m_pvthENCPItem->size() == 1)
		m_hENCPItem = (*m_pvthENCPItem)[0];
//	else 
//		m_hENCPItem = NULL;
}
void CEncpSettingPage::SetENCPItem(HENCPITEM hEncpItem)
{
	m_hENCPItem = hEncpItem;
}

BOOL CEncpSettingPage::IsMessageReceived() const
{
	return m_bMsgReceived;
}

void CEncpSettingPage::OnApply(int page)
{
	USES_CONVERSION;
	int err;
	int ci;
	CString temp1; //= "";
	CString temp2;// = "";
	switch( page )
	{
	case IDD_SETTING_PAGE_NETWORK:
		{
			((CSettingPageNetwork*)m_dlgPage[0])->GetSettingValue(&m_netInfo);
			char szSerial[16]={0};
			strncpy(szSerial, m_netInfo.STATICIP.ip,15);
			char szTok[] =".";
			char* szIp[4];
			szIp[0] = strtok(szSerial, szTok);
			szIp[1] = strtok(NULL, szTok);
			szIp[2] = strtok(NULL, szTok);
			szIp[3] = strtok(NULL, szTok);
			for (ci =0; ci < m_pvthENCPItem->size(); ci++)
			{
				m_hENCPItem = (*m_pvthENCPItem)[ci];
				int nSerial = atoi(szIp[3])+ci;
				sprintf(m_netInfo.STATICIP.ip, "%s.%s.%s.%d", szIp[0], szIp[1], szIp[2], nSerial);
				if(int nRet = encp_SetNetworkInfo(m_hENCPItem, (LPCSTR)W2A(m_strAdminID), (LPCSTR)W2A(m_strAdminPass), &m_netInfo) != ENERR_OK )
				{
					return;
				}
			}
		}
		break;
	case IDD_SETTING_PAGE_HOSTNAME:
		((CSettingPageHostname*)m_dlgPage[1])->GetSettingValue(temp1, temp2);
		for (ci =0; ci < m_pvthENCPItem->size(); ci++)
		{
			m_hENCPItem = (*m_pvthENCPItem)[ci];
			err = encp_SetHostName(m_hENCPItem, W2A(m_strAdminID), W2A(m_strAdminPass), W2A(temp1), W2A(temp2));
			if( err != ENERR_OK )
			{
				// Error Handling here...
				CString msg;
				msg.Format(_T("Error %d"),err);
				ETROMSG(msg);
				return;
			}		
		}
		break;
	case IDD_SETTING_PAGE_ACCOUNT:
		((CSettingPageAccount*)m_dlgPage[2])->GetSettingValue(temp1, m_bGuestAllow );
		for (ci =0; ci < m_pvthENCPItem->size(); ci++)
		{
			m_hENCPItem = (*m_pvthENCPItem)[ci];
			if( ::encp_SetAccountInfo(m_hENCPItem, W2A(m_strAdminID), W2A(m_strAdminPass), W2A(temp1), &m_bGuestAllow) != ENERR_OK )
			{
				// Error Handling here...
				return;
			}
		}
		break;
	}
	EndDialog(IDOK);
}
void CEncpSettingPage::OnRestore(int page)
{
	// Add code here
}

int CEncpSettingPage::ShowAuthDialog()
{
	m_strAdminID =_T("root");
	m_strAdminPass= _T("pass");
	return IDOK;
/*
	int result;
	CAuthentication dlg;
	result = dlg.DoModal();
	if( result != IDOK ) return result;

	m_strAdminID = dlg.GetID();
	m_strAdminPass = dlg.GetPassword();
*/
}

void CEncpSettingPage::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if( m_dlgPage[0] )
	{
		m_dlgPage[0]->DestroyWindow();
		delete (CSettingPageNetwork*)m_dlgPage[0];
		m_dlgPage[0] = NULL;
	}

	if( m_dlgPage[1] )
	{
		m_dlgPage[1]->DestroyWindow();
		delete (CSettingPageHostname*)m_dlgPage[1];
		m_dlgPage[1] = NULL;
	}

	if( m_dlgPage[2] )
	{
		m_dlgPage[2]->DestroyWindow();
		delete (CSettingPageAccount*)m_dlgPage[2];
		m_dlgPage[2] = NULL;
	}
}
