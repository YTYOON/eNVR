// DlgSettingGate.cpp : implementation file

#include "stdafx.h"
#include "DlgSettingGate.h"
#include "MainFrm.h"

// CDlgSettingGate dialog

IMPLEMENT_DYNAMIC(CDlgSettingGate, CBaseDialog)

CDlgSettingGate::CDlgSettingGate(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgSettingGate::IDD, pParent)
{

}

CDlgSettingGate::~CDlgSettingGate()
{
}

void CDlgSettingGate::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABMAIN, m_tab);
}


BEGIN_MESSAGE_MAP(CDlgSettingGate, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTSAVE, &CDlgSettingGate::OnBnClickedButsave)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABMAIN, &CDlgSettingGate::OnTcnSelchangeTabmain)
END_MESSAGE_MAP()


// CDlgSettingGate message handlers

HBRUSH CDlgSettingGate::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
 
	return hbr;
}

BOOL CDlgSettingGate::OnEraseBkgnd(CDC* pDC)
{
	return CBaseDialog::OnEraseBkgnd(pDC);
}

BOOL CDlgSettingGate::OnInitDialog()
{
		CBaseDialog::OnInitDialog();

		//Database initialized
		pDB = &(((CMainFrame*)AfxGetMainWnd())->m_DB);
		m_pagecam.psDB = pDB;
		m_pagepark.psDB = pDB;
		m_pageplate.psDB = pDB;

		m_tab.ShowWindow(SW_SHOW);

			
		//LOADSTRING(csTxt,  IDS_ACSETCAM);
		//m_tab.InsertItem(0,csTxt.GetBuffer());
		CString	csTxt;
		//Tabpage 0 
		LOADSTRING(csTxt, IDS_ACSETCAM);
		m_tab.InsertItem(0, csTxt.GetBuffer());
		//m_tab.InsertItem(0, _T("Camera Controller"));

		LOADSTRING(csTxt, IDS_GTSETCHAIN);
		m_tab.InsertItem(1, csTxt.GetBuffer());
		//m_tab.InsertItem(1, _T("Chains / Parking Gates"));
		
		LOADSTRING(csTxt, IDS_GTSETCARD);
		m_tab.InsertItem(2, csTxt.GetBuffer());
		//m_tab.InsertItem(2, _T("Cards / License Plate"));

		m_pagecam.Create(IDD_TABPAGEACCAM, &m_tab);
		m_pagecam.mc_parentMode='G';
		m_pagepark.mc_Pmode='G';
		m_pagepark.Create(IDD_TABPAGEGATEPARK, &m_tab);
		m_pageplate.Create(IDD_TABPAGEGATEPLATE, &m_tab);   
 
		pDialog[0] = &m_pagecam;
		pDialog[1] = &m_pagepark;
		pDialog[2] = &m_pageplate;
 	 
		pDialog[0]->ShowWindow(SW_SHOW);
		pDialog[1]->ShowWindow(SW_HIDE);
		pDialog[2]->ShowWindow(SW_HIDE);

		 CRect	rc;
		 int ll_gap = 1;
		m_tab.GetClientRect(rc);
		
		rc.top += 20;
		rc.bottom -= ll_gap;
		rc.left += ll_gap;
		rc.right -= ll_gap;

		m_pagecam.MoveWindow(&rc);
		m_pagepark.MoveWindow(&rc);
		m_pageplate.MoveWindow(&rc);
		m_tab.SetCurSel(0);

		m_pagecam.fnInitCamGate();
		m_pagecam.fnPopulateCamGate();

		return TRUE;  // return TRUE unless you set the focus to a control
 
}

void CDlgSettingGate::OnOK()
{	
	m_pagecam.fnSaveGate(true);
	m_pagepark.fnSave(true);
	m_pageplate.fnSave(true);
	CBaseDialog::OnOK();
}

void CDlgSettingGate::OnBnClickedButsave()
{
	m_pagecam.fnSaveGate(false);
	m_pagepark.fnSave(false);
	m_pageplate.fnSave(false);
}

// populate group into combobox
void CDlgSettingGate::fnPopulateGroup(void)
{
}

void CDlgSettingGate::fnFreeMemory(void)
{
	//for (int i=0; i<m_vpChain
}

void CDlgSettingGate::OnTcnSelchangeTabmain(NMHDR *pNMHDR, LRESULT *pResult)
{
		int ll_sel = m_tab.GetCurSel();
		if (ll_sel ==0) 	{
			pDialog[0]->ShowWindow(SW_SHOW);
			pDialog[1]->ShowWindow(SW_HIDE);	 
			pDialog[2]->ShowWindow(SW_HIDE);
		}
		else if (ll_sel ==1) 	{
			pDialog[0]->ShowWindow(SW_HIDE);
			pDialog[1]->ShowWindow(SW_SHOW);
			pDialog[2]->ShowWindow(SW_HIDE);	
		}
		else if (ll_sel ==2) 	{
			pDialog[0]->ShowWindow(SW_HIDE);
			pDialog[1]->ShowWindow(SW_HIDE);
			pDialog[2]->ShowWindow(SW_SHOW);	
		}

		*pResult = 0;
}
