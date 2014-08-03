#include "BaseDialog.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "DlgChain.h"
#include "DlgPlate.h"

#pragma once

// CTabpageGatePark dialog

class CTabpageGatePark : public CBaseDialog
{
	DECLARE_DYNAMIC(CTabpageGatePark)

public:
	CTabpageGatePark(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabpageGatePark();

// Dialog Data
	enum { IDD = IDD_TABPAGEGATEPARK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	struct  STRCHAINPARK{
			int			chainid;
			int			parklotid;
			int			groupid;
			TCHAR	chaindesc[50];
			TCHAR	parklotdesc[50];
			TCHAR  groupdesc[50];
			TCHAR  mapfile[50];
			char		cFlag;
	};		

	vector <STRCHAINPARK*> m_vpChain;
	vector <STRCHAINPARK*> m_vpModChain;
	vector <STRCHAINPARK*> m_vpModPark;

	CDatabase* psDB;

	void fnBuildTree(); //Build Chain Tree
	void fnBuildTreeAC(); //Build Chain Tree

	virtual BOOL OnInitDialog();
	CTreeCtrl m_tree;
	CBitmapButton m_butchainadd,	m_butchainmod, m_butchaindel, m_butparkadd, m_butparkmod, m_butparkdel;
	
	afx_msg void OnTvnSelChangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButchainadd();
	afx_msg void OnBnClickedButchainmod();
	afx_msg void OnBnClickedButchaindel();
	afx_msg void OnBnClickedButparkadd();
	afx_msg void OnBnClickedButparkmod();
	afx_msg void OnBnClickedButparkdel();
	
	void fnSave(bool obClose);
	void fnSaveAC(bool obClose);


	CString	m_group, m_map;

public:
		CToolTipCtrl* m_pToolTipCtrl;
	//Group structure & vector
	struct STRGROUP{
			int			idx;
			int			cameragroupid;
 			TCHAR		desc[200];			
	};
	vector <STRGROUP> m_vtpGroup;
 
	int	m_chainid;
	int	m_newchainid;
	int	m_parklotid;
	int	m_groupid;

	char mc_Pmode;

	void fnFreeMemory(void);
	// Get Max parklot id from ecgt_parklot
	int fnGetMaxParklotid(void);
	int fnGetIdent(CString oCol, CString oTable);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void fnLocalize();
	void fnDeleteTB(CString oTabname, CString oColname, int oChainid);
};

