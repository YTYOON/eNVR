#pragma once
#include "resource.h"
#include "BtnST.h"
// CWizardPage3 dialog

class CWizardPage3 : public CPropertyPage
{
	DECLARE_DYNAMIC(CWizardPage3)

public:
	CWizardPage3();
	virtual ~CWizardPage3();

// Dialog Data
	enum { IDD = IDD_DLG_WIZARD_P3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	CButtonST	*m_pBtnRecording;
	CButtonST	*m_pBtnSystem;
	CButtonST   *m_pBtnApp;
	afx_msg void OnRangeCmds(UINT nID);
};
