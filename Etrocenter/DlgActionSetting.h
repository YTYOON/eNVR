#include "resource.h"
#pragma once


// CDlgActionSetting dialog
struct ActionInfo{

	DWORD	dwActionType;
	bool	bEmail;
	TCHAR	tcTargetMail[1000];
	bool	bFTP;
	bool	bRecording;
	bool	bSnapshot;
	bool	bDO;
	bool	bDO_Setting;
	bool	bPTZ;
};


class CDlgActionSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgActionSetting)

public:
	CDlgActionSetting(CWnd* pParent = NULL);   // standard constructor
	CDlgActionSetting(int iType, ActionInfo* pActionInfo, CWnd* pParent = NULL);
	virtual ~CDlgActionSetting();

// Dialog Data
	enum { IDD = IDD_DLG_ACTION_SETTING };
	enum ACTION_SETTING_TYPE {AST_EMAIL=0, AST_PTZ, AST_DO};
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_iType;	//Action Type
	virtual BOOL OnInitDialog();
	CComboBox* m_PTZ_P;
	CComboBox* m_PTZ_T;
	CComboBox* m_PTZ_Z;
	CComboBox* m_DO;
	afx_msg void OnBnClickedOk();
	ActionInfo* pActionInfo;
};
