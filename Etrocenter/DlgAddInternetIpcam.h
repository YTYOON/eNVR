#if !defined(AFX_DLGADDINTERNETIPCAM_H__A39F1644_5406_43E5_96A8_E6348FBFAE65__INCLUDED_)
#define AFX_DLGADDINTERNETIPCAM_H__A39F1644_5406_43E5_96A8_E6348FBFAE65__INCLUDED_

#include "BaseDialog.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAddInternetIpcam.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAddInternetIpcam dialog

class CDlgAddInternetIpcam : public CBaseDialog
{
// Construction
public:
	CDlgAddInternetIpcam(NODEITEM* pNode, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAddInternetIpcam)
	enum { IDD = IDD_DLG_ADD_IPCAM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAddInternetIpcam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	NODEITEM* m_pNode;

	// Generated message map functions
	//{{AFX_MSG(CDlgAddInternetIpcam)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckShowPassword();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual INT_PTR DoModal();
//	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADDINTERNETIPCAM_H__A39F1644_5406_43E5_96A8_E6348FBFAE65__INCLUDED_)
