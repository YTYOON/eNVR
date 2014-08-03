#pragma once
#include "resource.h"
#include "BaseDialog.h"
#include "afxwin.h"
#include "PictureCtrl.h"
#include "afxext.h"

//#include "TabpageAcEmp.h"

class CDlgAcEmpEdit : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgAcEmpEdit)

public:
	CDlgAcEmpEdit(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAcEmpEdit();
 
 
	enum { IDD = IDD_DLGACEMPEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


public:
//	CTabpageAcEmp m_parent;	
	virtual BOOL OnInitDialog();

	int			m_acid;
	CString	m_entryMode; //DA: department add, DM Department Modify; EA Employee Add; EM Employee Modify

	CString	m_dept;
	CString	m_empname;
	CString	m_empid;
	CString	m_from;
	CString	m_to;
	CString	m_picFile;
	CDatabase*	m_DB;
	
	bool	mb_picChanged;

	struct	STRCHECK
	{
		CString	csCamName;
		int			camIdx;
		char		cPermit[1];
	};
	vector<STRCHECK*> m_vtCheck;

	struct	STREMP
	{
		CString	csEmp;
		CString	csAcid;
		TCHAR	picFile[50];
 	};
	vector<STREMP*> m_vtEmp;

public:
	CButton* ma_check[10];	

protected:
	virtual void OnOK();
public:
	/*afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);*/
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedMore();

	afx_msg void OnClose();
	afx_msg void OnBnClickedButpic();

	bool fnLoadBmp(CString o_file);
	//CPictureCtrl	m_pic;
	CBitmapButton		m_butpic;
	CPictureCtrl m_pic2;
	CBitmapButton m_butdel;
	afx_msg void OnBnClickedButpicdel();
};
