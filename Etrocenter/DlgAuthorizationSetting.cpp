// DlgAuthorizationSetting.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"
#include "DlgAuthorizationSetting.h"
#include "BaseDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// CDlgAuthorizationSetting dialog

const TCHAR* szProtocolName[] ={_T("TCP"),_T("CGI"),_T("RTSP(UDP)")};
CDlgAuthorizationSetting::CDlgAuthorizationSetting(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgAuthorizationSetting::IDD, pParent) //(CDlgAuthorizationSetting::IDD, pParent),m_pNode(NULL),m_pcsGroup(NULL)
{
	//{{AFX_DATA_INIT(CDlgAuthorizationSetting)
	m_csAlias = _T("");
	m_csPassword = _T("");
	m_csUsername = _T("");
	m_csSelectedGroup = _T("");
	m_csProtocol = _T("");
 
	//}}AFX_DATA_INIT
}


void CDlgAuthorizationSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAuthorizationSetting)
	DDX_Text(pDX, IDC_EDIT_ALIASNAME, m_csAlias);
	DDV_MaxChars(pDX, m_csAlias, 31);
	DDX_Text(pDX, IDC_EDIT_LOGINPASSWORD, m_csPassword);
	DDV_MaxChars(pDX, m_csPassword, 15);
	DDX_Text(pDX, IDC_EDIT_LOGINUSERNAME, m_csUsername);
	DDV_MaxChars(pDX, m_csUsername, 15);
	DDX_CBString(pDX, IDC_COMBO_GROUP, m_csSelectedGroup);
	DDX_CBString(pDX, IDC_COMBO_PROTOCOL, m_csProtocol);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAuthorizationSetting, CDialog)
	//{{AFX_MSG_MAP(CDlgAuthorizationSetting)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CDlgAuthorizationSetting::OnBnClickedOk)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAuthorizationSetting message handlers



void CDlgAuthorizationSetting::OnOK() 
{	
	UpdateData();

	if (((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
	{
		if (m_csUsername.GetLength() == 0 || m_csPassword.GetLength() == 0 || m_csSelectedGroup.GetLength()==0)
		{
			CString csString;
			LOADSTRING(csString, IDS_AUTH_WARNING_1);
			//csString.LoadString(CGS::g_hResStr, IDS_AUTH_WARNING_1);
			AfxMessageBox(csString);
			return;
		}

		if(m_csPassword.GetLength() > 8 || m_csPassword.GetLength() < 4)
		{
			CString csString;
			LOADSTRING(csString, IDS_PWD_LENGTH);
			//csString.LoadString(CGS::g_hResStr, IDS_PWD_LENGTH);
			AfxMessageBox(csString);
			return;
		}
	}

	CDialog::OnOK();
}

bool CDlgAuthorizationSetting::bSetAuthDataInfo(NODEITEM &nodeitem)
{
	USES_CONVERSION;
//	if (  m_csUsername.GetLength() == 0 || m_csPassword.GetLength() == 0 ) return false;
//	if (m_csAlias.GetLength() >0)
	m_csAlias.TrimRight();
	_tcsncpy(nodeitem.wcAlias, m_csAlias.LockBuffer(), sizeof(nodeitem.wcAlias)/sizeof(TCHAR)-1);

	if (m_csUsername.GetLength() >0)
		strncpy(nodeitem.szUser, W2A(m_csUsername.LockBuffer()), sizeof(nodeitem.szUser)-1);
	if (m_csPassword.GetLength() >0)
		strncpy(nodeitem.szPass, W2A(m_csPassword.LockBuffer()), sizeof(nodeitem.szPass)-1);
	if (m_csProtocol == szProtocolName[0])
	{
		nodeitem.nProtocol = VSMP;
	}else 	if (m_csProtocol == szProtocolName[1])
	{
		nodeitem.nProtocol = CGI;
	}else 
	if (m_csProtocol == szProtocolName[2])
	{
		nodeitem.nProtocol = RTSP;
	}
	return true;

}

void CDlgAuthorizationSetting::SetNodeItemInfo(NODEITEM &nodeitem, CString* pcs)
{
	USES_CONVERSION;
	if (&nodeitem != NULL)
	{
		m_csAlias = nodeitem.wcAlias;
		m_csUsername = A2W(nodeitem.szUser);
		m_csPassword = A2W(nodeitem.szPass);
		m_pNode = &nodeitem;
	}

		m_pcsGroup = pcs;
}

BOOL CDlgAuthorizationSetting::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	//m_SilverBrush.CreateSolidBrush(GLOBALBKCOLOR);
 	

	if (NULL == m_pNode) 
	{
		GetDlgItem(IDC_EDIT_ALIASNAME)->EnableWindow(false);
		CString csString;
		LOADSTRING(csString, IDS_AUTH_SETTING_TITLE);
		//csString.LoadString(CGS::g_hResStr, IDS_AUTH_SETTING_TITLE);
		AfxMessageBox(csString);
		GetDlgItem(IDC_STATIC_IP)->SetWindowText(csString);
	}
	else
	{
		USES_CONVERSION;
		CString cs;
		cs.Format(_T("IP: %s"), A2W(m_pNode->ip));
		GetDlgItem(IDC_STATIC_IP)->SetWindowText( cs );
	}

	//Insert Group Names into the group combobox
	int ci=0;
	CComboBox* pComboBox =(CComboBox*) GetDlgItem(IDC_COMBO_GROUP);

	while (m_pcsGroup && m_pcsGroup[ci] && m_pcsGroup[ci].GetLength() > 0)
	{
		pComboBox->InsertString(ci, m_pcsGroup[ci]);
		ci++;
	}
	if (m_pNode == NULL || m_pNode->nGroup == 0)
		pComboBox->SetCurSel(0);
	else
		pComboBox->SetCurSel(m_pNode->nGroup);

	CComboBox* pComboBoxProtocol =(CComboBox*) GetDlgItem(IDC_COMBO_PROTOCOL);
	if (m_pNode)
	{
		WORD nCapabilityCount = 0;
		for (int ci=0; ci < 3; ci++)
		{
			WORD nFlag = 1 << ci;
			if (m_pNode->nProtocolCapability & nFlag )
			{
				pComboBoxProtocol->InsertString(nCapabilityCount, szProtocolName[ci]);
				nCapabilityCount++;
			}
			if (m_pNode->nProtocol == nFlag)
			{
				pComboBoxProtocol->SetCurSel(nCapabilityCount-1);
			}
		}
	}

	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(false);
	GetDlgItem(IDC_EDIT_LOGINUSERNAME)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_LOGINPASSWORD)->EnableWindow(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAuthorizationSetting::OnCheck1() 
{
	if (((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
	{
		GetDlgItem(IDC_EDIT_LOGINUSERNAME)->EnableWindow();
		GetDlgItem(IDC_EDIT_LOGINPASSWORD)->EnableWindow();
	}else
	{
		GetDlgItem(IDC_EDIT_LOGINUSERNAME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_LOGINPASSWORD)->EnableWindow(false);
	}
}

void CDlgAuthorizationSetting::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL CDlgAuthorizationSetting::OnEraseBkgnd(CDC* pDC)
{
	return CBaseDialog::OnEraseBkgnd(pDC);

	// TODO: Add your message handler code here and/or call default

	//CRect rect;
	//GetClientRect(rect);
	//pDC->FillRect(&rect, &m_SilverBrush);
	// true; 
}

HBRUSH CDlgAuthorizationSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}


INT_PTR CDlgAuthorizationSetting::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class

	return CBaseDialog::DoModal(IDD);
}
