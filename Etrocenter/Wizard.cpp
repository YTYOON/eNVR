// Wizard.cpp : implementation file
//

#include "stdafx.h"
#include "Wizard.h"
#define PAGENUM 3

// CWizard

IMPLEMENT_DYNAMIC(CWizard, CPropertySheet)

CWizard::CWizard(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page2);
	AddPage(&m_page3);
	SetWizardMode();
}

CWizard::CWizard(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	
	AddPage(&m_page1);
	AddPage(&m_page2);
	AddPage(&m_page3);
	SetWizardMode();
}

CWizard::~CWizard()
{
}


BEGIN_MESSAGE_MAP(CWizard, CPropertySheet)

END_MESSAGE_MAP()


// CWizard message handlers

BOOL CWizard::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

//	m_pList = new CListCtrl;
//	m_pList->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT|LVS_EDITLABELS,CRect(0,0,100,300),this, 0);
	
	//CBitmap bitmap[3];
	//bitmap[0].LoadBitmapW(IDB_CAMERA);
	//bitmap[1].LoadBitmapW(IDB_LV3X3);
	//bitmap[2].LoadBitmapW(IDB_SETTING_NEW);
	//m_ImageList.Create(48,48,ILC_COLORDDB, 3,3);
	//
	//for(int i = 0 ; i < PAGENUM; i++)
	//{
	//	m_ImageList.Add(&bitmap[i],RGB(250,250,250));
	//}
	//m_pList->SetImageList(&m_ImageList,LVSIL_SMALL);
	//m_pList->InsertColumn(0,_T(""),LVCFMT_LEFT,100,3);

	//for(int i = 0 ; i < PAGENUM; i++)
	//{	
	//	m_pList->InsertItem(i,NULL,i);		
	//}
	
	return bResult;
}
void CWizard::vSetListIdx(int i)
{

	
	
	
}
