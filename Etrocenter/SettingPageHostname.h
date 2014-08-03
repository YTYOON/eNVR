#if !defined(AFX_SETTINGPAGEHOSTNAME_H__A841A41D_2EDD_4F1E_A78F_E1FAF804ACC6__INCLUDED_)
#define AFX_SETTINGPAGEHOSTNAME_H__A841A41D_2EDD_4F1E_A78F_E1FAF804ACC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingPageHostname.h : header file
//
#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////
// CSettingPageHostname dialog

class CSettingPageHostname : public CDialog
{
// Construction
public:
	void GetSettingValue(CString & strHostname, CString & strDomain );
	CSettingPageHostname(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSettingPageHostname)
	enum { IDD = IDD_SETTING_PAGE_HOSTNAME };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingPageHostname)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSettingPageHostname)
	afx_msg void OnApply();
	afx_msg void OnChangeDomainname();
	afx_msg void OnChangeHostname();
	virtual BOOL OnInitDialog();
	afx_msg void OnRestore();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void LoadHostnameData();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGPAGEHOSTNAME_H__A841A41D_2EDD_4F1E_A78F_E1FAF804ACC6__INCLUDED_)
