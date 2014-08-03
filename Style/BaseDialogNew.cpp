// BaseDialog.cpp : implementation file
//

#include "stdafx.h"
#include "BaseDialogNew.h"

#define WHITECOLOR RGB(255,255,255)


IMPLEMENT_DYNAMIC(CBaseDialog, CDialog)

CBaseDialog::CBaseDialog(UINT nIDTemplate, CWnd* pParent): CDialog(nIDTemplate,  pParent)
,m_clrBk(GLOBALBKCOLORLAYER3),m_clrFont(FONTCOLOR)
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
	m_WhiteBrush.CreateSolidBrush(GLOBALBKCOLORLAYER7);	 

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBaseDialog::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &m_SilverBrush);
	return true;
}

HBRUSH CBaseDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hBrush=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    CBrush* pBrush;
    pDC->SetTextColor(m_clrFont);
    switch (nCtlColor)
    {
    case CTLCOLOR_MSGBOX:
    case CTLCOLOR_EDIT:
    case CTLCOLOR_LISTBOX:
    case CTLCOLOR_BTN:
    case CTLCOLOR_DLG:
    case CTLCOLOR_SCROLLBAR:
    case CTLCOLOR_STATIC:
    case CTLCOLOR_MAX:
		pDC->SetBkColor(m_clrBk);//GLOBALBKCOLORLAYER7);
        pDC->SetBkMode(TRANSPARENT);
        ::DeleteObject(hBrush);
        return (HBRUSH)CreateSolidBrush(m_clrBk);
		pBrush = &m_SilverBrush;
        hBrush= HBRUSH(m_SilverBrush.m_hObject);
        break;
    default: 
		pDC->SetBkColor(m_clrBk);
        pBrush = &m_SilverBrush;
        
        ::DeleteObject(hBrush);
        return (HBRUSH)CreateSolidBrush(m_clrBk);

        hBrush=HBRUSH(m_SilverBrush.m_hObject);
        break;                    
    }
	return hBrush;
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

   dlt.SetFont((LPCTSTR) STDFONT, STDFONTSIZE);
	
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
