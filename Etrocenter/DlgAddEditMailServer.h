#pragma once

#include "resource.h"
#include "BaseDialog.h"

#define AddServer 1
#define EditServer 2
#define AddMailAddress 3
#define EditMailAddress 4

// CDlgAddEditMailServer dialog

struct MailInfo{

	TCHAR cSMTPServer[100];
	TCHAR cUserName[100];
	TCHAR cPassword[100];
};

class CDlgAddEditMailServer : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgAddEditMailServer)

public:
	//CDlgAddEditMailServer(CWnd* pParent = NULL);   // standard constructor
	CDlgAddEditMailServer(int ClassType, CWnd* pParent = NULL);  // standard constructor
	virtual ~CDlgAddEditMailServer();

// Dialog Data
	enum { IDD = IDD_DLG_ADD_EDIT_MAIL_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	int iClassType;
	MailInfo m_MailInfo;
	
	void vSetServerInfo(MailInfo o_MailInfo);

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
