#pragma once
#include "resource.h"

// CDlgSMTPSetting dialog

class CDlgSMTPSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgSMTPSetting)

public:
	CDlgSMTPSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSMTPSetting();

// Dialog Data
	enum { IDD = IDD_SETTING_SMTP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CComboBox* m_cbSMTPSecure;
	void vSetSMTP2Reg();

	CCrypto m_CCrypto;
	void strtohex(const char* str, unsigned char* hex);
	CString DecryptSMTPPassword(CString csEncrypt);
//	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
