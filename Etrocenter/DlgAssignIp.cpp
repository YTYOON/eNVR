// DlgAddIpCam.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"
#include "DlgAssignIp.h"
#include "BaseDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAddIpCam dialog


CDlgAddIpCam::CDlgAddIpCam(CWnd* pParent) : CBaseDialog(CDlgAddIpCam::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAddIpCam)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgAddIpCam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAddIpCam)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAddIpCam, CDialog)
	//{{AFX_MSG_MAP(CDlgAddIpCam)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAddIpCam message handlers

BOOL CDlgAddIpCam::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CBaseDialog::OnEraseBkgnd(pDC);
}
