#pragma once
//#include "stdafx.h"
#include "resource.h"
#include "afxcmn.h"
#include "INI.h"
#include "DlgAddModifyRecTime.h"
#include "BaseDialog.h"

// CDlgRecSchedule dialog

class CDlgRecSchedule : public CDialog
{
	DECLARE_DYNAMIC(CDlgRecSchedule)

public:
	CDlgRecSchedule(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgRecSchedule();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

// Dialog Data
	enum { IDD = IDD_DLG_RECORDING_SCHEDULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CBitmapButton m_cbRecTabDelete;
	CBitmapButton m_cbRecTabAdd;
	CBitmapButton m_cbRecTabEdit;
	CToolTipCtrl *m_ToolTip;

	int m_iCameraIDX;
	//CDatabase DBQuery;
	CString csDSN, csDBUser, csDBPass, csODBC;	//For Database

	CDlgAddModifyRecTime m_CDlgAddModifyRecTime;
	RecTimeInfo m_RecTimeInfo;

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ScheduleListCtrl;

	void vSetCameraIDX(int iCameraIDX);
	//void vDBConnect();
	void vSetScheduleListCtrl(int m_iCameraIDX);
	//CString csSwitchDayType(CString csDayType);
	CString csINTtoDAY(CString csDayType);
	void vSaveRecTime(CString csRecordingID);

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonRecTabDelete();
	afx_msg void OnBnClickedButtonRecTabAdd();
	afx_msg void OnBnClickedButtonRecTabEdit();
	CBrush	m_SilverBrush;
	CBrush	m_WhiteBrush;

	bool ScheduleDirtyFlag;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
