#pragma once
#include "resource.h"

// CEventTabLog dialog

class CEventTabLog : public CDialog
{
	DECLARE_DYNAMIC(CEventTabLog)

public:
	CEventTabLog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEventTabLog();

// Dialog Data
	enum { IDD = IDD_DLG_TABEVENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
