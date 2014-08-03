#include "resource.h"
#include "BaseDialog.h"
#include "atlimage.h"
#include "PictureCtrl.h"
#include "VideoThin.h"
#include "INI.h"

#pragma once


// CDlgANPR dialog

class CDlgANPR : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgANPR)

public:
	CDlgANPR(CWnd* pParent = NULL);   // standard constructor
	CDlgANPR(NODEITEM* pNode, CWnd* pParent = NULL);  
	virtual ~CDlgANPR();

// Dialog Data
	enum { IDD = IDD_DLG_ANPR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void vScanAllShot();
	afx_msg void OnNMClickListAnprFiles(NMHDR *pNMHDR, LRESULT *pResult);

	CVideoThin	m_videovv;
	NODEITEM* m_Node;
	bool bRefreshShotList;

	static UINT RefreshShotList(LPVOID lpParam);
};
