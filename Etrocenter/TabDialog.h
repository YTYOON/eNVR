#pragma once
#include "resource.h"		// main symbols

// CTabDialog dialog

class CTabDialog : public CDialog
{
	DECLARE_DYNAMIC(CTabDialog)

public:
	CTabDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabDialog();

// Dialog Data
	enum { IDD = IDD_TABDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
