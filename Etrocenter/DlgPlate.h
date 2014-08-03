#include "BaseDialog.h"
#include "resource.h"
#pragma once#include "afxwin.h"



// CDlgPlate dialog

class CDlgPlate : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgPlate)

public:
	CDlgPlate(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPlate();

// Dialog Data
	enum { IDD = IDD_DLGPLATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	
	char	mc_mode;
	CString	m_card;
	CString	m_name;
	CString	m_plate1, m_plate2, m_plate3;

	CEdit m_lplate1;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
