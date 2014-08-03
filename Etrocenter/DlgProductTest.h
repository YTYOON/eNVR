#if !defined(AFX_DLGPRODUCTTEST_H__FD7F630A_BA1E_4BA9_BC0D_45E744D4FE2A__INCLUDED_)
#define AFX_DLGPRODUCTTEST_H__FD7F630A_BA1E_4BA9_BC0D_45E744D4FE2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProductTest.h : header file
//
#include <afxinet.h>
/////////////////////////////////////////////////////////////////////////////
// CDlgProductTest dialog

class CDlgProductTest : public CDialog
{
// Construction
public:
	struct FWUPGRADEPARAM
	{
		CDlgProductTest* pThis;
		NODEITEM* pNode;
	};

	static UINT fnFWUpgrade(LPVOID param);
	void vAssignNode(vector <NODEITEM*>* pvtNode);
#ifdef FACTORY
	vector <NODEITEM*>* GetUpgradeOKNode(){ return &m_vtNodeOK; }
#endif
	CDlgProductTest(CWnd* pParent = NULL);   // standard constructor
	~CDlgProductTest();
// Dialog Data
	//{{AFX_DATA(CDlgProductTest)
	enum { IDD = IDD_DLG_PRODUCT_TEST };
	CListCtrl	m_listctrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProductTest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool bTurnLed(CInternetSession& session, NODEITEM *pNode, bool bOn);
	void vCreateListData();
	bool bGetFWVersion(CInternetSession& session, NODEITEM* pNode, CString& csMsg);
	bool bSendRebootCmd(CInternetSession& session, NODEITEM* pNode, CString& csMsg);
	void vUpdataStatus(NODEITEM* pItem, CString csStatus, WORD nColumn = 4);
	WORD m_Counter;
	char* m_pBuffer;
	bool bPrepareFirmwareBuffer();
	bool bUpdateFirmware(NODEITEM* pNode);
//	CString m_csFile;
	SYSTEMTIME m_TimerStart;
	ULONG m_lTimerStartSec;
	CString m_csHead;
	int m_nBufferSize;
	vector <NODEITEM*>* m_pvtNode;
	vector <NODEITEM*> m_vtNodeOK;
	// Generated message map functions
	//{{AFX_MSG(CDlgProductTest)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnUpgradeFw();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPRODUCTTEST_H__FD7F630A_BA1E_4BA9_BC0D_45E744D4FE2A__INCLUDED_)
