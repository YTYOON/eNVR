#if !defined(AFX_XPGROUPBOX_H__F70D755B_9C4B_4F4A_A1FB_AFF720C29717__INCLUDED_)
#define AFX_XPGROUPBOX_H__F70D755B_9C4B_4F4A_A1FB_AFF720C29717__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseGroupBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaseGroupBox window

class CBaseGroupBox : public CButton
{
	DECLARE_DYNAMIC(CBaseGroupBox);

// Construction
public:
	CBaseGroupBox();

	enum XPGroupBoxStyle
		{ XPGB_FRAME,  
		  XPGB_WINDOW};

// Attributes
public:

// Operations
public:
   virtual CBaseGroupBox&	SetBorderColor(COLORREF clrBorder);
   virtual CBaseGroupBox&	SetCatptionTextColor(COLORREF clrText);
   virtual CBaseGroupBox& SetBackgroundColor(COLORREF clrBKClient);
   virtual CBaseGroupBox&	SetBackgroundColor(COLORREF clrBKTilte,  COLORREF clrBKClient);
   virtual CBaseGroupBox&	SetXPGroupStyle(XPGroupBoxStyle eStyle); 
    
   virtual CBaseGroupBox& SetText(LPCTSTR lpszTitle);
   virtual CBaseGroupBox& SetFontBold(BOOL bBold);
   virtual CBaseGroupBox& SetFontName(const CString& strFont, BYTE byCharSet = ANSI_CHARSET);
   virtual CBaseGroupBox& SetFontUnderline(BOOL bSet);
   virtual CBaseGroupBox& SetFontItalic(BOOL bSet);
   virtual CBaseGroupBox& SetFontSize(int nSize);
   virtual CBaseGroupBox& SetFont(LOGFONT lf);

   virtual CBaseGroupBox& SetAlignment(DWORD dwType);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseGroupBox)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBaseGroupBox();

	// Generated message map functions
protected:
	void UpdateSurface();
	void ReconstructFont();
	//{{AFX_MSG(CBaseGroupBox)
	afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CString		m_strTitle;

	COLORREF	m_clrBorder;
	COLORREF	m_clrTitleBackground;
	COLORREF	m_clrClientBackground;
	COLORREF	m_clrTitleText;
	
	XPGroupBoxStyle		m_nType;
	DWORD       m_dwAlignment;  

	LOGFONT			m_lf;
	CFont			m_font;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XPGROUPBOX_H__F70D755B_9C4B_4F4A_A1FB_AFF720C29717__INCLUDED_)
