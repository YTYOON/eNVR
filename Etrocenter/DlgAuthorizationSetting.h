#if !defined(AFX_DLGAUTHORIZATIONSETTING_H__D5112708_6DEE_4DE6_BDC2_DC54495CAFCE__INCLUDED_)
#define AFX_DLGAUTHORIZATIONSETTING_H__D5112708_6DEE_4DE6_BDC2_DC54495CAFCE__INCLUDED_

#include "resource.h"
#include "BaseDialog.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAuthorizationSetting.h : header file
//
//#include "EtroExplorer.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgAuthorizationSetting dialog

class CDlgAuthorizationSetting : public CBaseDialog
{
// Construction
public:
	void SetNodeItemInfo(NODEITEM &nodeitem, CString* pcs);
	bool bSetAuthDataInfo(NODEITEM& nodeitem);
	CDlgAuthorizationSetting(CWnd* pParent = NULL);   // standard constructor
	CString* m_pcsGroup;
// Dialog Data
	//{{AFX_DATA(CDlgAuthorizationSetting)
	enum { IDD = IDD_DLGAUTHORIZATIONSETTING };
	CString	m_csAlias;
	CString	m_csPassword;
	CString	m_csUsername;
	CString	m_csSelectedGroup;
	CString	m_csProtocol;

	CFont*	m_Font;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAuthorizationSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	NODEITEM* m_pNode;
	//CBrush m_SilverBrush;
	// Generated message map functions
	//{{AFX_MSG(CDlgAuthorizationSetting)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheck1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
 
	virtual INT_PTR DoModal();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAUTHORIZATIONSETTING_H__D5112708_6DEE_4DE6_BDC2_DC54495CAFCE__INCLUDED_)
