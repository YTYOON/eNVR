//#include "DlgAcSearch.h"
#include "resource.h"
#include "BaseDialog.h"
#include "TabpageAcHol.h"

#pragma once
#define IDD_DLGACHOL                    146

// CDlgAcSearch dialog

class CDlgAcSearch : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgAcSearch)

public:
	CDlgAcSearch(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAcSearch();

// Dialog Data
	enum { IDD = IDD_DLGACHOL };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	//CString	m_dept;
	CString	m_hol;
	CDatabase*	psDB;

protected:
	virtual void OnOK();

public:
	bool fnDateExist(CString oDate);
	CTabpageAcHol*	 m_parent;
};

