#pragma once

#include "resource.h"
#include "INI.h"
#include "BtnST.h"
// CDlgAppTag dialog

class CDlgAppTag : public CDialog
{
	DECLARE_DYNAMIC(CDlgAppTag)

public:
	CDlgAppTag(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAppTag();

// Dialog Data
	enum { IDD = IDD_DLG_APP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedEmap();
	afx_msg void OnBnClickedPlayback();
	afx_msg void OnBnClickedLpr();
	afx_msg void OnBnClickedAc();
	afx_msg void OnBnClickedGate();

	CButtonST*	btnclose;
	CButtonST*	btnemap;
	CButtonST*	btnpback;
	CButtonST*	btnac;
	CButtonST*	btngate; //Parking gate
	CButtonST*	btnlpr; 

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	vector<CRect> m_rectButton;

	//Access Control
	bool		mb_AC, mb_gate;
	bool		mb_lpr;	//License Plate
	CIniReader		m_ini;

	vector<int> m_vtRValue;
};
