#pragma once
#include "resource.h"
#include "BtnST.h"
// CDlgAutoScanSetting dialog

class CDlgAutoScanSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgAutoScanSetting)

public:
	CDlgAutoScanSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAutoScanSetting();

// Dialog Data
	enum { IDD = IDD_DLG_AUTOSCAN_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	CButtonST *btnAutoScanSettingGroup;
	afx_msg void OnBnAutoScanSettingGroup();
};
