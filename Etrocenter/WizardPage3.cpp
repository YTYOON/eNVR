// WizardPage3.cpp : implementation file
//

#include "stdafx.h"
#include "WizardPage3.h"
#define BTNWIDTH 48
#define BTNDISTANCE 20
#include "DlgSettingRecording.h"
#include "DlgFtp.h"

// CWizardPage3 dialog

IMPLEMENT_DYNAMIC(CWizardPage3, CPropertyPage)

CWizardPage3::CWizardPage3()
	: CPropertyPage(CWizardPage3::IDD),m_pBtnRecording(NULL),m_pBtnSystem(NULL),m_pBtnApp(NULL)
{

}

CWizardPage3::~CWizardPage3()
{
	DELETEOBJ(m_pBtnRecording);
	DELETEOBJ(m_pBtnSystem);
	DELETEOBJ(m_pBtnApp);

}

void CWizardPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWizardPage3, CPropertyPage)
	ON_COMMAND_RANGE(IDC_TREECTRL+2, IDC_TREECTRL+4, OnRangeCmds)
END_MESSAGE_MAP()


// CWizardPage3 message handlers

BOOL CWizardPage3::OnSetActive()
{
	CPropertySheet* parent = (CPropertySheet*)GetParent();
	parent->SetWizardButtons(PSWIZB_BACK | PSWIZB_FINISH);
	CString csMsg;
	LOADSTRING(csMsg,IDS_MAIN_SETTINGS);
	parent->SetWindowTextW(csMsg);
	return CPropertyPage::OnSetActive();
}

BOOL CWizardPage3::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_pBtnRecording = new CButtonST;
	m_pBtnSystem = new CButtonST;
	m_pBtnApp = new CButtonST;

	CRect rect,rectClient;
	GetClientRect(rectClient);
	COLORREF clBG = GLOBALBKCOLOR;
    int nHoveOver = (int)BTNST_AUTO_GRAY;
	DWORD dwStyle = BS_PUSHBUTTON |BS_NOTIFY |WS_VISIBLE;
	CString csCaption,csTmp;
	// set some static messages
	LOADSTRING(csCaption,IDS_WIZARD_P3_1);
	GetDlgItem(IDC_STATIC_WIZARD_P3_1)->SetWindowTextW(csCaption);
	LOADSTRING(csCaption,IDS_WIZARD_P3_2);
	GetDlgItem(IDC_STATIC_WIZARD_P3_2)->SetWindowTextW(csCaption);
	LOADSTRING(csTmp,IDS_WIZARD_P3_3);
	csCaption.Format(csTmp, CProductDefine::m_tzProductName);
	GetDlgItem(IDC_STATIC_WIZARD_P3_3)->SetWindowTextW(csCaption);	
	//Recording Button
	GetDlgItem(IDC_STATIC_WIZARD_P3_2)->GetWindowRect(&rect);
	
	m_pBtnRecording->Create(NULL, dwStyle, CRect(rectClient.left+BTNDISTANCE, 120, BTNWIDTH+rectClient.left+BTNDISTANCE, BTNWIDTH+130), this, IDC_TREECTRL+2);
	m_pBtnRecording->SetBitmaps(IDB_RECORDING, clBG ,nHoveOver);
	
	GetDlgItem(IDC_STATIC_WIZARD_P3_3)->GetWindowRect(&rect);
	m_pBtnSystem->Create(NULL, dwStyle, CRect(rectClient.left+BTNDISTANCE, 220, BTNWIDTH+rectClient.left+BTNDISTANCE, BTNWIDTH+230), this, IDC_TREECTRL+3);
	m_pBtnSystem->SetBitmaps(IDB_SYSTEM, clBG ,nHoveOver);

	/*m_pBtnApp->Create(NULL, dwStyle, CRect(rect.left,rect.top+ BTNDISTANCE*2,BTNWIDTH,BTNWIDTH+ BTNDISTANCE*2), this, IDC_TREECTRL+4);
	m_pBtnApp->SetBitmaps(IDB_APPNEW, clBG ,nHoveOver);*/


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CWizardPage3::OnRangeCmds(UINT nID)
{
    switch (nID)
    {
    case IDC_TREECTRL+2:
		{
			CDlgSettingRecording RecordingDlg;
			RecordingDlg.DoModal();
	    break;
		}
    case IDC_TREECTRL+3:
		{
			CDlgFtp dlg;
			dlg.DoModal();
        break;
		}
    case IDC_TREECTRL+4:
      
	    break;
    default:
        ASSERT(false);
    }
}
