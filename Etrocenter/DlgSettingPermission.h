#pragma once
#include "resource.h"
#include "BaseDialog.h"

// CDlgSettingPermission dialog


class CDlgSettingPermission : public CDialog
{
	DECLARE_DYNAMIC(CDlgSettingPermission)

public:
	CDlgSettingPermission(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSettingPermission();

// Dialog Data
	enum { IDD = IDD_DLG_PERMISSION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	void vAttach(CString csUser, CString csPass, bool bLv, bool bSetting, bool bApp);
public:
	CString m_csUser, m_csPass, m_csOrgPass;
	bool m_bLV, m_bSetting, m_bApp, m_bORG;
	virtual BOOL OnInitDialog();
};
