#pragma once

#include "resource.h"
#include "BaseDialog.h"

// CDlgAddEditFTPServer dialog

struct FTPInfo
{
	TCHAR tcHost[100];
	TCHAR tcUser[100];
	TCHAR tcPassword[100];
};

class CDlgAddEditFTPServer : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgAddEditFTPServer)

public:
	CDlgAddEditFTPServer(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAddEditFTPServer();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

// Dialog Data
	enum { IDD = IDD_DLG_ADD_MODIFY_FTP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	FTPInfo m_FTPInfo;

	void vSetFTPInfo(FTPInfo o_FTPInfo);

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
