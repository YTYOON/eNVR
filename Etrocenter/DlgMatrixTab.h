#pragma once
#include "afxcmn.h"
#include "resource.h"
#include "DlgMatrixTag.h"
#include "DlgMatrixEvent.h"
#include "EnTabCtrl.h"


enum RESTORE_BTN_TYPE{LIVEVIEWBTN=0,EVENTBTN};

class CCustomTabCtrl :public CTabCtrl
{
	DECLARE_DYNAMIC(CCustomTabCtrl)
public:
    CCustomTabCtrl(){m_Brushbk.CreateSolidBrush(BKCOLOR);};
    virtual ~CCustomTabCtrl(){};
	//{{AFX_MSG(CCustomTabCtrl)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
    CBrush m_Brushbk;

};
// DlgMatrixTab dialog

class DlgMatrixTab : public CDialog
{
	DECLARE_DYNAMIC(DlgMatrixTab)


public:
	DlgMatrixTab(CWnd* pParent = NULL, DWORD nID = IDD_DLG_MATRIX_TAB);   // standard constructor
	virtual ~DlgMatrixTab();

// Dialog Data
	enum { IDD = IDD_DLG_MATRIX_TAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    void vSetWindowStylePopup(){m_bPopupDialog = true;};
    void vSetWindowStyleChild(){m_bPopupDialog = false;};
    bool bDialogStyle() {return m_bPopupDialog;}
	CEnTabCtrl m_TabCtrl;
	int m_nCurTabIdx;
	CDlgMatrixTag* m_dlgMatrixTag;
	CDlgMatrixEvent* m_dlgMatrixEvent;
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedClose();
	CButtonST *btnclose;
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void vRestoreBtnState(int nType);
protected:
    bool m_bPopupDialog;
    CBrush m_Brushbk;
protected:
    void vSetTabCtrlStyle(void);

    //virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
    //virtual INT_PTR DoModal();
};
