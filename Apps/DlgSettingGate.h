#include "resource.h"
#include "BaseDialog.h"
#include "TabpageGatePark.h"
#include "TabpageGatePlate.h"
#include "TabpageAcCam.h"
#include "afxcmn.h"

#pragma once

// CDlgSettingGate dialog

class CDlgSettingGate : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgSettingGate)

public:
		CDatabase*	pDB;
	CDlgSettingGate(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSettingGate();

	CTabpageAcCam		m_pagecam;
	CTabpageGatePark	m_pagepark;
	CTabpageGatePlate	m_pageplate;

	CDialog*	pDialog[3];

// Dialog Data
	enum { IDD = IDD_SETTING_GATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CTabCtrl m_tab;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	CToolTipCtrl*  m_pToolTipCtrl;  


protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedButsave();
	// populate group into combobox
	void fnPopulateGroup(void);
	void fnFreeMemory(void);
	afx_msg void OnTcnSelchangeTabmain(NMHDR *pNMHDR, LRESULT *pResult);
};
