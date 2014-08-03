#pragma once
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"

// CDlgAutoScanGroupSetting dialog

class CDlgAutoScanGroupSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgAutoScanGroupSetting)

public:
	CDlgAutoScanGroupSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAutoScanGroupSetting();

// Dialog Data
	enum { IDD = IDD_DLG_AUTOSCAN_GROUP_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_TreeCtrlGroup;
	virtual BOOL OnInitDialog();
	void vBuildGroupTree(void);
protected:
	virtual void OnOK();
public:
	void vCheckSelectedGroup(void);
	void vSetSelectedGroupId(void);
	vector <HTREEITEM> m_vcthTree;
	CButton m_CheckBox_AutoscanEnable;
};
