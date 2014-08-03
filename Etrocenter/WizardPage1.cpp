// WizardPage1.cpp : implementation file
//
#include "stdafx.h"
#include "WizardPage1.h"
#include "MainFrm.h"
//#include "DlgAddManageCam.h"
#include "DlgMassAssignIP.h"
#include "IPCamTreeView.h"
#include "NVRBk.h"
#include "AddDevice.h"
// CWizardPage1 dialog
#define BTNWIDTH 48
#define BTNHEIGHT 78
#define BTNDISTANCE 50

IMPLEMENT_DYNAMIC(CWizardPage1, CPropertyPage)

CWizardPage1::CWizardPage1()
	: CPropertyPage(CWizardPage1::IDD),m_pBtnAddCam(NULL),m_pBtnMass(NULL)
{

}

CWizardPage1::~CWizardPage1()
{
	DELETEOBJ(m_pBtnAddCam);
	DELETEOBJ(m_pBtnMass);
}

void CWizardPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWizardPage1, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_WIZARD_P1, &CWizardPage1::OnBnClickedCheckWizardP1)
	ON_WM_PAINT()
	ON_COMMAND_RANGE(IDC_TREECTRL+2, IDC_TREECTRL+4, OnRangeCmds)
END_MESSAGE_MAP()


// CWizardPage1 message handlers

BOOL CWizardPage1::OnSetActive()
{
	CPropertySheet* parent = (CPropertySheet*)GetParent();

	parent->SetWizardButtons(PSWIZB_NEXT);
	parent->SetWindowTextW(_T("Welcome"));
	
	return CPropertyPage::OnSetActive();
}

void CWizardPage1::OnBnClickedCheckWizardP1()
{
	bool bRet = ((CButton*)GetDlgItem(IDC_CHECK_WIZARD_P1))->GetCheck();
	bRet ^= bRet;
	((CMainFrame*)AfxGetMainWnd())->m_bWizardEnable = bRet;
}

BOOL CWizardPage1::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
	m_pBtnAddCam = new CButtonST;
	m_pBtnMass = new CButtonST;
	
	CRect rect;
	//GetClientRect(rect);
	 COLORREF clBG = GLOBALBKCOLOR;
    int nHoveOver = (int)BTNST_AUTO_GRAY;
	DWORD dwStyle = BS_PUSHBUTTON |BS_NOTIFY |WS_VISIBLE;
	CString csCaption,csTmp;
	// set some static messages
	LOADSTRING(csTmp,IDS_WIZARD_P1_1);
	csCaption.Format(csTmp, CProductDefine::m_tzProductName);
	GetDlgItem(IDC_STATIC_WIZARD_P1_1)->SetWindowTextW(csCaption);
	LOADSTRING(csCaption,IDS_WIZARD_P1_2);
	GetDlgItem(IDC_STATIC_WIZARD_P1_2)->SetWindowTextW(csCaption);
	LOADSTRING(csCaption,IDS_WIZARD_P1_3);
	GetDlgItem(IDC_STATIC_WIZARD_P1_3)->SetWindowTextW(csCaption);	


	//addmanage Button
	GetDlgItem(IDC_STATIC_WIZARD_P1_2)->GetWindowRect(rect);
	ScreenToClient(rect);
	m_pBtnAddCam->Create(NULL, dwStyle, CRect(rect.left, rect.bottom, BTNWIDTH+rect.left, BTNWIDTH+rect.bottom), this, IDC_TREECTRL+2);
	m_pBtnAddCam->SetBitmaps(IDB_CAMERA, clBG ,nHoveOver);	
	/*LOADSTRING(csCaption,IDS_HINT_ADD_CAMERA);
	m_pBtnAddCam->SetWindowTextW(csCaption);
	m_pBtnAddCam->SetAlign(CButtonST::ST_ALIGN_VERT);*/
	GetDlgItem(IDC_STATIC_WIZARD_P1_3)->GetWindowRect(rect);
	ScreenToClient(rect);
	m_pBtnMass->Create(NULL, dwStyle, CRect(rect.left, rect.bottom, BTNWIDTH+rect.left, BTNWIDTH+rect.bottom), this, IDC_TREECTRL+3);
	m_pBtnMass->SetBitmaps(IDB_IP, clBG ,nHoveOver);
	/*LOADSTRING(csCaption,IDS_HINT_ASSIGN_IP);
	m_pBtnMass->SetWindowTextW(csCaption);
	m_pBtnMass->SetAlign(CButtonST::ST_ALIGN_VERT);*/
		
	SETITEMTEXT(IDC_CHECK_WIZARD_P1,IDS_WIZARD_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWizardPage1::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//m_pBtn1 = new CBitmap;
	//m_pBtn1->LoadBitmapW(IDB_CAMERA);
	//CDC dcMemory;
	//dcMemory.CreateCompatibleDC(&dc);

	//// Select the bitmap into the in-memory DC
	//CBitmap* pOldBitmap = dcMemory.SelectObject(m_pBtn1);

	//// Find a centerpoint for the bitmap in the client area
	// RECT rect;
	//GetClientRect(&rect);

	//// Copy the bits from the in-memory DC into the on-
	//// screen DC to actually do the painting. Use the centerpoint
	//// we computed for the target offset.
	//dc.BitBlt(10, 10, 150, 150, &dcMemory,0, 0, SRCCOPY);
	//dcMemory.SelectObject(pOldBitmap);
	
}
void CWizardPage1::OnRangeCmds(UINT nID)
{
    switch (nID)
    {
    case IDC_TREECTRL+2:
		{
// 		 CDlgAddManageCam dlg(0);
// 		int  DlgTemp = dlg.DoModal();
// 		 if(DlgTemp == IDCANCEL)
// 		 {
// 			dlg.ScanMap.clear();				
// 		 } 
// 	    break;
			CAddDevice dlg;
			if( dlg.DoModal() == IDOK)
			{
				CNVRBk::GetBkObject()->SendMessage(MsgUpdateView, UM_INSERT_TREENODE, NULL);
				return;
			}
			break;
		}
    case IDC_TREECTRL+3:
		{
			CDlgMassAssignIP dlg;
			dlg.DoModal();
			CNVRBk::GetBkObject()->SendMessage(MsgUpdateView, UM_REFRESH_DEVICETREE, 0);
        break;
		}
    case IDC_TREECTRL+4:
       // OpenDialogTag("App");
	    break;
    default:
        ASSERT(false);
    }
}
LRESULT CWizardPage1::OnWizardNext()
{
	int nCams = ((CTaskTreeCtrl*)((CMainFrame*)AfxGetMainWnd())->GetTreeView())->m_vctpAllNodesInTree.size();
	
	if(0 == nCams)
	{
		// there is no cam added, pop a error msg , stay here
		CString csMsg;
		LOADSTRING(csMsg,IDS_WIZARD_NOCAM);
		AfxMessageBox(csMsg);
		return -1;
	}

	return CPropertyPage::OnWizardNext();
	
}
