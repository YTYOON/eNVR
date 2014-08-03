#pragma once

#include "resource.h"		// main symbols
// CTabpagePTZDialog dialog

class CTabpagePTZDialog : public CDialog
{
	DECLARE_DYNAMIC(CTabpagePTZDialog)

public:
	CTabpagePTZDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabpagePTZDialog();

// Dialog Data
	enum { IDD = IDD_TABPAGEPTZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
