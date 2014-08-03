#if !defined(AFX_EVENTLOGVIEW_H__95D6BD8B_014D_42A2_B3E7_C61A2C670D3A__INCLUDED_)
#define AFX_EVENTLOGVIEW_H__95D6BD8B_014D_42A2_B3E7_C61A2C670D3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EventLogView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEventLogView view

class CEventLogView : public CView
{
protected:
	CEventLogView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEventLogView)

// Attributes
private:
	CListCtrl m_listctrl;
public:

// Operations
public:
	void vSetListStyle();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEventLogView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CEventLogView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CEventLogView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTLOGVIEW_H__95D6BD8B_014D_42A2_B3E7_C61A2C670D3A__INCLUDED_)
