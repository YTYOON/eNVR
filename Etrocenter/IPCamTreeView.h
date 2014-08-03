// IPCamTreeView.h: interface for the CIPCamTreeView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPCAMTREEVIEW_H__336E4CC6_A974_4A73_A5FA_5E76105593C6__INCLUDED_)
#define AFX_IPCAMTREEVIEW_H__336E4CC6_A974_4A73_A5FA_5E76105593C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include   <afxcview.h>   
#include "IPCamTreeCtrl.h"
#include "IPCamDiscovery.h"
#include "TreeButton.h"
#include "ColorBox.h"
#include "BaseBitmapButton.h"
//#include "BaseButMngr.h" 
#include "DlgAppTag.h"
//#include "DlgMatrixTag.h"
#include "DlgSettingTag.h"
#include "DlgSettingSystem.h"
#include "BtnST.h"
#include "DlgMatrixTab.h"


class CIPCamTreeView : public CDockablePane {
public:
	void vResetData();
	void OnAddInternetIpCam();
	CIPCamTreeCtrl* GetTreeCtrl(){return &m_TreeCtrl;};
	vector <NODEITEM*>* GetNodePtr(){return m_pvtNode;};
	NODEITEM* GetCurrentNodeItem() {return m_TreeCtrl.GetCurrentNodeItem();};
	void vLiveView();
	CIPCamTreeView();

	virtual ~CIPCamTreeView();
	//void OpenDialogTag(CString i_mode);
	CBrush	m_SilverBrush;
	CIPCamTreeCtrl m_TreeCtrl;
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIPCamTreeView)
	//}}AFX_VIRTUAL
protected:
	//CColorBox	m_TreeButton;
	vector <NODEITEM*>* m_pvtNode;
	CIPCamDiscovery* pDoc;
	bool lb_open;
	int		m_GroupIdx;  //Selected Group Index

	//CBitmap	m_BitmapMatrix, m_LeftArrow, m_Setting, m_App;
	//CDC		m_CDCMatrix, m_CDCLeftArrow, m_CDCSetting, m_CDCApp;
 
	//CStatic		*pstMatrix, *pstSetting, *pstApp;
	//CButtonST	*pMatrix;
	//CButtonST	*pSetting;
	//CButtonST	*pApplication;
	//CButtonST	*mp_ToolRefresh;

	//DlgMatrixTab		m_MatrixDlg;
	//CDlgSettingTag		m_SettingDlg;
 //   CDlgAppTag          m_AppTagDlg;

// Implementation
protected:
	//{{AFX_MSG(CIPCamTreeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDestroy();
   
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	static UINT _stdcall RefreshThread(LPVOID lpParam);


public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//afx_msg void CIPCamTreeView::OnRangeCmds(UINT nID);
	CFont* m_Font;

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg void OnPaint();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//afx_msg void OnDestroy();
public:
	// Manage the settings - reRoute into different direction
	int vSettingManager(int o_SettingID);
 
//	afx_msg void OnBnClickedRefreshTree();
	bool vRefreshTree(void);
//	bool bChangeBmp(int o_Matrix);
	LRESULT OnChangeBMP(WPARAM wParam, LPARAM lParam); 

//	CBitmap myBmp1;
};

#endif // !defined(AFX_IPCAMTREEVIEW_H__336E4CC6_A974_4A73_A5FA_5E76105593C6__INCLUDED_)
