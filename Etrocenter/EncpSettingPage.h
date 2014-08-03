#if !defined(AFX_ENCPSETTINGPAGE_H__E1D0F405_796E_4851_94D8_F88C4B818DF6__INCLUDED_)
#define AFX_ENCPSETTINGPAGE_H__E1D0F405_796E_4851_94D8_F88C4B818DF6__INCLUDED_

#include "SettingPageNetwork.h"	// Added by ClassView
#include "SettingPageHostname.h"	// Added by ClassView
#include "SettingPageAccount.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EncpSettingPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEncpSettingPage dialog

class CEncpSettingPage : public CDialog
{
// Construction
public:
	friend class CSettingPageNetwork;
	friend class CSettingPageHostname;
	friend class CSettingPageAccount;

	void OnRestore( int page );
	void OnApply( int page );
	BOOL IsMessageReceived() const;
	void SetENCPItem(HENCPITEM hEncpItem );
	void SetENCPItem(vector <HENCPITEM>* pvthENCPItem);
	void OnENCPItemReceived(HENCPITEM hItem, ENCPCBMSG msg, LPARAM lp);
	void Close(UINT nID);
	CEncpSettingPage(CWnd* pParent = NULL);   // standard constructor
//	virtual ~CEncpSettingPage();

// Dialog Data
	//{{AFX_DATA(CEncpSettingPage)
	enum { IDD = IDD_SETTING_PAGE_MAIN };
	CButton	m_buttonNetwork;
	CButton	m_buttonHostName;
	CButton	m_buttonAccount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEncpSettingPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEncpSettingPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonNetwork();
	afx_msg void OnButtonHostname();
	afx_msg void OnButtonAccount();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString m_strAdminID;
	CString m_strAdminPass;

	BOOL m_bGuestAllow;
	ENCPNODEINFO m_nodeInfo;
	ENCPNETWORK m_netInfo;

	BOOL m_bMsgReceived;
	HENCPITEM m_hENCPItem;
	vector <HENCPITEM>* m_pvthENCPItem;
	CDialog * m_dlgPage[3];

	void SelectPage( UINT button );
	int ShowAuthDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENCPSETTINGPAGE_H__E1D0F405_796E_4851_94D8_F88C4B818DF6__INCLUDED_)
