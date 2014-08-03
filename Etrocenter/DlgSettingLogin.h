#pragma once
#include "resource.h"
#include "BaseDialog.h"

// CDlgSettingLogin dialog

class CDlgSettingLogin : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgSettingLogin)

public:
	CDlgSettingLogin(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSettingLogin();

// Dialog Data
	enum { IDD = IDD_DLG_AUTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_csUser, m_csPass;
	CBrush	m_SilverBrush;
	CBrush	m_WhiteBrush;
	bool m_bOK;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
