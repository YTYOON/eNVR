#pragma once
#include "resource.h"
#include "DlgModifyStream.h"
#include "DlgModifyPTZ.h"
#include "afxcmn.h"
#include "BaseDialog.h"

// CDlgModifyStreamPTZ dialog

class CDlgModifyStreamPTZ : public CDialog
{
	DECLARE_DYNAMIC(CDlgModifyStreamPTZ)

public:
	CDlgModifyStreamPTZ(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgModifyStreamPTZ();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CBrush	m_SilverBrush;
	CBrush	m_WhiteBrush;

// Dialog Data
	enum { IDD = IDD_DLG_MODIFY_STREAM_PTZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int iStreamNum;

	vector<CDlgModifyStream*> veDlgStream;
	CTabCtrl m_TabCtrl;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);

	/*--Struct for Modify Stream and PTZ--*/
	MoPTZInfo m_MoPTZInfo;
	CDlgModifyPTZ * m_CDlgModifyPTZ;
	vector<MoStreamInfo> m_veMoStreamInfo;
	void vSetStreamPTZInfo(MoPTZInfo o_MoPTZInfo, vector<MoStreamInfo> o_veMoStreamInfo);

	afx_msg void OnBnClickedOk();
};
