#if !defined(AFX_SETTINGPAGEACCOUNT_H__3B80ADE8_7BF6_4D9C_9182_D5520B7EBC43__INCLUDED_)
#define AFX_SETTINGPAGEACCOUNT_H__3B80ADE8_7BF6_4D9C_9182_D5520B7EBC43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingPageAccount.h : header file
//
#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////
// CSettingPageAccount dialog

class CSettingPageAccount : public CDialog
{
// Construction
public:
	void GetSettingValue( CString & pass, BOOL & bAllowGuest );
	CSettingPageAccount(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSettingPageAccount)
	enum { IDD = IDD_SETTING_PAGE_ACCOUNT };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingPageAccount)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSettingPageAccount)
	afx_msg void OnApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnAllowguest();
	afx_msg void OnChangeNewpass();
	afx_msg void OnRestore();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void LoadAccountData();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGPAGEACCOUNT_H__3B80ADE8_7BF6_4D9C_9182_D5520B7EBC43__INCLUDED_)
