#if !defined(AFX_DLGADDIPCAM_H__2D1326A2_AB13_4CE8_B78C_506FE2E87C5B__INCLUDED_)
#define AFX_DLGADDIPCAM_H__2D1326A2_AB13_4CE8_B78C_506FE2E87C5B__INCLUDED_

#include "BaseDialog.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAddIpCam.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAddIpCam dialog

class CDlgAddIpCam : public CBaseDialog
{
// Construction
public:
	CDlgAddIpCam(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAddIpCam)
	enum { IDD = IDD_DLG_NETWORK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAddIpCam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAddIpCam)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADDIPCAM_H__2D1326A2_AB13_4CE8_B78C_506FE2E87C5B__INCLUDED_)
