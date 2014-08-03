#pragma once


// CDlgAutoScanPause dialog

#include "resource.h"
#include "afxwin.h"
#include "BtnST.h"

class CDlgAutoScanPause : public CDialog
{
	DECLARE_DYNAMIC(CDlgAutoScanPause)

public:
	CDlgAutoScanPause(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAutoScanPause();

// Dialog Data
	enum { IDD = IDD_DLG_AUTOSCAN_PAUSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
protected:
	virtual void PostNcDestroy();
public:
	CStatic m_static_GroupName;
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void vSetDisplayGroupName(void);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnPauseAutoscan();
	void* m_pParentHwnd;
	bool m_bPause;
	CButtonST *pBtnPause;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
