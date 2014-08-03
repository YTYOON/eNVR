#pragma once
#include "resource.h"
#include "MainFrm.h"
#include "Wincrypt.h"
#include "VirtualKeyboard.h"

// CDlgUserLogin dialog

class CDlgUserLogin : public CDialog
{
	DECLARE_DYNAMIC(CDlgUserLogin)

public:
    enum USER_LOGIN_TYPE {LOGIN =0, PASSWORD_SETTING =1, UNKNOWN2=2, UNKNOWN3=3, LOGOUT =4, RESTART=5};

	CDlgUserLogin(CDatabase* pDB, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgUserLogin();

// Dialog Data
	enum { IDD = IDD_DLG_USER_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	CString m_csUserName, m_csPassword, m_csRegValue, m_csConfirmPwd;
	CDatabase* m_pDB;
	int m_iChecksum, m_iOption;
	int iCheck(CString csUser, CString csPwd);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	bool bAttach(CString csUser, CString csPwd);
	afx_msg void OnBnClickedCheckLoginAuto();
	void vSetOption(int iOption = 0);
	//encrypt password
	CCrypto m_Crypto;
	CString m_csCryptoKey;

	CString csDecryptAuthPassword(CString &csEncrypt);
	void strtohex(const char *str, unsigned char *hex);
protected:
    VK* m_pVk;
	CMainFrame* m_pMainFrm;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
