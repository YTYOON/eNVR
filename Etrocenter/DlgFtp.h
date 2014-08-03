#pragma once
#include "resource.h"
#include "BaseDialog.h"
#include "afxcmn.h"
#include "DlgSettingFtpFtp.h"
#include "DlgSettingFtpEmail.h"
#include "DlgSettingSystem.h"
//#include "DlgSettingLogin.h"
//#include "DlgSettingLoginAuth.h"
#include "RemoteSrvPort.h"
#include "DlgSMTPSetting.h"

#define GENERALPAGE 0
#define FTPPAGE 1
#define EMAILPAGE 2
//#define USERLOGIN 3
#define USERAUTH 4
#define WEBPORT 5
#define SMTPSERVER	6
#define EXPnIMP	7
// CDlgFtp dialog

class CDlgSettingLoginAuth;
class CDlgSettingLogin;
class CDlgExpImp;

class CDlgFtp : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgFtp)

public:
	CDlgFtp(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFtp();

// Dialog Data
	enum { IDD = IDD_DLG_FTP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	CTabCtrl* m_tabFtp;
	//afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

	 CDlgSettingLoginAuth* m_DlgSettingLoginAuth;
	 CDlgSettingFtpFtp* m_DlgFtpPage;
	 CDlgSettingFtpEmail* m_DlgEmailPage;
	 CDlgSettingSystem* m_DlgSystemPage;
	 CDlgSettingLogin* m_DlgSettingLogin;
	 CRemoteSrvPort* m_RemoteSrvPort;
	 CDlgSMTPSetting* m_CDlgSMTPSetting;

//	 afx_msg void OnPaint();
//	 afx_msg void OnSize(UINT nType, int cx, int cy);
//	 afx_msg void OnTcnSelchangeTabftp(NMHDR *pNMHDR, LRESULT *pResult);

	 afx_msg void OnBnClickedOk();
	 bool vSaveSystem(void);
	 bool vSaveFtp(void);
	 bool vSaveEmail(void);
	 bool vSaveUserLogin(void);
	 afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	 CTabCtrl* m_mainTab;
	 afx_msg void OnTcnSelchangeTabftp(NMHDR *pNMHDR, LRESULT *pResult);
	 afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	 bool vCheckDirty(void);
	 bool m_dirty;
	 afx_msg void OnBnClickedButsave();
	 int m_curTabPage;
	 virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void OnCancel();
	void ChangeControlLanguage();
public:
	afx_msg void OnMove(int x, int y);
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
//    afx_msg void OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct);
//	afx_msg void OnDestroy();
};
