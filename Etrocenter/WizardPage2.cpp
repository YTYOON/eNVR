// WizardPage2.cpp : implementation file
//

#include "stdafx.h"
#include "WizardPage2.h"
#include "MainFrm.h"
#include "IPCamTreeView.h"
#include "DlgMatrixTab.h"
#include "NVR.h"
#define BTNWIDTH 48
#define BTNDISTANCE 50

// CWizardPage2 dialog

IMPLEMENT_DYNAMIC(CWizardPage2, CPropertyPage)

CWizardPage2::CWizardPage2()
	: CPropertyPage(CWizardPage2::IDD),m_pBtnMatrix(NULL)
{

}

CWizardPage2::~CWizardPage2()
{

	DELETEOBJ(m_pBtnMatrix);
}

void CWizardPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWizardPage2, CPropertyPage)
	ON_COMMAND_RANGE(IDC_TREECTRL+2, IDC_TREECTRL+4, OnRangeCmds)
END_MESSAGE_MAP()


// CWizardPage2 message handlers

BOOL CWizardPage2::OnSetActive()
{
	CPropertySheet* parent = (CPropertySheet*)GetParent();
	parent->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
	CString csMsg;
	LOADSTRING(csMsg,IDR_MAINFRAME);
	parent->SetWindowTextW(csMsg);
	
	return CPropertyPage::OnSetActive();
}

BOOL CWizardPage2::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_pBtnMatrix = new CButtonST;

	CRect rect;
//	GetClientRect(rect);
	 COLORREF clBG = GLOBALBKCOLOR;
    int nHoveOver = (int)BTNST_AUTO_GRAY;
	DWORD dwStyle = BS_PUSHBUTTON |BS_NOTIFY |WS_VISIBLE;

	CString csCaption;
	// set some static messages
	LOADSTRING(csCaption,IDS_WIZARD_P2_1);
	GetDlgItem(IDC_STATIC_WIZARD_P2_1)->SetWindowTextW(csCaption);

	//LIVEVIEW Button
	GetDlgItem(IDC_STATIC_WIZARD_P2_1)->GetWindowRect(rect);
	m_pBtnMatrix->Create(NULL, dwStyle, CRect(0, BTNDISTANCE, BTNWIDTH, BTNWIDTH+BTNDISTANCE), this, IDC_TREECTRL+2);
	m_pBtnMatrix->SetBitmaps(IDB_LV3X3, clBG ,nHoveOver);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CWizardPage2::OnRangeCmds(UINT nID)
{
    switch (nID)
    {
    case IDC_TREECTRL+2:
		{
//		((CIPCamTreeView*)((CMainFrame*)AfxGetMainWnd())->GetTreeView())->OnRangeCmds(nID);
        //GetParent()->ShowWindow(SW_HIDE);
        //ReleaseCapture();
        DlgMatrixTab matrixTab(this, IDD_DLG_MATRIX_TAB_DLG); // AfxGetMainWnd()
        matrixTab.vSetWindowStylePopup();
        
        matrixTab.DoModal();
        //GetCapture();
        //GetParent()->ShowWindow(SW_SHOW);
	    break;
		}
    case IDC_TREECTRL+3:
		{
			
        break;
		}
    case IDC_TREECTRL+4:
      
	    break;
    default:
        ASSERT(false);
    }
}