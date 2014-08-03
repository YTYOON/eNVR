 #pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "resource.h"
#include "BaseDialog.h"
#include "INI.h"

struct RecTimeInfo
{
	TCHAR cDayType[30];
	TCHAR cDate[30];
	TCHAR cTimeFrom[30];
	TCHAR cTimeTo[30];
};

// CDlgAddModifyRecTime dialog

class CDlgAddModifyRecTime : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddModifyRecTime)

public:
	CDlgAddModifyRecTime(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAddModifyRecTime();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CBrush	m_SilverBrush;
	CBrush	m_WhiteBrush;

// Dialog Data
	enum { IDD = IDD_DLG_REC_TIME_ADD_MODIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CComboBox pDayTypeCombo;
	CDateTimeCtrl pDatePicker;
	CDateTimeCtrl pTimeFromPicker;
	CDateTimeCtrl pTimeToPicker;

	CDatabase* pDB;
	CString csDSN, csDBUser, csDBPass, csODBC;	//For Database

	bool EditFlag;
	CString m_csDayType, m_csDate, m_csTimeFrom, m_csTimeTo;
public:
	RecTimeInfo m_RecTimeInfo;

	/*void vDBConnect();
	void vDBClose();*/
	virtual BOOL OnInitDialog();
	void vSetScheduleTime(CString csDayType, CString csDate, CString csTimeFrom, CString csTimeTo);
	afx_msg void OnCbnSelchangeComboDayType();
	afx_msg void OnBnClickedOk();
};
	