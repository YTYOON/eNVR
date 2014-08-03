#pragma once
#include "resource.h"
#include "WizardPage1.h"
#include "WizardPage2.h"
#include "WizardPage3.h"

// CWizard

class CWizard : public CPropertySheet
{
	DECLARE_DYNAMIC(CWizard)

public:
	CWizard(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CWizard(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CWizard();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CWizardPage1 m_page1;
	CWizardPage2 m_page2;
	CWizardPage3 m_page3;
	CListCtrl * m_pList;
	CImageList m_ImageList;
	void vSetListIdx(int i);
};


