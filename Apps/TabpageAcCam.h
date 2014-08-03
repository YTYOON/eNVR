#include "resource.h"
#include "BaseDialog.h"
#pragma once#include "afxcmn.h"
#include "afxwin.h"



// CTabpageAcCam dialog

class CTabpageAcCam : public CBaseDialog
{
	DECLARE_DYNAMIC(CTabpageAcCam)

public:
	CTabpageAcCam(CWnd* pParent = NULL);   // standard constructor
	//vector <STRCAMAC> m_vpCamAccess;	
	
	virtual ~CTabpageAcCam();

// Dialog Data
	enum { IDD = IDD_TABPAGEACCAM };

	struct STRCAM{
		int			cam_idx;
		CString	brand;
		CString	model;
		char		gatecontrol;
		char		active_;
		char		mode;
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CDatabase*	psDB;
	CToolTipCtrl* m_pToolTipCtrl;
	CNVRBk *m_pbk;

	bool	mb_dirty;
	int		m_camidx;
	int		m_curitem;

	CListCtrl m_CamList;

	vector <STRCAM> m_vpCamAccess;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeCombo1();

	void fnRetrieveData(void);
	void CTabpageAcCam::fnDisplayBrand(void);
	void fnSave(CString o_mode);
	void fnSaveGate(bool o_close);

	void fnInitCam(void);
	void fnInitCamGate(void);

	void fnPopulateCam(void);
	void fnPopulateCamGate(void);

	void fnClearControl(void);

	CImageList			m_SmallImg;
	afx_msg void OnBnClickedCheckactive();
	CBitmapButton m_butdel;
	CBitmapButton m_butadd;
	afx_msg void OnBnClickedButadd();
	afx_msg void OnCbnSelchangeCombomodel();
	afx_msg void OnBnClickedButdel();

	void fnRefreshCam(void);
	void fnRefreshCamGate(void);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// Set transparent mode cgi 
	void fnSetTrans(int oCamidx);
	void fnInsertPriv(int oCamidx);

	void  fnRebuildCam();
	void  fnRebuildCamGate();
	void fnLocalize();

	char	mc_parentMode;
};
