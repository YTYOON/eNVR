#include "resource.h"
#include "BaseDialog.h"

#pragma once


// CTabpageEmapAc dialog

class CTabpageEmapAc : public CBaseDialog
{
	DECLARE_DYNAMIC(CTabpageEmapAc)

public:
	CTabpageEmapAc(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabpageEmapAc();

// Dialog Data
	enum { IDD = IDD_DLGEMAP_TABAC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButset();
	virtual BOOL OnInitDialog();
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CBitmapButton		m_set;
	CBitmapButton		m_butrep;
	CImageList			m_images;
	//CListCtrl*				m_listac;
	CListCtrl				m_listac;

	afx_msg void OnBnClickedButrep();
	//bool fnInsertRow(CString oTime, CString oCamidx, CString oDept, CString oName);
	bool fnInsertRow(CString oDate, CString oTime, CString oCamidx, CString oDept, CString oName);

	bool fnDisplayAccess(CString oCamIdx, CString oCamIP, CString oEventType, CString o_curHMS);
};
