 #include "resource.h"
#include "BaseDialog.h"
#include "INI.h"
#pragma once
#include "afxext.h"
#include "afxcmn.h"
#include "afxdtctl.h"
#include "afxwin.h"


// CTabpageAcHol dialog

class CTabpageAcHol : public CBaseDialog
{
	DECLARE_DYNAMIC(CTabpageAcHol)

public:
	 

	struct STRCAM{
			int			cam_idx;
			CString	csname;
	};

	struct STRHOL{
			//int		camera_idx;
			//int		acgroupid;
			int			holid;
 			TCHAR	date_[20];		
			TCHAR	csDept[50];
			TCHAR	csName[50];
			TCHAR	csOrgName[50];
			char	csID[20];
			char	csActive;
			char	cFlag;	
			char	mode;
			TCHAR	csCode[20];
		};	

	vector <STRHOL*> m_vpHol;

	CIniReader		m_ini;
	CDatabase*		psDB;
	bool					mb_dirty;
	bool					mb_datedirty;
	bool					mb_eydirty;
	bool					mb_weekenddirty;

	CTabpageAcHol(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabpageAcHol();

// Dialog Data
	enum { IDD = IDD_TABPAGEACHOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	CBitmapButton m_butadd;
	CBitmapButton m_butdel;
	//CListCtrl m_listhol;
	CToolTipCtrl*	m_pToolTipCtrl; 

	int	m_curitem;

	void fnInitCtrl(void);
	void fnPopulateHol(void);
	void fnSave(CString oMode);

	//afx_msg void OnLvnItemchangedListhol(NMHDR *pNMHDR, LRESULT *pResult);
	int fnparse(CString oSource, CString oSeperator);
	afx_msg void OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCkweekend();
	afx_msg void OnBnClickedButadd();
	CDateTimeCtrl m_date;

	CTime	tmDate;
	STRHOL* mp_curHol; 

	afx_msg void OnBnClickedButdel();
	//void fnRefreshList(void);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_butSamedate;
	afx_msg void OnCbnSelchangeCombostart();
	afx_msg void OnCbnSelchangeCombogate();
//	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	//CTreeCtrl m_tree;
	CComboBox m_comboGate;


public:
	HTREEITEM			m_newItem; 

	vector<STRCAM*> m_vtpcam;
	int m_curcamidx;
	int	m_curdeptid;

	//void fnBuildTree(int oCamidx);	
	void fnDisplayList();
	void fnBuildCamCombo(void);

	CComboBox m_comGate;
	CTreeCtrl m_tree;

	CString	m_deptname;
	afx_msg void OnBnClickedButmod();
	CBitmapButton m_butmod;
	CListCtrl m_list;
	CString		m_curDate;
	
	void fnLocalize();

	afx_msg void OnLvnItemchangedListhol(NMHDR *pNMHDR, LRESULT *pResult);
};
