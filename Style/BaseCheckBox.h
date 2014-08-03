#pragma once


// CBaseCheckBox

class CBaseCheckBox : public CButton
{
	DECLARE_DYNAMIC(CBaseCheckBox)
public:
	CBaseCheckBox();
	virtual ~CBaseCheckBox();
    int GetCheck(){return m_bChecked;};
    void SetCheck(int nCheck){m_bChecked = nCheck;UpdateWindow();};
protected:
    bool m_bChecked;
    CBrush m_BrushBg;
    COLORREF m_clrFg;
		
public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//{{AFX_MSG(CMyCheckBox)
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT nCtlColor);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
   	virtual void PreSubclassWindow();


};


