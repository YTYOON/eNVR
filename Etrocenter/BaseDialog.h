#pragma once
#include "resource.h"
#include "afxwin.h"

// CBaseDialog dialog

class CBaseDialog : public CDialog
{
	DECLARE_DYNAMIC(CBaseDialog)

public:
	CBaseDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor
	virtual ~CBaseDialog();

// Dialog Data
//	explicit enum { IDD };
	
	//virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// Standard Font
	CFont*	m_Font;
	CBrush	m_SilverBrush;
	CBrush	m_WhiteBrush;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	virtual INT_PTR DoModal(UINT nIDTemplate);
	virtual INT_PTR DoModal();
protected:
    COLORREF m_clrBk;
    COLORREF m_clrFont;

protected:
	virtual void OnOK();
};
