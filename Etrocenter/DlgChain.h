#include "BaseDialog.h"
#pragma once#include "afxwin.h"


// CDlgChain dialog

class CDlgChain : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgChain)

public:
	CDlgChain(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgChain();

// Dialog Data
	enum { IDD = IDD_DLGCHAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	char	mc_Pmode; // A for Access or G for Gate
	CString	m_mode;
	CString	m_chain;
	CString	m_parklot;
	CString	m_group;
	CString	m_map;
	int			m_chainid;
	int			m_parklotid;
	int			m_groupid;

	virtual BOOL OnInitDialog();
	void	fnHidePark();
	void		fnPopulateGroup();

protected:
	virtual void OnOK();

public:
		struct STRGROUP{
			int			idx;
			int			cameragroupid;
 			TCHAR		desc[200];			
		};
	vector <STRGROUP> m_vtpGroup;

	CComboBox m_comboGroup;
	CDatabase*	pDB;
	// Get combobox index from group desc
	int fnGetGroupidx(int oGroupid);
 
	afx_msg void OnBnClickedButfolder();
	CBitmapButton m_butfolder, m_butmap;
	afx_msg void OnBnClickedButmap();
	void fnFolder();
	afx_msg void OnCbnSelchangeCombogroup();
	void fnChangeTxt(void);
};
