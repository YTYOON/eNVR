#if !defined(AFX_TABONEDIALOG_H__DABF0DAC_6EEC_4BF9_A071_57759E7D12B4__INCLUDED_)
#define AFX_TABONEDIALOG_H__DABF0DAC_6EEC_4BF9_A071_57759E7D12B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabOneDialog.h : header file
#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTabOneDialog dialog

class CTabOneDialog : public CDialog
{
// Construction
public:
	CTabOneDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabOneDialog)
	enum { IDD = IDD_TABPAGEPTZ };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabOneDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabOneDialog)
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABONEDIALOG_H__DABF0DAC_6EEC_4BF9_A071_57759E7D12B4__INCLUDED_)
