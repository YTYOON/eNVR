// TabOneDialog.cpp : implementation file
//

#include "stdafx.h"
//#include "TabDialog.h"
#include "TabOneDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabOneDialog dialog


CTabOneDialog::CTabOneDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTabOneDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabOneDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTabOneDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabOneDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabOneDialog, CDialog)
	//{{AFX_MSG_MAP(CTabOneDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CTabOneDialog::OnCancel() {}

void CTabOneDialog::OnOK() {}
