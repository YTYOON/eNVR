#include "resource.h"
#pragma once
#include "afxext.h"
#include "BaseDialog.h"
#include "afxcmn.h"


// CTabpageEmapEvent dialog

class CTabpageEmapEvent : public CBaseDialog
{
	DECLARE_DYNAMIC(CTabpageEmapEvent)

public:
	CTabpageEmapEvent(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabpageEmapEvent();

// Dialog Data
	enum { IDD = IDD_DLGEMAP_TABEVENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButsaveas();

	CToolTipCtrl* m_ToolTip;
	bool fnDisplayEvent(CString oCamIdx, CString oCamIP, CString oEventType, CString o_curHMS);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CBitmapButton m_butClear;
	CBitmapButton m_butSaveas;
	CListCtrl m_listevent;
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CString SmartEventType(CString oOrgEventType, bool obLong);
	CString SmartEventType(CString oOrgEventType);

	bool fnSaveEventLog(void);
	afx_msg void OnBnClickedButclear();
 
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
