#include "resource.h"
#include "DlgActionSetting.h"

#pragma once

// CDlgAction dialog

class CDlgAction : public CDialog
{
	DECLARE_DYNAMIC(CDlgAction)

	enum ACTION_INIT {ADD=0, MODIFY};

public:
	CDlgAction(CWnd* pParent = NULL);   // standard constructor
	CDlgAction(ActionInfo pActionInfo, CWnd* pParent = NULL);
	virtual ~CDlgAction();

// Dialog Data
	enum { IDD = IDD_DLG_SETTING_ACTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

public:
	ActionInfo m_ActionInfo;
	virtual BOOL OnInitDialog();
	int iINIT;
	afx_msg void OnBnClickedCheckActionEmail();
	afx_msg void OnBnClickedCheckActionDo();
	afx_msg void OnBnClickedCheckActionPtz();
};
