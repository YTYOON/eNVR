#pragma once
#include "resource.h"
#include "BaseDialog.h"
#include "DlgNASSetting.h"
#include "MainFrm.h"

// CDlgNAS dialog

class CDlgNAS : public CDialog
{
	DECLARE_DYNAMIC(CDlgNAS)

public:
	CDlgNAS(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgNAS();

// Dialog Data
	enum { IDD = IDD_DLG_NAS_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:

	CDatabase *mp_DB;
	CBitmapButton m_cbNASDelete;
	CBitmapButton m_cbNASAdd;
	CBitmapButton m_cbNASEdit;
	CToolTipCtrl *m_ToolTip;
	CCheckListBox pNASList;
	vector<NASInfo> veNASInfo;
	NASInfo m_NASInfo;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnAddNas();
	afx_msg void OnBnClickedBtnModifyNas();
	afx_msg void OnBnClickedBtnDeleteNas();
	afx_msg void OnLbnSelchangeListNasStorage();
};
