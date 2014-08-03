#pragma once
#include "resource.h"
#include "BaseDialog.h"
#include "DlgAddEditMailServer.h"
#include "INI.h"

// CDlgSettingFtpEmail dialog

class CDlgSettingFtpEmail : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgSettingFtpEmail)

public:
	CDlgSettingFtpEmail(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSettingFtpEmail();

// Dialog Data
	enum { IDD = IDD_DLGSETTINGFTPEMAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CBitmapButton m_butSAdd;
	CBitmapButton m_butSDel;
	CBitmapButton m_butSMod;
	CBitmapButton m_butAdd;
	CBitmapButton m_butDel;
	CBitmapButton m_butMod;
	CToolTipCtrl *m_ToolTip;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	void vInitEmail(void);
	 
	afx_msg void OnBnClickedButServeradd();
	afx_msg void OnBnClickedButServerdel();

	CListCtrl* pServerList;
	CListCtrl* pEmailList;
	MailInfo m_MailInfo;
	vector<MailInfo> veMailServer;
	map<CString, CString> MailPwdMap;
	//CDatabase m_DB;

	CCrypto m_Crypto;
	CString csCryptoKey;

	//void vDBConnect();
	void vRetrieveEmail(void);
	void vSaveEmail();
	MailInfo mGetMailServerVactor(CString csSMTPServer);

	//CCrypto
	CCrypto m_CCrypto;
	void strtohex(const char* str, unsigned char* hex);
	CString DecryptEMailServerPassword(CString csEncrypt);
	
	afx_msg void OnBnClickedButServermod();
	afx_msg void OnBnClickedButAdd();
	afx_msg void OnBnClickedButMod();
	afx_msg void OnBnClickedButDel();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	bool DirtyFlag;
};
