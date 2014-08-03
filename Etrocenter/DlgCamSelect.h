#pragma once
#include "Resource.h"
#include "BaseDialog.h"
#include "INI.h"

// CDlgCamSelect dialog

class CDlgCamSelect : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgCamSelect)

	public:
		struct strCAM
	{
		int		CameraIdx;	
		int		CameraId;	
		CString	IP;
		CString	csModel;
	} ;

	CDlgCamSelect(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCamSelect();

// Dialog Data
	enum { IDD = IDD_DLG_CAMSELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl * m_camList;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	//CDatabase	m_DB;
	CDatabase*	pDB;
	int		m_camidx;

	//void vDBConnect();
	void vBuildList();
	afx_msg void OnLvnItemchangedListcam(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
//	afx_msg void OnNMDblclkListcam(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListcam(NMHDR *pNMHDR, LRESULT *pResult);
};
