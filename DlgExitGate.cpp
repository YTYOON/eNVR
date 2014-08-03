// DlgExitGate.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"
#include "DlgExitGate.h"
#include "StyleButton\Templates.h"

// CDlgExitGate dialog

IMPLEMENT_DYNAMIC(CDlgExitGate, CDialog)

CDlgExitGate::CDlgExitGate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExitGate::IDD, pParent)
{

}

CDlgExitGate::~CDlgExitGate()
{
}

void CDlgExitGate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK,		m_BtnExit);
	DDX_Control(pDX, IDC_LOGOUT,	m_BtnLogout);
	DDX_Control(pDX, IDCANCEL,	m_BtnCancel);
}


BEGIN_MESSAGE_MAP(CDlgExitGate, CDialog)
    ON_BN_CLICKED(IDC_LOGOUT, &CDlgExitGate::OnBnClickedLogout)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

#define GETCLIENTRECT(rc, control) CRect rc; control.GetClientRect(rc); rc.OffsetRect(-rc.left, -rc.top);
#define GETSCREENRECT(rc, control) CRect rc; control.GetWindowRect(rc); ScreenToClient(rc);
#define GETCONTROLRECT(rc, control) CRect rc; control.GetWindowRect(rc); rc.OffsetRect(-rc.left, -rc.top);

// CDlgExitGate message handlers
void CDlgExitGate::LayoutButton(StyleButton& btn)
{
    GETCONTROLRECT(rc, btn);

    {
	    VistaButton1 vbs(rc);
	    Stack Top;
	    Top.SetRect(rc);
        CString cs;
        btn.GetWindowText(cs);
	    Top.AddAlignString(cs, ALIGN_CENTER, CPoint(0,0), White, 24);
	    //Top1.AddImage(CPoint(8, 8), IDR_DB16, _T("PNG"));
    	
	    // add the standard style
	    Style std; std.AddStack(vbs.VBStd + Top);
	    btn.LoadStdStyle(std);

	    // add the hot style
	    Style hot; hot.AddStack(vbs.VBHot + Top);
	    btn.LoadHotStyle(hot);

	    // add the pressed style
	    Style press; press.AddStack(vbs.VBPress + Top);
	    btn.LoadPressedStyle(press);
    }
}

BOOL CDlgExitGate::OnInitDialog()
{
    CDialog::OnInitDialog();
    int cx = ::GetSystemMetrics(SM_CXSCREEN);
    int cy = ::GetSystemMetrics(SM_CYSCREEN);
    //CRect rect;
    //GetClientRect(&rect);
    //cy = rect.bottom;
    //MoveWindow(0, 0, cx, cy, true);
    SetWindowPos(this, 0, 0, cx, cy, SWP_SHOWWINDOW);
    LayoutButton(m_BtnExit);
    LayoutButton(m_BtnLogout);
    LayoutButton(m_BtnCancel);
	return TRUE;

}

void CDlgExitGate::OnBnClickedLogout()
{
    this->EndDialog(IDC_LOGOUT);
}

BOOL CDlgExitGate::OnEraseBkgnd(CDC* pDC)
{
    //return CDialog::OnEraseBkgnd(pDC);
    CRect rect;
    GetClientRect(rect);
    BOOL nRet = afxGlobalData.DrawParentBackground(this, pDC, rect);
    return nRet;

}
IMPLEMENT_DYNAMIC(CDlgTransparent, CDialog)
BEGIN_MESSAGE_MAP(CDlgTransparent, CDialog)
	//ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

