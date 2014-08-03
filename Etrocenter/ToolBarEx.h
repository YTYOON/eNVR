#if !defined(AFX_TOOLBAREX_H__A6D10847_513D_44C1_8C92_ED4F97F183A5__INCLUDED_)
#define AFX_TOOLBAREX_H__A6D10847_513D_44C1_8C92_ED4F97F183A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolBarEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolBarEx window

class CToolBarEx : public CToolBar
{
// Construction
public:
	CToolBarEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolBarEx();

	// Generated message map functions
protected:
	HCURSOR hCursor;
	//{{AFX_MSG(CToolBarEx)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnIconEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBAREX_H__A6D10847_513D_44C1_8C92_ED4F97F183A5__INCLUDED_)
