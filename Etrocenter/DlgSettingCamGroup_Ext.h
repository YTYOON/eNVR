#pragma once
#include "BaseDialog.h"
#include "afxdb.h"
#include "INI.h"
#define MAXMODULE 5

// CDlgSettingCamGroup_Ext dialog

class CDlgSettingCamGroup_Ext : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgSettingCamGroup_Ext)

public:
	//CDlgSettingCamGroup_Ext(CWnd* pParent = NULL);   // standard constructor
	CDlgSettingCamGroup_Ext(int nGroupId = 0, const CString& strName = _T(""), CWnd* pParent = NULL);   // standard constructor

	virtual ~CDlgSettingCamGroup_Ext();

// Dialog Data
	enum { IDD = IDD_SETTING_CAMGROUP_EXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	struct ECGROUP
	{
		int		idx;
		TCHAR	default_[1];
		TCHAR	module[16];
		TCHAR	desc_[128];
	};

public:
	int m_Groupid;
	int vPopulateModule(void);
	virtual BOOL OnInitDialog();

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//int vDBConnect(void);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	int vInitGroup(CString o_EntryMode);
protected:
	virtual void OnOK();
	CString ma_Module[MAXMODULE];
public:
	CString m_GroupName;
	CString m_ModuleCode;
	
	bool mb_OK;
	CString m_EntryMode;

	HTREEITEM	m_itemSpec;
	ECGROUP		m_sGroup;

	virtual INT_PTR DoModal();
	virtual INT_PTR DoModal(HTREEITEM hitem);

protected:
	virtual void OnCancel();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
