#pragma once
#include "resource.h"
#include "BaseDialog.h"
#include "INI.h"
#include "afxext.h"
#include "afxdtctl.h"
#include "DlgAddEditFTPServer.h"

#include "Wincrypt.h"

#define Const1 52845
#define Const2 22719

struct strFTP
{
	int			ftpid;
	CString	csActive;
	CString	csHost;
	CString	csUser;
	CString	csPassword;
};

// CDlgSettingFtpFtp dialog

class CDlgSettingFtpFtp : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgSettingFtpFtp)

public:
	CDlgSettingFtpFtp(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSettingFtpFtp();

// Dialog Data
	enum { IDD = IDD_DLGSETTINGFTPFTP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CBitmapButton m_butAdd;
	CBitmapButton m_butDel;
	CBitmapButton m_butEdit;
	CToolTipCtrl *m_ToolTip;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	
	//CDatabase m_DB;
	//bool vDBConnect(void);
	CDatabase* pDB;
	void vRetrieveFtp(void);
	bool vInitFtp(void);

	CListCtrl * pFtpList;
	afx_msg void OnBnClickedButadd();
	afx_msg void OnBnClickedButdelete();
	afx_msg void OnBnClickedButtonedit();

	//CBitmapButton* m_butDel;
	CListCtrl*	m_listCtrl;
	void vSaveFtp(void);
//	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);

	CString m_winMode;
	int	m_ftpid;
	CDateTimeCtrl* m_Pick;
	
	FTPInfo m_FTPInfo;
	vector<FTPInfo> veFTPInfo;

	FTPInfo mGetFTPServerVector(CString csFTPServer);

	//CCrypto
	CCrypto m_CCrypto;
	void strtohex(const char* str, unsigned char* hex);
	CString DecryptFTPPassword(CString csEncrypt);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	bool DirtyFlag;
};
