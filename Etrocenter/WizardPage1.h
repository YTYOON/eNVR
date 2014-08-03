#pragma once
#include "resource.h"
#include "BtnST.h"
// CWizardPage1 dialog

class CWizardPage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(CWizardPage1)

public:
	CWizardPage1();
	virtual ~CWizardPage1();

// Dialog Data
	enum { IDD = IDD_DLG_WIZARD_P1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
	afx_msg void OnBnClickedCheckWizardP1();
	virtual BOOL OnInitDialog();
	CButtonST	*m_pBtnAddCam;
	CButtonST	*m_pBtnMass;
	afx_msg void OnPaint();
	afx_msg void OnRangeCmds(UINT nID);
	virtual LRESULT OnWizardNext();
};
