// BaseDialog.cpp : implementation file
//

#include "stdafx.h"
#include "BaseDialog.h"

#define WHITECOLOR RGB(255,255,255)

// CBaseDialog dialog

IMPLEMENT_DYNAMIC(CBaseDialog, CDialog)

CBaseDialog::CBaseDialog(UINT nIDTemplate, CWnd* pParent): CDialog(nIDTemplate,  pParent)
,m_clrBk(GLOBALBKCOLOR),m_clrFont(GLOBALFONTCOLOR)
{
}

CBaseDialog::~CBaseDialog()
{
}

void CBaseDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBaseDialog, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CBaseDialog message handlers

BOOL CBaseDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_SilverBrush.CreateSolidBrush(m_clrBk);
	m_WhiteBrush.CreateSolidBrush(WHITECOLOR);	 

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBaseDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &m_SilverBrush);
	
	return true;
}

HBRUSH CBaseDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetTextColor(m_clrFont);

	if( nCtlColor == CTLCOLOR_EDIT )//|| nCtlColor == CTLCOLOR_MSGBOX)
	{
		pDC->SetBkColor(WHITECOLOR);
		return m_WhiteBrush;
	}
	
	pDC->SetBkColor(m_clrBk);	
	return  m_SilverBrush;
}

 

void CBaseDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	EndDialog(IDOK);
	//CDialog::OnOK();
}

INT_PTR CBaseDialog::DoModal()
{
	return CDialog::DoModal();
}

INT_PTR CBaseDialog::DoModal(UINT nIDTemplate)
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogTemplate dlt;
   int             nResult;

   // load dialog template
   if (!dlt.Load(MAKEINTRESOURCE(nIDTemplate))) return -1;

   dlt.SetFont((LPCTSTR)STDFONT, STDFONTSIZE);
	
   // get pointer to the modified dialog template
   LPSTR pdata = (LPSTR)GlobalLock(dlt.m_hTemplate);
	
   // let MFC know that you are using your own template
   m_lpszTemplateName = NULL;
   InitModalIndirect(pdata);

   // display dialog box
   nResult = CDialog::DoModal();

   // unlock memory object
   GlobalUnlock(dlt.m_hTemplate);

   return nResult;	

}
