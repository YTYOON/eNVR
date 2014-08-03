#include "resource.h"
#include "BaseDialog.h"
#include "TabpageAcCam.h"
#include "TabpageGatePark.h"
#include "TabpageAcEmp.h"
#include "TabpageAcHol.h"

#pragma once


// CDlgSettingAC dialog

class CDlgSettingAC : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgSettingAC)

public:
	CDlgSettingAC(CWnd* pParent = NULL);   // standard constructor
	CDatabase*	pDB;
	virtual ~CDlgSettingAC();

// Dialog Data
	enum { IDD = IDD_SETTING_AC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:	
		CTabCtrl		m_tab;
		CTabpageAcCam		m_pagecam;
		CTabpageGatePark	m_pageac;
		CTabpageAcEmp		m_pageemp;
		CTabpageAcHol			m_pagehol;
		CDialog*	pDialog[4];

		//vector <STRCAMAC> m_vpCamAccess;	

		virtual BOOL OnInitDialog();
		void		fnPopulateCam(void);
		int		m_camidx;
		bool	mb_camChanged;
		bool	mb_acChanged;	//for Ac chain
		bool	mb_openNormal;

		bool	mb_holChanged; //holiday changed
		bool	mb_priChanged; //work hour & privilege changed

		CListCtrl*	m_CamList;
		afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);

		CImageList			m_SmallImg;

		//Functions
		void fnInitControl(void);
		void fnInitAccess(void);
		void fnClearAc(void);
		void fnRetrieveData(void);
		void fnFreeMemory(void);

		afx_msg void OnBnClickedButsave();
		afx_msg void OnBnClickedButdel();

		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnTcnSelchangeTabmain(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg void OnBnClickedButapply();

		void fnDisplayBrand(void);
		void fnSave(CString o_mode);

protected:
	virtual void OnOK();
public:
//	afx_msg void OnBnClickedOk();
};
