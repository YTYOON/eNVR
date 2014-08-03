#pragma once
#include "resource.h"
#include "BtnST.h"

// CWizardPage2 dialog

class CWizardPage2 : public CPropertyPage
{
	DECLARE_DYNAMIC(CWizardPage2)

public:
	CWizardPage2();
	virtual ~CWizardPage2();

// Dialog Data
	enum { IDD = IDD_DLG_WIZARD_P2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	CButtonST	*m_pBtnMatrix;
	afx_msg void OnRangeCmds(UINT nID);
};
