#pragma once
#include "resource.h"
#include "MainFrm.h"
#include "DlgSettingLogin.h"
#include "DlgSettingPermission.h"

// CDlgSettingLoginAuth dialog

class CDlgSettingLoginAuth : public CDialog
{
	DECLARE_DYNAMIC(CDlgSettingLoginAuth)

public:
	CDlgSettingLoginAuth(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSettingLoginAuth();

// Dialog Data
	enum { IDD = IDD_DLG_USER_AUTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	CListCtrl* m_pAuthList;
	CDatabase* m_pDB;

	CBitmapButton m_cbDelete;
	CBitmapButton m_cbAdd;
	CBitmapButton m_cbEdit;
	CToolTipCtrl *m_ToolTip;

	CString m_csRegUser, m_csRegPass, m_csRegValue;;

	map<CString, CString> mapUserPwd;
	
	CCrypto m_Crypto;
	CString csDecryptPassword( CString &csEncrypt );
	void strtohex(const char* str, unsigned char* hex);

public:
	void vGetAllUser(void);
	bool bCheckUserExist(CString &csUserName);
	afx_msg void OnLvnItemchangedListAuth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemActivateListAuth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemclickListAuth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListAuth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAuthAdd();
	afx_msg void OnBnClickedButtonAuthDelete();
	afx_msg void OnBnClickedButtonAuthModify();
	bool bSaveInfo2DB(void);
	afx_msg void OnBnClickedCheckAutoLogin();

private:
	void vGetForderString(CString& strFolder);
};
