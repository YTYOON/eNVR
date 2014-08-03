#if !defined(AFX_LISTCTRLEX_H__60734E91_5026_4FF6_8445_955A9CC73675__INCLUDED_)
#define AFX_LISTCTRLEX_H__60734E91_5026_4FF6_8445_955A9CC73675__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCtrlEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx window
#define CR_BASE_HIGHLIGHT_FG  BKCOLOR
#define CR_BASE_HIGHLIGHT_BG FONTCOLOR
#define CR_BASE_BG  BKCOLOR
#define CR_BASE_FG FONTCOLOR

class CListCtrlEx : public CListCtrl
{
// Construction
public:
	CListCtrlEx(COLORREF bgColor = CR_BASE_BG, COLORREF fgColor = CR_BASE_FG, COLORREF heightLightbgColor = CR_BASE_HIGHLIGHT_BG, COLORREF heightLightfgColor = CR_BASE_HIGHLIGHT_FG);
	virtual ~CListCtrlEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
    void SetColor(COLORREF bgColor, COLORREF fgColor, COLORREF heightLightbgColor, COLORREF heightLightfgColor);
	int nGetCheckedItem() {return nCheckedItem;};
	int nSetCheckedItem(int nItem){nCheckedItem=nItem; return nCheckedItem;};
	void OnCustomDrawList( NMHDR* pNMHDR, LRESULT* pResult );

	// Generated message map functions
protected:
    COLORREF m_crHlFg;
    COLORREF m_crHlBg;
    COLORREF m_crFg;
    COLORREF m_crBg;

	int nCheckedItem;
	//{{AFX_MSG(CListCtrlEx)
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLEX_H__60734E91_5026_4FF6_8445_955A9CC73675__INCLUDED_)
