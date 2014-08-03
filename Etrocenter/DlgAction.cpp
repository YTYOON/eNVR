// DlgAction.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAction.h"

// CDlgAction dialog

IMPLEMENT_DYNAMIC(CDlgAction, CDialog)

CDlgAction::CDlgAction(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAction::IDD, pParent)
{
	::ZeroMemory(&m_ActionInfo, sizeof(m_ActionInfo));
	m_ActionInfo.dwActionType = -1;
	m_ActionInfo.bDO = false;
	m_ActionInfo.bEmail = false;
	m_ActionInfo.bFTP = false;
	m_ActionInfo.bPTZ = false;
	m_ActionInfo.bRecording = false;
	m_ActionInfo.bSnapshot = false;
	iINIT = ACTION_INIT::ADD;
}	

CDlgAction::~CDlgAction()
{
}

void CDlgAction::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAction, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgAction::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_ACTION_EMAIL, &CDlgAction::OnBnClickedCheckActionEmail)
	ON_BN_CLICKED(IDC_CHECK_ACTION_DO, &CDlgAction::OnBnClickedCheckActionDo)
	ON_BN_CLICKED(IDC_CHECK_ACTION_PTZ, &CDlgAction::OnBnClickedCheckActionPtz)
END_MESSAGE_MAP()


// CDlgAction message handlers

CDlgAction::CDlgAction(ActionInfo pActionInfo, CWnd* pParent)
	: CDialog(CDlgAction::IDD, pParent)
{
	iINIT = ACTION_INIT::MODIFY;
	::memcpy(&m_ActionInfo, &pActionInfo, sizeof(ActionInfo));
}


void CDlgAction::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	
	//--Set Trigger Type From UI
	if( ((CButton*)GetDlgItem(IDC_RADIO_ACTION_MD))->GetCheck() )
	{
		m_ActionInfo.dwActionType = EA_TRIGGER_TYPE::MD;
	}
	else if( ((CButton*)GetDlgItem(IDC_RADIO_ACTION_VL))->GetCheck() )
	{
		m_ActionInfo.dwActionType = EA_TRIGGER_TYPE::VL;
	}
	else if( ((CButton*)GetDlgItem(IDC_RADIO_ACTION_DI))->GetCheck() )
	{
		m_ActionInfo.dwActionType = EA_TRIGGER_TYPE::DI;
	}
	else if( ((CButton*)GetDlgItem(IDC_RADIO_ACTION_REBOOT))->GetCheck() )
	{
		m_ActionInfo.dwActionType = EA_TRIGGER_TYPE::EA_REBOOT;
	}
	else
	{
		CString csErrorMSG = _T("Please select a Event Trigger Type.");
		AfxMessageBox(csErrorMSG);
		return;
	}

	//--Set Action Type From UI
	if(m_ActionInfo.dwActionType != -1)
	{
		if( ((CButton*)GetDlgItem(IDC_CHECK_ACTION_EMAIL))->GetCheck() )
			m_ActionInfo.bEmail = true;
		else
			m_ActionInfo.bEmail = false;

		if( ((CButton*)GetDlgItem(IDC_CHECK_ACTION_FTP))->GetCheck() )
			m_ActionInfo.bFTP = true;
		else
			m_ActionInfo.bFTP = false;

		if( ((CButton*)GetDlgItem(IDC_CHECK_ACTION_RECORDING))->GetCheck() )
			m_ActionInfo.bRecording = true;
		else
			m_ActionInfo.bRecording = false;

		if( ((CButton*)GetDlgItem(IDC_CHECK_ACTION_SNAPSHOT))->GetCheck() )
			m_ActionInfo.bSnapshot = true;
		else
			m_ActionInfo.bSnapshot = false;

		if( ((CButton*)GetDlgItem(IDC_CHECK_ACTION_DO))->GetCheck() )
			m_ActionInfo.bDO = true;
		else
			m_ActionInfo.bDO = false;

		if( ((CButton*)GetDlgItem(IDC_CHECK_ACTION_PTZ))->GetCheck() )
			m_ActionInfo.bPTZ = true;
		else
			m_ActionInfo.bPTZ = false;
	}

	

	OnOK();
}

BOOL CDlgAction::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	if(iINIT == ACTION_INIT::MODIFY)
	{
		switch(m_ActionInfo.dwActionType)
		{
		case EA_TRIGGER_TYPE::MD:
			((CButton*)GetDlgItem(IDC_RADIO_ACTION_MD))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_ACTION_VL))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_ACTION_DI))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_ACTION_REBOOT))->SetCheck(0);
			break;
		case EA_TRIGGER_TYPE::VL:
			((CButton*)GetDlgItem(IDC_RADIO_ACTION_MD))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_ACTION_VL))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_ACTION_DI))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_ACTION_REBOOT))->SetCheck(0);
			break;
		case EA_TRIGGER_TYPE::DI:
			((CButton*)GetDlgItem(IDC_RADIO_ACTION_MD))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_ACTION_VL))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_ACTION_DI))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_ACTION_REBOOT))->SetCheck(0);
			break;
		case EA_TRIGGER_TYPE::EA_REBOOT:
			((CButton*)GetDlgItem(IDC_RADIO_ACTION_MD))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_ACTION_VL))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_ACTION_DI))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_ACTION_REBOOT))->SetCheck(1);
			break;
		default:
			ASSERT(true);
			break;
		};

		if(m_ActionInfo.bDO)
			((CButton*)GetDlgItem(IDC_CHECK_ACTION_DO))->SetCheck(1);

		if(m_ActionInfo.bEmail)
			((CButton*)GetDlgItem(IDC_CHECK_ACTION_EMAIL))->SetCheck(1);

		if(m_ActionInfo.bFTP)
			((CButton*)GetDlgItem(IDC_CHECK_ACTION_FTP))->SetCheck(1);

		if(m_ActionInfo.bPTZ)
			((CButton*)GetDlgItem(IDC_CHECK_ACTION_PTZ))->SetCheck(1);

		if(m_ActionInfo.bRecording)
			((CButton*)GetDlgItem(IDC_CHECK_ACTION_RECORDING))->SetCheck(1);

		if(m_ActionInfo.bSnapshot)
			((CButton*)GetDlgItem(IDC_CHECK_ACTION_SNAPSHOT))->SetCheck(1);

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAction::OnBnClickedCheckActionEmail()
{
	// TODO: Add your control notification handler code here
	if(	((CButton*)GetDlgItem(IDC_CHECK_ACTION_EMAIL))->GetCheck() )
	{
		CDlgActionSetting m_DlgActionSetting = CDlgActionSetting(0, &m_ActionInfo);
		if( m_DlgActionSetting.DoModal() == IDOK)
		{
			
		}
	}

}

void CDlgAction::OnBnClickedCheckActionDo()
{
	// TODO: Add your control notification handler code here
	if(	((CButton*)GetDlgItem(IDC_CHECK_ACTION_DO))->GetCheck() )
	{
		CDlgActionSetting m_DlgActionSetting = CDlgActionSetting(2, &m_ActionInfo);
		if( m_DlgActionSetting.DoModal() == IDOK)
		{
			
		}
	}
}

void CDlgAction::OnBnClickedCheckActionPtz()
{
	// TODO: Add your control notification handler code here
	if(	((CButton*)GetDlgItem(IDC_CHECK_ACTION_PTZ))->GetCheck() )
	{
		CDlgActionSetting m_DlgActionSetting = CDlgActionSetting(1, &m_ActionInfo);
		if( m_DlgActionSetting.DoModal() == IDOK)
		{
			
		}
	}
}
