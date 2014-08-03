#include "BaseDialog.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "DlgPlate.h"

#pragma once


// CTabpageGatePlate dialog

class CTabpageGatePlate : public CBaseDialog
{
	DECLARE_DYNAMIC(CTabpageGatePlate)

public:
	CTabpageGatePlate(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabpageGatePlate();
	CDatabase* psDB;

// Dialog Data
	enum { IDD = IDD_TABPAGEGATEPLATE };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	CToolTipCtrl*	 m_pToolTipCtrl;
	afx_msg void OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	void fnBuildTree();
	void fnLocalize();
	
	struct STRPLATE{
			int			id;
 			TCHAR		cardcode[20];			
			TCHAR		name[50];		
			TCHAR		plate1[20];
			TCHAR		plate2[20];
			TCHAR		plate3[20];
			char			cFlag;

	};
	vector <STRPLATE*> m_vtpPlate;
	vector <STRPLATE*> m_vpModPlate;

	CBitmapButton m_butadd;
	CBitmapButton m_butmod;
	CBitmapButton m_butdel;
	CTreeCtrl m_tree;

	int	m_cardid;
	CString	m_card, m_name, m_plate1, m_plate2, m_plate3;
 
	afx_msg void OnBnClickedButadd();
	afx_msg void OnBnClickedButmod();
	afx_msg void OnBnClickedButdel();

	void fnSave(bool obClose);

	bool mb_dirty;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
