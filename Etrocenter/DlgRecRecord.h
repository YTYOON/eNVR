#pragma once
//#include "stdafx.h"
#include "resource.h"
#include "INI.h"
#include "BaseDialog.h"

// CDlgRecRecord dialog

class CDlgRecRecord : public CDialog
{
	DECLARE_DYNAMIC(CDlgRecRecord)

public:
	CDlgRecRecord(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgRecRecord();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CBrush	m_SilverBrush;
	CBrush	m_WhiteBrush;

// Dialog Data
	enum { IDD = IDD_DLG_RECORDING_RECORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	int m_iCameraIDX;
	CDatabase*  pDB;
	CString csDSN, csDBUser, csDBPass, csODBC;	//For Database

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString vSetCameraIDX(int iCameraIDX);
	//void vDBConnect();
	CString csGetRecordingType(int m_iCameraIDX);
//	afx_msg void OnBnClickedRadioRecordingRecordContinuous();
//	afx_msg void OnBnClickedRadioRecordingRecordEvent();

	bool RecordDirtyFlag;
//	afx_msg void OnBnHotItemChangeRadioRecordingRecordContinuous(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnBnHotItemChangeRadioRecordingRecordEvent(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadioRecordingRecordContinuous();
	afx_msg void OnBnClickedRadioRecordingRecordEvent();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedRadioRecordingRecordScheduling();
};
