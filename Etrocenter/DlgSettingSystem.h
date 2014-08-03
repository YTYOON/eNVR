#pragma once
#include "resource.h"
#include "BaseDialog.h"
#include "afxwin.h"
#include "gs.h"
#include "INI.h"

enum LANGIDX{EN=0,CH_T, CH_S, RU, DE, FR, ESMODERN};
#define numOfLang 7

// CDlgSettingSystem dialog

class CDlgSettingSystem : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgSettingSystem)

public:
	CDlgSettingSystem(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSettingSystem();

// Dialog Data
	virtual enum { IDD = IDD_SETTING_SYSTEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
	void RestartService(const CString& str);
	bool bSelectDatabaseFile(CString& strFilePath);
	void GetDatabaseFileName(CString& strFileName);
	void ExportDatabase();
	void ExportReg();
	void ImportDatabase(const CString& strFilePath);
	void ImportReg(const CString& strFilePath);
	void MergeRegFile(const CString& strTempFile, const CString& strRegFile);
	bool IsFileExist();
	void ChangeControlLaguage();
public:
	afx_msg void OnBnClickedOk();
protected:
	CButton m_AutoRecording;
public:
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	CComboBox m_Stream;
	CWnd* m_pParent;

	void RestarProcess();
public:
	bool vSaveSystem(void);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	CIniReader m_ini;
	bool DirtyFlag;
	afx_msg void OnBnClickedSettingSystemRun();
	bool m_bEnableRun;
	CGS m_gs;
	afx_msg void OnCbnSelchangeCombostream();
	afx_msg void OnBnClickedCheck1();

	bool bSaveLanguageToRegistry(int iLanguage);
	DWORD dwGetLanguageFromRegistry();
	LCID m_nLocaleID,m_nLocaleIDInit;
	afx_msg void OnBnClickedRadioEnglish();
	afx_msg void OnBnClickedRadioRussian();
	afx_msg void OnBnClickedRadioChineseTran();
	afx_msg void OnBnClickedChineseSimp();
	void vFindDLL();
	void vDisplayLanguageOption(CString csFileName);	
	afx_msg void OnBnClickedCheckSettingSystemFullscreen();	
	afx_msg void OnBnClickedCheckSettingSystemWizard();
	bool m_bEnableFullScreen;
	bool m_bEnableWizard;
	DWORD m_nLangIDCArray [numOfLang];
	afx_msg void OnBnClickedRadioGerman();
	afx_msg void OnBnClickedRadioFrench();
	afx_msg void OnBnClickedRadioSpanish();
	afx_msg void OnBnClickedExpbtn();
	afx_msg void OnBnClickedImpbtn();

};
