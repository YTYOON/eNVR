#if !defined(AFX_SETTINGPAGENETWORK_H__6EC19A47_FB3D_4708_9726_EA72ACD6EE1E__INCLUDED_)
#define AFX_SETTINGPAGENETWORK_H__6EC19A47_FB3D_4708_9726_EA72ACD6EE1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingPageNetwork.h : header file
//
#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////
// CSettingPageNetwork dialog

class CSettingPageNetwork : public CDialog
{
// Construction
public:
	void GetSettingValue(PENCPNETWORK pNetInfo );
	CSettingPageNetwork(CWnd* pParent = NULL);   // standard constructor

	void ShowNetworkControls(BOOL bShow=TRUE);
	void EnablePPPControls(BOOL bEnable=TRUE);
	void EnableNetworkControls(BOOL bEnable=TRUE);
	void ShowPPPControls(BOOL bShow=TRUE);
	void SetReadOnly(BOOL bReadOnly);
// Dialog Data
	//{{AFX_DATA(CSettingPageNetwork)
	enum { IDD = IDD_SETTING_PAGE_NETWORK };
	CEdit	m_editPPPService;
	CEdit	m_editPPPPassword;
	CEdit	m_editPPPMTU;
	CEdit	m_editPPPID;
	CIPAddressCtrl	m_netIP;
	CIPAddressCtrl	m_netIPNS;
	CIPAddressCtrl	m_netIPNM;
	CIPAddressCtrl	m_netIPGW;
	CIPAddressCtrl	m_pppDns;
	int		m_nChoice;
	int		m_pppDnsType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingPageNetwork)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CBrush m_BrushDocBar;
	// Generated message map functions
	//{{AFX_MSG(CSettingPageNetwork)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnFieldchangedIpGw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFieldchangedIpNetmask(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFieldchangedIpNs(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFieldchangedIpaddress(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChoiceChanged();
	afx_msg void OnRadioDnsType();
	afx_msg void OnChangeEdit();
	afx_msg void OnApply();
	afx_msg void OnRestore();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void LoadNetworkInfo();
	BOOL m_bEnablePPPoE;
	BOOL m_bEnableNetwork;
	int m_nOldChoice;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGPAGENETWORK_H__6EC19A47_FB3D_4708_9726_EA72ACD6EE1E__INCLUDED_)
