#include "resource.h"
#include "BaseDialog.h"
#include "DlgAcEmpEdit.h"
#include "afxcmn.h"
#include "afxext.h"
#include "afxwin.h"

#pragma once
	
// CTabpageAcEmp dialog

class CTabpageAcEmp : public CBaseDialog
{
	DECLARE_DYNAMIC(CTabpageAcEmp)

public:
	struct STRDEPT{
			int			camidx;
			int			acgroupid;
			TCHAR		csCode[20];
			TCHAR		csOrgDept[50];
			TCHAR		csDept[50];
			TCHAR		csFrom[5];
			TCHAR		csTo[5];
			TCHAR		csPriv[50];
			TCHAR		csYN[50];
			char		cFlag;		
	};
	
	struct STREMP{
			int		camera_idx;
			int		acgroupid;
			int		acid;
			TCHAR	csDept[50];
			TCHAR	csName[50];
			TCHAR	csOrgName[50];
			char	csID[20];
			char	csActive;
			char	cFlag;		
			TCHAR	csCode[50];
			TCHAR	csFrom[10];
			TCHAR	csTo[10];
			TCHAR	picFile[50];
		};	

	struct STRPERM{
			int			acgroupid;
			int			cam_idx;
			char		cPerm[1];
	};

	struct STRCAM{
			int			cam_idx;
			CString	csname;
			int			selitem;
	};

	struct STRPRIV{
			int			cam_idx;
			CString	camname;
			CString	csDeptCode;
			char		cYN;
	};

	CButton*	ma_check[10];

	CTabpageAcEmp(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabpageAcEmp();

	void fnGetPermission(int oDeptid);

// Dialog Data
	enum { IDD = IDD_TABPAGEACEMP };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

		DECLARE_MESSAGE_MAP()

	public:
		vector <STRDEPT*> m_vpDept;
		vector <STREMP*> m_vpEmp;
		vector<int> m_vtcam;

		vector<STRCAM*> m_vtpcam;
		vector<STRPRIV*> m_vpPriv;

		CDatabase*	psDB;

		bool	mb_dirty;
		bool	mb_privdirty;
		char	mc_deptmode;
		int			m_acid;
		int			m_curdeptid;
		int			m_curcamidx;
		int			m_maxcam;
		int			ma_camidx[10];
		bool		ma_pricheck[10];

		CString	m_curDeptCode;
		CString	m_deptname;
		CString	m_empname;
		CString	m_empid;
		CString	m_newDeptCode;
		CString	m_camList;
		CString	m_privyn;

		virtual BOOL OnInitDialog();

		CTreeCtrl				m_tree;
		CToolTipCtrl*		m_pToolTipCtrl; 
		HTREEITEM			m_newItem; 

		void		fnBuildTree(void);
		void		fnBuildTree(int oCamidx);
		void		fnInsertTreeEmp(CString oGroupCode);
		void		fnSave(CString oMode);
		void		fnFreeMemory();

		afx_msg void OnTvnSelchangedTreeemp(NMHDR *pNMHDR, LRESULT *pResult);
		CBitmapButton m_butdeptadd, m_butdeptmod, m_butdeptdel, m_butempadd, m_butempmod, m_butempdel, m_butRefresh;

		afx_msg void OnBnClickedButdeptadd();
		afx_msg void OnBnClickedButdeptdel();
		afx_msg void OnBnClickedButempadd();
		afx_msg void OnBnClickedButempdel();
		 
		CEdit* m_editDept;
		afx_msg void OnBnClickedButdeptmod();

		CString fnGenString(CString osource, int olen);
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		afx_msg void OnBnClickedButempmod();
		// get department code from department ID
		CString fnGetDeptCode(int oDeptid);

		void fnClearCheck();
		// Check if checkbox rows for every camera-group
		//void fnRefreshCheck(void);
		bool fnFoundCheckRow(int oCamIdx, int oGroupid);
		// add new rows into ecac_group_camera based on new group id
		
		void fnAddCheckRow(int oacgroupid);
		void fnAddCheckRow(CString oDeptCode);

		afx_msg void OnCbnSelchangeCombogate();
		void fnBuildCamCombo(void);
		CComboBox m_comboGate;
		 
		void fnSetCheckText(int oItem, CString oText);
		
		void fnPrivilege(CString oGroupCode);
		void fnAllCheck(bool obShow, bool obEnable);
		void fnAllCheck(bool obShow, bool obEnable, bool obCheck);
		int fnInitPriv(void);
		bool fnInsertDeptPriv(CString oDeptCode);
		bool fnLoadBmp(CString o_file);
		void  fnLocalize();

		afx_msg void OnBnClickedPcheck1();
		afx_msg void OnBnClickedPcheck2();
		afx_msg void OnBnClickedPcheck3();
		afx_msg void OnBnClickedPcheck4();
		afx_msg void OnBnClickedPcheck5();
		afx_msg void OnBnClickedPcheck6();
		afx_msg void OnBnClickedPcheck7();
		afx_msg void OnBnClickedPcheck8();
		afx_msg void OnBnClickedPcheck9();
		afx_msg void OnBnClickedPcheck10();
		void fnClickPrivilege(int oCheckNum);
		void fnHideAllCheckbox();

		CString fnGetCheckState(int oIdx);
		afx_msg void OnBnClickedButrefresh();
		CPictureCtrl m_pic;
};
