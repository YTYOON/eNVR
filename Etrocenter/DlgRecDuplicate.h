#pragma once
#include "resource.h"
#include "afxwin.h"
#include "INI.h"
#include "BaseDialog.h"

// CDlgRecDuplicate dialog
class CNVRBk;
class CDlgRecDuplicate : public CDialog
{
	DECLARE_DYNAMIC(CDlgRecDuplicate)

public:
	CDlgRecDuplicate(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgRecDuplicate();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CBrush	m_SilverBrush;
	CBrush	m_WhiteBrush;

	CNVRBk *m_pbk;

// Dialog Data
	enum { IDD = IDD_DLG_RECORDING_DUPLICATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CEdit pRecTypeEdit;
	CEdit pRecScheduleEdit;
	CCheckListBox* pDuplicateCameraList;
	CString m_csRecType, m_csSchedule, m_csCameraName;

	CDatabase* pDB;
	CString csDSN, csDBUser, csDBPass, csODBC;	//For Database

public:
	vector<CString> veDupCamereName;
	virtual BOOL OnInitDialog();
	//void vDBConnect();
	void vDBClose();
	void vGetAllCameraName();
	void vSetRecInfo(CString csCameraName);
	void vSetRecType(CString csRecordingType);

	afx_msg void OnLbnSelchangeListDuplicateCamera();
	afx_msg void OnBnClickedOk();
};
