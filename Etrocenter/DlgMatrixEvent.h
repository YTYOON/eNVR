#pragma once
#include "resource.h"
#include "BtnST.h"
// CDlgMatrixEvent dialog

class CDlgMatrixEvent : public CDialog
{
	DECLARE_DYNAMIC(CDlgMatrixEvent)

public:
	CDlgMatrixEvent(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMatrixEvent();

// Dialog Data
	enum { IDD = IDD_DLG_MATRIX_EVENT };

protected:   
    CBrush m_Brushbk;
    BOOL CreateMatrixs();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	vector <CButtonST*> m_vtBtnMatrixs;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CButtonST *btnE2x2;
	CButtonST *btnE1P5;
	CButtonST *btnE2P8;
	CButtonST *btnE1P12_2;
	CButtonST *btnE1P16;
	vector<CRect> m_rectButton;
	//afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedE2x2();
	afx_msg void OnBnClickedE1P5();
	afx_msg void OnBnClickedE2P8();
	afx_msg void OnBnClickedE1P12_2();
	afx_msg void OnBnClickedE1P16();
	afx_msg void OnMatrixBnClicked(UINT uid);
	void vSetMatrixBtnHot(UINT nId);
	map <int, int> mapTable;
	

protected:
	virtual void OnCancel();
};
