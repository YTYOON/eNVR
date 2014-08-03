#pragma once
#include "StyleButton\StyleButton.h"

// CDlgExitGate dialog

class CDlgExitGate : public CDialog
{
	DECLARE_DYNAMIC(CDlgExitGate)

public:
	CDlgExitGate(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgExitGate();

// Dialog Data
	enum { IDD = IDD_DLG_GATE_EXIT };

protected:
private:
    StyleButton m_BtnExit, m_BtnLogout, m_BtnCancel;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    void LayoutButton(StyleButton& btn);

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedLogout();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

class CDlgTransparent : public CDialog
{

    	DECLARE_DYNAMIC(CDlgTransparent)
public:
    CDlgTransparent(CWnd* pWnd):CDialog(IDD_SPLASH, pWnd){};
//BEGIN_MESSAGE_MAP(CDlgTransparent, CDialog)
//	//ON_WM_CTLCOLOR()
//	ON_WM_ERASEBKGND()
//END_MESSAGE_MAP()
//

    virtual BOOL OnInitDialog()
    {
        CDialog::OnInitDialog();
        int cx = ::GetSystemMetrics(SM_CXSCREEN);
        int cy = ::GetSystemMetrics(SM_CYSCREEN);
        SetWindowPos(this, 0, 0, cx, cy, SWP_SHOWWINDOW);
        CGS::SetTransparent(m_hWnd, 127);
        return true;
    }
    afx_msg BOOL OnEraseBkgnd(CDC* pDC)
    {
	    CRect rect;
	    GetClientRect(rect);
        CBrush brush(RGB(50, 50, 50));
	    pDC->FillRect(&rect, &brush);
	    return true;
    };
    DECLARE_MESSAGE_MAP()

};
