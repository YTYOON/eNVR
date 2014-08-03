#if !defined(AFX_DLGGROUPNAME_H__14E5DB32_C7A3_4C80_AC47_AB4BA0AD4442__INCLUDED_)
#define AFX_DLGGROUPNAME_H__14E5DB32_C7A3_4C80_AC47_AB4BA0AD4442__INCLUDED_

#include "BaseDialog.h"
#if _MSC_VER > 1000
#pragma once
#endif 

// _MSC_VER > 1000
// DlgGroupName.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGroupName dialog

class CDlgGroupName : public CBaseDialog
{
// Construction
public:
	CDlgGroupName(CWnd* pParent = NULL);   // standard constructor
	CString csGetGroupName();

// Dialog Data
	//{{AFX_DATA(CDlgGroupName)
	enum { IDD = IDD_DLG_GROUPNAME };
	CString	m_csGroupName;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGroupName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGroupName)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	CString m_GrpName;
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual INT_PTR DoModal();
	virtual INT_PTR DoModal(HTREEITEM hitem);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGROUPNAME_H__14E5DB32_C7A3_4C80_AC47_AB4BA0AD4442__INCLUDED_)
