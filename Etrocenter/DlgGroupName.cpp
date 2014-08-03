// DlgGroupName.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"
#include "DlgGroupName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGroupName dialog


CDlgGroupName::CDlgGroupName(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgGroupName::IDD, pParent), m_GrpName(_T(""))
{
	//{{AFX_DATA_INIT(CDlgGroupName)
	m_csGroupName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgGroupName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGroupName)
	DDX_Text(pDX, IDC_EDIT_GROUPNAME, m_csGroupName);
	DDV_MaxChars(pDX, m_csGroupName, 31);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGroupName, CDialog)
	//{{AFX_MSG_MAP(CDlgGroupName)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CDlgGroupName::OnBnClickedOk)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGroupName message handlers
CString  CDlgGroupName::csGetGroupName()
{
	return m_csGroupName;
}


void CDlgGroupName::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_GROUPNAME, m_csGroupName);
	OnOK();
}

BOOL CDlgGroupName::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CBaseDialog::OnEraseBkgnd(pDC);
}

HBRUSH CDlgGroupName::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

INT_PTR CDlgGroupName::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class
	//To change font here...
	return CBaseDialog::DoModal(IDD);
}

INT_PTR CDlgGroupName::DoModal(HTREEITEM hitem)
{
	
	return CBaseDialog::DoModal(IDD);
}