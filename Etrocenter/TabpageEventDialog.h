#pragma once
#include "resource.h"

// CTabpageEventDialog dialog

class CTabpageEventDialog : public CDialog
{
	DECLARE_DYNAMIC(CTabpageEventDialog)

public:
	CTabpageEventDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabpageEventDialog();

// Dialog Data
	enum { IDD = IDD_TABPAGEEVENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
